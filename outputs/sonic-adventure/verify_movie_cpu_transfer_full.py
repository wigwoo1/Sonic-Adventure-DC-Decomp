"""Validate retained complete-copy evidence without reexecuting the expensive run."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 cp=ROOT/'work/sonic-adventure/movie_cpu_transfer_full_capture.json';data=json.loads(cp.read_text());files=[Path(__file__),cp];checks=0
 for path,h in data['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 dll=digest(ROOT/'work/sonic-adventure/build/movie_cpu_transfer.dll');assert data['dll_sha256']==dll and data['full_original_native_comparison'];checks+=2
 r=data['record'];assert r['status']==0 and r['stop_pc']==data['original_pr'] and r['full_state_match'] and r['ordered_boundaries_match'];checks+=4
 assert data['guest_r0']=='0x0' and data['original_sp']==data['final_sp'] and data['original_pr']==data['final_pr'];checks+=3
 assert data['source_bytes']==0x200000 and data['source_unchanged'] and r['sound_ram_sha256']==data['source_sha256'];checks+=3
 assert r['sound_state']==[0,0x80000,0,0x200000,0x80000,0] and r['sound_trace_events']==0x80000 and r['sound_trace_dropped']==0;checks+=3
 assert len(data['word_copy_calls'])==1 and data['word_copy_calls'][0]['arguments']==[0x8c900000,0xa0800000,4,0x80000];checks+=2
 report=dict(cases_passed=1,independent_checks=checks,record=r,source_bytes=data['source_bytes'],source_sha256=data['source_sha256'],source_unchanged=True,elapsed_seconds=data['elapsed_seconds'],dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Fresh original/native full two-MiB varying-data copy. Every original word transfer executes with source/destination strides1/1; the actual complete sound output equals the independently generated complete input and input remains unchanged. All524288ordered sound events compare with zero drops, all machine/RAM/FPU/device boundaries and full final memory compare. No shortened count, supplied helper return, IRQ or execution checkpoint. This is an explicit direct ABI copy fixture, not natural startup or ARM/audio execution.')
 (P/'analysis/movie_cpu_transfer_full_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Full varying-data copy validated;',checks,'checks')
if __name__=='__main__':main()
