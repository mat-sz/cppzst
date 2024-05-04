export interface StreamDecompressorInstance {
  getBlockSize(): number;
  decompress(
    callback: (err: any, output: Uint8Array[]) => void,
    sync: boolean
  ): void;
  copy(chunk: Uint8Array): void;
}

export interface StreamDecompressorConstructor {
  new (opts: any): StreamDecompressorInstance;
}

export interface StreamCompressorInstance {
  getBlockSize(): number;
  compress(
    isLast: boolean,
    callback: (err: any, output: Uint8Array[]) => void,
    sync: boolean
  ): void;
  copy(chunk: Uint8Array): void;
}

export interface StreamCompressorConstructor {
  new (opts: any): StreamCompressorInstance;
}
