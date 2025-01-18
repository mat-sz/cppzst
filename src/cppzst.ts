import { resolve } from "path";
import type {
  StreamCompressorConstructor,
  StreamDecompressorConstructor,
} from "./types";

const cppzst = require("node-gyp-build")(resolve(__dirname, ".."));

const StreamCompressor = cppzst.StreamCompressor as StreamCompressorConstructor;
const StreamDecompressor =
  cppzst.StreamDecompressor as StreamDecompressorConstructor;

export { StreamCompressor, StreamDecompressor };
