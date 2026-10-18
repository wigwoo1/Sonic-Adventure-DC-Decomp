"""Original initialized-object draw path through native motion mesh packets."""
import collections,ctypes,hashlib,json,sys
from pathlib import Path
import verify_overlay_draw_motion_formats as previous
import verify_draw_motion_mesh as motion
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
ARENA_SOURCE,ARENA_END,ARENA_DEST=0x8cd00000,0x8ce00000,0x8cf00000
VIEW=0x8ce90000
def relocate(v):return ARENA_DEST+v-ARENA_SOURCE if ARENA_SOURCE<=v<ARENA_END else v

def make_runtime():
 seed,_,_=motion.prepare({},0);binary=motion.previous.previous.state.gate.support.morph.MAIN.read_bytes();baseline=bytearray(16<<20);baseline[0x10000:0x10000+len(binary)]=binary
 patches=[]
 for a in range(0x8c880000,0x8c8f0000,4):
  x=load(seed,a)
  if x!=load(baseline,a):
   assert not 0x8cc00000<=x<ARENA_SOURCE,(hex(a),'constructed object pointer cannot enter original-model fixture')
   patches.append((a,relocate(x)))
 # Ninja texture-list/cache cells also live in the main executable's data area.
 for literal in (0x8c606b3c,0x8c6069d4,0x8c6069c0,0x8c606b40,0x8c606b44,0x8c606b48,0x8c620b70,0x8c620b74):
  a=motion.renderer.base.literal(literal);patches.append((a,relocate(load(seed,a))))
 # Zero is significant: the older state-only fixture intentionally supplied
 # arbitrary render masks. A changed-word diff alone would retain those masks.
 for a in (motion.renderer.mesh.ENV,motion.renderer.mesh.MODE):patches.append((a,load(seed,a)))
 block=bytearray(seed[ARENA_SOURCE&0xffffff:ARENA_END&0xffffff])
 for i in range(0,len(block),4):
  x=int.from_bytes(block[i:i+4],'little');new=relocate(x)
  if new!=x:block[i:i+4]=new.to_bytes(4,'little')
 view=bytes(seed[motion.VIEW&0xffffff:(motion.VIEW&0xffffff)+0x130])
 return patches,bytes(block),view
PATCHES,RUNTIME_BLOCK,VIEW_BYTES=make_runtime()

def prepare(ram,c):
 m,s,f=previous.previous.prepare(ram,c)
 stack={a:load(m,a) for a in (0x8c88f698,0x8c88f69c,0x8c88f5f8)}
 m[ARENA_DEST&0xffffff:]=RUNTIME_BLOCK
 for a,v in PATCHES:store(m,a,v)
 for a,v in stack.items():store(m,a,v)
 # The original initialized object heap and model/animation records are retained.
 m[VIEW&0xffffff:(VIEW&0xffffff)+len(VIEW_BYTES)]=VIEW_BYTES;store(m,motion.VIEW_SLOT,VIEW)
 state_a=motion.previous.previous.STATE_A;store(m,0x8c88fcb4,state_a);store(m,0x8c88fcd4,state_a+0x100)
 for i in range(5):store(m,state_a+0x100+i*4,relocate(motion.renderer.PACKETS) if i==0 else relocate(0x8cde0000) if i in (2,4) else 0)
 store(m,motion.renderer.mesh.LIMIT,4096);store(m,0x8c890074,0)
 # Supplied runtime texture descriptors, with bounded valid metadata rather than
 # fabricated texture-service return values. Physical texture sampling is absent.
 table,records=relocate(0x8cd31000),relocate(0x8cd32000)
 if c.get('nonnull'):store(m,0x8cc60000,table)
 for i in range(256):
  a=records+i*32;store(m,table+i*12+8,a)
  for j,v in enumerate((0x11000000+i,0x22000000+i,0x13579bdf,0x80000)):store(m,a+j*4,v)
 # The secondary path selects the shipped 221-name texture list. Its on-disc
 # runtime descriptor slots are zero until texture loading; supply those slots
 # while retaining original filename and attribute words, geometry and motion.
 original_list=0x0ca53c14;names,count=load(m,original_list),load(m,original_list+4)
 assert names==0x0ca531b8 and count==221
 for i in range(count):store(m,names+i*12+8,records+i*32)
 return m,s,f

CURRENT={}
def reference_run(memory,state,f,c,tracked=()):
 ref=motion.QueueReference();ref.ram=memory;motion.namespace['queue_reference']=ref;CURRENT['reference']=ref
 return motion.reference_run(memory,state,f,c,tracked)

def native_call(*args):
 mem=motion.Memory();bus=motion.Bus();motion.LIB.sa_vertex_bus_init(ctypes.byref(mem),args[0],ctypes.byref(bus))
 status=motion.fn(*args,ctypes.byref(bus));CURRENT['native']=mem;return status

ns=dict(previous.ns,fn=native_call,reference_run=reference_run,CONNECTED=motion.CONNECTED)
exec(compile(previous.previous.previous.compare_code,'motion_mesh_original_caller','exec'),ns)
compare_state=ns['compare']

def compare(m,s,f,c):
 record,path,b=compare_state(m,s,f,c);native,ref=CURRENT['native'],CURRENT['reference']
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'original caller queue state/events')
 record.update(queue_match=True,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped)
 return record,path,b

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 proofs=[PROJECT/'analysis'/n for n in ('draw_motion_mesh_connection.json','draw_motion_mesh_verification.json','overlay_draw_motion_formats_verification.json')]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 mainbytes=motion.previous.previous.state.gate.support.morph.MAIN.read_bytes();overlay=motion.previous.previous.state.gate.support.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(overlay)]=overlay
 selected=list(previous.previous.previous.cases());records=[];coverage=set();targets=collections.Counter();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(draw_flags=0x10,semantic=True),dict(type=7,bank=1,draw_flags=0x10,animated=True,tail=0x40,semantic=True),dict(draw_flags=4,semantic=True),dict(draw_flags=0x10,null_position=True,semantic=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in motion.CONNECTED)
  if (n+1)%32==0:print(n+1,'mesh-connected original initialized-model comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Mesh-connected original-model smoke passed:',len(records));return
 candidate=set(motion.previous.previous.state.gate.CANDIDATE);stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2)
 assert '0x8c621574' not in stops and targets[motion.PREP]>0 and targets[motion.WRAP]>0
 files=[Path(__file__),PROJECT/'verify_draw_motion_mesh.py',PROJECT/'verify_overlay_draw_motion_formats.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),direct_service_cases=sum(c.get('direct',False) for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),independent_distance_prefixes=sum(r['independent_distance_prefixes'] for r in records),new_motion_mesh_rows_reached=len(set(motion.OWN)&coverage),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0,dll_sha256=digest(motion.DLL),sha256={str(p):digest(p) for p in files},scope='Partial native original object drawing through original motion mesh preprocessing and complete existing material/primitive queue chain. Full reached state/ordered entries and queue events/state; original constructor-created model and animation records retained.',limitations=['Supplied renderer globals, valid view/basis/light records and 256 runtime texture descriptors. Renderer scratch storage is isolated in f00000..ffffff, preserving original object heap; no natural renderer/texture/player initialization or physical sampling claim.','Other draw services remain explicit boundaries; parent recovery credit stays zero pending all paths. Legacy lower ABI/valid-RAM/numerical/queue support limits remain.','No physical GPU pixels, whole frame/gameplay/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_motion_mesh_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','candidate_unexecuted_rows','boundary_comparisons','independent_cases','new_motion_mesh_rows_reached','queue_flushes','queue_events','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
