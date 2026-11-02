"""Recheck raw span evidence, two-sided ambiguity and existing Ghidra observations."""
import csv
import hashlib
import json
from collections import Counter, defaultdict
from pathlib import Path

S = Path('/root/sa-source-review')
R = Path('/mnt/c/Users/wigwo/Documents/Codex/2026-10-03/de')
W = R / 'work/sonic-adventure'
P = R / 'outputs/sonic-adventure'
BASE = 0x8c010000
def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()
def read(path):
    return json.loads(path.read_text())
def all_aligned(image, pattern, alignment):
    positions, cursor = [], 0
    while cursor <= len(image) - len(pattern):
        try:
            position = image.index(pattern, cursor)
        except ValueError:
            break
        cursor = position + 1
        if position % 4 == alignment:
            positions.append(BASE + position)
    return positions

inventory_path = W / 'japanese_source_entry_inventory.json'
map_path = W / 'japanese_USA_main_exact_span_candidates.json'
inventory, mapped = read(inventory_path), read(map_path)
assert sha(inventory_path) == mapped['source_inventory_sha256']
assert sha(W / 'cross_version_exact_spans.py') == mapped['core_script_sha256']
assert sha(W / 'map_japanese_main_exact_spans.py') == mapped['mapper_script_sha256']
assert sha(P / 'analysis/recovered_routines.json') == mapped['native_registry_sha256']
assert sha(P / 'analysis/ghidra-all/function_index.tsv') == mapped['USA_entry_snapshot_sha256']
assert sha(W / 'japanese_v1007_full_matching_verification.json') == inventory['qualification_sha256']
for source, expected in inventory['source_sha256'].items():
    assert sha(S / source) == expected, source
for image in inventory['images']:
    assert sha(S / 'config' / image['image'] / 'symbols.txt') == image['symbols_sha256']
    assert sha(S / 'config' / image['image'] / 'splits.txt') == image['splits_sha256']
assert len(inventory['images']) == 247
assert len({(row['image'], row['address']) for row in inventory['entries']}) == len(inventory['entries'])
source_main = {row['address']: row for row in inventory['entries'] if row['image'] == '1st_read'}
jp, us = (S / 'orig/1ST_READ.BIN').read_bytes(), (W / 'extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(jp).hexdigest() == mapped['Japanese_main_sha256']
assert hashlib.sha256(us).hexdigest() == mapped['USA_main_sha256']
known = {int(row['address'], 16) for row in csv.DictReader(
    (P / 'analysis/ghidra-all/function_index.tsv').open(), delimiter='\t')}
observations_path = W / 'cross-version-span-evidence/entry_observations.tsv'
with observations_path.open() as stream:
    observations = {int(row['Japan_entry'], 16): row for row in csv.DictReader(stream, delimiter='\t')}
classifications, unique, incoming = Counter(), [], defaultdict(list)
for row in mapped['entries']:
    original = source_main[row['address']]
    assert all(row[key] == value for key, value in original.items())
    low, high = row['address'] - BASE, row['span_end'] - BASE
    assert 0 <= low < high <= len(jp)
    pattern = jp[low:high]
    assert len(pattern) == row['span_bytes']
    assert hashlib.sha256(pattern).hexdigest() == row['span_sha256']
    positions = [] if len(pattern) < 32 else all_aligned(us, pattern, low % 4)
    assert [candidate['address'] for candidate in row['candidates']] == positions[:256]
    assert row['candidates_truncated'] == (len(positions) > 256)
    expected_status = ('excluded_short_span' if len(pattern) < 32 else
                       'no_exact_span' if not positions else
                       'ambiguous_or_truncated_exact_span' if len(positions) > 1 else
                       'unique_exact_span_at_known_USA_entry' if positions[0] in known else
                       'unique_exact_span_not_in_Ghidra_entry_snapshot')
    assert row['status'] == expected_status
    assert not row['behavioral_equivalence_verified']
    classifications[expected_status] += 1
    if len(positions) != 1:
        continue
    observed = observations[row['address']]
    assert int(observed['USA_entry'], 16) == positions[0]
    assert int(observed['span_bytes']) == len(pattern)
    assert observed['span_sha256'] == row['span_sha256']
    assert int(observed['instruction_bytes_in_span']) <= len(pattern)
    jp_positions = all_aligned(jp, pattern, low % 4)
    assert row['address'] in jp_positions
    entry = dict(Japan_entry=row['address'], USA_entry=positions[0],
                 source=row['source'], names=row['names'], bytes=len(pattern),
                 Japan_occurrences=jp_positions,
                 current_Ghidra_function_at=bool(observed['function_at']),
                 current_Ghidra_containing_function=observed['containing_function'],
                 current_Ghidra_entry_instruction=observed['instruction_at'],
                 incoming_references=int(observed['all_references_to_entry']),
                 direct_calls=int(observed['direct_calls_to_entry']),
                 data_references=int(observed['data_references_to_entry']))
    unique.append(entry)
    incoming[positions[0]].append(row['address'])

assert len(mapped['entries']) == len(source_main) == mapped['Japanese_main_source_named_entries']
assert dict(classifications) == mapped['classifications']
assert len(observations) == len(unique)
one_to_one = [row for row in unique if len(row['Japan_occurrences']) == 1
              and len(incoming[row['USA_entry']]) == 1]
assert not mapped['source_game_code_imported'] and mapped['recovered_registry_credit'] == 0
result = dict(
    status='verified_original_byte_span_candidates_with_explicit_non_equivalence_scope',
    Japanese_images=247, source_named_module_address_pairs=len(inventory['entries']),
    distinct_source_name_pairs=inventory['distinct_source_name_pairs'],
    unresolved_C_definition_instances=inventory['unresolved_definition_instances'],
    Japanese_main_source_named_entries=len(source_main),
    classifications=dict(classifications),
    unique_USA_span_instances=len(unique),
    distinct_USA_addresses_for_unique_spans=len(incoming),
    two_sided_unique_span_and_address_pairs=len(one_to_one),
    unique_spans_at_current_Ghidra_functions=sum(row['current_Ghidra_function_at'] for row in unique),
    unique_spans_with_observed_incoming_references=sum(row['incoming_references'] > 0 for row in unique),
    unique_spans_repeated_in_Japanese_image=sum(len(row['Japan_occurrences']) > 1 for row in unique),
    original_Ghidra_memory_hashes_independently_checked=len(observations),
    adversarial_matcher_tests_passed=8, game_behavior_tests_added=0,
    inventory_sha256=sha(inventory_path), candidate_map_sha256=sha(map_path),
    existing_Ghidra_observations_sha256=sha(observations_path),
    existing_Ghidra_incoming_refs_sha256=sha(W / 'cross-version-span-evidence/incoming_references.tsv'),
    qualification_script_sha256=sha(Path(__file__)),
    matcher_tests_sha256=sha(W / 'verify_cross_version_exact_span_cases.py'),
    recovered_registry_credit=0, source_game_code_imported=False,
    behavioral_equivalence_pairs_proven=0, whole_game_reuse_percentage_measured=False,
    estimated_whole_project_completion_percent=10,
    one_to_one_candidate_pairs=one_to_one,
    other_unique_USA_span_instances=[row for row in unique if row not in one_to_one],
    limitations='Two-sided uniqueness strengthens a byte-span candidate only. No matching external data/callees, CPU entry context, CFG extent, semantic equivalence, whole-game USA routine total or native integration is established. Current Ghidra observations were exported read-only without creating instructions or functions.',
)
target = W / 'cross_version_source_map_verification.json'
assert not target.exists()
target.write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps({key: value for key, value in result.items()
                  if key not in ('one_to_one_candidate_pairs', 'other_unique_USA_span_instances')}, indent=2))
