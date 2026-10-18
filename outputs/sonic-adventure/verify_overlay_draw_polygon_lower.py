"""Original initialized-model drawing through original object-effect bodies."""
import collections,ctypes,hashlib,json,sys
from pathlib import Path
import verify_overlay_draw_render_headers as previous
import verify_overlay_draw_hierarchy as template
import verify_overlay_draw_distance as baseline
import verify_draw_polygon_lower as effects
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;CURRENT={}

def prepare(ram,c):
 return previous.prepare(ram,c)


def reference_run(memory,state,f,c,tracked=()):
 ref=effects.QueueReference();ref.ram=memory;effects.namespace['queue_reference']=ref;CURRENT['reference']=ref
 return effects.reference_run(memory,state,f,c,tracked)

def native_call(*args):
 mem=effects.Memory();bus=effects.Bus();effects.LIB.sa_vertex_bus_init(ctypes.byref(mem),args[0],ctypes.byref(bus))
 status=effects.fn(*args,ctypes.byref(bus));CURRENT['native']=mem;return status

ns=dict(previous.ns,fn=native_call,reference_run=reference_run,CONNECTED=effects.CONNECTED,ram_digest=effects.ram_digest)
exec(compile(template.compare_code,'object_effects_original_caller','exec'),ns);compare_state=ns['compare']

def compare(m,s,f,c):
 record,path,b=compare_state(m,s,f,c);native,ref=CURRENT['native'],CURRENT['reference']
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'original caller queue state/events')
 record.update(queue_match=True,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped)
 return record,path,b

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 names=('draw_polygon_lower_connection.json','overlay_draw_point_verification.json') if '--smoke' in sys.argv else ('draw_polygon_lower_connection.json','draw_polygon_lower_verification.json','overlay_draw_point_verification.json')
 proofs=[PROJECT/'analysis'/n for n in names]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 seed=effects.SEED;mainbytes=seed.state.gate.support.morph.MAIN.read_bytes();overlay=seed.state.gate.support.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(overlay)]=overlay
 selected=list(baseline.cases());records=[];coverage=set();targets=collections.Counter();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(draw_flags=0x10,semantic=True),dict(type=7,bank=1,draw_flags=0x10,animated=True,tail=0x40,semantic=True),dict(draw_flags=4,semantic=True),dict(draw_flags=0x10,null_position=True,semantic=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in effects.CONNECTED)
  if (n+1)%32==0:print(n+1,'polygon-connected original initialized-model comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Polygon-connected original-model smoke passed:',len(records));return
 candidate=set(seed.state.gate.CANDIDATE);stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2)
 assert '0x8c64f840' not in stops and targets[effects.CLIP]>0
 files=[Path(__file__),PROJECT/'verify_draw_polygon_lower.py',PROJECT/'verify_overlay_draw_render_headers.py',PROJECT/'verify_overlay_draw_object_effects.py',PROJECT/'verify_overlay_draw_point.py',PROJECT/'verify_overlay_draw_hierarchy.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),direct_service_cases=sum(c.get('direct',False) for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),independent_distance_prefixes=sum(r['independent_distance_prefixes'] for r in records),new_polygon_rows_reached=len(set(effects.OWN)&coverage),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0,dll_sha256=digest(effects.DLL),sha256={str(p):digest(p) for p in files},scope='Partial original initialized-model drawing through the original polygon clipping/header/projection bodies and effect dispatcher/profiles/render-service bodies and existing native mesh/point/hierarchy/software queues. Reached machine/RAM/ordered entries and bounded queue trace/state compared; Original model/motion records retained; supplied runtime metadata extended for the selected 14-name effect texture list.',limitations=['Compiled incomplete effect dispatcher/render-service bodies retain explicit missing lower-service boundaries and receive zero registration credit pending full path/dependency verification. The AL_MAIN parent stays unregistered.','Runtime renderer/scratch/view/texture metadata remain supplied inputs. Newly selected 14-name list 0c91c7b0 (names 0c91c6ec) has only runtime descriptor slots populated. Original geometry/motion/name/attribute records retained; natural initialization/loading unproved.','First 4096 queue events per invocation stored/compared individually; later counts, final queue/QACR/counters and full reached/final RAM compared. Existing scalar/vector/shared-core/MMU-off domains remain. No physical GPU pixels, complete frame/gameplay/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_polygon_lower_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','candidate_unexecuted_rows','boundary_comparisons','independent_cases','new_polygon_rows_reached','queue_flushes','queue_events','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
