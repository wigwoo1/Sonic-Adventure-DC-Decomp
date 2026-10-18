import csv,hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parents[2];out=root/'outputs/sonic-adventure'
binary=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
start,end=0x8c667844,0x8c667900
with (out/'analysis/instructions/sound_stream_requests_66777c.tsv').open(newline='') as file:
    rows=[r for r in csv.DictReader(file,delimiter='\t') if start<=int(r['address'],16)<end]
assert len(rows)==94
for row in rows:
    offset=int(row['address'],16)-0x8c010000
    assert int.from_bytes(binary[offset:offset+2],'little')==int(row['opcode'],16)
assert [int(r['address'],16) for r in rows]==list(range(start,end,2))
report=dict(instructions_opcode_verified=len(rows),address=f'0x{start:08x}',end_exclusive=f'0x{end:08x}',
            retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),
            source='source/sa_sound_stream_decode_schedule.c',build='build_sound_stream_decode_schedule.ps1',
            status='Compiled draft; retail opcode inventory checked, native behavior unverified',
            dependencies=['0x8c10ca5c signed division','0x8c10cbb8 signed remainder','0x8c6677e2 mono stream request','0x8c66780e stereo stream request'],
            scope='Only exported instruction words and span completeness checked. Compilation alone does not establish correctness. No recovered-routine registry credit; full scheduling/playback/fullgame unverified.')
(out/'analysis/sound_stream_schedule_draft_audit.json').write_text(json.dumps(report,indent=2)+'\n')
path=out/'project_status.json';status=json.loads(path.read_text());status.setdefault('unverified_drafts',{})['sound_stream_decode_schedule']=report;path.write_text(json.dumps(status,indent=2)+'\n')
path=out/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Unverified stream decode scheduler draft:'
if marker not in text:path.write_text(text+'\n'+marker+' '+report['source']+'; '+report['build']+'. '+report['scope']+' Overall completion10%.\n')
print(len(rows),'retail scheduler opcodes checked; behavior remains unverified')
