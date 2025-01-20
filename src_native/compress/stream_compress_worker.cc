#include "stream_compress_worker.h"

namespace ZSTD_NODE {

  StreamCompressWorker::StreamCompressWorker(const Napi::Function& callback, StreamCompressor* sc, bool isLast)
    : AsyncWorker(callback), sc(sc), isLast(isLast) {
    zInBuf = {sc->input, sc->inPos, 0};
    zOutBuf = {sc->dst, sc->dstSize, 0};
  }

  StreamCompressWorker::~StreamCompressWorker() {}

  void StreamCompressWorker::Execute() {
    while (zInBuf.pos < zInBuf.size) {
      zOutBuf.pos = 0;
      ret = ZSTD_compressStream(sc->zcs, &zOutBuf, &zInBuf);
      if (ZSTD_isError(ret)) {
        SetError(ZSTD_getErrorName(ret));
        return;
      }
      pushToPendingOutput();
    }

    if (isLast) {
      zOutBuf.pos = 0;
      ret = ZSTD_endStream(sc->zcs, &zOutBuf);
      if (ret != 0) {
        SetError("ZSTD compress failed, not fully flushed");
        return;
      }
      pushToPendingOutput();
    }
  }

  void StreamCompressWorker::pushToPendingOutput() {
    char *output = static_cast<char*>(sc->alloc.Alloc(zOutBuf.pos));
    if (output == NULL) {
      SetError("ZSTD compress failed, out of memory");
      return;
    }
    memcpy(output, zOutBuf.dst, zOutBuf.pos);
    Allocator::AllocatedBuffer* buf_info = Allocator::GetBufferInfo(output);
    buf_info->available = 0;
    sc->pending_output.push_back(output);
  }

  void StreamCompressWorker::OnOK() {
    Callback().Call({
      Env().Null(),
      sc->PendingChunksAsArray(Env())
    });

    sc->alloc.ReportMemoryToV8(Env());
  }

  void StreamCompressWorker::OnError(const Napi::Error& e) {
    Callback().Call({
      e.Value()
    });

    sc->alloc.ReportMemoryToV8(Env());
  }

}
