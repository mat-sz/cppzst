#ifndef STREAM_COMPRESS_WORKER_H
#define STREAM_COMPRESS_WORKER_H

#include <napi.h>
#include "stream_compressor.h"

namespace ZSTD_NODE {

  class StreamCompressWorker : public Napi::AsyncWorker {
  public:
    StreamCompressWorker(const Napi::Function& callback, StreamCompressor* sc, bool isLast);
    ~StreamCompressWorker();

    void Execute();
    void OnOK();
    void OnError(const Napi::Error& e);

  private:
    void pushToPendingOutput();

    StreamCompressor *sc;
    ZSTD_outBuffer zOutBuf;
    ZSTD_inBuffer zInBuf;
    bool isLast;
    size_t ret;
  };

}

#endif
