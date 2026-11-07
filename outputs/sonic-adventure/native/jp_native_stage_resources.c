#include <malloc.h>
#include <stdlib.h>
#include <windows.h>

#define JP_READ_QUEUE_API __declspec(dllimport)
#include "jp_native_read_queue.h"

#include "../third_party/prskit/prskit.h"

struct resource_view {
    const void *data;
    size_t bytes;
};

static const char *const names[] = {"STG01.PRS", "BEACH01.PRS"};
static struct resource_view decoded[2];
static void *input[2];
static DWORD owner_thread;
static int ready, pending;

enum { INPUT_CAPACITY = 8 << 20, OUTPUT_LIMIT = 32 << 20 };

static void release(void)
{
    for (unsigned i = 0; i < 2; ++i) {
        _aligned_free(input[i]);
        input[i] = NULL;
        free((void *)decoded[i].data);
        decoded[i] = (struct resource_view){0};
    }
    owner_thread = 0;
    ready = pending = 0;
}

/* Close after all borrowers have retired. Keep DMA destinations alive when
 * cancellation fails so the game thread can retry cleanup. */
__declspec(dllexport) int jp_stage_resources_close(void)
{
    if (!owner_thread) return 0;
    if (owner_thread != GetCurrentThreadId()) return -1;
    if (pending && jp_read_queue_cancel()) return -1;
    release();
    return 0;
}

static int decode(unsigned i, size_t bytes)
{
    size_t size = 0, end = 0;
    int result = prskit_decompress(input[i], bytes, NULL, 0, &size, &end);
    if ((result != PRSKIT_OK && result != PRSKIT_TRAILING) || !size || size > OUTPUT_LIMIT)
        return -1;
    void *output = malloc(size);
    if (!output) return -1;
    size_t written = 0, consumed = 0;
    if (prskit_decompress(input[i], bytes, output, size, &written, &consumed) != result ||
        written != size || consumed != end) {
        free(output);
        return -1;
    }
    decoded[i] = (struct resource_view){output, size};
    return 0;
}

/* Synchronous stage-start boundary on the queue's game thread. The empty
 * queue precondition makes rollback exclusive to this batch. */
__declspec(dllexport) int jp_stage_resources_open(int stage)
{
    struct jp_read_queue_info before, current;
    size_t bytes[2] = {0};
    if (stage != 1 || owner_thread || jp_read_queue_snapshot(&before) ||
        !before.initialized || before.queued || before.status)
        return -1;

    owner_thread = GetCurrentThreadId();
    for (unsigned i = 0; i < 2; ++i) {
        input[i] = _aligned_malloc(INPUT_CAPACITY, 32);
        if (!input[i]) goto failed;
        pending = 1;
        if (jp_read_queue_add(names[i], input[i], INPUT_CAPACITY)) goto failed;
        for (unsigned slot = 0; slot < 16; ++slot) {
            struct jp_read_queue_slot entry;
            if (jp_read_queue_slot_snapshot(slot, &entry)) goto failed;
            if (entry.state && entry.destination == (uintptr_t)input[i]) {
                if (!entry.file_bytes || entry.file_bytes > INPUT_CAPACITY) goto failed;
                bytes[i] = (size_t)entry.file_bytes;
                break;
            }
        }
        if (!bytes[i]) goto failed;
    }
    for (unsigned poll = 0; poll < 100000; ++poll) {
        if (jp_read_queue_serve() || jp_read_queue_snapshot(&current)) goto failed;
        if (!current.queued) {
            pending = 0;
            if (current.completed != before.completed + 2 ||
                decode(0, bytes[0]) || decode(1, bytes[1])) goto failed;
            for (unsigned i = 0; i < 2; ++i) {
                _aligned_free(input[i]);
                input[i] = NULL;
            }
            ready = 1;
            return 0;
        }
    }
failed:
    (void)jp_stage_resources_close();
    return -1;
}

__declspec(dllexport) int jp_stage_resources_get(unsigned index, struct resource_view *out)
{
    if (!ready || owner_thread != GetCurrentThreadId() || index >= 2 || !out)
        return -1;
    *out = decoded[index];
    return 0;
}
