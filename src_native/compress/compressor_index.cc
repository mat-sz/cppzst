#include <nan.h>
#include "stream_compressor.h"

namespace ZSTD_NODE {

  NAN_MODULE_INIT(Init) {
    StreamCompressor::Init(target);
  }

  NAN_MODULE_WORKER_ENABLED(compressor, Init)

}
