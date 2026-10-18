"""Accepted original-model draw paths through the native distance dependency."""
import collections,ctypes,hashlib,inspect,itertools,json,sys
from pathlib import Path
import verify_overlay_draw_state as previous
import verify_draw_distance_helpers as distance
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
DIST,SQRT=distance.DIST,distance.SQRT
CONNECTED=previous.CONNECTED|{DIST,SQRT};ROWS=dict(previous.ROWS);ROWS.update(distance.ROWS)
MISSING_TARGETS=previous.MISSING_TARGETS-{DIST,SQRT}
def reference_vector(word,f):
 if word&0xf0ff==0xf06d:return distance.reference_sqrt(word,f)
 return previous.reference_vector(word,f)
namespace=dict(previous.namespace,ROWS=ROWS,MISSING_TARGETS=MISSING_TARGETS,sqrt_probe=distance.sqrt_probe,reference_vector=reference_vector)
code=previous.code;marker='  scalar=w>>12==15';assert code.count(marker)==1
code=code.replace(marker,'  if w&0xf0ff==0xf06d:\n   rc,probe=sqrt_probe(w,f)\n   if rc:stop(pc,r,sr,rc,probe)\n'+marker)
exec(compile(code,'overlay_draw_distance_reference','exec'),namespace)
reference_run=namespace['reference_run']
DLL=distance.ROOT/'work/sonic-adventure/build/overlay_draw_distance.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_draw_distance_connected
fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
VIEW_POSITION=0x8cc70000;POSITION_SLOT=0x8c18b7ec
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def cases():
 yield from previous.cases()
 for t,bank,z,angles in itertools.product(range(8),range(2),(0,4,16),(False,True)):
  yield dict(type=t,bank=bank,point=(0,0,z),angles=angles,draw_flags=0x10,semantic=True)
 for t,bank,animated,tail in itertools.product(range(8),range(2),(False,True),(0,0x40)):
  yield dict(type=t,bank=bank,draw_flags=0x10,animated=animated,tail=tail,semantic=True)
 for bank,animated in itertools.product(range(2),(False,True)):
  yield dict(bank=bank,draw_flags=0x10,animated=animated,null_position=True,semantic=True)
def prepare(ram,c):
 m,s,f=previous.prepare(ram,c)
 store(m,POSITION_SLOT,0 if c.get('null_position') else VIEW_POSITION)
 for i,v in enumerate(c.get('reference_position',(1,2,3))):store(m,VIEW_POSITION+0x20+i*4,distance.morph.bits(v))
 if c.get('animated'):store(m,previous.gate.ENTITY+0x11a,load(m,previous.gate.ENTITY+0x11a,2)|1,2)
 if c.get('tail'):store(m,previous.gate.APP+0x36,load(m,previous.gate.APP+0x36,2)|c['tail'],2)
 return m,s,f
# Reuse the proven complete-state/boundary/independent global and stack checks,
# with the new DLL/executor and an extra independent distance-tail boundary.
compare_code=inspect.getsource(previous.compare)
compare_code=compare_code.replace('float_words=None;depth=1;', 'float_words=None;distance_prefixes=[];pending=[];depth=1;')
marker='   if target==0x8c056990:';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,"""   if target==DIST and all((r[i]&0x1f000000)==0xc000000 and (r[i]&0xffffff)+12<=0x1000000 for i in (4,5)):
    rm=fp.contents.fpscr&1
    a=[load(mem,r[4]+i*4) for i in range(3)];b=[load(mem,r[5]+i*4) for i in range(3)]
    delta=[distance.arithmetic.add_sub(x,y,True,rm)[0] for x,y in zip(a,b)]
    x,y,z=delta;acc=distance.arithmetic.multiply(y,y,rm)[0]
    acc=distance.arithmetic.multiply_add(x,x,acc,rm)[0];acc=distance.arithmetic.multiply_add(z,z,acc,rm)[0]
    pending.append(acc)
   if target==SQRT and pending:
    assert fp.contents.banks[fp.contents.fpscr>>21&1][4]==pending.pop(),'independent caller distance accumulation'
    distance_prefixes.append(True)
"""+marker)
compare_code=compare_code.replace('independent_state_fields=checked,','independent_state_fields=checked,independent_distance_prefixes=len(distance_prefixes),')
ns=dict(previous.__dict__,fn=fn,reference_run=reference_run,CONNECTED=CONNECTED,DIST=DIST,SQRT=SQRT,distance=distance)
exec(compile(compare_code,'distance_observed_compare','exec'),ns)
compare=ns['compare']
def main():
 proofs=[PROJECT/'analysis'/n for n in ('overlay_draw_distance_connection.json','draw_distance_helpers_verification.json','overlay_draw_state_verification.json')]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 mainbytes=distance.morph.MAIN.read_bytes();binary=distance.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(binary)]=binary
 selected=list(cases());records=[];coverage=set();targets=collections.Counter();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(draw_flags=0x10,semantic=True),dict(type=7,bank=1,draw_flags=0x10,animated=True,tail=0x40,semantic=True),dict(draw_flags=4,semantic=True),dict(draw_flags=0x10,null_position=True,semantic=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  if (n+1)%64==0:print(n+1,'distance-connected original draw comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Distance-connected draw smoke passed:',len(records));return
 candidate=set(previous.gate.CANDIDATE);stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2)
 assert '0x8c640128' not in stops and targets[DIST]>0 and targets[SQRT]>0
 files=[Path(__file__),PROJECT/'verify_overlay_draw_state.py',PROJECT/'verify_draw_distance_helpers.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),direct_service_cases=sum(c.get('direct',False) for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),independent_distance_prefixes=sum(r['independent_distance_prefixes'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Partial native original draw callback with distance/signed-square-root connected. Full reached original state and ordered entries plus independent state/matrix-stack/spring/distance-accumulation checks. Shipped initialized objects, supplied runtime position record, animation flag, appearance-tail and angle inputs.',limitations=['Candidate remains incomplete and unregistered. No game-service fixtures; unconnected services remain explicit -2 stops. Null position records retain expected partial-memory stops.','The position record at global 8c18b7ec and its contents are supplied runtime data, not proof of natural player/task initialization or gameplay reachability.','Inherits legacy narrow-FPU/valid-RAM and qualified scalar/vector/FSQRT support policies. No physical pixels, whole frame, high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_distance_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','direct_service_cases','status_counts','candidate_rows_covered','candidate_unexecuted_rows','boundary_comparisons','independent_cases','independent_distance_prefixes','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
