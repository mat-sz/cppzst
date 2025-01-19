#include "stream_coder.h"

namespace ZSTD_NODE {

  StreamCoder::StreamCoder() {};

  StreamCoder::~StreamCoder() {
    size_t nChunks = pending_output.size();
    for (size_t i = 0; i < nChunks; i++) {
      alloc.Free(pending_output[i]);
    }
    alloc.ReportMemoryToV8();
  }

  Napi::Array StreamCoder::PendingChunksAsArray(const Napi::Env& env) {
    size_t nChunks = pending_output.size();
    Napi::Array chunks = Napi::Array::New(env);

    for (size_t i = 0; i < nChunks; i++) {
      char *cur = pending_output[i];
      Allocator::AllocatedBuffer *bufInfo = Allocator::GetBufferInfo(cur);
      chunks.Set(i, Napi::Buffer<char>::Copy(env, reinterpret_cast<char*>(cur),
        bufInfo->size - bufInfo->available
      ));
      Allocator::Free(NULL, cur);
    }

    pending_output.clear();

    return chunks;
  }

}
