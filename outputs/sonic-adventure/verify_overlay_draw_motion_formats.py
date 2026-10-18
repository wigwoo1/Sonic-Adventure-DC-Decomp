"""Original initialized model drawing through native animation formats/curves."""
import collections,hashlib,json,sys
from pathlib import Path
import verify_overlay_draw_motion as previous
import verify_draw_motion_formats as motion
PROJECT=Path(__file__).resolve().parent
fn=motion.fn;reference_run=motion.reference_run;CONNECTED=motion.CONNECTED
ns=dict(previous.ns,fn=fn,reference_run=reference_run,CONNECTED=CONNECTED)
exec(compile(previous.previous.compare_code,'motion_formats_caller_comparison','exec'),ns)
compare=ns['compare']

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 proofs=[PROJECT/'analysis'/n for n in ('draw_motion_formats_connection.json','draw_motion_formats_verification.json','overlay_draw_motion_verification.json')]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 mainbytes=motion.previous.state.gate.support.morph.MAIN.read_bytes();overlay=motion.previous.state.gate.support.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(overlay)]=overlay
 selected=list(previous.previous.cases());records=[];coverage=set();targets=collections.Counter();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(draw_flags=0x10,semantic=True),dict(type=7,bank=1,draw_flags=0x10,animated=True,tail=0x40,semantic=True),dict(draw_flags=4,semantic=True),dict(draw_flags=0x10,null_position=True,semantic=True)]
 for n,c in enumerate(selected):
  m,s,f=previous.prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  if (n+1)%64==0:print(n+1,'animation-connected original model comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Animation-connected draw smoke passed:',len(records));return
 candidate=set(motion.previous.state.gate.CANDIDATE);stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2)
 assert '0x8c60d094' not in stops and targets[0x8c60d094]>0
 files=[Path(__file__),PROJECT/'verify_draw_motion_formats.py',PROJECT/'verify_overlay_draw_motion.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),direct_service_cases=sum(c.get('direct',False) for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),independent_distance_prefixes=sum(r['independent_distance_prefixes'] for r in records),new_animation_rows_reached=len(set(motion.OWN)&coverage),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0,dll_sha256=digest(motion.DLL),sha256={str(p):digest(p) for p in files},scope='Partial native original draw callback through format and keyframe interpolation. Full reached original state and ordered entries, inherited independent global/stack/spring/distance checks. Native initialized shipped objects and supplied valid renderer/position records.',limitations=['Distinct motion mesh renderer and other draw services remain explicit -2 boundaries; no fabricated service results. Parent remains unregistered.','Renderer/position records, drawing flags and animation states are supplied runtime inputs, not proof of natural scene/player initialization or physical GPU output.','Inherits bounded valid-RAM, legacy narrow-FPU and qualified numerical support. No full frame, gameplay, high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_motion_formats_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','candidate_unexecuted_rows','boundary_comparisons','independent_cases','independent_distance_prefixes','new_animation_rows_reached','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
