#ifndef STREAM_DECOMPRESSOR_H
#define STREAM_DECOMPRESSOR_H

#include <napi.h>

#define ZSTD_STATIC_LINKING_ONLY
#include "zstd.h"
#include "stream_coder.h"

namespace ZSTD_NODE {

  class StreamDecompressor : public StreamCoder, public Napi::ObjectWrap<StreamDecompressor> {
  public:
    friend class StreamDecompressWorker;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    StreamDecompressor(const Napi::CallbackInfo& info);

  private:
    Napi::Value GetBlockSize(const Napi::CallbackInfo& info);
    void Copy(const Napi::CallbackInfo& info);
    void Decompress(const Napi::CallbackInfo& info);
    void Cleanup(const Napi::Env& env);

    ZSTD_DStream *zds;
    
    size_t inputSize;
    size_t dstSize;

    size_t inPos;
    size_t dstPos;
    
    void *input;
    void *dst;
    void *dict;
  };

}

#endif
