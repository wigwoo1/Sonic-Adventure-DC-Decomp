"""Original object/model drawing on the explicit-bank complete clipping DLL."""
import collections,ctypes,hashlib,itertools,json,random,sys
from pathlib import Path
import verify_draw_clipping_complete as effects
import verify_overlay_draw_angle_blend as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def compare(m,s,f,c):
 other=effects.State.from_buffer_copy(s);ef=effects.Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=effects.QueueReference();ref.ram=expected;effects.namespace['queue_reference']=ref
 status,path,wanted,ml_expected,gb_expected,mh_expected,hidden,live=effects.reference_run(expected,other,ef,c,tracked=tuple(effects.CONNECTED))
 memory=effects.Memory();bus=effects.Bus();view=effects.RAM_TYPE.from_buffer(actual);effects.LIB.sa_vertex_bus_init(ctypes.byref(memory),view,ctypes.byref(bus))
 ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c['gbr']);extra=effects.Extra();extra.mach=c['mach'];extra.other[:]=c['other'];events=[]
 @effects.OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  events.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,effects.ram_digest(mem),gb.value,extra.mach,tuple(extra.other)))
 rc=effects.fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,effects.Fpu.fpul.offset),effects.core.P),ctypes.byref(ml),s.pc,None,observe,ctypes.byref(bus),ctypes.byref(gb),ctypes.byref(extra))
 assert rc==status and events==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==ml_expected and gb.value==gb_expected and extra.mach==mh_expected and list(extra.other)==hidden,(c,'full object machine/RAM/boundaries',rc,status,hex(s.pc),hex(other.pc),len(events),len(wanted))
 emitted=[(e.kind,e.address,e.value,e.width) for e in memory.trace[:memory.trace_count]]
 assert emitted==ref.events[:4096] and memory.trace_dropped==max(0,len(ref.events)-4096) and list(memory.queue)==ref.queue and list(memory.qacr)==ref.qacr and (memory.flushes,memory.prefetches,memory.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'ordered object queues/cache')
 record=dict(input=dict(c),status=rc,stop_pc=hex(s.pc),steps=len(path),bank0_live_in=[[hex(a),i] for a,i in live],full_state_match=True,ordered_boundaries_match=True,queue_match=True,boundary_comparisons=len(events),queue_flushes=memory.flushes,queue_events=memory.trace_count+memory.trace_dropped)
 return record,path,actual,s,f,gb.value

def extra(c,seed):
 rng=random.Random(seed);c.update(mach=rng.getrandbits(32),other=[rng.getrandbits(32) for _ in range(8)])

def stage(m,entry):
 _,s,f=effects.SEED.prepare(m,dict(entry=entry,mode=0));s.pc=entry;s.sr=0x600003f3
 c=dict(entry=entry,gbr=0,preparation=True);extra(c,entry)
 record,path,actual,*_=compare(m,s,f,c)
 assert record['status']==0 and len(path)==(41 if entry==effects.INIT else 51)
 return actual

def prepare(ram,c):
 m,s,f=previous.constructor.prepare(ram,c);s.sr=0x600003f3
 if c.get('renderer_view_profile'):
  screen=load(m,0x8c604e44)
  for off,x in ((0,c['viewport_distance']),(4,c['viewport_width']),(8,c['viewport_height'])):store(m,screen+off,effects.bits(x))
  for a,x in ((0x8c88f61c,1),(0x8c88f610,1),(0x8c88f614,100)):store(m,a,effects.bits(x))
  m=stage(m,previous.effects.previous.previous.previous.previous.geometry.VIEW_RESET)
 m=stage(m,effects.INIT);c['gbr']=load(m,effects.ORIGINAL_GBR_LITERAL);extra(c,0x38516+c.get('bank',0));c['entry']=s.pc
 return m,s,f

def cases():
 prior=json.loads((PROJECT/'analysis/overlay_draw_vector_blend_verification.json').read_text())
 for i,r in enumerate(prior['cases']):yield dict(r['input'],prior_108_index=i)
 for typ,bank,z,tail in itertools.product((0,7),range(2),(.25,.5,.75,1,1.5,2),(0,0x40)):
  yield dict(type=typ,bank=bank,point=(0,0,z),draw_flags=0x10,animated=True,tail=tail,renderer_view_profile=True,viewport_width=640,viewport_height=480,viewport_distance=320,viewport_reset_stage=True,semantic=True,near_plane_profile=True)

def main():
 proofs=[PROJECT/'analysis'/n for n in ('draw_clipping_complete_verification.json','overlay_draw_angle_blend_verification.json','overlay_draw_vector_blend_verification.json')]
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((effects.SEED.state.gate.support.morph.MAIN,0x10000),(effects.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[selected[0],next(c for c in selected if c.get('near_plane_profile')),selected[-1]]
 records=[];coverage=set();targets=collections.Counter()
 for n,c in enumerate(selected):
  record,path,*_=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);targets.update(a for a in path if a in effects.CONNECTED)
  print(n+1,'object clipping comparison passed',record['status'],record['stop_pc'],'clippers',sum(a in effects.NEW for a in path),flush=True)
 if '--smoke' in sys.argv:print('Current-DLL object smoke passed:',len(records));return
 files=[Path(__file__),PROJECT/'verify_overlay_draw_angle_blend.py',PROJECT/'verify_overlay_draw_motion_sampling.py',PROJECT/'verify_draw_clipping_complete.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 candidate=set(effects.SEED.state.gate.CANDIDATE)
 report=dict(cases_passed=len(records),preserved_prior_corpus_cases=108,near_plane_profiles=sum(bool(c.get('near_plane_profile')) for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_static_rows=911,candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),clipping_rows_reached=len(set(effects.OWN)&coverage),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),normal_bank1_live_in=sorted({tuple(x) for r in records for x in r['bank0_live_in']}),boundary_comparisons=sum(r['boundary_comparisons'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),native_original_initializer_stages=len(records),native_original_viewport_stages=sum(bool(c.get('renderer_view_profile')) for c in selected),newly_registered_entries=0,recovered_caller_credit=0,external_game_service_fixtures=0,dll_sha256=digest(effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original constructor-created object/model/motion records through current explicit-bank/MACH native drawing pipeline. All 108 preceding vector-stage inputs and additional saved near-plane camera/entity profiles. Each real initializer/viewport preparation stage is separately original/native checked. Full reached/final machine/RAM/banks/MACH/GBR and ordered bounded queues.',limitations=['Camera/entity state, renderer scratch, runtime texture metadata and initial CPU banks are supplied fixtures; natural startup/scheduling/loading and GPU output remain unproved.','No new independent geometry assertions are claimed for the outer object corpus. Separate complete clipping-family proof retains its own independent geometry checks.','Floating CPU entry explicitly privileged bank 1 and FD=0. Inherited valid-RAM/MMU-off and numerical domains; typed guard/stop statuses are not complete hardware exception handling.','First 4096 queue events per object invocation are individually compared, followed by exact later counts and final full RAM/queue/QACR/counters. Preparation stages are counted separately from object events.','Whole AL_MAIN draw parent remains partial, zero recovery credit. Full gameplay, high FPS/8K, settings and ports remain incomplete.'],cases=records)
 (PROJECT/'analysis/overlay_draw_clipping_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','clipping_rows_reached','first_unconnected_targets','normal_bank1_live_in')},indent=2))

if __name__=='__main__':main()
