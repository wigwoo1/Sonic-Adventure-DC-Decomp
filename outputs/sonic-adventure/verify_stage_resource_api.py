"""Focused new resource/matrix API checks; fixtures stay out of normal states."""
import json,pickle,struct
from model_stage_resources_verified_state import normal,sha,P
from model_scene_verified_state import Scene
def put(s,a,x,n=4):s.m[a&0xffffff:(a&0xffffff)+n]=(x&((1<<(8*n))-1)).to_bytes(n,'little')
def read(s,a,n=4):return int.from_bytes(s.m[a&0xffffff:(a&0xffffff)+n],'little')
def clone(base,q):
 s=Scene.__new__(Scene);s.m=bytearray(base.m);s.s=q.v.effects.State.from_buffer_copy(bytes(base.s));s.f=q.v.effects.Fpu.from_buffer_copy(bytes(base.f));s.c=pickle.loads(pickle.dumps(base.c,protocol=5));s.records=[];s.usable=True;s.prefix=base.prefix;q.v.bind_vectors(s.c['vbr']);return s
def main():
 target=P/'analysis/stage_resource_api_verification.json';assert not target.exists()
 base,q=normal();cases=[]
 def call(s,name,entry,args=(),status=0):
  r,path=s.call(entry,args,compare=q.fast.compare);assert r['status']==status,(name,r['status'],r['stop_pc'])
  assert s.usable==(status==0) and r['full_state_match'] and r['ordered_boundaries_match']
  cases.append(dict(name=name,entry=hex(entry),arguments=list(args),status=status,stop_pc=r['stop_pc'],steps=len(path),declared_API_fixture=True,full_state_match=True,ordered_boundaries_match=True,faulted_state_resumed=False));print(name,status,len(path),flush=True)
 for bank in (0,1):
  for size in (0,1):
   for pr in (0,1):
    s=clone(base,q);src=0x8cee8001;dst=0x8cee8103;text=(b'' if size else b'Stage resource ABI')+b'\0'
    s.m[src&0xffffff:(src&0xffffff)+len(text)]=text;s.m[(dst&0xffffff)-4:(dst&0xffffff)+len(text)+4]=b'\xa5'*(len(text)+8)
    s.f.fpscr=(s.f.fpscr&~0x380000)|(bank<<21)|(size<<20)|(pr<<19)
    call(s,f'string_FR{bank}_SZ{size}_PR{pr}',0x8c10d3dc,(dst,src))
    assert bytes(s.m[dst&0xffffff:(dst&0xffffff)+len(text)])==text
    assert s.m[(dst&0xffffff)-4:dst&0xffffff]==b'\xa5'*4 and s.m[(dst&0xffffff)+len(text):(dst&0xffffff)+len(text)+4]==b'\xa5'*4
 for bank in (0,1):
  for size in (0,1):
   s=clone(base,q);slot=read(s,0x8c02e80c);pointer=read(s,slot);start=pointer-64
   values=[0x3f000000+i*0x100 for i in range(16)]
   for i,value in enumerate(values):put(s,start+4*i,value)
   s.f.fpscr=(s.f.fpscr&~0x380000)|(bank<<21)|(size<<20)
   for b in range(2):
    for i in range(16):s.f.banks[b][i]=0x41000000+b*0x10000+i*0x100
   call(s,f'matrix_restore_FR{bank}_SZ{size}',0x8c02e7e4)
   assert ((s.f.fpscr>>20)&1)==size
   if size==0:assert list(s.f.banks[bank^1])==values
   else:assert [s.f.banks[bank][i] for i in range(1,16,2)]==values[:8]
 for bank in (0,1):
  s=clone(base,q);s.f.fpscr=(s.f.fpscr&~0x380000)|(bank<<21)|0x80000
  call(s,f'matrix_restore_FR{bank}_reached_PR_guard',0x8c02e7e4,status=-1)
  assert cases[-1]['stop_pc']=='0x8c02e7f2'
 report=dict(status='verified_explicit_string_ABI_and_matrix_restore_cases',original_native_comparisons=len(cases),normal_returns=sum(x['status']==0 for x in cases),expected_host_guard_stops=sum(x['status']!=0 for x in cases),cases=cases,fixture_states_saved=False,all_stage_selectors_qualified=False,invalid_stage_fatal_loop_exercised=False,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in [P/'verify_stage_resource_api.py',P/'model_stage_resources_verified_state.py',q.v.effects.DLL]})
 target.write_text(json.dumps(report,indent=2)+'\n');print('RESOURCE API CASES VERIFIED:',len(cases),flush=True)
if __name__=='__main__':main()
