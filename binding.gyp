{
  "targets": [
    {
      "target_name": "cppzst",
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "deps/zstd/lib",
        "src_native/common"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [
        "NAPI_VERSION=9",
        "NODE_ADDON_API_DISABLE_DEPRECATED",
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        "NODE_API_SWALLOW_UNTHROWABLE_EXCEPTIONS",
        "NODE_ADDON_API_ENABLE_TYPE_CHECK_ON_AS"
      ],
      "dependencies": [
        "<(module_root_dir)/deps/zstd.gyp:compress",
        "<(module_root_dir)/deps/zstd.gyp:decompress"
      ],
      "sources": [
        "src_native/common/stream_coder.cc",
        "src_native/common/allocator.cc",
        "src_native/compress/stream_compressor.cc",
        "src_native/compress/stream_compress_worker.cc",
        "src_native/decompress/stream_decompressor.cc",
        "src_native/decompress/stream_decompress_worker.cc",
        "src_native/index.cc"
      ],
    }
  ]
}
