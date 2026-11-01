"""Original frame continuity under declared backend acknowledgements, no pixels."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_frame_rendering.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
def completion_change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
code=code.replace('import renderer_frame_rendering_fixtures as q','import renderer_render_completion_fixtures as q\nimport render_completion_native as render_model').replace('renderer_frame_rendering_verification.json','renderer_render_completion_verification.json').replace('renderer_frame_rendering_frontier.json','renderer_render_completion_frontier.json')
completion_change('records=[];seen=set();','records=[];backend_controls=[];seen=set();')
completion_change('phases=records,','phases=records,backend_controls=backend_controls,backend_control_comparisons=len(backend_controls),backend_acknowledgements_are_declared_inputs=True,pixel_renderer_implemented=False,')
completion_change("  if entry==q.TEXTURE_LOAD:c['instruction_budget']=25000000","  if c.get('render_completion_seed') is not None:c['backend_completed_job']=c['render_completion_seed']['active_id'] if entry==0x8c6050b0 else 0\n  if entry==q.TEXTURE_LOAD:c['instruction_budget']=25000000")
completion_change('records.append(dict(pvr_register_writes=',"records.append(dict(render_completion_out=result['render_completion_out'],backend_completed_job=c.get('backend_completed_job',0),pvr_register_writes=")
completion_change("  if result['frame_clock_out'] is not None:c['frame_clock_seed']=result['frame_clock_out']","  if result['render_completion_out'] is not None:c['render_completion_seed']=result['render_completion_out']\n  if result['frame_clock_out'] is not None:c['frame_clock_seed']=result['frame_clock_out']")
marker=' frame_counter=q.get(base[0],q.get(base[0],0x8c60520c))'
completion_change(marker,"""
 actual_registers=base[3]['graphics_in'].arrays[0]
 render_state=render_model.initialized([actual_registers[offset//4] for offset in render_model.OFFSETS])
 base[3]['render_completion_seed']=render_model.serialize(render_state)
"""+marker)
marker=" exact(frame['frame_clock_out']['host_frames']==1,'one explicit host frame advance')"
completion_change(marker,marker+"\n exact(frame['render_completion_out']['submitted']==1 and frame['render_completion_out']['active_id']==1 and frame['render_completion_out']['completed']==0,'actual first render start creates an unfinished backend job')")
marker=' for frame_index in range(1,4):'
completion_change(marker,"""
 # Ablate only the host completion device, keeping all game state and guards.
 cm=bytearray(base[0]);cs=q.v.effects.State.from_buffer_copy(base[1]);cf=q.v.effects.Fpu.from_buffer_copy(base[2]);cc=dict(base[3])
 cs.pc=cc['entry']=0x8c6050b0;cs.pr=0x8cfed000
 cc['render_completion_seed']=dict(cc['render_completion_seed']);cc['render_completion_seed']['enabled']=0;cc['backend_completed_job']=0
 control,path,cm,cs,cf=q.fast.compare(cm,cs,cf,cc)
 exact(control['status']==0 and control['full_state_match'] and control['ordered_boundaries_match'],'disabled-completion baseline matches the original normally')
 exact(path.count(0x8c651bc0)==12,'disabled completion reproduces twelve original VBlank services')
 exact(control['render_completion_out']['completed']==0 and control['render_completion_out']['pending']==0,'disabled device never fabricates backend completion')
 exact(any(x['pc']=='0x8c651be6' and x['after']==11 for x in control['pacing_stores']),'disabled baseline retains original counter11 wait')
 backend_controls.append(dict(kind='completion_device_disabled',status=control['status'],steps=len(path),VBlank_handlers=path.count(0x8c651bc0),full_state_match=True,ordered_boundaries_match=True,matched_return_state_discarded=True,render_completion_out=control['render_completion_out'],pacing_stores=control['pacing_stores']))
 print('disabled render completion control',control['status'],path.count(0x8c651bc0),flush=True)
"""+marker)
marker="  exact(now['host_frames']==frame_index+1,'one explicit host advance per whole-frame call')"
completion_change(marker,marker+"""
  render=next_frame['render_completion_out']
  exact(render['submitted']==frame_index+1 and render['completed']==frame_index and render['active_id']==frame_index+1,'one acknowledged prior job and one actual new render submission per frame')
  exact(render['pending']==0 and render['cancelled']==0,'original IRQ code acknowledges completion without cancellation')
  exact(records[-1]['VBlank_handler_calls']==2,'backend acknowledgement removes the eleven-tick list wait')
  exact(not any(x['pc']=='0x8c651be6' and x['after']>2 for x in next_frame['pacing_stores']),'normal job ownership avoids the counter11 release path')
""")
completion_change("P/'disc_inventory.json',asset,","P/'disc_inventory.json',asset,P/'verify_renderer_render_completion.py',P/'renderer_render_completion_fixtures.py',P/'renderer_render_completion_reference.py',P/'renderer_render_completion_reference_code.py',P/'renderer_render_completion_comparison.py',P/'source/render_completion.c',P/'source/render_completion.h',P/'source/renderer_render_completion_probe.c',P/'build_renderer_render_completion.ps1',P/'render_completion_native.py',P/'render_completion_reference.py',")
code=code.replace('build/renderer_frame_clock.dll','build/renderer_render_completion.dll')
code=code.replace('not physical hardware continuity, reset boot, instruction-cycle timing or displayed pixels.','Explicit matching backend job acknowledgements are supplied between frames; this tests the host renderer lifecycle and original IRQ handling, without implementing a pixel backend. Not physical hardware continuity, reset boot, instruction-cycle timing or displayed pixels.')
exec(compile(code,str(parent),'exec'),globals())
