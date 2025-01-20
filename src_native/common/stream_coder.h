#ifndef STREAM_CODER_H
#define STREAM_CODER_H

#include <napi.h>
#include "allocator.h"

namespace ZSTD_NODE {

  using Napi::ObjectWrap;

  using std::vector;

  class StreamCoder {
  public:
    Allocator alloc;

    vector<char*> pending_output;
    Napi::Array PendingChunksAsArray(const Napi::Env& env);
    void Cleanup(const Napi::Env& env);

    StreamCoder();
  };

}

#endif
