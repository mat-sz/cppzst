{
  "targets": [
    {
      "target_name": "cppzst",
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "deps/zstd/lib",
        "src_native/common"
      ],
      "cflags_cc": [
        "-std=c++20",
        "-fexceptions",
        "-Wall",
        "-Os"
      ],
      "xcode_settings": {
        "CLANG_CXX_LANGUAGE_STANDARD": "c++20",
        "MACOSX_DEPLOYMENT_TARGET": "10.13",
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "OTHER_CPLUSPLUSFLAGS": [
          "-fexceptions",
          "-Wall",
          "-Oz"
        ]
      },
      "defines": [
        "NAPI_VERSION=9",
        "NODE_ADDON_API_DISABLE_DEPRECATED",
        "NAPI_CPP_EXCEPTIONS",
        "NODE_API_SWALLOW_UNTHROWABLE_EXCEPTIONS",
        "NODE_ADDON_API_ENABLE_TYPE_CHECK_ON_AS"
      ],
      "dependencies": [
        "<(module_root_dir)/deps/zstd.gyp:zstd",
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
