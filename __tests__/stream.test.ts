import { Writable } from "stream";
import fs from "fs/promises";
import { createReadStream } from "fs";
import path from "path";
import { compressStream, decompressStream } from "../esm/index.js";

class BufferWriter extends Writable {
  data = Buffer.alloc(0);

  constructor() {
    super();
  }

  _write(chunk: any, encoding: any, next: () => void) {
    this.data = Buffer.concat(
      [this.data, chunk],
      this.data.length + chunk.length
    );
    next();
  }
}

function testStream(
  method: any,
  bufferFile: string,
  resultFile: string,
  params?: any
) {
  const writeStream = new BufferWriter();

  createReadStream(path.join(__dirname, "/fixtures/", bufferFile))
    .pipe(method(params))
    .pipe(writeStream);

  return new Promise<void>((resolve) => {
    writeStream.on("finish", async () => {
      const result = await fs.readFile(
        path.join(__dirname, "/fixtures/", resultFile)
      );
      expect(writeStream.data).to.deep.equal(result);
      resolve();
    });
  });
}

describe("ZSTD Stream", function () {
  describe("compress", function () {
    it("should compress binary data", async () => {
      testStream(compressStream, "data10k.bin", "data10k.bin.zst");
    });

    it("should compress text data", async () => {
      testStream(compressStream, "data.txt", "data.txt.zst");
    });

    it("should compress text data with level=3", async () => {
      testStream(compressStream, "data.txt", "data.txt.zst.03", {
        level: 3,
      });
    });

    it("should compress text data with level=9", async () => {
      testStream(compressStream, "data.txt", "data.txt.zst.09", {
        level: 9,
      });
    });

    it('should compress text data with dict=(new Buffer("hello zstd"))', async () => {
      testStream(compressStream, "data.txt", "data.txt.zst.09.dict", {
        level: 9,
        dict: Buffer.from("hello zstd"),
      });
    });

    it("should compress an empty buffer", async () => {
      testStream(compressStream, "empty", "empty.zst");
    });

    it("should compress a large buffer", async () => {
      testStream(compressStream, "large.txt", "large.txt.zst");
    });
  });

  describe("decompress", function () {
    it("should decompress binary data", async () => {
      testStream(decompressStream, "data10k.bin.zst", "data10k.bin");
    });

    it("should decompress text data", async () => {
      testStream(decompressStream, "data.txt.zst", "data.txt");
    });

    it('should decompress text data with dict=(new Buffer("hello zstd"))', async () => {
      testStream(decompressStream, "data.txt.zst.09.dict", "data.txt", {
        level: 9,
        dict: Buffer.from("hello zstd"),
      });
    });

    it("should decompress to an empty buffer", async () => {
      testStream(decompressStream, "empty.zst", "empty");
    });

    it("should decompress to a large buffer", async () => {
      testStream(decompressStream, "large.zst", "large");
    });

    it("should decompress to another large buffer", async () => {
      testStream(decompressStream, "large.txt.zst", "large.txt");
    });
  });
});
