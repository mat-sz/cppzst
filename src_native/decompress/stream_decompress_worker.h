#ifndef STREAM_DECOMPRESSOR_WORKER_H
#define STREAM_DECOMPRESSOR_WORKER_H

#include <napi.h>
#include "stream_decompressor.h"

namespace ZSTD_NODE {

  class StreamDecompressWorker : public Napi::AsyncWorker {
  public:
    StreamDecompressWorker(const Napi::Function& callback, StreamDecompressor* sd);
    ~StreamDecompressWorker();

    void Execute();
    void OnOK();
    void OnError(const Napi::Error& e);

  private:
    void pushToPendingOutput();

    StreamDecompressor *sd;
    ZSTD_outBuffer zOutBuf;
    ZSTD_inBuffer zInBuf;

    size_t ret;
  };

}

#endif
