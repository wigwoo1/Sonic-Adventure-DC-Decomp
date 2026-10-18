"""Real original view-builder prefixes; unresolved services remain explicit."""
import collections,itertools,json
from pathlib import Path
import verify_draw_view_setup as body
import verify_draw_geometry_stream as geometry
from verify_overlay_selector_initializer import load
PROJECT=Path(__file__).resolve().parent;ENTRY=geometry.VIEW_BUILD

def main():
 proofs=[PROJECT/'analysis/draw_view_setup_verification.json',PROJECT/'analysis/draw_view_setup_connection.json',PROJECT/'analysis/draw_geometry_stream_translation.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert body.machine.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((body.previous.SEED.state.gate.support.morph.MAIN,0x10000),(body.previous.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 records=[];coverage=set();targets=collections.Counter()
 for bank,size in itertools.product(range(2),((1,1),(640,480),(1920,1080),(7680,4320))):
  c=dict(entry=ENTRY,mode=bank*4,width=size[0],height=size[1],distance=320)
  m,s,f=geometry.prepare(ram,c);s.sr=0x600003f3;body.machine.extra(c,ENTRY+bank)
  r,path,*_=body.compare_state(m,s,f,c);records.append(r);coverage.update(path);targets.update(a for a in path if a in body.effects.CONNECTED)
  assert r['status']==-2 and r['stop_pc']=='0x8c652d3a',(c,r['status'],r['stop_pc'])
  print(len(records),'real view prefix compared; explicit stop',r['stop_pc'],flush=True)
 own=set(geometry.PER_BODY[ENTRY]);files=[Path(__file__),PROJECT/'verify_draw_view_setup.py',PROJECT/'verify_draw_geometry_stream.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),first_unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records)),parent_static_rows=len(own),parent_rows_covered=len(own&coverage),parent_unexecuted_rows=len(own-coverage),new_dependency_rows_reached=len(set(body.OWN)&coverage),target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(r['boundary_comparisons'] for r in records),queue_events=sum(r['queue_events'] for r in records),newly_registered_entries=0,view_parent_recovery_credit=0,external_game_service_fixtures=0,dll_sha256=body.machine.digest(body.effects.DLL),sha256={str(p):body.machine.digest(p) for p in files},scope='Eight actual original/native view-builder prefixes with explicit screen dimensions and actual native dependency calls. Full reached machine/RAM/banks/MACH/GBR and ordered boundaries match to the first unconnected original service 8c652d3a. Existing original/native initializer supplies GBR preparation.',limitations=['This is a checked prefix ending at an unresolved dependency, not a complete view-builder return or natural startup proof. No parent registration credit.','Saved screen/renderer state is prepared directly. Testing dimensions through 7680x4320 here is not proof of working 8K output or a playable PC port.','Physical GPU registers/pixels, natural initialization, complete frames/gameplay, numerical modes outside qualified domains and exception delivery remain unproved.'],cases=records)
 assert len(records)==8 and report['view_parent_recovery_credit']==0 and targets[body.RESET]>0 and targets[body.RECORD]>0 and targets[body.PACK]>0
 (PROJECT/'analysis/draw_view_prefix_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','parent_static_rows','parent_rows_covered','parent_unexecuted_rows','new_dependency_rows_reached','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
