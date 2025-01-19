import fs from "fs";
import os from "os";
import type {
  StreamCompressorConstructor,
  StreamDecompressorConstructor,
} from "./types";

const arch = process.env.npm_config_arch || os.arch();
const platform = process.env.npm_config_platform || os.platform();

function isGNULibc() {
  if (platform !== "linux") {
    return true;
  }

  try {
    const text = fs.readFileSync("/usr/bin/ldd", { encoding: "utf8" });
    return text.includes("Free Software Foundation");
  } catch {
    return false;
  }
}

const libc = process.env.LIBC || (isGNULibc() ? "glibc" : "musl");
const paths = [
  "../build/Release/cppzst.node",
  `../prebuilds/${platform}-${arch}/cppzst.${libc}.node`,
  `@metastable/cppzst/prebuilds/${platform}-${arch}/cppzst.${libc}.node`,
];

let cppzst;
const errors = [];
for (const path of paths) {
  try {
    cppzst = require(path);
    break;
  } catch (err) {
    errors.push(err);
  }
}

if (!cppzst) {
  for (const error of errors) {
    console.error(error);
  }
  throw new Error("Unable to load cppzst");
}

const StreamCompressor = cppzst.StreamCompressor as StreamCompressorConstructor;
const StreamDecompressor =
  cppzst.StreamDecompressor as StreamDecompressorConstructor;

export { StreamCompressor, StreamDecompressor };
