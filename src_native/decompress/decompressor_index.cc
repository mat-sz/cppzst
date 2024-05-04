#include <nan.h>
#include "stream_decompressor.h"

namespace ZSTD_NODE {

  NAN_MODULE_INIT(Init) {
    StreamDecompressor::Init(target);
  }

  NAN_MODULE_WORKER_ENABLED(decompressor, Init)

}
