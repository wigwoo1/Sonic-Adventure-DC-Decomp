"""Replay old leaf/upload/IRQ/flash oracles in the corrected209-stage integration."""
import hashlib,json,types
from pathlib import Path
import write_watch_system_state as fast
P=Path(__file__).resolve().parent;v=fast.old
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 records=[];files=[Path(__file__),Path(fast.__file__),Path(v.__file__),P/'analysis/movie_system_state_connection.json']
 suites=[('float','verify_movie_float_initialize.py','movie_float_initialize_verification.json','movie_system_state_float_regression.json',128,2240),('upload','verify_movie_buffer_initialize.py','movie_buffer_initialize_verification.json','movie_system_state_upload_regression.json',78,1793),('interrupt','verify_movie_buffer_interrupt.py','movie_buffer_interrupt_verification.json','movie_system_state_interrupt_regression.json',23,334),('flash','verify_movie_packet_flash.py','movie_packet_flash_verification.json','movie_system_state_flash_regression.json',None,None)]
 for name,script,original,target,count,checks in suites:
  parent=P/script;code=parent.read_text();assert code.count(original)==1;code=code.replace(original,target).replace('analysis/movie_float_initialize_connection.json','analysis/movie_system_state_connection.json')
  if name=='flash':
   marker="copy=next(r for r in coverage_rows if r['address']=='0x8c603c08');assert copy['static_rows']==copy['covered_rows']==14 and copy['covered_outcomes']==copy['conditional_outcomes']==4";assert code.count(marker)==1
   code=code.replace(marker,'# The excluded word-copy grid receives no fresh complete-coverage claim.').replace('compiled_source_count=196','compiled_source_count=209')
  ns=dict(__name__='system_state_retained_suite',__file__=str(Path(__file__)));exec(compile(code,str(parent),'exec'),ns)
  view=v
  if name=='float':
   # The unchanged float oracle measures its original two-body manifest; the
   # actual observer/model and DLL remain the new209-stage integration.
   view=types.SimpleNamespace(**v.__dict__);view.manifest=v.old.manifest;view.MP=v.old.MP
  ns.update(fast=fast,v=view)
  if name=='flash':
   original_cases=ns['cases'];ns['cases']=lambda:(c for c in original_cases() if c['entry']==0x8c64bddc)
  ns['main']();path=P/'analysis'/target;r=json.loads(path.read_text());assert r['dll_sha256']==digest(v.effects.DLL)
  if count is not None:assert (r['cases_passed'],r['independent_checks'])==(count,checks),(name,r['cases_passed'],r['independent_checks'])
  r['sha256'][str(parent)]=digest(parent);r['sha256'][str(P/'analysis/movie_system_state_connection.json')]=digest(P/'analysis/movie_system_state_connection.json');r['scope']='Fresh209-stage '+name+' replay of existing original fixtures/oracles with correctedSYSINFO_INIT firmware status. Oldsource/reports unchanged;no additionalcanonicalcredit. '+r['scope'];path.write_text(json.dumps(r,indent=2)+'\n');files.extend((parent,path));records.append(dict(suite=name,cases=r['cases_passed'],independent_checks=r['independent_checks']))
 result=dict(total_original_native_comparisons=sum(r['cases'] for r in records),independent_checks=sum(r['independent_checks'] for r in records),suites=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Fresh209-stage floatinitializer/canonicalsetter,upload/IRQ/wait,andrealflashsettingsprovider oracles. SYSINFOsuccessnowreturnsdocumentedzero;no gamehelperreturns injected. Historicalproofs retainedunderoldpolicy;notalloldtests rerun. Bothfresh209startups verifiedseparately.')
 (P/'analysis/movie_system_state_retained_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('SYSTEM RETAINED',result['total_original_native_comparisons'],result['independent_checks'])
if __name__=='__main__':main()
