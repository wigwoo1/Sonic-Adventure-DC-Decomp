/* The two pinned JP archives use named, globally indexed non-paletted entries.
 * Remap the selected entries to the small texture lists owned by the stage. */
static uint32_t ring_archive_read(const unsigned char *p, unsigned width)
{
    uint32_t value = 0;
    for (unsigned i = 0; i < width; ++i) value |= (uint32_t)p[i] << (8*i);
    return value;
}

static unsigned char *select_ring_archive(struct resource_view view, unsigned first,
                                          unsigned count, size_t *result_bytes)
{
    enum { ROW_BYTES = 34 };
    const unsigned char *src = view.data;
    if (!src || view.bytes < 12 || memcmp(src, "PVMH", 4)) return 0;
    size_t header = ring_archive_read(src+4, 4);
    unsigned flags = ring_archive_read(src+8, 2);
    unsigned entries = ring_archive_read(src+10, 2);
    unsigned stride = ROW_BYTES + ((flags & 2) ? 2 : 0) + ((flags & 4) ? 2 : 0);
    if (!count || count > STAGE_SPARKLE_TEXTURES ||
        (flags & ~0x106u) != 9 || header < 4 || header > view.bytes-8 ||
        entries > (header-4)/stride || first+count > entries) return 0;

    size_t offsets[STAGE_SPARKLE_TEXTURES], sizes[STAGE_SPARKLE_TEXTURES];
    size_t total = 12 + count*ROW_BYTES, pos = 8 + header;
    for (unsigned i = 0; i < first+count;) {
        if (view.bytes-pos < 8) return 0;
        size_t bytes = ring_archive_read(src+pos+4, 4);
        if (bytes > view.bytes-pos-8 || !memcmp(src+pos, "PVPL", 4)) return 0;
        if (!memcmp(src+pos, "PVRT", 4)) {
            if (bytes < 8) return 0;
            if (i >= first) {
                offsets[i-first] = pos;
                sizes[i-first] = bytes+8;
                if (sizes[i-first] > SIZE_MAX-total) return 0;
                total += sizes[i-first];
            }
            ++i;
        }
        pos += bytes+8;
    }

    unsigned char *out = calloc(1, total);
    if (!out) return 0;
    uint32_t out_header = 4 + count*ROW_BYTES;
    memcpy(out, "PVMH", 4);
    memcpy(out+4, &out_header, sizeof out_header);
    out[8] = 9;
    out[10] = (unsigned char)count;
    pos = 8 + out_header;
    unsigned seen = 0;
    for (unsigned i = 0; i < entries; ++i) {
        const unsigned char *row = src+12+i*stride;
        unsigned number = ring_archive_read(row, 2);
        if (number < first || number >= first+count) continue;
        unsigned slot = number-first;
        if (seen & (1u << slot)) {
            free(out);
            return 0;
        }
        seen |= 1u << slot;
        unsigned char *dest = out+12+slot*ROW_BYTES;
        dest[0] = (unsigned char)slot;
        memcpy(dest+2, row+2, STAGE_TEXTURE_NAME_BYTES);
        memcpy(dest+30, row+stride-4, 4);
    }
    if (seen != (1u << count)-1) {
        free(out);
        return 0;
    }
    for (unsigned i = 0; i < count; ++i) {
        memcpy(out+pos, src+offsets[i], sizes[i]);
        pos += sizes[i];
    }
    *result_bytes = total;
    return out;
}

__declspec(dllexport) int jp_emerald_stage_ring_textures_open_disc(void)
{
    struct resource_view ring, sparkle;
    if (emerald_stage_stepping || stage_ring_textures_ready ||
        !(stage_session & STAGE_SET) || jp_stage_resources_get(8, &ring) ||
        jp_stage_resources_get(9, &sparkle)) return -1;
    size_t ring_bytes, sparkle_bytes;
    unsigned char *ring_pvm = select_ring_archive(ring, 50, 1, &ring_bytes);
    unsigned char *sparkle_pvm = select_ring_archive(sparkle, 0, STAGE_SPARKLE_TEXTURES,
                                                     &sparkle_bytes);
    int result = -1;
    if (ring_pvm && sparkle_pvm)
        result = jp_emerald_stage_ring_textures_open(ring_pvm, ring_bytes,
                                                     sparkle_pvm, sparkle_bytes);
    free(sparkle_pvm);
    free(ring_pvm);
    return result;
}
