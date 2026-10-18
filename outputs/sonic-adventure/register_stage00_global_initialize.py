import hashlib,json
from pathlib import Path

ROOT=Path(__file__).resolve().parent
binary=(ROOT.parent.parent/'work/sonic-adventure/decoded/SONICADV/STG00.BIN').read_bytes()
start,end=0x0c900e00,0x0c900e26
routine={'module':'STG00.BIN','name':'sa_stage00_global_initialize_0c900e00',
 'address':hex(start),'end_exclusive':hex(end),
 'retail_span_sha256':hashlib.sha256(binary[start-0x0c900000:end-0x0c900000]).hexdigest(),
 'source':'source/stage00_global_initialize.c',
 'verification':'analysis/stage00_global_initialize_verification.json',
 'status':'native source matches full RAM effects in 12 cases; six-word record meaning unknown'}
path=ROOT/'analysis/recovered_routines.json'
registry=json.loads(path.read_text(encoding='utf-8-sig'))
existing=next((r for r in registry['routines'] if r.get('module')==routine['module'] and int(r['address'],16)==start),None)
if existing is None:registry['routines'].append(routine)
else:existing.update(routine)
path.write_text(json.dumps(registry,indent=2)+'\n',encoding='utf-8')
print(json.dumps(routine,indent=2))
