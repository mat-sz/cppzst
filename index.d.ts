import { Transform } from "stream";
export interface ZstdOptions {
  level?: number;
  dict?: string;
}

export function decompressStream(params?: ZstdOptions): Transform;
export function compressStream(params?: ZstdOptions): Transform;
export function decompressSync(input: Buffer, params?: ZstdOptions): Buffer;
export function compressSync(input: Buffer, params?: ZstdOptions): Buffer;
export function decompress(
  input: Buffer,
  params?: ZstdOptions
): Promise<Buffer>;
export function compress(input: Buffer, params?: ZstdOptions): Promise<Buffer>;
