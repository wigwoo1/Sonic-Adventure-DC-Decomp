"""Actual original initializer progress with explicit unresolved dependencies."""
import collections,itertools,json,sys
from pathlib import Path
from verify_overlay_selector_initializer import load
import verify_draw_frame_setup as body
import verify_draw_geometry_stream as geometry
PROJECT=Path(__file__).resolve().parent;ENTRY=geometry.FRAME

def main():
 proofs=[body.MANIFEST,PROJECT/'analysis/draw_frame_setup_verification.json',PROJECT/'analysis/draw_frame_dependencies_connection.json',PROJECT/'analysis/draw_view_complete_verification.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert body.previous.machine.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((body.previous.previous.previous.SEED.state.gate.support.morph.MAIN,0x10000),(body.previous.previous.previous.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 records=[];coverage=set();targets=collections.Counter()
 selected=[dict(entry=ENTRY,mode=bank*4) for bank in range(2)]
 selected.extend(dict(entry=ENTRY,mode=bank*4,alias=alias) for alias,bank in itertools.product((0x0c000000,0xac000000),range(2)))
 selected.extend(dict(entry=ENTRY,mode=0,bad_stack=stack) for stack in (0,0x8d000004))
 if '--smoke' in sys.argv:selected=selected[:1]
 for i,c in enumerate(selected):
  m,s,f=body.prepare(ram,c)
  if 'bad_stack' in c:s.r[15]=c['bad_stack']
  r,path,actual,*_=body.compare_state(m,s,f,c)
  if 'bad_stack' not in c:
   assert r['status']==-2 and r['stop_pc']=='0x8c661300',(c,r['status'],r['stop_pc'])
   expected=((0x8c639074,1),(0x8c639078,0),(0x8c63907c,128),(0x8c639080,load(m,0x8c05461c)))
   assert all(load(actual,load(m,literal))==value for literal,value in expected),(c,'real initialized matrix-stack metadata')
   screen=load(m,0x8c054624)
   assert load(actual,screen+4)==load(m,0x8c054628) and load(actual,screen+8)==load(m,0x8c05462c),(c,'original initialized screen dimensions')
   r['independent_checks']=2
  else:
   assert r['status']==-3 and r['stop_pc']==hex(ENTRY)
   r['independent_checks']=0
  records.append(r);coverage.update(path);targets.update(a for a in path if a in body.effects.CONNECTED)
  print(i+1,'actual initializer comparison',r['status'],r['stop_pc'],flush=True)
  if '--smoke' in sys.argv:print('Last original instructions:',[(hex(a),body.ROWS.get(a)) for a in path[-16:]],flush=True)
 if '--smoke' in sys.argv:print('Initializer smoke completed:',len(records));return
 own=set(geometry.PER_BODY[ENTRY]);files=[Path(__file__),PROJECT/'verify_draw_frame_setup.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),first_unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),parent_static_rows=len(own),parent_rows_covered=len(own&coverage),parent_unexecuted_rows=len(own-coverage),dependency_rows_reached=len(set(body.OWN)&coverage),target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(r['boundary_comparisons'] for r in records),register_write_events=sum(len(r['register_writes']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),newly_registered_entries=0,parent_recovery_credit=0,external_game_service_fixtures=0,dll_sha256=body.previous.machine.digest(body.effects.DLL),sha256={str(p):body.previous.machine.digest(p) for p in files},scope='Actual original initialization entry through native matrix-stack, camera/math, descriptor and register-wrapper dependencies, compared to the first unresolved compile-table callback. Full reached/final machine/RAM/banks/MACH/GBR/register storage and ordered boundaries; independent real stack metadata and original screen dimensions.',limitations=['Six valid scalar privileged bank-1/FD=0 prefixes and two invalid-stack guards. A stop at the real callback is not a successful whole initializer return; parent recovery credit remains zero.','Baseline MAIN/AL_MAIN bytes are loaded into a bounded zero-initialized RAM fixture. No whole boot/loader scheduling, real framebuffer/GPU command execution or hardware timing proof.','The remaining compiled math/state proposals are exercised only where reached here; their untested paths and deeper dependencies are not granted complete-function credit.','Existing qualified numerical/vector/software register-store and MMU-off RAM domains apply. Typed host stops are not full exception delivery.'],cases=records)
 assert len(records)==8 and report['status_counts']=={-2:6,-3:2} and report['first_unconnected_targets']=={'0x8c661300':6} and report['independent_checks']==12
 (PROJECT/'analysis/draw_frame_prefix_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','first_unconnected_targets','parent_rows_covered','parent_static_rows','dependency_rows_reached')},indent=2))
if __name__=='__main__':main()
