"""Persistent original/native view-to-initializer sequence and warm reinitialization."""
import collections,ctypes,itertools,json,sys
from pathlib import Path
import verify_draw_frame_context as context
import verify_draw_geometry_stream as geometry
from verify_overlay_selector_initializer import load
PROJECT=Path(__file__).resolve().parent;VIEW=geometry.VIEW_BUILD;FRAME=geometry.FRAME
effects=context.effects;registers=context.previous.previous

def run_chain(ram,bank,alias):
 c0=dict(entry=VIEW,mode=bank*4,alias=alias);actual,s,f=context.previous.prepare(ram,c0);s.r[15]=(s.r[15]&0xffffff)|alias
 expected=bytearray(actual);other=effects.State.from_buffer_copy(s);ef=effects.Fpu.from_buffer_copy(f)
 memory=registers.Memory();bus=effects.Bus();view=effects.RAM_TYPE.from_buffer(actual);effects.LIB.sa_render_register_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));reference=registers.RegisterReference();reference.ram=expected
 ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(0);extra=effects.Extra();extra.mach=c0['mach'];extra.other[:]=c0['other'];records=[];paths=[];events=[];pending=[];packet_checks=[];errors=[]
 def check_packet(mem):
  if not pending:return
  output,words=pending.pop()
  got=[load(mem,output+i*4) for i in range(15)]
  if got!=words:errors.append('independent background packet mismatch')
  packet_checks.append(1)
 @effects.OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  events.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,effects.ram_digest(mem),gb.value,extra.mach,tuple(extra.other),tuple(memory.registers)))
  try:
   if target==0x8c605480:check_packet(mem)
   if target==context.COMPILE:
    assert not pending
    pointers=[load(mem,r[4]+i*4) for i in range(3)]
    words=[load(mem,load(mem,literal)) for literal in (0x8c6615e0,0x8c6615e4,0x8c6615e8)]
    for p in pointers:
     assert p&0x1f000000==0x0c000000 and (p&0xffffff)+28<=16<<20
     words.extend(load(mem,p+off) for off in (4,8,12,24))
    pending.append((load(mem,0x8c6615ec),words))
  except Exception as exc:errors.append(repr(exc))
 for ordinal,entry in enumerate((VIEW,FRAME,FRAME)):
  # Model ordinary external calls; no guest function or result is substituted.
  s.pc=other.pc=entry;s.pr=other.pr=0x8cfed000+ordinal*4;events.clear();before=len(reference.events);packets_before=len(packet_checks)
  c=dict(entry=entry,mode=bank*4,alias=alias,macl=ml.value,gbr=gb.value,mach=extra.mach,other=list(extra.other))
  context.namespace['queue_reference']=reference
  status,path,wanted,expected_ml,expected_gb,expected_mh,expected_hidden,live=effects.reference_run(expected,other,ef,c,tracked=tuple(effects.CONNECTED))
  rc=effects.fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,effects.Fpu.fpul.offset),effects.core.P),ctypes.byref(ml),entry,None,observe,ctypes.byref(bus),ctypes.byref(gb),ctypes.byref(extra))
  assert rc==status==0 and events==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_ml and gb.value==expected_gb and extra.mach==expected_mh and list(extra.other)==expected_hidden,('persistent sequence',bank,hex(alias),hex(entry),rc,status,hex(s.pc),hex(other.pc),len(events),len(wanted))
  v=memory.vertex;trace=[(e.kind,e.address,e.value,e.width) for e in v.trace[:v.trace_count]]
  assert trace==reference.events[:4096] and v.trace_dropped==max(0,len(reference.events)-4096) and list(v.queue)==reference.queue and list(v.qacr)==reference.qacr and (v.flushes,v.prefetches,v.invalidates)==(reference.flushes,reference.prefetches,reference.invalidates)
  assert list(memory.registers)==[reference.registers.get(0x005f8000+i*4,0) for i in range(2048)]
  assert not errors and not pending,(errors,pending)
  checks=0
  if entry==VIEW:
   assert load(actual,0x8c88fcb4)==load(actual,0x8c6051bc),('actual original view producer selects record zero',bank,alias)
   checks=1
  else:
   assert len(packet_checks)-packets_before==1,('background packet actually compiled',bank,alias)
   assert all(load(actual,load(actual,literal))==value for literal,value in ((0x8c639074,1),(0x8c639078,0),(0x8c63907c,128),(0x8c639080,load(actual,0x8c05461c))))
   assert load(actual,0x8c88fcb4)==load(actual,0x8c6051bc) and gb.value==load(actual,0x8c036fb4)
   checks=3
  stage_events=reference.events[before:];writes=[list(e) for e in stage_events if e[0]==7]
  records.append(dict(entry=hex(entry),status=rc,stop_pc=hex(s.pc),steps=len(path),full_state_match=True,ordered_boundaries_match=True,register_queue_match=True,independent_checks=checks,independent_background_packets=len(packet_checks)-packets_before,boundary_comparisons=len(events),register_writes=writes,bank0_live_in=[[hex(a),i] for a,i in live]));paths.append(path)
  print('persistent',bank,hex(alias),ordinal+1,'stage passed',hex(entry),'register writes',len(writes),flush=True)
 return dict(floating_bank=bank,stack_alias=hex(alias),stages=records,all_state_carried=True,independent_background_packets=len(packet_checks),events=len(reference.events),stored_event_prefix=min(4096,len(reference.events)),unstored_event_count=max(0,len(reference.events)-4096)),paths

def main():
 proofs=[PROJECT/'analysis/draw_frame_context_translation.json',PROJECT/'analysis/draw_frame_context_verification.json',PROJECT/'analysis/draw_frame_context_connection.json',PROJECT/'analysis/draw_frame_setup_verification.json',PROJECT/'analysis/draw_view_complete_verification.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert registers.machine.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((context.previous.previous.previous.previous.SEED.state.gate.support.morph.MAIN,0x10000),(context.previous.previous.previous.previous.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 selected=list(itertools.product(range(2),(0x8c000000,0x0c000000,0xac000000)))
 if '--smoke' in sys.argv:selected=selected[:1]
 records=[];coverage=set();edges=set();targets=collections.Counter()
 for bank,alias in selected:
  r,paths=run_chain(ram,bank,alias);records.append(r)
  for path in paths:
   coverage.update(path);targets.update(a for a in path if a in effects.CONNECTED)
   for j,a in enumerate(path[:-1]):
    if a not in geometry.PER_BODY[FRAME]:continue
    w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
    if w>>8 in (0x89,0x8b,0x8d,0x8f):
     k=j+(2 if w>>8 in (0x8d,0x8f) else 1)
     if k<len(path):edges.add((a,path[k]))
 if '--smoke' in sys.argv:print('Persistent view/frame/warm smoke passed');return
 cold=[]
 for bank in range(2):
  c=dict(entry=FRAME,mode=bank*4);r,*_=context.compare_state(*context.previous.prepare(ram,c),c)
  assert r['status']==-3 and r['stop_pc']=='0x8c60548e' and not r['register_writes']
  cold.append(r)
 own=set(geometry.PER_BODY[FRAME]);routine=next(r for r in geometry.manifest['routines'] if int(r['address'],16)==FRAME);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
 files=[Path(__file__),PROJECT/'verify_draw_frame_context.py',PROJECT/'verify_draw_frame_setup.py',PROJECT/'verify_draw_view_registers.py',PROJECT/'analysis/draw_geometry_stream_translation.json']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=geometry.manifest['source'],source_sha256=registers.machine.digest(PROJECT/geometry.manifest['source']),routines=[routine],sequence_cases_passed=len(records),stage_comparisons=sum(len(r['stages']) for r in records),view_producer_returns=sum(s['entry']==hex(VIEW) for r in records for s in r['stages']),initializer_returns=sum(s['entry']==hex(FRAME) for r in records for s in r['stages']),warm_initializer_returns=len(records),rows_covered=len(own&coverage),rows_static=len(own),conditional_edges_covered=len(wanted&edges),missing_rows=[hex(a) for a in sorted(own-coverage)],missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)],independent_checks=sum(s['independent_checks'] for r in records for s in r['stages']),independent_background_packets=sum(r['independent_background_packets'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(s['boundary_comparisons'] for r in records for s in r['stages']),register_write_events=sum(len(s['register_writes']) for r in records for s in r['stages']),stored_event_prefix=sum(r['stored_event_prefix'] for r in records),unstored_event_count=sum(r['unstored_event_count'] for r in records),cold_missing_producer_cases=cold,newly_registered_entries=0,external_game_service_fixtures=0,original_boot_call_order_proved=False,dll_sha256=registers.machine.digest(effects.DLL),sha256={str(p):registers.machine.digest(p) for p in files},scope='Six persistent original/native view-producer -> initializer -> repeated-initializer sequences, carrying RAM, CPU/FPU/FPUL, SR, both integer banks, MACH/MACL/GBR, queues/counters and complete software register map without reset. Full initializer code reached, ordered states/events and independent producer pointer/stack metadata/background packet checks. Two cold-entry tests demonstrate the actual missing-producer guard.',limitations=['This proves the real RAM producer and persistent tested call sequence, not naturally executed boot call order, loader/scheduler behavior or a complete game frame. Only PC/PR are set at ordinary external call boundaries.','Qualified successful CPU state is privileged bank 1, FD=0, scalar PR=0/SZ=0 and inherited numerical/MMU-off RAM domains. Prepared loaded MAIN/AL_MAIN RAM and initial CPU state remain explicit fixtures.','Graphics backend stores register effects and command words; it does not consume GPU commands, emulate status/interrupt/timing or render pixels.','Other local math/state proposal paths retain their own incomplete qualification; this initializer exercises real native callees with the original arguments, without substituted game services.','Complete gameplay, FPS/8K/display/HD settings, Android/stereo-3DS and later demakes remain unfinished.'],cases=records)
 assert len(records)==6 and report['stage_comparisons']==18 and report['view_producer_returns']==6 and report['initializer_returns']==12 and report['warm_initializer_returns']==6
 assert report['rows_covered']==report['rows_static']==141 and report['conditional_edges_covered']==0 and not report['missing_rows'] and not report['missing_edges']
 assert report['independent_checks']==42 and report['independent_background_packets']==12 and report['register_write_events']==report['stored_event_prefix']==1578 and report['unstored_event_count']==0
 (PROJECT/'analysis/draw_frame_sequence_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('sequence_cases_passed','stage_comparisons','initializer_returns','rows_covered','rows_static','conditional_edges_covered','missing_rows','missing_edges','independent_checks','independent_background_packets','register_write_events')},indent=2))
if __name__=='__main__':main()
