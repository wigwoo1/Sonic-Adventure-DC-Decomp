import json
from pathlib import Path

ROOT=Path(__file__).resolve().parent
report=json.loads((ROOT/'analysis/stage00_global_initialize_verification.json').read_text())
status_path=ROOT/'project_status.json'
status=json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['stage00_global_initialize']={
 'entry':'0x0c900e00','end_exclusive':'0x0c900e26',
 'global_address':'0x8c799788','source':'source/stage00_global_initialize.c',
 'report':'analysis/stage00_global_initialize_verification.json',
 'native_build_succeeded':True,'cases_passed':report['cases_passed'],
 'scope':'Complete 16 MiB RAM comparison against retail instructions; six-word record and surrounding bytes match.',
 'limitations':'The record semantic names and complete STG00 module code/data boundaries remain unresolved.'
}
status_path.write_text(json.dumps(status,indent=2)+'\n',encoding='utf-8')

handoff=ROOT/'SOURCE_HANDOFF.txt'
content=handoff.read_text(encoding='utf-8')
note=('STG00 global initializer: source/stage00_global_initialize.c recovers 0x0c900e00..0x0c900e25, writing six words at global 0x8c799788. '
 'The native build passed 12 comparisons against retail instruction execution; every byte of the 16 MiB RAM image matched, including all bytes surrounding the record. '
 'The record fields are still named by offset because their semantics are unknown. See verify_stage00_global_initialize.py and analysis/stage00_global_initialize_verification.json.')
if note not in content:handoff.write_text(content+'\n'+note+'\n',encoding='utf-8')
print(json.dumps({'cases_passed':report['cases_passed'],'status':'updated'}))
