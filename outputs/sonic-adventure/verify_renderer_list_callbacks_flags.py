"""Independent flag-effect controls include a previously clear deferred bit."""
import dataclasses,hashlib,itertools,json
from pathlib import Path
import renderer_list_callbacks_fixtures as q
P=q.P;R=P.parents[1]


def main():
 target=P/'analysis/renderer_list_callbacks_flags_verification.json';assert not target.exists()
 m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0));s.pc=c['entry']=0x8c6416ec;s.pr=0x8cfed000
 r,path,m,s,f=q.fast.compare(m,s,f,c);assert r['status']==0
 initial=(bytes(m),bytes(s),bytes(f),dict(c),r['graphics_out']);cases=[];checks=0
 for flag,mode,state in itertools.product((0,0x1004,0xffffffff),(0,1),(-1,0)):
  m=bytearray(initial[0]);s=q.v.effects.State.from_buffer_copy(initial[1]);f=q.v.effects.Fpu.from_buffer_copy(initial[2]);c=dict(initial[3])
  arrays=list(initial[4].arrays);registers=list(arrays[0]);registers[(0x8138-0x8000)//4]=0x9000;arrays[0]=tuple(registers);c['graphics_in']=dataclasses.replace(initial[4],arrays=tuple(arrays))
  s.pc=c['entry']=0x8c651f1e;s.pr=0x8cfed000;c['ticks']=[0]
  for a,value in [(0x8c8a30b0,0),(0x8c8a3098,state),(0x8c8a30c4,mode),(0x8c8a2ffc,flag),(0x8c8a2d5c,1),(0x8c8a31d0,0x1000),(0x8c8a2f00,0),(0x8c8a2f60,7),(0x8c8a323c+0x48,7)]:q.put(m,a,value)
  m[0x8a2df8:0x8a2df8+192]=bytes(192)
  r,path,a,s,f=q.fast.compare(m,s,f,c)
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];checks+=3
  expected=flag|32 if state==0 and mode==0 else flag
  assert q.get(a,0x8c8a2ffc)==expected;checks+=1
  assert q.get(a,0x8c8a3098)==(3 if state==0 and mode==0 else state&0xffffffff);checks+=1
  assert q.get(a,0x8c8a2f60)==0 and q.get(a,0x8c8a323c+0x4c)==8;checks+=2
  wanted=bytearray(initial[4].vram)
  if state==0:wanted[0x9000:0x90c0]=bytes(192)
  assert r['graphics_out'].vram==wanted;checks+=1
  cases.append(dict(input_flag=hex(flag),mode=mode,input_state=state,expected_flag=hex(expected)))
 files=[Path(__file__),P/'source/sa_renderer_list_callbacks.c',P/'renderer_list_callbacks_reference.py',P/'write_watch_renderer_list_callbacks.py',R/'work/sonic-adventure/build/renderer_list_callbacks.dll']
 result=dict(status='callback_deferred_flag_and_skip_gates_independently_verified',original_native_comparisons=1+len(cases),callback_cases=len(cases),checks=checks,cases=cases,scope='Explicit standalone callback inputs. Independently checkdeferredbit32fromclear,partiallyset andallset flags,withbothgates andskip/active states. Exact192-bytecopy andall8MiBVRAM. Fulloriginal/native machine/device/boundary checks retained.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('Deferred flag',len(cases),'cases',checks,'checks',flush=True)


if __name__=='__main__':main()
