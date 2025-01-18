#include <nan.h>
#include "compress/stream_compressor.h"
#include "decompress/stream_decompressor.h"

namespace ZSTD_NODE {

  NAN_MODULE_INIT(Init) {
    StreamCompressor::Init(target);
    StreamDecompressor::Init(target);
  }

  NAN_MODULE_WORKER_ENABLED(cppzst, Init)

}
