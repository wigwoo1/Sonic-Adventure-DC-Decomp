"""Original model callbacks with real renderer initialization and explicit GBR."""
import collections,ctypes,hashlib,itertools,json,sys
from pathlib import Path
import verify_overlay_draw_vector_curve as previous
import verify_overlay_draw_hierarchy as template
import verify_draw_angle_curve as effects
import verify_overlay_draw_motion_sampling as constructor
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;CURRENT={}

def viewport_runtime(ram):
 _,state,f=effects.SEED.prepare(ram,dict(entry=effects.previous.previous.geometry.VIEW_RESET,mode=0));state.pc=effects.previous.previous.geometry.VIEW_RESET
 other=effects.State.from_buffer_copy(state);ef=effects.Fpu.from_buffer_copy(f);expected=bytearray(ram);actual=bytearray(ram)
 ref=effects.QueueReference();ref.ram=expected;effects.namespace['queue_reference']=ref
 rc,path,_,macl,gbr=effects.reference_run(expected,other,ef,dict(gbr=0),tracked=())
 native=effects.Memory();bus=effects.Bus();view=effects.RAM_TYPE.from_buffer(actual);effects.LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(0)
 status=effects.fn(view,ctypes.byref(state),ctypes.byref(f),ctypes.cast(ctypes.byref(f,effects.Fpu.fpul.offset),effects.SEED.state.gate.support.morph.P),ctypes.byref(ml),effects.previous.previous.geometry.VIEW_RESET,None,effects.OBSERVE(),ctypes.byref(bus),ctypes.byref(gb))
 assert status==rc==0 and len(path)==51 and actual==expected and bytes(state)==bytes(other) and bytes(f)==bytes(ef) and ml.value==macl and gb.value==gbr==0
 return actual

def prepare(ram,c):
 m,s,f=constructor.prepare(ram,c)
 if c.get('renderer_view_profile'):
  screen=load(m,0x8c604e44)
  for off,x in ((0,c['viewport_distance']),(4,c['viewport_width']),(8,c['viewport_height'])):store(m,screen+off,effects.bits(x))
  for a,x in ((0x8c88f61c,1),(0x8c88f610,1),(0x8c88f614,100)):store(m,a,effects.bits(x))
  m=viewport_runtime(m)
 m,gbr=effects.runtime(m);CURRENT['initial_gbr']=gbr
 return m,s,f


def reference_run(memory,state,f,c,tracked=()):
 ref=effects.QueueReference();ref.ram=memory;effects.namespace['queue_reference']=ref;CURRENT['reference']=ref
 status,path,events,macl,gbr=effects.reference_run(memory,state,f,dict(c,gbr=CURRENT['initial_gbr']),tracked)
 CURRENT['expected_gbr']=gbr;return status,path,events,macl

def native_call(*args):
 mem=effects.Memory();bus=effects.Bus();effects.LIB.sa_vertex_bus_init(ctypes.byref(mem),args[0],ctypes.byref(bus))
 gbr=ctypes.c_uint32(CURRENT['initial_gbr']);CURRENT['native_gbr']=gbr
 status=effects.fn(*args,ctypes.byref(bus),ctypes.byref(gbr));CURRENT['native']=mem;return status

ns=dict(previous.ns,fn=native_call,reference_run=reference_run,CONNECTED=effects.CONNECTED,ram_digest=effects.ram_digest,gbr_native=lambda:CURRENT['native_gbr'].value)
code=template.compare_code;marker='ml.value,ram_digest(mem)))';assert code.count(marker)==1
code=code.replace(marker,'ml.value,ram_digest(mem),gbr_native()))')
exec(compile(code,'gbr_original_caller_compare','exec'),ns);compare_state=ns['compare']

def compare(m,s,f,c):
 record,path,b=compare_state(m,s,f,c);native,ref=CURRENT['native'],CURRENT['reference']
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'original GBR caller queue state')
 assert CURRENT['native_gbr'].value==CURRENT['expected_gbr'],(c,'original caller final GBR')
 record.update(queue_match=True,gbr_final_and_ordered_entries_match=True,native_original_initializer_stage_match=True,native_original_viewport_stage_match=bool(c.get('viewport_reset_stage')),initialized_gbr=hex(CURRENT['initial_gbr']),queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped)
 return record,path,b

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def cases():yield from previous.cases()

def camera_profiles():
 for c in previous.camera_profiles():yield dict(c,viewport_width=640,viewport_height=480,viewport_distance=320,viewport_reset_stage=True)

def select(prior):
 records=prior['cases'];affected={i for i,r in enumerate(records) if r['status']==-2 and int(r['stop_pc'],16)==effects.ENTRY}
 assert len(records)==108 and len(affected)==48
 return list(range(len(records))),affected


def main():
 names=('draw_angle_curve_connection.json','overlay_draw_vector_curve_verification.json')
 if '--smoke' not in sys.argv:names+=('draw_angle_curve_verification.json',)
 proofs=[PROJECT/'analysis'/n for n in names]
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert digest(f)==h,f
 prior=json.loads((PROJECT/'analysis/overlay_draw_vector_curve_verification.json').read_text());indices,affected=select(prior)
 if '--smoke' in sys.argv:indices=sorted({min(affected),max(affected),0,next(i for i,r in enumerate(prior['cases']) if 'label_name' in r['input']),next(i for i,r in enumerate(prior['cases']) if r['input'].get('renderer_view_profile') and i not in affected)})
 seed=effects.SEED;ram=bytearray(16<<20)
 for p,off in ((seed.state.gate.support.morph.MAIN,0x10000),(seed.state.gate.support.morph.MODULE,0x900000)):
  b=p.read_bytes();ram[off:off+len(b)]=b
 records=[];coverage=set();targets=collections.Counter();boundaries=0
 for n,i in enumerate(indices):
  c=prior['cases'][i]['input'];m,s,f=prepare(ram,c);r,path,b=compare(m,s,f,c);records.append(r);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in effects.CONNECTED)
  if (n+1)%16==0:print(n+1,'angle-curve-connected original-model comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Angle curve original-model smoke passed:',len(records));return
 stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2);assert hex(effects.ENTRY) not in stops and targets[effects.ENTRY]>0
 files=[Path(__file__),PROJECT/'verify_draw_angle_curve.py',PROJECT/'verify_overlay_draw_vector_curve.py',PROJECT/'verify_overlay_draw_motion_sampling.py',PROJECT/'verify_overlay_draw_hierarchy.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 candidate=set(seed.state.gate.CANDIDATE)
 report=dict(cases_passed=len(records),prior_corpus_cases=108,original_source_corpus_cases=420,source_210_case_indices=prior['selected_prior_case_indices'],selected_prior_case_indices=indices,affected_prior_case_indices=sorted(affected),affected_prior_cases_retested=48,unaffected_regressions=60,direct_service_cases=sum(r['input'].get('direct',False) for r in records),label_event_profiles=sum('label_name' in r['input'] for r in records),camera_profiles=sum(r['input'].get('renderer_view_profile',False) for r in records),native_original_initializer_stages=len(records),native_original_viewport_stages=sum(r['input'].get('viewport_reset_stage',False) for r in records),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),independent_distance_prefixes=sum(r['independent_distance_prefixes'] for r in records),new_curve_rows_reached=len(set(effects.OWN)&coverage),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),external_game_service_fixtures=0,newly_recovered_entries=0,recovered_caller_credit=0,reused_routine_credit=0,dll_sha256=digest(effects.DLL),sha256={str(p):digest(p) for p in files},scope='All 48 previously blocked callback inputs plus 60 stratified unchanged regressions from the preserved 108-case vector-helper corpus. Original model/motion/name/lookup records, real native/original viewport/GBR preparation, ordered/final machine/RAM and bounded software queues compared.',limitations=['This reexecutes all 108 vector-helper corpus cases; the earlier 210 and 420 case proofs remain preserved. Source case indices and inputs are preserved explicitly. Regressions cover twenty-eight direct service/bank pairs, twelve labels/events, sixteen camera/entity pairs and four ordinary profiles.','Initialization/camera settings are checked supplied preparation, not natural startup/scheduler proof. First 4096 queue events per invocation are compared individually, later counts and final RAM/queue/QACR checked.','Inherited numerical and MMU-off valid-RAM/bus domains remain. Partial parents/clippers are unregistered. No physical GPU pixels, full gameplay/high-FPS/8K or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_angle_curve_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','boundary_comparisons','new_curve_rows_reached','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()

