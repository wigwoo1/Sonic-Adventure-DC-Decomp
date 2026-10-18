"""Full original view builder with real native services and explicit register writes."""
import collections,itertools,json,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_view_registers as registers
import verify_draw_geometry_stream as geometry
from verify_overlay_selector_initializer import load
PROJECT=Path(__file__).resolve().parent;ENTRY=geometry.VIEW_BUILD
routine=next(r for r in geometry.manifest['routines'] if int(r['address'],16)==ENTRY)

def cases():
 for bank,size in itertools.product(range(2),((1,1),(640,480),(1920,1080),(7680,4320))):yield dict(entry=ENTRY,mode=bank*4,width=size[0],height=size[1],distance=320,check_view_copies=True)
 for mode in range(8):yield dict(entry=ENTRY,mode=mode,width=640,height=480,distance=320,check_view_copies=True)
 for alias,bank in itertools.product((0x0c000000,0xac000000),range(2)):yield dict(entry=ENTRY,mode=bank*4,width=640,height=480,distance=320,alias=alias,check_view_copies=True)
 for bank,failure in itertools.product(range(2),('null_bus','reject_register')):yield dict(entry=ENTRY,mode=bank*4,width=640,height=480,distance=320,check_view_copies=True,**{failure:True})
 for stack in (0,0x8d000004):yield dict(entry=ENTRY,mode=0,width=640,height=480,distance=320,bad_stack=stack,check_view_copies=True)
 for bank in range(2):yield dict(entry=ENTRY,mode=bank*4,rm=2,width=640,height=480,distance=320,check_view_copies=True)

def main():
 proofs=[PROJECT/'analysis/draw_view_registers_connection.json',PROJECT/'analysis/draw_view_registers_verification.json',PROJECT/'analysis/draw_view_setup_verification.json',PROJECT/'analysis/draw_geometry_stream_translation.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert registers.machine.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((registers.previous.previous.SEED.state.gate.support.morph.MAIN,0x10000),(registers.previous.previous.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:1]
 records=[];coverage=set();edges=set();targets=collections.Counter()
 for i,c in enumerate(selected):
  m,s,f=geometry.prepare(ram,c);s.sr=0x600003f3;registers.machine.extra(c,ENTRY+c['mode'])
  if c.get('alias'):s.r[15]=(s.r[15]&0xffffff)|c['alias'];c['gbr']=(c['gbr']&0xffffff)|c['alias']
  if 'bad_stack' in c:s.r[15]=c['bad_stack']
  initial=registers.effects.State.from_buffer_copy(s);r,path,actual,final,_,mem=registers.compare(m,s,f,c)
  if r['status']==0:
   assert final.r[15]==initial.r[15] and final.pr==initial.pr and all(final.r[n]==initial.r[n] for n in range(9,15)),(c,'independent view callee-saved registers')
   assert r['register_writes']==[[7,0xa05f8078,load(m,0x8c604d2c)&0x7fffffff,4]] and len(r['independent_copy_lengths'])==2,(c,'independent exact register write and two native copies')
   screen=load(m,0x8c604ce0)
   for source,dest in ((4,12),(8,16)):
    expected=registers.previous.previous.round_fraction(Fraction(registers.previous.previous.value(load(m,screen+source)))/2)
    assert load(actual,screen+dest)==expected,(c,'independent screen half-size',source)
   assert load(actual,screen)==load(m,0x8c604cec),(c,'independent original default screen distance')
   r['independent_checks']=5
  else:r['independent_checks']=0
  records.append(r);coverage.update(path);targets.update(a for a in path if a in registers.effects.CONNECTED)
  for j,a in enumerate(path[:-1]):
   if a not in geometry.PER_BODY[ENTRY]:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    k=j+(2 if w>>8 in (0x8d,0x8f) else 1)
    if k<len(path):edges.add((a,path[k]))
  print(i+1,'view-builder comparison',r['status'],r['stop_pc'],flush=True)
 if '--smoke' in sys.argv:print('View-builder smoke passed:',len(records));return
 own=set(geometry.PER_BODY[ENTRY]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
 files=[Path(__file__),PROJECT/'verify_draw_view_registers.py',PROJECT/'verify_draw_geometry_stream.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=geometry.manifest['source'],source_sha256=registers.machine.digest(PROJECT/geometry.manifest['source']),routines=[routine],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(own&coverage),rows_static=len(own),conditional_edges_covered=len(wanted&edges),missing_rows=[hex(a) for a in sorted(own-coverage)],missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)],target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(r['boundary_comparisons'] for r in records),independent_checks=sum(r['independent_checks'] for r in records),independent_copies=sum(len(r['independent_copy_lengths']) for r in records),register_write_events=sum(len(r['register_writes']) for r in records),newly_registered_entries=0,external_game_service_fixtures=0,dll_sha256=registers.machine.digest(registers.effects.DLL),sha256={str(p):registers.machine.digest(p) for p in files},scope='Complete original view builder with actual native reset/default-record/packing/header/slot/word-copy services and explicit software register effects. Full reached/final RAM, machine, both integer banks, MACH/MACL/FPU/FPUL/GBR, complete register map at entries/final state and ordered register/queue events. Independent screen half-sizes/default distance, exact register value, preserved registers/stack and two captured 192-byte native copies.',limitations=['Explicit screen/renderer inputs and existing checked initializer preparation, not naturally scheduled full engine startup, loading or physical GPU pixels. Dimensions through 8K prove this state routine only.','Qualified successful entry uses privileged bank 1, FPU enabled, scalar PR=0/SZ=0 and inherited rounding/RAM domains. PR guards, paired-move overwritten-return guards, host-memory and missing/rejected-bus stops preserve reached state; they do not emulate all hardware exceptions.','Software register store represents values and write/read order in a bounded window. No read-only/status-register behavior, command execution, rasterization, timing, interrupts or hardware acknowledgement is claimed.','The full AL_MAIN renderer/gameplay, PC FPS/8K output and settings, HD textures, Android/stereo-3DS and demakes remain incomplete.'],cases=records)
 assert len(records)==28 and report['rows_covered']==report['rows_static']==166 and report['conditional_edges_covered']==6 and not report['missing_rows'] and not report['missing_edges']
 assert report['status_counts']=={0:14,-1:6,-2:2,-3:6} and report['independent_checks']==70 and report['independent_copies']==28 and report['register_write_events']==16
 (PROJECT/'analysis/draw_view_complete_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','rows_static','conditional_edges_covered','missing_rows','missing_edges','independent_copies','register_write_events')},indent=2))
if __name__=='__main__':main()
