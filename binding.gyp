{
  "targets": [
    {
      "target_name": "cppzst",
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "deps/zstd/lib",
        "src_native/common"
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
      "conditions": [
        [
          "OS == 'mac'", {
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
              "MACOSX_DEPLOYMENT_TARGET": "10.12",
              "OTHER_CFLAGS": [
                "-std=c++20",
                "-stdlib=libc++"
              ],
            }
          }
        ],
        [
          "OS=='linux'", {
            "cflags_cc": [ "-std=c++20" ]
          }
        ]
      ]
    }
  ]
}
