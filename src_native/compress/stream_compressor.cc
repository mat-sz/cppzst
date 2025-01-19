#include "stream_compressor.h"
#include "stream_compress_worker.h"

namespace ZSTD_NODE {

  Napi::Object StreamCompressor::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func =
      DefineClass(env,
                  "StreamCompressor",
                  {InstanceMethod("getBlockSize", &StreamCompressor::GetBlockSize),
                   InstanceMethod("copy", &StreamCompressor::Copy),
                   InstanceMethod("compress", &StreamCompressor::Compress)});

    exports.Set("StreamCompressor", func);
    return exports;
  }

  StreamCompressor::StreamCompressor(const Napi::CallbackInfo& info)
      : StreamCoder(), Napi::ObjectWrap<StreamCompressor>(info), zcs(NULL), dict(NULL) {
    Napi::Env env = info.Env();

    if (info.Length() <= 0 || !info[0].IsObject()) {
      Napi::TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
      return;
    }

    Napi::Object userParams = info[0].As<Napi::Object>();

    int32_t level = 1;
    size_t dictSize = 0;

    if (userParams.Has("level")) {
      Napi::Number levelNum = userParams.Get("level").As<Napi::Number>();
      level = levelNum.Int32Value();
    }
    if (userParams.Has("dict")) {
      Napi::Buffer<char> dictBuf = userParams.Get("dict").As<Napi::Buffer<char>>();
      dictSize = dictBuf.Length();
      dict = alloc.Alloc(dictSize);
      memcpy(dict, dictBuf.Data(), dictSize);
    }

    inputSize = ZSTD_CStreamInSize();
    input = alloc.Alloc(inputSize);
    inPos = 0;

    dstSize = ZSTD_CStreamOutSize();
    dst = alloc.Alloc(dstSize);
    dstPos = 0;

    ZSTD_customMem zcm = {Allocator::Alloc, Allocator::Free, &alloc};
    zcs = ZSTD_createCStream_advanced(zcm);

    if (dict != NULL && dictSize > 0) {
      ZSTD_CCtx_reset(zcs, ZSTD_reset_session_only);
      ZSTD_CCtx_setParameter(zcs, ZSTD_c_compressionLevel, level);
      ZSTD_CCtx_loadDictionary(zcs, dict, dictSize);
    } else {
      ZSTD_initCStream(zcs, level);
    }
  }

  StreamCompressor::~StreamCompressor() {
    if (dict != NULL) {
      alloc.Free(dict);
    }
    if (input != NULL) {
      alloc.Free(input);
    }
    if (dst != NULL) {
      alloc.Free(dst);
    }
    ZSTD_freeCStream(zcs);
  }

  Napi::Value StreamCompressor::GetBlockSize(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), ZSTD_CStreamInSize());
  }

  void StreamCompressor::Copy(const Napi::CallbackInfo& info) {
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

  void StreamCompressor::Compress(const Napi::CallbackInfo& info) {

    Napi::Boolean isLast = info[0].As<Napi::Boolean>();
    Napi::Function callback = info[1].As<Napi::Function>();
    Napi::Boolean sync = info[2].As<Napi::Boolean>();

    StreamCompressWorker *worker = new StreamCompressWorker(callback, this, isLast.Value());
    if (sync.Value()) {
      worker->Queue();
    } else {
      worker->Execute();
      worker->OnWorkComplete(info.Env(), napi_ok);
    }
  }

}
