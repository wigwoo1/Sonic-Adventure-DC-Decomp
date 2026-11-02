"""Make strict, provisional cross-region byte-span candidates from original inputs."""
import csv
import hashlib
import json
import subprocess
from collections import Counter
from pathlib import Path
from cross_version_exact_spans import exact_offsets

S = Path('/root/sa-source-review')
R = Path('/mnt/c/Users/wigwo/Documents/Codex/2026-10-03/de')
W = R / 'work/sonic-adventure'
P = R / 'outputs/sonic-adventure'
BASE = 0x8c010000
def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

assert Path.cwd() == S
inventory_path = W / 'japanese_source_entry_inventory.json'
inventory = json.loads(inventory_path.read_text())
assert inventory['upstream_commit'] == subprocess.check_output(['git', 'rev-parse', 'HEAD'], text=True).strip()
jp_path = S / 'orig/1ST_READ.BIN'
us_path = W / 'extracted/1ST_READ.BIN'
jp, us = jp_path.read_bytes(), us_path.read_bytes()
assert hashlib.sha256(jp).hexdigest() == 'e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
assert hashlib.sha256(us).hexdigest() == '45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
known_path = P / 'analysis/ghidra-all/function_index.tsv'
with known_path.open() as stream:
    known = {int(row['address'], 16): row for row in csv.DictReader(stream, delimiter='\t')}
registry_path = P / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text())
registered = {int(row['address'], 16) for row in registry['routines']
              if row.get('module', '1ST_READ.BIN') == '1ST_READ.BIN'}
rows = []
for entry in inventory['entries']:
    if entry['image'] != '1st_read':
        continue
    start, end = entry['address'], entry['span_end']
    assert BASE <= start < end <= BASE + len(jp)
    pattern = jp[start - BASE:end - BASE]
    result = exact_offsets(us, pattern, start)
    offsets = result['offsets']
    candidates = [dict(address=BASE + offset,
                       known_Ghidra_entry=BASE + offset in known,
                       registered_native_entry=BASE + offset in registered)
                  for offset in offsets]
    status = ('excluded_short_span' if result['excluded'] else
              'no_exact_span' if not offsets else
              'ambiguous_or_truncated_exact_span' if len(offsets) != 1 or result['truncated'] else
              'unique_exact_span_at_known_USA_entry' if candidates[0]['known_Ghidra_entry'] else
              'unique_exact_span_not_in_Ghidra_entry_snapshot')
    rows.append(dict(entry, span_sha256=hashlib.sha256(pattern).hexdigest(),
                     status=status, candidates=candidates,
                     candidates_truncated=result['truncated'],
                     behavioral_equivalence_verified=False))
counts = dict(Counter(row['status'] for row in rows))
result = dict(
    status='strict_original_byte_span_candidates_not_semantic_equivalence',
    Japanese_source_commit=inventory['upstream_commit'],
    Japanese_main_sha256=hashlib.sha256(jp).hexdigest(),
    USA_main_sha256=hashlib.sha256(us).hexdigest(),
    Japanese_main_source_named_entries=len(rows), classifications=counts,
    minimum_span_bytes=32, match_alignment_modulus=4, relocation_masking=False,
    source_inventory_sha256=sha(inventory_path),
    USA_entry_snapshot_sha256=sha(known_path),
    native_registry_sha256=sha(registry_path),
    core_script_sha256=sha(W / 'cross_version_exact_spans.py'),
    mapper_script_sha256=sha(Path(__file__)),
    entries=rows, recovered_registry_credit=0, source_game_code_imported=False,
    estimated_whole_project_completion_percent=10,
    limitations='Every match compares a whole contiguous source-anchored span, including any pools/gaps in that span, without masking constants or addresses. Span ends are placement/source-entry bounds, not proven CFG extents. A known Ghidra entry is a prior analysis observation. Identical bytes do not prove matching referenced data, external calls, CPU context, behavior, or USA source portability. Ambiguous matches are retained, not chosen.',
)
target = W / 'japanese_USA_main_exact_span_candidates.json'
assert not target.exists()
target.write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps({key: value for key, value in result.items() if key != 'entries'}, indent=2))
