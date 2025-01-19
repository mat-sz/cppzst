#include <napi.h>
#include "compress/stream_compressor.h"
#include "decompress/stream_decompressor.h"

namespace ZSTD_NODE {

  Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    StreamCompressor::Init(env, exports);
    StreamDecompressor::Init(env, exports);
    return exports;
  }

  NODE_API_MODULE(addon, InitAll)

}
