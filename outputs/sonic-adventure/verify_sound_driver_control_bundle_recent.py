import ctypes,json,re
from pathlib import Path
from verify_startup import ROOT
out=ROOT/'outputs/sonic-adventure'
original_cdll=ctypes.CDLL
bundled={'sound_driver_resource_query','sound_resource_index_get','sound_bank_section_lookup','sound_driver_state_query','sound_driver_fixed_block_transfer','sound_driver_zero_queries'}
def load(path,*args,**kwargs):
 p=Path(path)
 if p.stem in bundled:p=p.with_name('sound_driver_control_bundle.dll')
 return original_cdll(str(p),*args,**kwargs)
ctypes.CDLL=load
reports=[]
try:
 for name in ['sound_driver_resource_query_native_index','sound_driver_state_query','sound_driver_fixed_block_transfer_native_bus','sound_driver_zero_queries']:
  text=(out/('verify_'+name+'.py')).read_text()
  text=text.replace(name+'_verification.json',name+'_bundle_verification.json')
  exec(compile(text,str(out/('verify_'+name+'.py')),'exec'),{'__name__':'__main__'})
  report=json.loads((out/'analysis'/(name+'_bundle_verification.json')).read_text())
  reports.append(dict(verifier=name,**report))
finally:ctypes.CDLL=original_cdll
report=dict(cases_passed=sum(x['cases_passed'] for x in reports),checks=reports,scope='Recent recovered routines execute from the combined25-source driver library with existing native dependencies. Synthetic fixtures and MMIO; full game remains unverified.')
(out/'analysis/sound_driver_control_bundle_recent_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(report['cases_passed'],'combined driver library comparisons passed')
