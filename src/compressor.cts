import type { StreamCompressorConstructor } from "./types";

const compressor = require("../build/Release/compressor.node");
const StreamCompressor =
  compressor.StreamCompressor as StreamCompressorConstructor;

export { StreamCompressor };
