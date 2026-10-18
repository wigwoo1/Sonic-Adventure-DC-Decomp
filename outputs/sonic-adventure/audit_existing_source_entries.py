"""Locate existing SH4 source entries before selecting a recovery candidate.

This is a discovery inventory, not verification or completion evidence.
"""
import json
import re
from pathlib import Path

OUT = Path(__file__).resolve().parent

def main():
    entries = {}
    for path in sorted((OUT / 'source').glob('*.c')):
        source = path.read_text(encoding='utf-8-sig')
        # An entry is the first explicit jump after the exported function starts.
        for match in re.finditer(r'SA_STAGE_EXPORT int (\w+)\([^\n]*\)\{(.*?)(?=SA_STAGE_EXPORT|\Z)', source, re.S):
            entry = re.search(r'goto L([0-9a-f]{8});', match[2])
            if entry:
                entries.setdefault(entry[1], []).append({'symbol': match[1], 'source': str(path.relative_to(OUT)).replace('\\', '/')})
    report = {'scope': 'Explicit entry jumps in C sources; may omit handwritten implementations. Addresses require module identification. No verification or completion credit.', 'entries': entries, 'multiple_sources_at_same_address': {a: rows for a, rows in entries.items() if len(rows) > 1}}
    (OUT / 'analysis/existing_source_entries.json').write_text(json.dumps(report, indent=2) + '\n')
    print(len(entries), 'entry addresses;', len(report['multiple_sources_at_same_address']), 'addresses with multiple sources')

if __name__ == '__main__':
    main()
