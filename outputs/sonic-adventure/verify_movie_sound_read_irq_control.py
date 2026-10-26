"""Retain finite status-input exhaustion after real data and original IRQ entry."""
import hashlib,itertools,json,types
from pathlib import Path
import write_watch_sound_read as fast
import verify_movie_transfer_runtime as previous
import movie_cpu_transfer_reference as cpu
v=fast.old;P=v.P;ROOT=v.ROOT
def main():
 v.bind_vectors(0x8ce50000);ram=v.seed_ram();records=[];checks=0
 setup=types.FunctionType(previous.setup.__code__,dict(previous.setup.__globals__,fast=fast,v=v,cpu=cpu),'setup')
 for mode,context in itertools.product((0,1,4),(v.NORMAL,v.NESTED)):
  c=v.Input(entry=cpu.REQUEST,mode=mode,sdk_mode=1,current_record=0,channel=0,length=64,context=context,masks=[0,0],irq_status=0,irq_samples=[],table=0xa05f7800,transfer_registers=[0]*48,runtime_attached=True)
  a,s,f,stages=setup(ram,c);source=bytes(a[cpu.SRC&0xffffff:(cpu.SRC&0xffffff)+64]);s.pc=cpu.REQUEST;s.r[4]=v.RECORD;s.r[5]=0;c['entry']=s.pc
  r,path,a,s,f=fast.compare(a,s,f,c);state=r['runtime_state']
  assert r['status']==-4 and r['stop_pc']=='0x8c645bc0' and r['full_state_match'] and r['ordered_boundaries_match'];checks+=4
  assert state[4]==1<<15 and state[5]==state[6]==1 and state[7]==64 and state[8]==state[9]==state[10]==0;checks+=7
  assert v.load(a,v.RECORD+24)==0 and v.load(a,v.DESCRIPTOR+44)==1 and v.HANDLER not in path and r['exception_state'][-1]==0;checks+=4
  wanted=bytearray([0xa5])*0x200000;wanted[:64]=source
  assert r['sound_ram_sha256']==hashlib.sha256(wanted).hexdigest() and r['sound_trace_events']==16 and not r['sound_trace_dropped'];checks+=3
  assert r['irq_cursor']==0 and all(stage['status']==0 for stage in stages);checks+=2
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],runtime_state=state,exception_state=r['exception_state'],sound_ram_sha256=r['sound_ram_sha256'],original_native_comparisons=3));print('Finite IRQ control',mode,hex(context),r['stop_pc'],flush=True)
 files=[Path(__file__),Path(previous.__file__),Path(v.__file__),Path(fast.__file__),P/'analysis/movie_sound_read_connection.json']
 report=dict(lifecycles_passed=len(records),original_native_comparisons=3*len(records),independent_checks=checks,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Six actual vector-init/registration/REQUEST lifecycles x three ABI calls. Real64-byte copy and original IRQ/vector/context reach ASIC status read; explicit empty raw observation input returns actual-4at645bc0,leaving completeddata,pendingbit,active descriptor and completion0. No fallback observation/handler return/completed flag supplied. Full partial CPU/FPU/RAM/platform/Sound/runtime state compares against original instructions for both contexts/three FPU modes.')
 (P/'analysis/movie_sound_read_irq_control_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Finite IRQ controls',len(records),'comparisons',3*len(records),'checks',checks)
if __name__=='__main__':main()
