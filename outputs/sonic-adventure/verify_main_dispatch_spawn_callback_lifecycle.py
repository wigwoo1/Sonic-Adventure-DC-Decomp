"""Native spawn -> parent callback/emission -> later parent disposal, same state."""
import gc,hashlib,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_spawn_callback as current

PROJECT=Path(__file__).resolve().parent
base=current.base;load,store=current.load,current.store
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def cases():
 for character,pool,heap,bank,descriptor in itertools.product((0,128,255),(0,1,2,4,8),(0,64,80,160,320,640),range(2),(False,True)):
  yield dict(entry='spawn',character=character,pool=pool,heap=heap,mode=bank<<21,descriptor=descriptor)
 for mask,bank in itertools.product(range(4),range(2)):
  yield dict(entry='spawn',character=127,pool=8,heap=640,mode=bank<<21,angles=(0x4000 if mask&1 else 0,0,0x4000 if mask&2 else 0))


def main():
 *_,allowed=current.setup()
 baseline=PROJECT/'analysis/main_dispatch_spawn_callback_verification.json'
 report=json.loads(baseline.read_text())
 for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 records=[];stages=boundaries=ops=parents=children=0
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[selected[i] for i in (0,47,119,239,359,367)]
 for n,c in enumerate(selected):
  m,s,f=current.prepare(c,n);macl=0xFEED1234;local=[]
  def run(entry,stage):
   nonlocal macl,stages,boundaries,ops
   path,status,b,num,macl=current.execute(c,m,s,f,entry,macl,allowed)
   assert status==0
   stages+=1;boundaries+=b;ops+=num;local.append(dict(stage=stage,entry=entry,steps=len(path),boundary_comparisons=b,full_state_match=True))
  run('spawn','create_parent')
  parent=load(m,base.LEVELS+24)
  created=c['pool']>0 and c['heap']>=80
  assert parent==(current.NODES[0] if created else 0)
  if created:
   work=load(m,parent+0x20)
   assert work==base.ARENA+16 and load(m,work+8,1)==c['character']
   assert load(m,parent+16)==current.CALLBACK
   parents+=1
  s.r[4]=6;run('schedule','execute_parent_callback')
  if created:
   child_count=current.independent(dict(c,pool=c['pool']-1,heap=c['heap']-80),m,work,'callback')
   assert load(m,parent+16)==current.previous.DELETE
   assert load(m,base.LEVELS+24)==parent
   assert current.previous.chain(m,load(m,base.FREE_TASK))==list(current.NODES[1+child_count:c['pool']])
   children+=child_count
  else:
   child_count=0;assert current.children(m)==[] and load(m,current.SEED)==c.get('seed',0x12345678)
  particle_nodes=current.children(m)
  particle_payload={t:bytes(m[(load(m,t+32)&0xFFFFFF):(load(m,t+32)&0xFFFFFF)+64]) for t in particle_nodes}
  s.r[4]=6;run('schedule','delete_parent_only')
  assert load(m,base.LEVELS+24)==0
  assert current.children(m)==particle_nodes
  assert all(bytes(m[(load(m,t+32)&0xFFFFFF):(load(m,t+32)&0xFFFFFF)+64])==particle_payload[t] for t in particle_nodes)
  expected_free=([current.NODES[0]]+list(current.NODES[1+child_count:c['pool']])) if created else list(current.NODES[:c['pool']])
  assert current.previous.chain(m,load(m,base.FREE_TASK))==expected_free
  if not child_count:
   assert load(m,base.FREE_HEAP)==(base.ARENA if c['heap'] else 0)
   if c['heap']:assert [load(m,base.ARENA+i*4) for i in range(4)]==[base.ARENA+c['heap'],0,base.ARENA+c['heap'],0]
  records.append(dict(input=c,parent_created=created,particle_tasks=child_count,stages=local,independent_invariants=True))
  if (n+1)%80==0:print(n+1,'native spawn/callback/disposal lifecycles passed',flush=True)
  if (n+1)%20==0:gc.collect()
 if '--smoke' in sys.argv:print('Spawn callback lifecycle smoke checks passed:',len(records));return
 files=[Path(__file__),Path(current.__file__),baseline,PROJECT/'build_main_dispatch_spawn_callback.ps1',
        PROJECT/'source/sa_main_dispatch_spawn_callback_connected.c']
 result=dict(lifecycles_passed=len(records),stage_comparisons=stages,boundary_comparisons=boundaries,
             numerical_operations=ops,created_parents=parents,created_particle_tasks=children,
             external_game_service_calls=0,sha256={str(p):digest(p) for p in files},dll_sha256=digest(current.DLL),
             scope='Actual native spawn, level-6 scheduling of callback 8c044680 with native transform/emission dependencies, and subsequent parent deletion over the same state, compared at every stage to original instructions plus independent list/pool/heap/payload assertions.',
             limitations=['Host orchestrates separate native scheduler passes; no frame timing, complete game loop, hardware/MMIO or gameplay proof.',
                          'Level-5 particle tasks remain allocated with stored callback 8c04481a; its body is not executed or claimed recovered here.',
                          'Inherits numerical and fixture limits of the 612-case body report.'],cases=records)
 (PROJECT/'analysis/main_dispatch_spawn_callback_lifecycle_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('lifecycles_passed','stage_comparisons','boundary_comparisons','numerical_operations','created_parents','created_particle_tasks','external_game_service_calls')},indent=2))


if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
