import fs from "fs/promises";
import path from "path";
import { compress, decompress } from "../esm/index.js";

async function testBufferAsync(
  method: any,
  bufferFile: string,
  resultFile: string,
  params: any = {}
) {
  const buffer = await fs.readFile(
    path.join(import.meta.dirname, "/fixtures/", bufferFile)
  );
  const result = await fs.readFile(
    path.join(import.meta.dirname, "/fixtures/", resultFile)
  );

  const output = await method(buffer, params);
  expect(output).to.deep.equal(result);
}

describe("ZSTD Buffer Async", function () {
  describe("compress", function () {
    it("should compress binary data", async () => {
      await testBufferAsync(compress, "data10k.bin", "data10k.bin.zst");
    });

    it("should compress text data", async () => {
      await testBufferAsync(compress, "data.txt", "data.txt.zst");
    });

    it("should compress text data with level=3", async () => {
      await testBufferAsync(compress, "data.txt", "data.txt.zst.03", {
        level: 3,
      });
    });

    it("should compress text data with level=9", async () => {
      await testBufferAsync(compress, "data.txt", "data.txt.zst.09", {
        level: 9,
      });
    });

    it('should compress text data with dict=(new Buffer("hello zstd"))', async () => {
      await testBufferAsync(compress, "data.txt", "data.txt.zst.09.dict", {
        level: 9,
        dict: Buffer.from("hello zstd"),
      });
    });

    it("should compress an empty buffer", async () => {
      await testBufferAsync(compress, "empty", "empty.zst");
    });

    it("should compress a large buffer", async () => {
      await testBufferAsync(compress, "large.txt", "large.txt.zst");
    });
  });

  describe("decompress", function () {
    it("should decompress binary data", async () => {
      await testBufferAsync(decompress, "data10k.bin.zst", "data10k.bin");
    });

    it("should decompress text data", async () => {
      await testBufferAsync(decompress, "data.txt.zst", "data.txt");
    });

    it('should decompress text data with dict=(new Buffer("hello zstd"))', async () => {
      await testBufferAsync(decompress, "data.txt.zst.09.dict", "data.txt", {
        level: 9,
        dict: Buffer.from("hello zstd"),
      });
    });

    it("should decompress to an empty buffer", async () => {
      await testBufferAsync(decompress, "empty.zst", "empty");
    });

    it("should decompress to a large buffer", async () => {
      await testBufferAsync(decompress, "large.zst", "large");
    });

    it("should decompress to another large buffer", async () => {
      await testBufferAsync(decompress, "large.txt.zst", "large.txt");
    });
  });
});
