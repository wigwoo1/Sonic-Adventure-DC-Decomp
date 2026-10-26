"""Do not unwind a guest call frame when a real descriptor access fails."""
import itertools,json
from pathlib import Path
import verify_movie_render_buffer_format as suite
v=suite.v;fast=suite.fast;P=suite.P;ROOT=suite.ROOT

def main():
 ram=v.seed_ram();records=[];checks=0
 for entry,tag in itertools.product((0x8c6536ec,0x8c653f08),(0x100,0x1000)):
  c=v.Input(dict(entry=entry,mode=0,render_arguments=[0,128,128,tag]));m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c)
  stop=0x8c653f32 if tag==0x1000 else 0x8c653f62;depth=40 if entry==0x8c6536ec else 28
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==-3 and s.pc==stop;checks+=4
  assert s.r[15]==before.r[15]-depth and bytes(f)==fp;checks+=2
  assert s.pr==(0x8c6536f6 if entry==0x8c6536ec else before.pr);checks+=1
  assert v.sample(a,0,256)==v.sample(m,0,256) and not r['vram_writes'] and not any(r['sound_state']);checks+=3
  records.append(dict(input=dict(c),status=r['status'],stop_pc=hex(s.pc),remaining_guest_frame_bytes=depth,guest_pr=hex(s.pr),steps=len(path)));print(hex(entry),hex(tag),r['status'],hex(s.pc),depth,flush=True)
 files=[Path(__file__),Path(suite.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_render_buffer_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,dll_sha256=suite.digest(v.effects.DLL),sha256={str(p):suite.digest(p) for p in files},scope='Four actual null-descriptor failureprefixes throughformatter andrealrenderdispatcher. Original/native completeRAM/CPU/FPU/device/orderedboundaries match;independentstopaddresses/28or40byte gueststackframe/PR/memory oracles. No synthetichelpererror orunwind. Notphysicaladdress-exceptiontiming orbroaderbadmemorycontract.')
 (P/'analysis/movie_render_buffer_failure_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Failureprefix',len(records),checks)

if __name__=='__main__':main()
