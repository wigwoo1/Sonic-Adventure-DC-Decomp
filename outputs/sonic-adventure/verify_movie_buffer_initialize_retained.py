"""Fresh unchanged root/constructor/accounting oracles in the206-source DLL."""
import hashlib,json
from pathlib import Path
import write_watch_buffer_initialize as fast
P=Path(__file__).resolve().parent;ROOT=P.parents[1];v=fast.old
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__),Path(fast.__file__),Path(v.__file__),P/'analysis/movie_buffer_initialize_connection.json'];records=[]
 for name,script,count,checks in (('root_arithmetic','verify_movie_graphics_root_arithmetic.py',66,1334),('constructor','verify_movie_graphics_constructor.py',28,588),('accounting','verify_movie_graphics_handoff.py',135,2155)):
  parent=P/script;target=P/f'analysis/movie_buffer_initialize_{name}_regression.json';code=parent.read_text();old='movie_graphics_handoff_verification.json' if name=='accounting' else f'movie_graphics_{name}_verification.json';assert code.count(old)==1;code=code.replace(old,target.name)
  if name=='root_arithmetic':
   marker="body=next(b for b in v.manifest['routines'] if int(b['address'],16)==ENTRY)";assert code.count(marker)==1;code=code.replace(marker,"body=next(b for b in v.old.old.old.manifest['routines'] if int(b['address'],16)==ENTRY)")
  ns=dict(__name__='retained_buffer_suite',__file__=str(Path(__file__)));exec(compile(code,str(parent),'exec'),ns);ns.update(fast=fast,v=v)
  if name=='accounting':
   original_cases=ns['cases']
   # These two historical negative cases called A400 physically invalid. The
   # new platform now supports that aperture; the old accounting adapters still
   # deliberately reject it. Preserve their old bounds and exclude that narrow
   # unsupported adapter domain rather than changing old code/proof or CPU rules.
   ns['cases']=lambda:(c for c in original_cases() if c['account_arguments'][0]!=0xa4000000)
  ns['main']()
  report=json.loads(target.read_text());assert (report['cases_passed'],report['independent_checks'])==(count,checks) and report['dll_sha256']==digest(v.effects.DLL)
  report['sha256'][str(parent)]=digest(parent);target.write_text(json.dumps(report,indent=2)+'\n');files.extend((parent,target));records.append(dict(suite=name,comparisons=count,independent_checks=checks))
 report=dict(total_original_native_comparisons=229,independent_checks=4077,suites=records,excluded_historical_negative_cases=2,exclusion_reason='Old accounting adapters reject A400 output while new platform supports it. Their old guard/proof is preserved; that narrow adapter domain remains unsupported. No altered original CPU result or fake failure is used.',dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Fresh206-source66root/28constructor/135accounting unchangedinput/oracle replay. Twoobsoletephysical-invalidA400negativecases excluded witholdadapterguards preserved. Earlierproofs retained;otheroldtests notallrerun;no duplicatecredit.')
 (P/'analysis/movie_buffer_initialize_retained_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('RETAINED229/4077')
if __name__=='__main__':main()
