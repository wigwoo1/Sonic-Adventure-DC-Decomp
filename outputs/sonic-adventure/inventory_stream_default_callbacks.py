import json,struct
from pathlib import Path
p=Path(__file__).resolve().parent;b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();base=0x8c010000
records=[]
for name,address in [('input',0x8c66747c),('output',0x8c667480)]:
    target=struct.unpack_from('<I',b,address-base)[0]
    records.append(dict(role=name,literal=hex(address),target=hex(target),first_words=[hex(struct.unpack_from('<H',b,target-base+i)[0]) for i in range(0,32,2)]))
report=dict(scope='Original pointer words associated with draft stream constructor6673b4. Constructor writes and callback instruction boundaries require verification; no completed stream construction claimed.',callbacks=records)
(p/'analysis/stream_default_callback_inventory.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
e=(p/'ghidra/export_sound_source_locks_65e368.ps1').read_text()
target=int(records[1]['target'],16)
e=e.replace('sound_source_locks_65e368.tsv','stream_default_output.tsv').replace('8c65e368 8c65e3c6',f'{target:x} {target+0x100:x}')
(p/'ghidra/export_stream_default_output.ps1').write_text(e)
