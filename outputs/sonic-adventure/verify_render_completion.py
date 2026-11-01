"""Independent job ownership, acknowledgement and delegation contract tests."""
import ctypes,hashlib,json,random
from pathlib import Path
import render_completion_native as n
from render_completion_reference import Completion,OFFSETS,MAX64
P=Path(__file__).resolve().parent;R=P.parents[1]
def main():
 target=P/'analysis/render_completion_verification.json';assert not target.exists()
 checks=0;operations=0;rng=random.Random(0x5f8014)
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 for case in range(80):
  config=[rng.getrandbits(32) for _ in OFFSETS];state=n.initialized(config);ref=Completion(config)
  calls=[];failure=[0]
  @n.READ
  def read(ctx,a,width,out):calls.append(('read',a,width));out[0]=0x80;return failure[0]
  @n.WRITE
  def write(ctx,a,value,width):calls.append(('write',a,value,width));return failure[0]
  @n.CACHE
  def cache(ctx,a,op):return -9
  parent=n.Bus(None,read,write,cache);binding=n.Binding();bus=n.Bus()
  require(n.LIB.sa_render_completion_bind(ctypes.byref(binding),ctypes.byref(state),ctypes.byref(parent),ctypes.byref(bus))==0,'bind explicit state')
  for step in range(150):
   choice=rng.randrange(5);failure[0]=-8 if rng.randrange(11)==0 else 0;before=n.snapshot(state);old_calls=len(calls)
   if choice==0:
    token=rng.choice((ref.active_id,0,ref.submitted+1))
    rc=n.LIB.sa_render_completion_finish(ctypes.byref(state),token);expected=ref.finish(token)
    require(len(calls)==old_calls,'backend completion does not write guest state or parent registers')
   elif choice==1:
    a=0xa05f6900;out=n.U(0xdeadbeef);rc=bus.read(bus.context,a,4,ctypes.byref(out));expected=failure[0]
    require(out.value==(0x80|ref.pending if not expected else 0x80),'ASIC read merges only completed jobs')
    require(n.snapshot(state)==before,'reads neither complete nor advance jobs')
   else:
    a=rng.choice((0xa05f8014,0xa05f8008,0xa05f6900,0xa05f8000+rng.choice(OFFSETS)))
    value=rng.choice((0,0xffffffff,1)) if a==0xa05f8014 else rng.randrange(8) if a in (0xa05f8008,0xa05f6900) else rng.getrandbits(32)
    width=2 if rng.randrange(19)==0 else 4
    expected=ref.check(a,value,width)
    if not expected:
     expected=failure[0]
     if not expected:ref.committed(a,value,width)
     delegated=True
    else:delegated=False
    rc=bus.write(bus.context,a,value,width)
    require(len(calls)-old_calls==int(delegated),'unsupported or capacity-failed writes do not reach parent')
   require(rc==expected and n.snapshot(state)==ref.snapshot(),'independent lifecycle state and exact status')
   if rc:require(n.snapshot(state)==before,'failed owned operations preserve lifecycle state')
   operations+=1
  require(bus.cache(bus.context,0x8c100000,2)==-9,'cache callback status is preserved')
 # Exact ownership checks complement the randomized state-machine exploration.
 state=n.initialized(list(range(16)));ref=Completion(list(range(16)))
 failure[0]=0;binding=n.Binding();bus=n.Bus();require(n.LIB.sa_render_completion_bind(ctypes.byref(binding),ctypes.byref(state),ctypes.byref(parent),ctypes.byref(bus))==0,'exact-profile binding')
 require(bus.write(bus.context,0xa05f8014,0xffffffff,4)==0 and state.pending==0 and state.active_id==1,'render start submits a job and never invents completion')
 require(bus.write(bus.context,0xa05f8020,0x12345678,4)==0 and state.job[0]==0 and state.parameters[0]==0x12345678,'job parameters are immutable while next configuration changes')
 require(n.LIB.sa_render_completion_finish(ctypes.byref(state),2)==-6 and state.pending==0,'wrong backend token cannot release a live job')
 require(n.LIB.sa_render_completion_finish(ctypes.byref(state),1)==0 and state.pending==7 and state.completed==1,'matching explicit backend acknowledgement raises render stage events')
 require(bus.write(bus.context,0xa05f6900,1,4)==0 and state.pending==6,'partial IRQ acknowledgement preserves other stages')
 require(bus.write(bus.context,0xa05f8014,0xffffffff,4)==-4,'pending completion cannot be silently overwritten by a new job')
 require(bus.write(bus.context,0xa05f6900,6,4)==0 and bus.write(bus.context,0xa05f8014,0xffffffff,4)==0 and state.active_id==2,'acknowledged ownership permits the next submission')
 require(bus.write(bus.context,0xa05f8008,1,4)==0 and state.active_id==2,'TA-only reset does not cancel ISP/TSP work')
 require(bus.write(bus.context,0xa05f8008,2,4)==0 and state.active_id==0 and state.cancelled==1,'ISP/TSP reset cancels the in-flight backend token')
 require(n.LIB.sa_render_completion_finish(ctypes.byref(state),2)==-6 and state.pending==0,'cancelled token cannot generate an interrupt')
 state.submitted=MAX64;saved=n.snapshot(state);require(bus.write(bus.context,0xa05f8014,0xffffffff,4)==-6 and n.snapshot(state)==saved,'job identifiers never wrap silently')
 state=n.initialized();binding=n.Binding();bus=n.Bus();require(n.LIB.sa_render_completion_bind(ctypes.byref(binding),ctypes.byref(state),ctypes.byref(parent),ctypes.byref(bus))==0,'disabled-profile binding');state.enabled=0
 require(bus.write(bus.context,0xa05f8014,0xffffffff,4)==0 and state.submitted==state.pending==0,'disabled lifecycle delegates without fake work or events')
 files=[Path(__file__),P/'render_completion_native.py',P/'render_completion_reference.py',P/'source/render_completion.c',P/'source/render_completion.h',P/'build_render_completion.ps1',n.DLL]
 report=dict(status='explicit_host_render_job_lifecycle_contract_verified',random_operations=operations,checks=checks,profiles=80,completion_requires_backend_acknowledgement=True,reads_complete_jobs=False,automatic_rasterization=False,physical_render_timing_qualified=False,pixels_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('VERIFIED render completion lifecycle',operations,'operations;',checks,'checks',flush=True)
if __name__=='__main__':main()
