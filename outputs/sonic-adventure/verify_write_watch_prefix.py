"""Validate four fresh executions against every frozen caller-result field."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 baseline=P/'analysis/movie_driver_control_prefix_verification.json';prior=json.loads(baseline.read_text());files=[Path(__file__),baseline];checks=0;records=[]
 dll=ROOT/'work/sonic-adventure/build/movie_driver_control.dll';dll_hash=digest(dll)
 for mode in ('dual','fast'):
  for cold in (False,True):
   path=ROOT/f'work/sonic-adventure/write_watch_prefix_{mode}_{"cold" if cold else "warm"}_capture.json';capture=json.loads(path.read_text());files.append(path)
   for pinned,h in capture['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
   assert capture['full_original_native_comparison'] and capture['dll_sha256']==dll_hash and capture['mode']==mode;checks+=3
   r=capture['record'];saved=next(x for x in prior['cases'] if x['input']['cold_filesystem']==cold)
   assert set(r)==set(saved)|{'write_watch'};checks+=1
   for key,value in saved.items():
    if key=='input':assert {k:v for k,v in r[key].items() if k!='write_watch_legacy_hashes'}==value,(mode,cold,key)
    else:assert r[key]==value,(mode,cold,key)
    checks+=1
   metrics=r['write_watch'];assert metrics['legacy_hashes_also_compared']==(mode=='dual') and set(metrics['regions'])=={'ram','vram'};checks+=2
   for name,size in (('ram',0x1000000),('vram',0x800000)):
    region=metrics['regions'][name]
    assert region['bytes']==region['initial_full_bytes_compared']==region['final_full_bytes_compared']==size;checks+=3
    assert region['snapshots']==r['boundary_comparisons'] and region['compared_pages']>0 and region['exact_page_bytes_compared']>0;checks+=3
   records.append(dict(mode=mode,cold_filesystem=cold,elapsed_seconds=capture['elapsed_seconds'],status=r['status'],stop_pc=r['stop_pc'],boundary_comparisons=r['boundary_comparisons'],disc_read_attempts=r['disc_read_attempts'],masaru_sha256=r['masaru_sha256_at_stop'],masaru_payload_matches=r['masaru_payload_matches'],clear_writes=r['clear_native']['writes'],write_watch=metrics))
 report=dict(cases_passed=4,independent_saved_record_checks=checks,unchanged_native_dll=True,dll_sha256=dll_hash,all_prior_caller_result_fields_matched=True,dual_profiles=2,fast_profiles=2,cold_profiles_freshly_reexecuted=2,cases=records,sha256={str(p):digest(p) for p in files},scope='Four newly executed original/native caller comparisons: cold and warm with old full-memory hashes plus exact dirty-page unions simultaneously, then cold and warm with exact page unions. Every old caller-result field matches, including all ordered boundaries, all native/reference clear statistics, VRAM digest, reads, device/CPU state and actual frontier. Initial/final RAM/VRAM bytes compare completely. No guest execution checkpoint, skipped decoder/callee, sampled boundary or supplied return. Original full-clear fingerprint/decoder limitations remain unchanged; no new game routine credit.')
 (P/'analysis/write_watch_prefix_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Validated four fresh caller executions:',checks,'saved-result checks')
if __name__=='__main__':main()
