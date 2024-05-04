import stream, { TransformOptions } from "stream";
import compressor from "./compressor.cjs";
import decompressor from "./decompressor.cjs";

export interface ZstdOptions {
  level?: number;
  dict?: string;
}

var Transform = stream.Transform;

export function compress(input: Buffer, opts: ZstdOptions = {}) {
  return new Promise((resolve, reject) => {
    if (!Buffer.isBuffer(input)) {
      reject(new Error("Input is not a buffer."));
      return;
    }

    const stream = new TransformStreamCompressor(opts);
    const chunks: Uint8Array[] = [];
    let length = 0;
    stream.on("error", (e) => reject(e));
    stream.on("data", function (c) {
      chunks.push(c);
      length += c.length;
    });
    stream.on("end", () => {
      resolve(Buffer.concat(chunks, length));
    });
    stream.end(input);
  });
}

export function decompress(input: Buffer, opts: ZstdOptions = {}) {
  return new Promise((resolve, reject) => {
    if (!Buffer.isBuffer(input)) {
      reject(new Error("Input is not a buffer."));
      return;
    }

    const stream = new TransformStreamDecompressor(opts);
    const chunks: Uint8Array[] = [];
    let length = 0;
    stream.on("error", (e) => reject(e));
    stream.on("data", function (c) {
      chunks.push(c);
      length += c.length;
    });
    stream.on("end", function () {
      resolve(Buffer.concat(chunks, length));
    });
    stream.end(input);
  });
}

interface StreamStatus {
  blockSize: number;
  remaining: number;
}

export class TransformStreamCompressor extends Transform {
  compressor = new compressor.StreamCompressor(this.opts || {});
  status: StreamStatus;

  constructor(
    private opts?: ZstdOptions & TransformOptions,
    private sync = false
  ) {
    super(opts);

    const blockSize = this.compressor.getBlockSize();
    this.status = {
      blockSize: blockSize,
      remaining: blockSize,
    };
  }

  _transform(chunk: Uint8Array, encoding: string, next: () => void) {
    compressStreamChunk(
      this,
      chunk,
      this.compressor,
      this.status,
      this.sync,
      next
    );
  }

  _flush(done: (err?: any) => void) {
    var that = this;
    this.compressor.compress(
      true,
      function (err: any, output: Uint8Array[]) {
        if (err) {
          return done(err);
        }
        if (output) {
          for (var i = 0; i < output.length; i++) {
            that.push(output[i]);
          }
        }
        return done();
      },
      !this.sync
    );
  }
}

// We need to fill the blockSize for better compression results
export function compressStreamChunk(
  stream: TransformStreamCompressor,
  chunk: Uint8Array,
  compressor: any,
  status: StreamStatus,
  sync: boolean,
  done: (err?: any) => void
) {
  var length = chunk.length;

  if (length > status.remaining) {
    var slicedChunk = chunk.slice(0, status.remaining);
    chunk = chunk.slice(status.remaining);
    status.remaining = status.blockSize;

    compressor.copy(slicedChunk);
    compressor.compress(
      false,
      function (err: any, output: Uint8Array[]) {
        if (err) {
          return done(err);
        }
        if (output) {
          for (var i = 0; i < output.length; i++) {
            stream.push(output[i]);
          }
        }
        return compressStreamChunk(
          stream,
          chunk,
          compressor,
          status,
          sync,
          done
        );
      },
      !sync
    );
  } else if (length <= status.remaining) {
    status.remaining -= length;
    compressor.copy(chunk);
    return done();
  }
}

export function compressStream(opts?: ZstdOptions) {
  return new TransformStreamCompressor(opts);
}

export class TransformStreamDecompressor extends Transform {
  decompressor = new decompressor.StreamDecompressor(this.opts || {});
  status: StreamStatus;

  constructor(
    private opts?: ZstdOptions & TransformOptions,
    private sync = false
  ) {
    super(opts);

    const blockSize = this.decompressor.getBlockSize();
    this.status = {
      blockSize: blockSize,
      remaining: blockSize,
    };
  }

  _transform(chunk: Uint8Array, encoding: string, next: () => void) {
    decompressStreamChunk(
      this,
      chunk,
      this.decompressor,
      this.status,
      this.sync,
      next
    );
  }

  _flush(done: (err?: any) => void) {
    var that = this;
    this.decompressor.decompress(function (err: any, output: Uint8Array[]) {
      if (err) {
        return done(err);
      }
      if (output) {
        for (var i = 0; i < output.length; i++) {
          that.push(output[i]);
        }
      }
      return done();
    }, !this.sync);
  }
}

// We need to fill the blockSize for better compression results
export function decompressStreamChunk(
  stream: TransformStreamDecompressor,
  chunk: Uint8Array,
  decompressor: any,
  status: StreamStatus,
  sync: boolean,
  done: (err?: any) => void
) {
  var length = chunk.length;

  if (length > status.remaining) {
    var slicedChunk = chunk.slice(0, status.remaining);
    chunk = chunk.slice(status.remaining);
    status.remaining = status.blockSize;

    decompressor.copy(slicedChunk);
    decompressor.decompress(function (err: any, output: Uint8Array[]) {
      if (err) {
        return done(err);
      }
      if (output) {
        for (var i = 0; i < output.length; i++) {
          stream.push(output[i]);
        }
      }
      return decompressStreamChunk(
        stream,
        chunk,
        decompressor,
        status,
        sync,
        done
      );
    }, !sync);
  } else if (length <= status.remaining) {
    status.remaining -= length;
    decompressor.copy(chunk);
    return done();
  }
}

export function decompressStream(opts?: ZstdOptions) {
  return new TransformStreamDecompressor(opts);
}
