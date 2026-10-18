import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent;root=p.parents[1]
binary=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();report='analysis/signed_division_verification.json';e=json.loads((p/report).read_text())
assert e['cases_passed']==8800
assert e['source_sha256']==hashlib.sha256((p/'source/sa_signed_division.c').read_bytes()).hexdigest()
assert e['binary_sha256']==hashlib.sha256(binary).hexdigest()
assert e['dll_sha256']==hashlib.sha256((root/'work/sonic-adventure/build/signed_division.dll').read_bytes()).hexdigest()
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());start=0x8c10ca5c;end=0x8c10cb06
assert not any(int(r['address'],16)==start and r['name']!='sa_signed_division' for r in registry['routines'])
if not any(r['name']=='sa_signed_division' for r in registry['routines']):
 registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_signed_division',address=hex(start),end_exclusive=hex(end),source='source/sa_signed_division.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope',additional_verification=['analysis/sound_subordinate_field8_set_boundaries_verification.json']))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['signed_division_registry']=e;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Signed division registry reconciliation:'
if marker not in text:path.write_text(text+'\n'+marker+' Existing native helper now independently registered with build_signed_division.ps1/verify_signed_division.py. '+e['scope']+' Caller boundary evidence includes2016 sound-object rescaling cases. This is a tracking correction,not newly recovered code. Overall project10%.\n')
