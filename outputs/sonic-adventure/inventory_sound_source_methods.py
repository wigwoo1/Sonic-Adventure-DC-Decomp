import csv,json,struct
from pathlib import Path
root=Path(__file__).resolve().parents[2];out=root/'outputs/sonic-adventure'
binary=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();base=0x8c010000
literal=0x8c65e4ac;table=struct.unpack_from('<I',binary,literal-base)[0]
assert table==0x8c67c234
with (out/'analysis/ghidra-all/function_index.tsv').open(newline='') as file:
    indexed={int(row['address'],16):row for row in csv.DictReader(file,delimiter='\t')}
entries=[]
for slot,value in enumerate(struct.unpack_from('<12I',binary,table-base)):
    entries.append(dict(slot_offset=f'0x{slot*4:x}',pointer_address=f'0x{table+slot*4:08x}',target=f'0x{value:08x}',indexed_function=indexed.get(value),role='input provider virtual query' if slot==6 else 'unresolved'))
assert int(entries[6]['target'],16)==0x8c65e4b8
report=dict(constructor='0x8c65e3c6',constructor_literal=f'0x{literal:08x}',method_table=f'0x{table:08x}',entries=entries,
            scope='Original binary pointer words verified. Draft constructor pseudocode assigns literal table to object+0 and source buffer to+1c; constructor instruction-level behavior not yet verified. Input provider instruction65b8c6 loads vtable+0x18 then calls target. This identifies query65e4b8 for this object implementation; other source implementations may use other tables. No new function recovery or whole-project percentage credit.')
(out/'analysis/sound_source_method_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
path=out/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_source_method_inventory']=report;path.write_text(json.dumps(status,indent=2)+'\n')
path=out/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Retail sound source virtual methods:'
if marker not in text:path.write_text(text+'\n'+marker+' inventory_sound_source_methods.py reproduces analysis/sound_source_method_inventory.json. '+report['scope']+' Overall completion10%.\n')
print('Original table',hex(table),'query slot+0x18 ->',entries[6]['target'],'indexed:',entries[6]['indexed_function'] is not None)
