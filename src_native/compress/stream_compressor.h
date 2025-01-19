#ifndef STREAM_COMPRESSOR_H
#define STREAM_COMPRESSOR_H

#include <napi.h>

#define ZSTD_STATIC_LINKING_ONLY
#include "zstd.h"
#include "stream_coder.h"

namespace ZSTD_NODE {

  class StreamCompressor : public StreamCoder, public Napi::ObjectWrap<StreamCompressor> {
  public:
    friend class StreamCompressWorker;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    StreamCompressor(const Napi::CallbackInfo& info);
    ~StreamCompressor();

  private:
    Napi::Value GetBlockSize(const Napi::CallbackInfo& info);
    void Copy(const Napi::CallbackInfo& info);
    void Compress(const Napi::CallbackInfo& info);

    ZSTD_CStream *zcs;

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
