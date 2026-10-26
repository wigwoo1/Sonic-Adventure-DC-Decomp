#ifndef SA_FLASH_STORAGE_H
#define SA_FLASH_STORAGE_H
#include <stdint.h>
#ifdef _WIN32
#define SA_FLASH_EXPORT __declspec(dllexport)
#else
#define SA_FLASH_EXPORT
#endif
/* Authored host storage, not recovered BIOS code. All bytes and partition
 * extents are explicit borrowed mutable inputs. No default identity, settings
 * or successful game-helper result is manufactured. No filesystem I/O occurs.
 * Single-threaded; callers retain these spans until reconfiguration.
 *
 * Selectors follow the documented flash syscall ABI:0 info,1 read,2 program,
 * 3 erase. Program rejects a requested zero-to-one change before any writes;
 * this atomic failure policy and whole-partition erase are untimed host policy,
 * not a model of physical failure timing/partial programming or protection.
 * Read-only mode returns guest -1 on program/erase. Host errors leave state,
 * RAM, flash and result unchanged. Guest errors increment the selector counter
 * and return guest -1 with host status0. Unsupported selectors return host-8.
 */
struct sa_flash_partition {uint32_t start,size;};
struct sa_flash_storage {
 uint8_t *bytes;
 uint32_t size,configured,writable,partition_count;
 struct sa_flash_partition partitions[5];
 uint64_t calls[4],read_bytes,programmed_bytes,erased_bytes;
};
SA_FLASH_EXPORT uint32_t sa_flash_storage_size(void);
SA_FLASH_EXPORT int sa_flash_storage_init(struct sa_flash_storage *);
SA_FLASH_EXPORT int sa_flash_storage_configure(struct sa_flash_storage *,uint8_t *,uint32_t,const struct sa_flash_partition *,uint32_t,uint32_t);
SA_FLASH_EXPORT int sa_flash_storage_service(struct sa_flash_storage *,uint8_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
#endif
