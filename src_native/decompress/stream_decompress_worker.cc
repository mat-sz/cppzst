#include "stream_decompress_worker.h"

namespace ZSTD_NODE {

  StreamDecompressWorker::StreamDecompressWorker(const Napi::Function& callback, StreamDecompressor* sd)
    : AsyncWorker(callback), sd(sd) {
    zInBuf = {sd->input, sd->inPos, 0};
    zOutBuf = {sd->dst, sd->dstSize, 0};
  }

  StreamDecompressWorker::~StreamDecompressWorker() {}

  void StreamDecompressWorker::Execute() {
    while (zInBuf.pos < zInBuf.size) {
      zOutBuf.pos = 0;
      ret = ZSTD_decompressStream(sd->zds, &zOutBuf, &zInBuf);
      if (ZSTD_isError(ret)) {
        SetError(ZSTD_getErrorName(ret));
        return;
      }
      pushToPendingOutput();
    }
  }

  void StreamDecompressWorker::pushToPendingOutput() {
    char *output = static_cast<char*>(sd->alloc.Alloc(zOutBuf.pos));
    if (output == NULL) {
      SetError("ZSTD decompress failed, out of memory");
      return;
    }
    memcpy(output, zOutBuf.dst, zOutBuf.pos);
    Allocator::AllocatedBuffer* buf_info = Allocator::GetBufferInfo(output);
    buf_info->available = 0;
    sd->pending_output.push_back(output);
  }

  void StreamDecompressWorker::OnOK() {
    Callback().Call({
      Env().Null(),
      sd->PendingChunksAsArray(Env())
    });

    sd->alloc.ReportMemoryToV8();
  }

  void StreamDecompressWorker::OnError(const Napi::Error& e) {
    Callback().Call({
      e.Value()
    });

    sd->alloc.ReportMemoryToV8();
  }

}
