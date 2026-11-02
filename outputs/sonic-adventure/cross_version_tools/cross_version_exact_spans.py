"""Conservative byte-span candidates, without relocation masking or equivalence claims."""
def exact_offsets(image, pattern, source_address, minimum_bytes=32, limit=256):
    if len(pattern) < minimum_bytes:
        return dict(offsets=[], truncated=False, excluded='span_shorter_than_minimum')
    if source_address & 1 or len(pattern) & 1:
        raise ValueError('SH-4 source spans must start and end on halfwords')
    if limit < 2:
        raise ValueError('At least two candidates must be retained to detect ambiguity')
    found, at = [], 0
    while True:
        offset = image.find(pattern, at)
        if offset < 0:
            return dict(offsets=found, truncated=False, excluded=None)
        at = offset + 1
        # Matching mod 4 avoids silently changing PC-aligned literal addressing.
        if offset & 3 != source_address & 3:
            continue
        if len(found) == limit:
            return dict(offsets=found, truncated=True, excluded=None)
        found.append(offset)

