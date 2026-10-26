"""Replay the complete published711-case request/runtime evidence in the199-source DLL."""
import hashlib,json
from pathlib import Path
import write_watch_midi_packet as fast
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
SUITES=(
 ('verify_movie_transfer_runtime_regression.py','movie_transfer_runtime_regression_verification','movie_midi_packet_retained_regression_verification','total_comparisons',596),
 ('verify_movie_transfer_runtime.py','movie_transfer_runtime_verification','movie_midi_packet_retained_runtime_verification','original_native_comparisons',85),
 ('verify_movie_transfer_runtime_compatibility.py','movie_transfer_runtime_compatibility_verification','movie_midi_packet_retained_compatibility_verification','legacy_abi_comparisons',6),
 ('verify_movie_transfer_request_closure.py','movie_transfer_request_closure_verification','movie_midi_packet_retained_rejection_verification','cases_passed',12),
 ('verify_movie_request_complete.py','movie_request_complete_verification','movie_midi_packet_retained_request_verification','cases_passed',12),
)
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports=[];files=[Path(__file__),Path(fast.__file__),Path(fast.old.__file__)];total=0
 fast.old.bind_vectors(0x8ce50000)
 for script,old_name,new_name,key,expected in SUITES:
  parent=P/script;code=parent.read_text().replace('import write_watch_transfer_runtime as fast','import write_watch_midi_packet as fast')
  code=code.replace('cpu=v.old.old.old.old','cpu=__import__("movie_cpu_transfer_reference")')
  code=code.replace('sa_movie_transfer_runtime_observed','sa_movie_midi_packet_observed').replace('movie_transfer_runtime_connection.json','movie_midi_packet_connection.json')
  code=code.replace(old_name+'.json',new_name+'.json')
  marker='files=[Path(__file__),';assert code.count(marker)==1;code=code.replace(marker,'files=[Path(__file__),Path(parent_script),')
  scope=dict(__name__='sound_read_retained_suite',__file__=str(Path(__file__)),parent_script=str(parent))
  exec(compile(code,str(parent),'exec'),scope);scope['main']()
  path=P/f'analysis/{new_name}.json';r=json.loads(path.read_text());assert r[key]==expected and r['dll_sha256']==digest(fast.old.effects.DLL)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned
  total+=expected;reports.append(dict(suite=script,report=path.relative_to(P).as_posix(),comparisons=expected));files.extend((parent,path));print('Retained suite completed',script,expected,flush=True)
 assert total==711
 report=dict(total_original_native_comparisons=total,suites=reports,dll_sha256=digest(fast.old.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All711previous runtime/request original-native comparisons execute freshly against199-source integration. Existing script bodies are loaded unchanged except explicit module/export/report names and dependency pins; canonical source/proofs stay intact. Suites cover594prior own-body cases,2default waits,85runtime/lifecycle/failure comparisons,6legacy ABI and24request rejection/completion cases. No older early-caller/full-copy/video-clear proof is substituted for fresh startup captures.')
 (P/'analysis/movie_midi_packet_retained_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Fresh retained comparisons',total)
if __name__=='__main__':main()
