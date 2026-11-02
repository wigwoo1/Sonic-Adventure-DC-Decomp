"""Inventory source-named Japanese entries; do not infer semantic equivalence.

Run in the independently qualified, pinned upstream Linux checkout. The upstream
MIT tools parse C/assembly declarations and placement tables. No game C body or
retail payload is copied into this metadata output.
"""
import hashlib
import json
import subprocess
import sys
from collections import Counter, defaultdict
from functools import lru_cache
from pathlib import Path

S = Path('/root/sa-source-review')
W = Path('/mnt/c/Users/wigwo/Documents/Codex/2026-10-03/de/work/sonic-adventure')
assert Path.cwd() == S
sys.path.insert(0, str(S / 'tools'))
import csource
import tags
from config import Config
from images import binaries, files
from tables import named

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

PIN = '25a3d3154dbe657325e97dbe8c5291086f6af5c0'
assert subprocess.check_output(['git', 'rev-parse', 'HEAD'], text=True).strip() == PIN
assert not subprocess.check_output(['git', 'status', '--porcelain'], text=True).strip()
qualified = W / 'japanese_v1007_full_matching_verification.json'
q = json.loads(qualified.read_text())
assert q['upstream_commit'] == PIN and q['all_image_count'] == 247
image_hashes = {row['image']: row for row in q['images']}
image_paths = files()

@lru_cache(maxsize=None)
def declarations(source):
    path = Path(source)
    if path.suffix == '.c':
        text = csource.code(path, csource.SHC)
        result = [(block.name, block.opening.line, 'C_definition')
                  for block in csource.parse(text).blocks if block.name]
        # Retain the small declaration records, not token trees for every C.
        csource.read.cache_clear()
        csource.derived.cache_clear()
        csource.MADE.clear()
        return result
    if path.suffix == '.src':
        lines = path.read_text(encoding='latin-1').splitlines()
        return [(tags.symbol(tags.LABEL.match(lines[line])[1]), line + 1,
                 'assembly_code_label_run')
                for run in tags.starts(lines) for line in run]
    return []

rows, unresolved, images = [], [], []
source_hashes = {}
for image in binaries():
    config = Config(image)
    symbols = dict((name, address) for address, name in named(config.path / 'symbols.txt'))
    original = S / 'orig' / image_paths[image]
    assert sha(original) == image_hashes[image]['sha256']
    per_address = {}
    for source in config.sources():
        code_ranges = [(section, start, end) for section, start, end in config.ranges(source)
                       if section.startswith('P')]
        if not code_ranges or Path(source).suffix not in ('.c', '.src'):
            continue
        source_hashes.setdefault(source, sha(Path(source)))
        for name, line, kind in declarations(source):
            address = symbols.get(name)
            placement = next(((section, start, end) for section, start, end in code_ranges
                              if address is not None and start <= address < end), None)
            if placement is None:
                # Includes inlined/unemitted functions and local assembly labels.
                if kind == 'C_definition':
                    unresolved.append(dict(image=image, source=source, name=name,
                                           line=line, reason='no_own_code_symbol_in_placement'))
                continue
            section, low, high = placement
            row = per_address.setdefault(address, dict(
                image=image, disc_path=image_paths[image], address=address,
                source=source, names=[], declarations=[], section=section,
                placement_start=low, placement_end=high))
            assert row['source'] == source, (image, address, source, row['source'])
            if name not in row['names']:
                row['names'].append(name)
                row['declarations'].append(dict(name=name, line=line, kind=kind))
    ordered = sorted(per_address.values(), key=lambda row: row['address'])
    for row in ordered:
        later = [other['address'] for other in ordered
                 if other['source'] == row['source'] and other['section'] == row['section']
                 and row['address'] < other['address'] < row['placement_end']]
        row['span_end'] = min(later, default=row['placement_end'])
        row['span_bytes'] = row['span_end'] - row['address']
        row['span_kind'] = 'entry_to_next_source_named_entry_or_placement_end_not_CFG_extent'
        row['names'].sort()
        rows.append(row)
    images.append(dict(image=image, entries=len(ordered),
                       original_sha256=image_hashes[image]['sha256'],
                       symbols_sha256=sha(config.path / 'symbols.txt'),
                       splits_sha256=sha(config.path / 'splits.txt')))
    print(f'{image}: {len(ordered)} source-named entries', flush=True)

assert len(images) == 247
assert len({(row['image'], row['address']) for row in rows}) == len(rows)
assert all(row['span_bytes'] > 0 and not row['address'] & 1 for row in rows)
implementations = {(row['source'], name) for row in rows for name in row['names']}
result = dict(
    status='source_anchored_entry_inventory_not_a_USA_routine_recovery_claim',
    upstream_commit=PIN, Japanese_images=len(images),
    source_named_module_address_pairs=len(rows),
    distinct_source_name_pairs=len(implementations),
    entries_by_source_family=dict(Counter(row['source'].split('/')[0] for row in rows)),
    qualification_sha256=sha(qualified), source_sha256=source_hashes,
    images=images, entries=rows,
    C_definitions_without_own_placed_symbol=unresolved,
    unresolved_definition_instances=len(unresolved),
    game_code_copied=False, recovered_registry_credit=0,
    estimated_whole_project_completion_percent=10,
    limitations='Source-named entry instances include shared implementations linked into several images and aliases grouped at one address. They are not a proven total routine count for either region. Span ends are source-placement bounds, may contain pools/gaps, and are not CFG/function extents. Functions without own placed symbols are recorded separately.',
)
target = W / 'japanese_source_entry_inventory.json'
assert not target.exists()
target.write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps({key: value for key, value in result.items()
                  if key not in ('source_sha256', 'images', 'entries',
                                 'C_definitions_without_own_placed_symbol')}, indent=2))
