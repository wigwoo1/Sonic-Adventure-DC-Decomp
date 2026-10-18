import hashlib,json
from pathlib import Path

ROOT=Path(__file__).resolve().parent
binary=(ROOT.parent.parent/'work/sonic-adventure/decoded/SONICADV/STG00.BIN').read_bytes()
start,end=0x0c900d00,0x0c900d72
routine={
 'module':'STG00.BIN','name':'sa_stage00_initialize_0c900d00',
 'address':hex(start),'end_exclusive':hex(end),
 'retail_span_sha256':hashlib.sha256(binary[start-0x0c900000:end-0x0c900000]).hexdigest(),
 'source':'source/stage00_initialize.c',
 'verification':'analysis/stage00_initialize_recovery.json',
 'status':'native C source compiled; call-boundary comparison report at analysis/stage00_initialize_recovery.json; external service bodies unresolved'
}
path=ROOT/'analysis/recovered_routines.json'
registry=json.loads(path.read_text(encoding='utf-8-sig'))
existing=next((r for r in registry['routines'] if r.get('module')==routine['module'] and int(r['address'],16)==start),None)
if existing is None:registry['routines'].append(routine)
else:existing.update(routine)
path.write_text(json.dumps(registry,indent=2)+'\n',encoding='utf-8')
print(json.dumps(routine,indent=2))
