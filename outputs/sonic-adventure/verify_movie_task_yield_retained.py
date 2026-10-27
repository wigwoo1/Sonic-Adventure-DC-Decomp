"""Scoped context/SR/FD controls in217 DLL; retain historical full cold proof."""
import hashlib,json,types
from pathlib import Path
import write_watch_task_yield as fast
P=Path(__file__).resolve().parent;v=fast.old
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__),Path(fast.__file__),Path(v.__file__),P/'analysis/movie_task_yield_connection.json'];records=[]
 parent=P/'verify_record_context_vectors.py';target='movie_task_yield_context_regression.json';code=parent.read_text().replace('movie_record_context_vectors_verification.json',target)
 ns=dict(__name__='address_context_replay',__file__=str(Path(__file__)));exec(compile(code,str(parent),'exec'),ns)
 proxy=types.SimpleNamespace(**ns['parent'].__dict__);proxy.v=v;proxy.fast=fast;ns.update(parent=proxy,v=v);ns['main']()
 path=P/'analysis'/target;r=json.loads(path.read_text());assert (r['cases_passed'],r['independent_checks'])==(32,320) and r['dll_sha256']==digest(v.effects.DLL)
 r['sha256'][str(parent)]=digest(parent);path.write_text(json.dumps(r,indent=2)+'\n');files.extend((parent,path));records.append(dict(suite='context',cases=32,independent_checks=320))
 parent=P/'verify_movie_record_register.py';target='movie_task_yield_fault_regression.json';code=parent.read_text().replace('movie_record_register_verification.json',target)
 ns=dict(__name__='address_fault_replay',__file__=str(Path(__file__)));exec(compile(code,str(parent),'exec'),ns)
 original_cases=ns['cases'];ns.update(fast=fast,v=v);ns['cases']=lambda:(c for c in original_cases() if c['kind']=='fault');ns['main']()
 path=P/'analysis'/target;r=json.loads(path.read_text());assert (r['cases_passed'],r['independent_checks'])==(3,15) and r['dll_sha256']==digest(v.effects.DLL)
 r['sha256'][str(parent)]=digest(parent);r['scope']='Three fresh reached SR/FD fault prefixes in217-source DLL; no new-family coverage claim. '+r['scope'];path.write_text(json.dumps(r,indent=2)+'\n');files.extend((parent,path));records.append(dict(suite='fault-prefix',cases=3,independent_checks=15))
 cold_path=P.parents[1]/'work/sonic-adventure/movie_record_register_startup_cold_capture.json';cold=json.loads(cold_path.read_text());files.append(cold_path)
 assert cold['full_original_native_comparison'] and cold['record']['boundary_comparisons']==585993 and cold['record']['stop_pc']=='0x8c601874'
 assert cold['dll_sha256']=='f5ab618ca44448d3a4afdffa892b3506fd07912e1581571378d26cc471ca0952'
 report=dict(total_original_native_comparisons=35,independent_checks=335,suites=records,dll_sha256=digest(v.effects.DLL),historical_cold_profile=dict(source_stage='211-source movie_record_register',boundaries=585993,stop_pc='0x8c601874',dll_sha256=cold['dll_sha256'],freshly_rerun=False,relabelled_as217_source=False),sha256={str(p):digest(p) for p in files},scope='35 fresh217-source context/SR/FD comparisons335 checks. Existing full211-source cold capture retained unchanged as historical evidence, not a new217 cold execution. Other prior cases not freshly replayed for this frame/display integration. Legacy guards/runtime/bridge checked separately; zero new routine credit.')
 (P/'analysis/movie_task_yield_retained_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ADDRESS RETAINED35/335;211 cold retained/not rerun')
if __name__=='__main__':main()

