"""Native emission, particle lifetime, deferred deletion and total pool/heap release."""
import gc,hashlib,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_particle_callback as particle

PROJECT=Path(__file__).resolve().parent
current=particle.current;base=current.base;load,store=current.load,current.store
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def cases():
 for pool,heap,bank,pause in itertools.product((0,1,2,4,8),(0,64,80,160,320,640),range(2),(False,True)):
  yield dict(entry='primary',pool=pool,heap=heap,mode=bank<<21,game_mode=16 if pause else 0,pause_first=pause,
             expected_model=None,descriptor=False)


def main():
 *_,allowed,execute=particle.setup()
 baseline=PROJECT/'analysis/main_dispatch_particle_callback_verification.json'
 report=json.loads(baseline.read_text())
 for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 records=[];stages=boundaries=ops=geometry=parents=children=paused_passes=0
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[selected[i] for i in (0,23,47,71,95,119)]
 for n,c in enumerate(selected):
  m,s,f=particle.prepare(c,n);macl=0xFEED1234;local=[]
  def run(entry,stage,arg):
   nonlocal stages,boundaries,ops,geometry,macl
   s.r[4]=arg
   path,status,b,num,macl,calls=execute(c,m,s,f,entry,macl,allowed)
   assert status==0
   stages+=1;boundaries+=b;ops+=num;geometry+=calls
   local.append(dict(stage=stage,entry=entry,steps=len(path),boundary_comparisons=b,geometry_calls=calls,full_state_match=True))
  run('spawn','spawn_parent',base.previous.OBJECT)
  parent=load(m,base.LEVELS+24)
  assert parent==(current.NODES[0] if c['pool'] and c['heap']>=80 else 0)
  parents+=bool(parent)
  run('schedule','parent_emits_particles',6)
  nodes=current.children(m);count=len(nodes);children+=count
  assert count==(min(6,c['pool']-1,(c['heap']-80)//80) if parent else 0)
  assert all(load(m,t+16)==particle.PRIMARY and load(m,load(m,t+32),1)==0 for t in nodes)
  run('schedule','parent_deleted',6)
  assert load(m,base.LEVELS+24)==0
  def pass_(label,paused):
   before=current.children(m)
   data={t:dict(work=load(m,t+32),callback=load(m,t+16),state=load(m,load(m,t+32),1),timer=load(m,load(m,t+32)+6,2),
                spin=load(m,load(m,t+32)+0x1C),position=[current.value(load(m,load(m,t+32)+o)) for o in (0x20,0x24,0x28)],
                x=load(m,load(m,t+32)+0x14),y=load(m,load(m,t+32)+0x18)) for t in before}
   run('schedule',label,5)
   expected=[]
   for task in before:
    d=data[task];work=d['work']
    if d['callback']==current.previous.DELETE:continue
    expected.append(task)
    if d['state']==0:
     assert load(m,work,1)==1 and load(m,task+20)==particle.SECONDARY
     assert load(m,work+6,2)==d['timer'] and load(m,work+0x1C)==d['spin']
    elif d['state']==1:
     assert load(m,work+6,2)==(d['timer'] if paused else (d['timer']+1)&0xFFFF)
     if not paused and d['timer']>=10:
      assert load(m,work,1)==2
      assert load(m,work+0x1C)==d['spin']
     elif paused:
      assert load(m,work,1)==1 and load(m,work+0x1C)==d['spin']
      assert [current.value(load(m,work+o)) for o in (0x20,0x24,0x28)]==d['position']
     else:
      assert load(m,work,1)==1 and load(m,work+0x1C)==(d['spin']+0x1000)&0xFFFFFFFF
      x,y=((d[k]&0xFFFF)*(2*math.pi/65536) for k in ('x','y'))
      delta=(2*math.cos(y),2*math.sin(x)*math.sin(y),-2*math.cos(x)*math.sin(y))
      assert all(abs(current.value(load(m,work+o))-(v+dv))<0.001 for o,v,dv in zip((0x20,0x24,0x28),d['position'],delta))
    elif d['state']==2:assert load(m,task+16)==current.previous.DELETE
    else:raise AssertionError(d)
   assert current.children(m)==expected
   assert [load(m,particle.STATE_A+0x90+i*4) for i in range(4)]==[0x12340000+i*0x101 for i in range(4)]
   assert [load(m,particle.STATE_B+i*4) for i in range(5)]==[0x56780000+i*0x102 for i in range(5)]
  if c['pause_first'] and nodes:
   for i in range(3):pass_(f'paused_particle_pass_{i}',True);paused_passes+=1
  store(m,particle.GAME_MODE,0,2)
  passes=0
  while current.children(m):
   assert passes<14,'Particle lifetime exceeded its original state machine bound'
   pass_(f'active_particle_pass_{passes}',False);passes+=1
  if not nodes:pass_('empty_particle_pass',False)
  expected_free=(list(current.NODES[1:1+count])+[current.NODES[0]]+list(current.NODES[1+count:c['pool']])) if parent else list(current.NODES[:c['pool']])
  assert current.previous.chain(m,load(m,base.FREE_TASK))==expected_free
  assert all(load(m,base.LEVELS+i*4)==0 for i in range(8))
  assert load(m,base.FREE_HEAP)==(base.ARENA if c['heap'] else 0)
  if c['heap']:
   assert [load(m,base.ARENA+i*4) for i in range(4)]==[base.ARENA+c['heap'],0,base.ARENA+c['heap'],0]
   assert load(m,base.ARENA+c['heap']+12)==base.ARENA
  records.append(dict(input=c,parent_created=bool(parent),particles=count,active_particle_passes=passes,stages=local,independent_pool_heap_lifetime_checks=True))
  if (n+1)%20==0:print(n+1,'complete native particle lifetimes passed',flush=True)
  if (n+1)%10==0:gc.collect()
 # Admit the existing unregistered translation body with native arithmetic,
 # while retaining its older 1,024-case injected-arithmetic baseline.
 translation_rows=set();translation_cases=0
 for n,c in enumerate(c for c in particle.cases() if c['entry']=='translate'):
  m,s,f=particle.prepare(c,n)
  path,status,_,_,_,_=execute(c,m,s,f,'translate',0xFEED1234,allowed)
  translation_rows.update(a for a in path if particle.TRANSLATE<=a<0x8C63A874)
  translation_cases+=1
 assert translation_rows==set(range(particle.TRANSLATE,0x8C63A874,2))
 if '--smoke' in sys.argv:print('Particle lifetime smoke checks passed:',len(records));return
 files=[Path(__file__),Path(particle.__file__),baseline,PROJECT/'build_main_dispatch_particle_callback.ps1',
        PROJECT/'source/sa_main_dispatch_particle_callback_connected.c',PROJECT/'source/model_matrix_translate.c',
        PROJECT/'analysis/model_matrix_translate_verification.json']
 result=dict(lifecycles_passed=len(records),stage_comparisons=stages,boundary_comparisons=boundaries,numerical_operations=ops,
             geometry_service_invocations=geometry,created_parents=parents,created_particles=children,paused_scheduler_passes=paused_passes,
             translation_cases_passed=translation_cases,translation_rows_covered=len(translation_rows),
             sha256={str(p):digest(p) for p in files},dll_sha256=digest(particle.DLL),
             scope='Native spawn/emission, primary particle initialization/update/expiration/removal and full task/heap release over unchanged linked state, with original full-state comparisons each pass and independent lifetime/motion/pool/coalescing assertions; existing translation body admitted through native numerical paths.',
             limitations=['Geometry submission 8c620f2c is still a fixture; no actual visual rendering or hardware proof.',
                          'Host chooses scheduler-pass order and temporary pause/resume; this does not prove the complete original frame driver or primary/secondary phase ordering.',
                          'Inherits numerical guards and constructed-data limitations of the focused particle report. Full game/ports remain unfinished.'],cases=records)
 (PROJECT/'analysis/main_dispatch_particle_lifecycle_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('lifecycles_passed','stage_comparisons','boundary_comparisons','numerical_operations',
                  'geometry_service_invocations','created_parents','created_particles','paused_scheduler_passes','translation_cases_passed','translation_rows_covered')},indent=2))


if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
