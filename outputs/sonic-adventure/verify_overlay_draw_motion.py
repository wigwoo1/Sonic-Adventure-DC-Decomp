"""Original initialized model drawing through recovered motion-control bodies."""
import collections,ctypes,hashlib,inspect,json,sys
from pathlib import Path
import verify_overlay_draw_distance as previous
import verify_draw_motion_setup as motion
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
fn=motion.fn;reference_run=motion.reference_run;CONNECTED=motion.CONNECTED
# Retain all earlier independent global, distance and matrix-stack checks.
ns=dict(previous.ns,fn=fn,reference_run=reference_run,CONNECTED=CONNECTED)
exec(compile(previous.compare_code,'motion_caller_comparison','exec'),ns)
compare=ns['compare']

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def prepare(ram,c):
 m,s,f=previous.prepare(ram,c)
 # Runtime renderer records are supplied data, never substituted callee results.
 store(m,0x8c88fcb4,motion.STATE_A);store(m,0x8c88fcd4,motion.STATE_A+0x100)
 for i in range(4):store(m,motion.STATE_A+0x90+i*4,0x12345600+i)
 for i in range(5):store(m,motion.STATE_A+0x100+i*4,0xabc00000+i)
 store(m,0x8c890074,0)
 return m,s,f

def main():
 proofs=[PROJECT/'analysis'/n for n in ('draw_motion_setup_connection.json','draw_motion_setup_verification.json','overlay_draw_distance_verification.json')]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 mainbytes=motion.state.gate.support.morph.MAIN.read_bytes();overlay=motion.state.gate.support.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(overlay)]=overlay
 selected=list(previous.cases());records=[];coverage=set();targets=collections.Counter();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(draw_flags=0x10,semantic=True),dict(type=7,bank=1,draw_flags=0x10,animated=True,tail=0x40,semantic=True),dict(draw_flags=4,semantic=True),dict(draw_flags=0x10,null_position=True,semantic=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  if (n+1)%64==0:print(n+1,'motion-connected original model comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Motion-connected draw smoke passed:',len(records));return
 candidate=set(previous.previous.gate.CANDIDATE);stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2)
 assert '0x8c60e5e4' not in stops and targets[motion.SETUP]>0 and targets[motion.DRIVER]>0
 files=[Path(__file__),PROJECT/'verify_draw_motion_setup.py',PROJECT/'verify_overlay_draw_distance.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),direct_service_cases=sum(c.get('direct',False) for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),independent_distance_prefixes=sum(r['independent_distance_prefixes'] for r in records),new_motion_rows_reached=len(set(motion.OWN)&coverage),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0,dll_sha256=digest(motion.DLL),sha256={str(p):digest(p) for p in files},scope='Partial native original draw callback extended through motion setup/recursive dispatch. Complete reached original state and ordered entries, inherited independent global/stack/spring/distance checks. Shipped initialized objects and supplied valid renderer/position runtime records.',limitations=['Motion interpolation/format callbacks and motion mesh renderer remain explicit -2 boundaries. Candidate remains unregistered; no fabricated service results.','Runtime records, drawing flags and animation inputs are supplied data, not proof of natural scene/player initialization or physical GPU output.','Inherits bounded valid-RAM, legacy narrow-FPU and pinned numerical support policies. No full frame, gameplay, high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_motion_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','candidate_unexecuted_rows','boundary_comparisons','independent_cases','independent_distance_prefixes','new_motion_rows_reached','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
