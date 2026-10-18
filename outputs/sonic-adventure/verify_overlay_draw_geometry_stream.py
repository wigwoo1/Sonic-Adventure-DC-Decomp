"""Original model callbacks with real renderer initialization and explicit GBR."""
import collections,ctypes,hashlib,itertools,json,sys
from pathlib import Path
import verify_overlay_draw_gbr_geometry as previous
import verify_overlay_draw_hierarchy as template
import verify_draw_geometry_stream as effects
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;CURRENT={}

def viewport_runtime(ram):
 _,state,f=effects.SEED.prepare(ram,dict(entry=effects.VIEW_RESET,mode=0));state.pc=effects.VIEW_RESET
 other=effects.State.from_buffer_copy(state);ef=effects.Fpu.from_buffer_copy(f);expected=bytearray(ram);actual=bytearray(ram)
 ref=effects.QueueReference();ref.ram=expected;effects.namespace['queue_reference']=ref
 rc,path,_,macl,gbr=effects.reference_run(expected,other,ef,dict(gbr=0),tracked=())
 native=effects.Memory();bus=effects.Bus();view=effects.RAM_TYPE.from_buffer(actual);effects.LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(0)
 status=effects.fn(view,ctypes.byref(state),ctypes.byref(f),ctypes.cast(ctypes.byref(f,effects.Fpu.fpul.offset),effects.SEED.state.gate.support.morph.P),ctypes.byref(ml),effects.VIEW_RESET,None,effects.OBSERVE(),ctypes.byref(bus),ctypes.byref(gb))
 assert status==rc==0 and len(path)==51 and actual==expected and bytes(state)==bytes(other) and bytes(f)==bytes(ef) and ml.value==macl and gb.value==gbr==0
 return actual

def prepare(ram,c):
 m,s,f=previous.previous.prepare(ram,c)
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


def main():
 names=('draw_geometry_stream_connection.json','overlay_draw_gbr_geometry_verification.json','overlay_draw_motion_variants_verification.json') if '--smoke' in sys.argv else ('draw_geometry_stream_connection.json','draw_geometry_stream_verification.json','overlay_draw_gbr_geometry_verification.json','overlay_draw_motion_variants_verification.json')
 proofs=[PROJECT/'analysis'/n for n in names]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 seed=effects.SEED;mainbytes=seed.state.gate.support.morph.MAIN.read_bytes();overlay=seed.state.gate.support.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(overlay)]=overlay
 corpus=list(cases());prior=json.loads((PROJECT/'analysis/overlay_draw_gbr_geometry_verification.json').read_text());original=json.loads((PROJECT/'analysis/overlay_draw_motion_variants_verification.json').read_text())
 assert json.loads(json.dumps(corpus))==[r['input'] for r in original['cases']]
 assert prior['original_corpus_cases']==len(corpus)==420
 affected={i for i,r in zip(prior['selected_prior_case_indices'],prior['cases'][:146]) if r['status']==-2 and int(r['stop_pc'],16) in effects.NEW}
 assert len(affected)==48
 selected_indices=prior['selected_prior_case_indices'];selected=[corpus[i] for i in selected_indices]+list(camera_profiles())
 assert len(selected)==210
 if '--smoke' in sys.argv:selected=[dict(draw_flags=0x10,semantic=True),dict(type=7,bank=1,draw_flags=0x10,animated=True,tail=0x40,semantic=True),dict(draw_flags=0x12,label_name='A',label_alpha=80,semantic=True),next(camera_profiles()),list(camera_profiles())[-1]]
 records=[];coverage=set();targets=collections.Counter();boundaries=0
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in effects.CONNECTED)
  if (n+1)%32==0:print(n+1,'Geometry-stream-connected original-model comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Geometry-stream original-model smoke passed:',len(records));return
 candidate=set(seed.state.gate.CANDIDATE);stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2)
 assert not ({'0x8c037294','0x8c60a61a'}&set(stops))
 assert targets[effects.PROJECT_VERTICES]>0 and targets[effects.ROTATION]>0
 files=[Path(__file__),PROJECT/'verify_draw_geometry_stream.py',PROJECT/'verify_overlay_draw_gbr_geometry.py',PROJECT/'verify_overlay_draw_motion_sampling.py',PROJECT/'verify_overlay_draw_motion_variants.py',PROJECT/'verify_overlay_draw_hierarchy.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),original_corpus_cases=len(corpus),selected_prior_case_indices=selected_indices,prior_selected_cases_retested=len(selected_indices),affected_prior_cases_retested=len(affected),additional_camera_profiles=sum(bool(c.get('renderer_view_profile')) for c in selected),direct_service_cases=sum(bool(c.get('direct')) for c in selected),label_event_profiles=sum('label_name' in c for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),independent_distance_prefixes=sum(r['independent_distance_prefixes'] for r in records),new_geometry_rows_reached=len(set(effects.OWN)&coverage),native_original_initializer_stages=len(records),native_original_viewport_stages=sum(bool(c.get('viewport_reset_stage')) for c in selected),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),external_game_service_fixtures=0,newly_recovered_entries=0,recovered_caller_credit=0,reused_routine_credit=0,dll_sha256=digest(effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original initialized-model callbacks with real native/original 41-instruction renderer initialization, explicit GBR in ordered/final state, earlier 146 affected/regression cases retained and 64 camera/animation/position profiles using real native/original viewport-reset preparation from explicit 640x480 screen inputs. Full reached/final machine/RAM and bounded queues compared; original model/motion/texture-name records retained.',limitations=['Renderer initializer runs as an explicit checked preparation stage using supplied existing globals, not proof of natural whole-engine startup scheduling. Extra 64 profiles update the earlier camera fixture to explicit screen dimensions/distance and a checked real 51-instruction viewport-reset stage, then real GBR initialization; these are not naturally loaded camera/task events.','The earlier 420-case corpus remains preserved, not counted as 420 fresh checks. The 146 retained original-corpus cases are freshly rerun; the additional 64 entity/animation profiles have explicitly updated camera preparation; source case indices are retained. Unexecuted parent/dependency bodies remain unregistered.','Inherited qualified scalar/vector/FSCA/FSRRA/sqrt and MMU-off valid-RAM domains remain. First 4096 queue events per call are compared individually; later counts and final queue/QACR/counters/RAM checked. No physical GPU pixels, complete frame/gameplay or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_geometry_stream_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','boundary_comparisons','independent_cases','new_geometry_rows_reached','queue_flushes','queue_events','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
