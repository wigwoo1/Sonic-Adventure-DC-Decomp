"""Fresh normal producers, fault/discard controls, and consecutive normal frames."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_frame_clock_scene.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
def control_change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
code=code.replace('renderer_frame_clock_scene_verification.json','renderer_frame_clock_controls_verification.json').replace('renderer_frame_clock_scene_frontier.json','renderer_frame_clock_controls_frontier.json')
control_change('records=[];seen=set();','records=[];controls=[];seen=set();')
control_change('phases=records,','phases=records,negative_controls=controls,negative_control_comparisons=len(controls),')
marker=' frame_counter=q.get(base[0],q.get(base[0],0x8c60520c))'
control_change(marker,"""
 # Copy a normally returned public state for each independent negative input.
 # No faulted result is carried into another call or the positive sequence.
 for kind in ('disabled_clock','SR_BL','IMASK15','disabled_runtime'):
  cm=bytearray(base[0]);cs=q.v.effects.State.from_buffer_copy(base[1]);cf=q.v.effects.Fpu.from_buffer_copy(base[2]);cc=dict(base[3])
  cc['entry']=cs.pc=0x8c6050b0;cs.pr=0x8cfed000
  cc['frame_clock_seed']=dict(cc['frame_clock_seed'])
  before=cc['runtime_seed'];before_time=cc['frame_clock_seed']['timer']['peripheral_cycles']
  if kind=='disabled_clock':cc['frame_clock_seed']['enabled']=0
  elif kind=='SR_BL':cs.sr|=0x10000000
  elif kind=='IMASK15':cs.sr|=0xf0
  else:cc['runtime_seed']=(*before[:-1],0)
  control,path,cm,cs,cf=q.fast.compare(cm,cs,cf,cc)
  print('negative observation',kind,control['status'],control['stop_pc'],'IRQ deliveries',control['runtime_state'][8]-before[8],'VBlank handlers',path.count(0x8c651bc0),flush=True)
  exact(control['status']==-4,('unavailable clock/interrupt retains a bounded typed wait',kind,control['status'],control['stop_pc']))
  exact(control['full_state_match'] and control['ordered_boundaries_match'],('complete negative state and boundaries',kind))
  if kind=='disabled_clock':
   exact(control['runtime_state'][8]-before[8]==path.count(cc['vbr']+0x600) and path.count(0x8c651bc0)==0,'transfer IRQ may complete without any VBlank handler or time advance')
  else:exact(control['runtime_state'][8]==before[8],('no interrupt delivered under masked or disabled delivery',kind))
  clock=control['frame_clock_out']
  if kind=='disabled_clock':exact(clock['timer']['peripheral_cycles']==before_time and clock['in_events']==0,'disabled clock creates no time or events')
  else:exact(clock['timer']['peripheral_cycles']>before_time and clock['pending']&8,'host time and pending hardware events remain independent of delivery')
  if kind in ('SR_BL','IMASK15'):exact(control['runtime_state'][9]>before[9],('masked requests are counted as blocked',kind))
  controls.append(dict(kind=kind,status=control['status'],stop_pc=control['stop_pc'],steps=len(path),full_state_match=True,ordered_boundaries_match=True,partial_fault_state_discarded=True,runtime_state=control['runtime_state'],frame_clock_out=clock))
  print('negative control',kind,control['status'],control['stop_pc'],len(path),flush=True)
"""+marker)
control_change("P/'disc_inventory.json',asset,","P/'disc_inventory.json',asset,P/'verify_renderer_frame_clock_controls.py',P/'analysis/renderer_frame_clock_initial_source_preservation.json',")
exec(compile(code,str(parent),'exec'),globals())
