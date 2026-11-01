"""Check draft provenance and syntax only; do not award routine recovery credit."""
import ast
import csv
import hashlib
import json
import subprocess
from pathlib import Path

P = Path(__file__).resolve().parent
R = P.parents[1]
CLANG = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe')

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

translation = P/'analysis/player_work_initialize_translation.json'
manifest = json.loads(translation.read_text())
assert manifest['newly_registered_entries'] == 0
assert manifest['instruction_rows'] == 281
assert len(manifest['routines']) == 1
for path, expected in manifest['sha256'].items():
    assert sha(path) == expected, path
original = R/'work/sonic-adventure/extracted/1ST_READ.BIN'
assert sha(original) == '45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
raw = original.read_bytes()
listing = P/'analysis/player-work-initialize/8c06c350_instructions.tsv'
rows = list(csv.DictReader(listing.open(), delimiter='\t'))
assert len(rows) == 281
for row in rows:
    address = int(row['address'], 16)
    data = bytes.fromhex(row['bytes'])
    assert len(data) == int(row['length']) == 2
    offset = address - 0x8c010000
    assert raw[offset:offset+len(data)] == data, row['address']
calls = {hex(int(row['address'], 16)+4) for row in rows
         if row['instruction'].lstrip('_').startswith(('jsr ', 'bsr '))}
returns = json.loads((P/'analysis/player_work_initialize_returns.json').read_text())
assert calls == set(returns['original_call_returns'])
assert calls <= set(returns['allowed_call_returns'])
scripts = sorted(P.glob('*player_work_initialize*.py'))
for script in scripts:
    ast.parse(script.read_text(encoding='utf-8-sig'), filename=str(script))
source = P/'source/sa_player_work_initialize.c'
flags = ['-fsyntax-only', '-Wall', '-Wextra', '-Werror']
subprocess.run([str(CLANG), *flags, str(source)], check=True)
tracked = [*scripts, translation, listing, source,
           P/'analysis/player_work_initialize_returns.json',
           P/'analysis/player-work-initialize/diagnostics.tsv',
           P/'analysis/player-work-initialize/8c06c350_draft.c',
           P/'ghidra/export_player_work_initialize.ps1']
result = dict(status='draft_provenance_and_syntax_passed',
              original_entry='0x8c06c350', original_instruction_rows=len(rows),
              original_instruction_bytes=sum(int(x['length']) for x in rows),
              original_call_returns=len(calls), python_scripts_parsed=len(scripts),
              compiler_flags=flags, compiler_exit_code=0,
              linked_build_performed=False, differential_execution_performed=False,
              new_registered_routines=0, overall_completion_percent=10,
              limitations='Draft only. Bridge/build generation and full constructor execution remain unverified. No caller allocation/alias contract or gameplay qualification is claimed.',
              sha256={str(path):sha(path) for path in tracked})
(P/'analysis/player_work_initialize_draft_snapshot_checks.json').write_text(json.dumps(result, indent=2)+'\n')
print(json.dumps({k:v for k,v in result.items() if k != 'sha256'}, indent=2))
