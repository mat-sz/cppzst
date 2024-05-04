{
  "targets": [
    {
      "target_name": "compressor",
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "deps/zstd/lib",
        "src_native/common"
      ],
      "dependencies": [
        "<(module_root_dir)/deps/zstd.gyp:compress"
      ],
      "sources": [
        "src_native/common/stream_coder.cc",
        "src_native/common/allocator.cc",
        "src_native/compress/compressor_index.cc",
        "src_native/compress/stream_compressor.cc",
        "src_native/compress/stream_compress_worker.cc"
      ],
      "conditions": [
        [
          "OS == 'mac'", {
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
              "MACOSX_DEPLOYMENT_TARGET": "10.12"
            }
          }
        ],
        [
          "OS=='linux'", {
            "cflags_cc": [ "-std=c++17" ]
          }
        ]
      ]
    },
    {
      "target_name": "decompressor",
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "deps/zstd/lib",
        "src_native/common"
      ],
      "dependencies": [
        "<(module_root_dir)/deps/zstd.gyp:decompress"
      ],
      "sources": [
        "src_native/common/stream_coder.cc",
        "src_native/common/allocator.cc",
        "src_native/decompress/decompressor_index.cc",
        "src_native/decompress/stream_decompressor.cc",
        "src_native/decompress/stream_decompress_worker.cc"
      ],
      "conditions": [
        [
          "OS == 'mac'", {
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
              "MACOSX_DEPLOYMENT_TARGET": "10.12"
            }
          }
        ],
        [
          "OS=='linux'", {
            "cflags_cc": [ "-std=c++17" ]
          }
        ]
      ]
    }
  ]
}
