#include <napi.h>
#include "allocator.h"

namespace ZSTD_NODE {

  void* Allocator::Alloc(void* opaque, size_t size) {
    return static_cast<Allocator*>(opaque)->Alloc(size);
  }

  void* Allocator::Alloc(size_t size) {
    void* realbuffer = malloc(size + sizeof(AllocatedBuffer));
    AllocatedBuffer* buf = static_cast<AllocatedBuffer*>(realbuffer);
    if (!buf) {
      return NULL;
    }
    buf->size = size;
    buf->available = size;
    allocated_unreported_memory += size + sizeof(*buf);
    return static_cast<void*>(buf + 1);
  }

  Allocator::AllocatedBuffer* Allocator::GetBufferInfo(void* address) {
    return static_cast<AllocatedBuffer*>(address) - 1;
  }

  void Allocator::Free(void* opaque, void* address) {
    if (!address)
      return;

    AllocatedBuffer* buf = GetBufferInfo(address);
    Allocator* alloc = static_cast<Allocator*>(opaque);
    alloc->allocated_unreported_memory -= buf->size + sizeof(*buf);

    free(buf);
  }

  void Allocator::Free(void* address) {
    Free(this, address);
  }

  void Allocator::ReportMemoryToV8(const Napi::Env& env) {
    Napi::MemoryManagement::AdjustExternalMemory(env, allocated_unreported_memory);
    allocated_unreported_memory = 0;
  }

}
