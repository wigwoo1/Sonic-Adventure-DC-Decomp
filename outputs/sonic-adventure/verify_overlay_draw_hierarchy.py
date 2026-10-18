"""Original initialized-object drawing with connected hierarchy/matrix bodies."""
import collections,ctypes,hashlib,json,sys
from pathlib import Path
import verify_overlay_draw_motion_mesh as previous
import verify_draw_hierarchy as hierarchy
PROJECT=Path(__file__).resolve().parent
prepare=previous.prepare
CURRENT={}

def reference_run(memory,state,f,c,tracked=()):
 ref=hierarchy.QueueReference();ref.ram=memory;hierarchy.namespace['queue_reference']=ref;CURRENT['reference']=ref
 return hierarchy.reference_run(memory,state,f,c,tracked)

def native_call(*args):
 mem=hierarchy.Memory();bus=hierarchy.Bus();hierarchy.LIB.sa_vertex_bus_init(ctypes.byref(mem),args[0],ctypes.byref(bus))
 status=hierarchy.fn(*args,ctypes.byref(bus));CURRENT['native']=mem;return status

compare_code=previous.previous.previous.previous.compare_code
old_hash='hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()'
assert compare_code.count(old_hash)==1
compare_code=compare_code.replace(old_hash,'ram_digest(mem)')
ns=dict(previous.ns,fn=native_call,reference_run=reference_run,CONNECTED=hierarchy.CONNECTED,ram_digest=hierarchy.ram_digest)
exec(compile(compare_code,'hierarchy_original_caller','exec'),ns)
compare_state=ns['compare']

def compare(m,s,f,c):
 record,path,b=compare_state(m,s,f,c);native,ref=CURRENT['native'],CURRENT['reference']
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'original caller queue state/events')
 record.update(queue_match=True,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped)
 return record,path,b

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 names=('draw_hierarchy_connection.json','overlay_draw_motion_mesh_verification.json') if '--smoke' in sys.argv else ('draw_hierarchy_connection.json','draw_hierarchy_verification.json','overlay_draw_motion_mesh_verification.json')
 proofs=[PROJECT/'analysis'/n for n in names]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 mainbytes=hierarchy.seed.state.gate.support.morph.MAIN.read_bytes();overlay=hierarchy.seed.state.gate.support.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(overlay)]=overlay
 selected=list(previous.previous.previous.previous.cases());records=[];coverage=set();targets=collections.Counter();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(draw_flags=0x10,semantic=True),dict(type=7,bank=1,draw_flags=0x10,animated=True,tail=0x40,semantic=True),dict(draw_flags=4,semantic=True),dict(draw_flags=0x10,null_position=True,semantic=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in hierarchy.CONNECTED)
  if (n+1)%32==0:print(n+1,'hierarchy-connected original initialized-model comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Hierarchy-connected original-model smoke passed:',len(records));return
 candidate=set(hierarchy.seed.state.gate.CANDIDATE);stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2)
 assert '0x8c0583a0' not in stops and targets[hierarchy.PREP]>0 and targets[hierarchy.GEN]>0
 files=[Path(__file__),PROJECT/'verify_draw_hierarchy.py',PROJECT/'verify_overlay_draw_motion_mesh.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),direct_service_cases=sum(c.get('direct',False) for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),independent_distance_prefixes=sum(r['independent_distance_prefixes'] for r in records),new_hierarchy_rows_reached=len(set(hierarchy.OWN)&coverage),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0,dll_sha256=digest(hierarchy.DLL),sha256={str(p):digest(p) for p in files},scope='Partial native original object drawing through original hierarchy generation, cache retrieval, matrix/motion and existing material/primitive queue chain. Full reached state, ordered entries and bounded queue traces; original constructor-created model and animation records retained.',limitations=['Original-model/runtime profile is inherited unchanged from verify_overlay_draw_motion_mesh.py, including supplied renderer globals and runtime texture descriptors. Natural initialization and physical sampling remain unproved.','All 16 MiB are hashed at each ordered entry without a temporary RAM copy; the comparison scope is unchanged. Queue traces store/compare the first 4096 events per call, then compare event counts, final queue/QACR/counters and full reached/final guest RAM.','Other draw services remain explicit boundaries; parent recovery credit stays zero. Unsupported layout research stop and existing valid-RAM/numerical limitations remain. No physical GPU pixels, whole frame/gameplay/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_hierarchy_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','candidate_unexecuted_rows','boundary_comparisons','independent_cases','new_hierarchy_rows_reached','queue_flushes','queue_events','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
