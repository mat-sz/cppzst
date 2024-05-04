import type { StreamDecompressorConstructor } from "./types";

const decompressor = require("../build/Release/decompressor.node");
const StreamDecompressor =
  decompressor.StreamDecompressor as StreamDecompressorConstructor;

export { StreamDecompressor };
