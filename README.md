# cppzst [![version](https://img.shields.io/npm/v/@metastable/cppzst.svg)](https://www.npmjs.com/package/@metastable/cppzst)

Zstd wrapper for node.js (fork)

Changes:

- Added support for running inside V8 sandbox.
- Updated zstd to latest.
- Added proper support for ES modules and CJS.

## Installation

```bash
$ npm install @metastable/cppzst --save
```

## Usage

### Async

#### compress(buffer[, zstdCompressParams], callback)

```javascript
import { compress } from "cppzst";

await compress(input);
```

#### decompress(buffer[, zstdDecompressParams], callback)

```javascript
import { decompress } from "cppzst";

await decompress(input);
```

### Sync

#### compressSync(buffer[, zstdCompressParams])

```javascript
const compressSync = require("cppzst").compressSync;

try {
  var output = compressSync(input);
} catch (err) {
  // ...
}
```

#### decompressSync(buffer[, zstdCompressParams])

```javascript
const decompressSync = require("cppzst").decompressSync;

try {
  var output = decompressSync(input);
} catch (err) {
  // ...
}
```

### Stream

#### compressStream([zstdCompressParams])

```javascript
const compressStream = require("cppzst").compressStream;
const fs = require("fs");

fs.createReadStream("path/to/input")
  .pipe(compressStream())
  .pipe(fs.createWriteStream("path/to/output"));
```

#### decompressStream([zstdCompressParams])

```javascript
const decompressStream = require("cppzst").decompressStream;
const fs = require("fs");

fs.createReadStream("path/to/input")
  .pipe(decompressStream())
  .pipe(fs.createWriteStream("path/to/output"));
```

### ZSTD Params

The `compress`, `compressSync` and `compressStream` methods may accept an optional `zstdCompressParams` object to define compress level and/or dict.

```javascript
const zstdCompressParams = {
  level: 5, // default 1
  dict: new Buffer("hello zstd"), // if dict null, left level only.
  dictSize: dict.length, // if dict null, left level only.
};
```

The `decompress`, `decompressSync` and `decompressStream` methods may accept an optional `zstdDecompressParams` object to define dict.

```javascript
const zdtdDecompressParams = {
  dict: new Buffer("hello zstd"),
  dictSize: dict.length,
};
```

## Tests

```sh
$ npm test
```

## License

MIT
