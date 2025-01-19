#include "stream_decompressor.h"
#include "stream_decompress_worker.h"

namespace ZSTD_NODE {

  Napi::Object StreamDecompressor::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func =
      DefineClass(env,
                  "StreamDecompressor",
                  {InstanceMethod("getBlockSize", &StreamDecompressor::GetBlockSize),
                   InstanceMethod("copy", &StreamDecompressor::Copy),
                   InstanceMethod("decompress", &StreamDecompressor::Decompress)});

    exports.Set("StreamDecompressor", func);
    return exports;
  }

  StreamDecompressor::StreamDecompressor(const Napi::CallbackInfo& info)
      : StreamCoder(), Napi::ObjectWrap<StreamDecompressor>(info), zds(NULL), dict(NULL) {
    Napi::Env env = info.Env();

    if (info.Length() <= 0 || !info[0].IsObject()) {
      Napi::TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
      return;
    }

    Napi::Object userParams = info[0].As<Napi::Object>();

    size_t dictSize = 0;

    if (userParams.Has("dict")) {
      Napi::Buffer<char> dictBuf = userParams.Get("dict").As<Napi::Buffer<char>>();
      dictSize = dictBuf.Length();
      dict = alloc.Alloc(dictSize);
      memcpy(dict, dictBuf.Data(), dictSize);
    }

    inputSize = ZSTD_DStreamInSize();
    input = alloc.Alloc(inputSize);
    inPos = 0;

    dstSize = ZSTD_DStreamOutSize();
    dst = alloc.Alloc(dstSize);
    dstPos = 0;

    ZSTD_customMem zcm = {Allocator::Alloc, Allocator::Free, &alloc};
    zds = ZSTD_createDStream_advanced(zcm);

    if (dict != NULL && dictSize > 0) {
      ZSTD_DCtx_reset(zds, ZSTD_reset_session_only);
      ZSTD_DCtx_loadDictionary(zds, dict, dictSize);
    } else {
      ZSTD_initDStream(zds);
    }
  }

  StreamDecompressor::~StreamDecompressor() {
    if (dict != NULL) {
      alloc.Free(dict);
    }
    if (input != NULL) {
      alloc.Free(input);
    }
    if (dst != NULL) {
      alloc.Free(dst);
    }
    ZSTD_freeDStream(zds);
  }

  Napi::Value StreamDecompressor::GetBlockSize(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), ZSTD_DStreamInSize());
  }

  void StreamDecompressor::Copy(const Napi::CallbackInfo& info) {
    Napi::Buffer<char> chunkBuf = info[0].As<Napi::Buffer<char>>();
    char *chunk = chunkBuf.Data();
    size_t chunkSize = chunkBuf.Length();
    if (chunkSize != 0) {
      if (this->inPos == this->inputSize) {
        this->inPos = 0;
      }
      char *pos = static_cast<char*>(this->input) + this->inPos;
      memcpy(pos, chunk, chunkSize);
      this->inPos += chunkSize;
    }
  }

  void StreamDecompressor::Decompress(const Napi::CallbackInfo& info) {
    Napi::Function callback = info[0].As<Napi::Function>();
    StreamDecompressWorker *worker = new StreamDecompressWorker(callback, this);

    Napi::Boolean sync = info[1].As<Napi::Boolean>();
    if (sync.Value()) {
      worker->Queue();
    } else {
      worker->Execute();
      worker->OnWorkComplete(info.Env(), napi_ok);
    }
  }

}
