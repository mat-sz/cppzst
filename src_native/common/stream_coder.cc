#include "stream_coder.h"

namespace ZSTD_NODE {

  using Nan::CopyBuffer;
  using Nan::NewBuffer;
  using Nan::Set;

  StreamCoder::StreamCoder() {};

  StreamCoder::~StreamCoder() {
    size_t nChunks = pending_output.size();
    for (size_t i = 0; i < nChunks; i++) {
      alloc.Free(pending_output[i]);
    }
    alloc.ReportMemoryToV8();
  }

  Local<Array> StreamCoder::PendingChunksAsArray() {
    size_t nChunks = pending_output.size();
    Local<Array> chunks = Nan::New<Array>(nChunks);

    for (size_t i = 0; i < nChunks; i++) {
      char *cur = pending_output[i];
      Allocator::AllocatedBuffer *bufInfo = Allocator::GetBufferInfo(cur);
      Set(chunks, i, CopyBuffer(reinterpret_cast<char*>(cur),
                               bufInfo->size - bufInfo->available
                               ).ToLocalChecked());
      Allocator::Free(NULL, cur);
    }

    pending_output.clear();

    return chunks;
  }

}
