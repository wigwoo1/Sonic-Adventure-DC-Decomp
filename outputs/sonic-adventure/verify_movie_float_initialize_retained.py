"""Replay the last upload/interrupt oracles in207source DLL, preserving old reports."""
import hashlib,json
from pathlib import Path
import write_watch_float_initialize as fast
P=Path(__file__).resolve().parent;v=fast.old
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 records=[];files=[Path(__file__),Path(fast.__file__),Path(v.__file__),P/'write_watch_buffer_initialize.py']
 for script,original,target,count,checks in [('verify_movie_buffer_initialize.py','movie_buffer_initialize_verification.json','movie_float_initialize_upload_regression.json',78,1793),('verify_movie_buffer_interrupt.py','movie_buffer_interrupt_verification.json','movie_float_initialize_interrupt_regression.json',23,334)]:
  parent=P/script;code=parent.read_text();assert code.count(original)==1;code=code.replace(original,target)
  ns=dict(__name__='retained_float_initialize_suite',__file__=str(Path(__file__)));exec(compile(code,str(parent),'exec'),ns);ns.update(fast=fast,v=v);ns['main']()
  path=P/'analysis'/target;r=json.loads(path.read_text());assert (r['cases_passed'],r['independent_checks'])==(count,checks) and r['dll_sha256']==digest(v.effects.DLL)
  r['sha256'][str(parent)]=digest(parent);r['scope']='Fresh207-source replay of unchanged206-stage fixture/oracle suite. No additionalroutinecredit;newinitializercoverage separatelymeasured. '+r['scope'];path.write_text(json.dumps(r,indent=2)+'\n');files.extend((parent,path));records.append(dict(script=script,cases=count,independent_checks=checks))
 result=dict(total_original_native_comparisons=101,independent_checks=2127,suites=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Fresh207-source upload andIRQ/wait regressions. Prior fullcold206-source proof retained/hashchecked,notrerun orrelabeledas207-source startup;newwarmstartup verifiedseparately. Allold source/proofbytes preserved.')
 (P/'analysis/movie_float_initialize_retained_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('FLOAT RETAINED101/2127')
if __name__=='__main__':main()
