"""Replay existing settings/float/upload/interrupt/flash oracles in the210 DLL."""
import hashlib,json,types
from pathlib import Path
import write_watch_slot_initialize as fast
P=Path(__file__).resolve().parent;v=fast.old
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 records=[];files=[Path(__file__),Path(fast.__file__),Path(v.__file__),P/'analysis/movie_slot_initialize_connection.json']
 suites=[('system','verify_movie_system_state.py','movie_system_state_verification.json',121,2009),('float','verify_movie_float_initialize.py','movie_float_initialize_verification.json',128,2240),('upload','verify_movie_buffer_initialize.py','movie_buffer_initialize_verification.json',78,1793),('interrupt','verify_movie_buffer_interrupt.py','movie_buffer_interrupt_verification.json',23,334),('flash','verify_movie_packet_flash.py','movie_packet_flash_verification.json',60,472)]
 for name,script,original,count,checks in suites:
  parent=P/script;target=f'movie_slot_initialize_{name}_regression.json';code=parent.read_text();assert code.count(original)==1;code=code.replace(original,target).replace('analysis/movie_float_initialize_connection.json','analysis/movie_slot_initialize_connection.json').replace('analysis/movie_system_state_connection.json','analysis/movie_slot_initialize_connection.json')
  if name=='flash':
   marker="copy=next(r for r in coverage_rows if r['address']=='0x8c603c08');assert copy['static_rows']==copy['covered_rows']==14 and copy['covered_outcomes']==copy['conditional_outcomes']==4";assert code.count(marker)==1
   code=code.replace(marker,'# Excluded word-copy grid receives no fresh complete-coverage claim.').replace('compiled_source_count=196','compiled_source_count=210')
  ns=dict(__name__='slot_retained_suite',__file__=str(Path(__file__)));exec(compile(code,str(parent),'exec'),ns)
  view=v
  if name in ('float','system'):
   view=types.SimpleNamespace(**v.__dict__);ancestor=v.old if name=='system' else v.old.old;view.manifest=ancestor.manifest;view.MP=ancestor.MP
  ns.update(fast=fast,v=view)
  if name=='flash':
   original_cases=ns['cases'];ns['cases']=lambda:(c for c in original_cases() if c['entry']==0x8c64bddc)
  ns['main']();path=P/'analysis'/target;r=json.loads(path.read_text());assert r['dll_sha256']==digest(v.effects.DLL)
  assert (r['cases_passed'],r['independent_checks'])==(count,checks),(name,r['cases_passed'],r['independent_checks'])
  r['sha256'][str(parent)]=digest(parent);r['sha256'][str(P/'analysis/movie_slot_initialize_connection.json')]=digest(P/'analysis/movie_slot_initialize_connection.json');r['scope']='Fresh210-stage '+name+' replay of existing original fixtures/oracles. Source/proofs unchanged. '+r['scope'];path.write_text(json.dumps(r,indent=2)+'\n');files.extend((parent,path));records.append(dict(suite=name,cases=count,independent_checks=checks))
 result=dict(total_original_native_comparisons=sum(r['cases'] for r in records),independent_checks=sum(r['independent_checks'] for r in records),suites=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Fresh210-stage unchanged settings/float/upload/IRQ/flash-provider oracles. Historical source/proofs retained, not all older cases rerun; no additional canonical credit. Full fresh startup is verified separately.')
 (P/'analysis/movie_slot_initialize_retained_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('SLOT RETAINED',result['total_original_native_comparisons'],result['independent_checks'])
if __name__=='__main__':main()
