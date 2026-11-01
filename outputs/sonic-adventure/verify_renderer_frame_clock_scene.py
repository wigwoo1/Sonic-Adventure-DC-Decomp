"""Qualify consecutive original frame calls using explicit persistent time."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_input_tick_clock_scene.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())"
assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
def change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
change('import renderer_input_tick_clock_fixtures as q','import renderer_frame_clock_fixtures as q\nimport frame_clock_native as clock_model')
code=code.replace('renderer_input_tick_clock_scene_verification.json','renderer_frame_clock_scene_verification.json').replace('renderer_input_tick_clock_scene_frontier.json','renderer_frame_clock_scene_frontier.json').replace('build/renderer_input_tick_mmio.dll','build/renderer_frame_clock.dll')
change("if entry==0x8c6050b0:c.update(ticks=list(q.FRAME_TICKS),runtime_attached=True,irq_samples=[8]*256)","if entry==0x8c6050b0:assert c.get('frame_clock_seed') is not None and 'ticks' not in c and 'irq_samples' not in c and not c['scan_samples']")
change("records.append(dict(input_tick_calls=","records.append(dict(frame_clock_out=result['frame_clock_out'],frame_clock_trace=result['frame_clock_trace'],input_tick_calls=")
change("  c['frame_events_seed']=result['frame_events_state']","  if result['frame_clock_out'] is not None:c['frame_clock_seed']=result['frame_clock_out']\n  c['frame_events_seed']=result['frame_events_state']")
marker=' frame_counter=q.get(base[0],q.get(base[0],0x8c60520c))'
change(marker,"""
 # Attach after the real display/resource/draw producers have returned normally.
 # The mode comes from their actual output, not invented guest configuration.
 registers=base[3]['graphics_in'].arrays[0]
 def reg(offset):return registers[offset//4]
 state=clock_model.initialized(50000000,27000000,reg(0x44),reg(0xd0),reg(0xd8),reg(0xcc),reg(0xdc))
 base[3]['frame_clock_seed']=clock_model.serialize(state)
 base[3]['runtime_attached']=True
 base[3].pop('ticks',None);base[3].pop('irq_samples',None);base[3]['scan_samples']=[]
 timer=phase(0x8c6025e8,[],'original timer initializer on persistent TMU/scan state')
 exact(timer['frame_clock_out']['timer']['tstr']==1,'original initializer starts TMU0')
 exact(timer['frame_clock_out']['timer']['tcr']&7==2,'original initializer selects PCLK divide by64')
"""+marker)
change(" exact(frame['runtime_state'][8]==2 and frame['runtime_state'][9:11]==(0,0),'two complete IRQ/RTE paths with no pending borrowed interrupt frame')"," exact(frame['runtime_state'][9:11]==(0,0),'complete IRQ/RTE paths retain no pending borrowed interrupt frame')\n exact(frame['frame_clock_out']['host_frames']==1,'one explicit host frame advance')\n exact(frame['frame_clock_out']['in_events']==1+frame['frame_clock_out']['idle_waits'],'each scheduling advance crosses its configured VBlank comparison')")
marker=" phase(q.get(base[0],child+0x18),[child],'actual stored child cleanup')"
change(marker,"""
 for frame_index in range(1,4):
  previous_clock=dict(base[3]['frame_clock_seed'])
  previous_deliveries=base[3]['runtime_seed'][8]
  previous_counter=q.get(base[0],q.get(base[0],0x8c60520c))
  phase(q.get(base[0],child+0x14),[child],'consecutive original particle draw '+str(frame_index))
  next_frame=phase(0x8c6050b0,[],'consecutive original whole frame '+str(frame_index))
  now=next_frame['frame_clock_out']
  exact(now['host_frames']==frame_index+1,'one explicit host advance per whole-frame call')
  exact(now['in_events']==now['host_frames']+now['idle_waits'],'configured event count follows scheduling advances')
  exact(now['timer']['peripheral_cycles']>previous_clock['timer']['peripheral_cycles'],'shared timer advances between original frames')
  exact(now['timer']['tstr']==1,'original timer remains running across public calls')
  exact(q.get(base[0],q.get(base[0],0x8c60520c))==(previous_counter+1)&0xffffffff,'independent consecutive frame counter')
  exact(next_frame['runtime_state'][8]-previous_deliveries==records[-1]['IRQ_vector_calls'],'each interrupt delivery executes the original vector')
  exact(next_frame['runtime_state'][9:11]==(0,0),'consecutive calls retain no interrupted borrowed frame')
  exact(sum(b['target']=='0x8c646868' for b in next_frame['boundaries'])==24,'all24 input status accessors execute in every consecutive frame')
  exact(q.get(base[0],0x8c8a30c0)==0,'original submission clears its busy flag every frame')
  exact(not any(b['target']=='0x8c650b60' and b['pr'] in ('0x8c651d92','0x8c651eae') for b in next_frame['boundaries']),'consecutive frame avoids SDK watchdog recovery')
"""+marker)
marker="P/'disc_inventory.json',asset,"
change(marker,"P/'disc_inventory.json',asset,P/'verify_renderer_frame_clock_scene.py',P/'renderer_frame_clock_fixtures.py',P/'renderer_frame_clock_reference.py',P/'renderer_frame_clock_reference_code.py',P/'renderer_frame_clock_comparison.py',P/'frame_clock_reference_cpu.py',P/'source/sa_renderer_frame_clock_connected.c',P/'source/renderer_frame_clock_probe.c',P/'build_renderer_frame_clock.ps1',P/'source/frame_clock.c',P/'source/frame_clock.h',P/'frame_clock_native.py',P/'frame_clock_reference.py'," )
code=code.replace('CPU_executor_unchanged=True','CPU_executor_unchanged=False,CPU_instruction_semantics_unchanged=True,explicit_host_and_idle_scheduler_domain=True')
code=code.replace("dict(whole_6050b0_frame_control_flow_return_qualified=True,","dict(consecutive_whole_frame_calls=4,automatic_phase_timer_samples_used=False,automatic_phase_scan_samples_used=False,automatic_phase_IRQ_samples_used=False,guest_instruction_cycle_accuracy_qualified=False,whole_6050b0_frame_control_flow_return_qualified=True,")
code=code.replace('External GPU identity/display-mode/scan/IRQ samples andcaller arena/scene inputs remain declared;','External GPU identity/display-mode inputs and caller arena/scene inputs remain declared; startup retains legacy profiles, then original timer initialization and consecutive frames use persistent explicit elapsed PCLK and register-derived VBlank events;')
code=code.replace('not full hardware continuity,reset boot,scheduler,frame consumption ordisplayed pixels.','not physical hardware continuity, reset boot, instruction-cycle timing or displayed pixels.')
exec(compile(code,str(parent),'exec'),globals())
