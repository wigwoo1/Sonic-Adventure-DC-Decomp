import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parent
report=json.loads((ROOT/'analysis'/'bounded_array_store_8c646eee_verification.json').read_text())
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes()
registry_path=ROOT/'analysis'/'recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest()==registry['binary_sha256']
start,end=int(report['address'],16),int(report['end_exclusive'],16);offset=start-int(report['load_address'],16)
assert hashlib.sha256(binary[offset:offset+end-start]).hexdigest()==report['retail_span_sha256']
source=ROOT/'source'/'sa_bounded_array_store_8c646eee.c';assert hashlib.sha256(source.read_bytes()).hexdigest()==report['source_sha256']
routine={'module':report['module'],'load_address':report['load_address'],'name':'sa_bounded_array_store_8c646eee','address':report['address'],'end_exclusive':report['end_exclusive'],'retail_span_sha256':report['retail_span_sha256'],'source':'source/sa_bounded_array_store_8c646eee.c','verification':'analysis/bounded_array_store_8c646eee_verification.json','status':f'Retail bounded array store matched in {report["cases_passed"]} full RAM/CPU cases','comparison_level':report['comparison_level'],'limitations':report['limitations']}
found=next((r for r in registry['routines'] if r.get('module')==routine['module'] and int(r['address'],16)==start),None)
if found is None:registry['routines'].append(routine)
else:found.update(routine)
registry_path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=ROOT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
progress=status.setdefault('verified_progress',{});progress[routine['name']]={'verification':routine['verification'],'cases':report['cases_passed'],'comparison_level':report['comparison_level'],'retail_span_sha256':report['retail_span_sha256'],'source_sha256':report['source_sha256'],'scope':report['scope'],'limitations':report['limitations']}
progress['recovered_buildable_routines']=len(registry['routines']);status_path.write_text(json.dumps(status,indent=2)+'\n')
print(f'Registered bounded array store at {report["address"]}; {len(registry["routines"])} routines total')
