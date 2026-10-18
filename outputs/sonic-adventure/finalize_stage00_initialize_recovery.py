import hashlib,json
from pathlib import Path

ROOT=Path(__file__).resolve().parent
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

report_path=ROOT/'analysis/stage00_initialize_recovery.json'
report=json.loads(report_path.read_text())
report['instruction_listing_sha256']=digest(ROOT/report['instruction_listing'])
report['source_sha256']=digest(ROOT/report['source'])
report_path.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')

status_path=ROOT/'project_status.json'
status=json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['stage00_initializer_recovery']={
 'entry':'0x0c900d00','end_exclusive':'0x0c900d72',
 'source':'source/stage00_initialize.c',
 'report':'analysis/stage00_initialize_recovery.json',
 'native_build_succeeded':True,
 'scope':'Compiled C recovery of STG00 initializer with nine direct call boundaries, task/work updates, and six-case comparison against 342 executed original instructions.',
 'limitations':'Called service bodies and complete volatile register/FPU side effects remain unresolved; full stage runtime and playable gameplay remain unverified.'
}
status_path.write_text(json.dumps(status,indent=2)+'\n',encoding='utf-8')
registry_path=ROOT/'analysis/recovered_routines.json'
registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
entry=next(r for r in registry['routines'] if r.get('module')=='STG00.BIN' and int(r['address'],16)==0x0c900d00)
entry['status']='native C source compiled; six cases matched direct call sequence/arguments and work records against 342 original instructions; external service bodies unresolved'
entry['verification']='analysis/stage00_initialize_recovery.json'
registry_path.write_text(json.dumps(registry,indent=2)+'\n',encoding='utf-8')

handoff=ROOT/'SOURCE_HANDOFF.txt'
content=handoff.read_text(encoding='utf-8')
old='STG00 initializer recovery: source/stage00_initialize.c recovers 0x0c900d00..0x0c900d71 from the checked-in SH-4 listing and compiles as a native DLL. The routine stores the low-byte variant, clears stage flags, configures/starts/dispatches timer services, then tail-calls 0x8c04c680 through an explicit service. See analysis/stage00_initialize_recovery.json. Service bodies and register effects, differential verification, full stage behavior, the complete game and ports remain unfinished.'
new='STG00 initializer recovery: source/stage00_initialize.c covers 0x0c900d00..0x0c900d71 and builds as a native DLL. Six comparisons exercise the full routine against 342 original SH-4 instructions, matching its direct call sequence and observed arguments plus caller/created work records. Calls 0x8c07c65e, 0x8c07c676, 0x8c60526a, 0x8c09852e, 0x8c014242, 0x8c014308, 0x8c08a07c and tail target 0x8c04c680 remain explicit boundaries; their complete implementations and register effects are not established. See analysis/stage00_initialize_recovery.json and verify_stage00_initialize.py. Full stage runtime, the complete game and ports remain unfinished.'
if old in content:content=content.replace(old,new)
elif new not in content:content+='\n'+new+'\n'
handoff.write_text(content,encoding='utf-8')
print(json.dumps({'source_sha256':report['source_sha256'],'instruction_listing_sha256':report['instruction_listing_sha256'],'status':'updated'}))
