"""Whole original/native scene with real native pixel work before completion."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_render_completion.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())

def pixel_change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)

code=code.replace('import renderer_render_completion_fixtures as q','import renderer_pixel_jobs_fixtures as q\nimport pixel_jobs_native as job_model').replace('renderer_render_completion_verification.json','renderer_pixel_jobs_verification.json').replace('renderer_render_completion_frontier.json','renderer_pixel_jobs_frontier.json')
pixel_change("  if c.get('render_completion_seed') is not None:c['backend_completed_job']=c['render_completion_seed']['active_id'] if entry==0x8c6050b0 else 0","  c['backend_completed_job']=0")
pixel_change('backend_acknowledgements_are_declared_inputs=True,pixel_renderer_implemented=False,','backend_acknowledgements_are_declared_inputs=False,native_pixel_work_precedes_completion=True,pixel_renderer_implemented=True,physical_PowerVR_pixels_qualified=False,visible_retail_texture_rendering_qualified=False,')
pixel_change('records.append(dict(render_completion_out=',"records.append(dict(pixel_job_summary=result['pixel_job_summary'],render_completion_out=")
pixel_change("  if result['render_completion_out'] is not None:c['render_completion_seed']=result['render_completion_out']","  if result['pixel_job_out'] is not None:c['pixel_job_seed']=result['pixel_job_out']\n  if result['render_completion_out'] is not None:c['render_completion_seed']=result['render_completion_out']")
pixel_change(" base[3]['render_completion_seed']=render_model.serialize(render_state)"," base[3]['render_completion_seed']=render_model.serialize(render_state)\n base[3]['pixel_job_seed']=job_model.initial()")
marker=" phase(q.get(base[0],child+0x18),[child],'actual stored child cleanup');exact(q.get(base[0],actor+0x10)==0,'actual particle cleanup')"
pixel_change(marker,marker+'''
 final_pixels=base[3]['pixel_job_seed'].summary()
 exact(final_pixels['processed']==4 and final_pixels['last_job']==4 and not final_pixels['ready'],'all four actual submitted jobs complete through native pixel work')
 exact(base[3]['render_completion_seed']['completed']==4,'completion count follows actual successful native work, including final queued job')
 exact(final_pixels['stats'][3]==4 and final_pixels['stats'][4]==4,'final native job samples four textured fragments')
''')
pixel_change("P/'disc_inventory.json',asset,","P/'disc_inventory.json',asset,"+','.join("P/'"+name+"'" for name in ('verify_renderer_pixel_jobs.py','renderer_pixel_jobs_fixtures.py','renderer_pixel_jobs_reference.py','renderer_pixel_jobs_reference_code.py','renderer_pixel_jobs_state.py','renderer_pixel_jobs_comparison.py','write_watch_renderer_pixel_jobs_base.py','write_watch_renderer_pixel_jobs_state.py','write_watch_renderer_pixel_jobs_capture.py','write_watch_renderer_pixel_jobs.py','source/pixel_jobs.c','source/pixel_jobs.h','pixel_jobs_native.py','pixel_jobs_reference.py','source/pvr_pixels.c','source/pvr_pixels.h','pvr_pixels_native.py','pvr_pixels_reference.py'))+",R/'work/sonic-adventure/build/pixel_jobs.dll',")
code=code.replace('Explicit matching backend job acknowledgements are supplied between frames; this tests the host renderer lifecycle and original IRQ handling, without implementing a pixel backend. Not physical hardware continuity, reset boot, instruction-cycle timing or displayed pixels.','Actual native pixel work on captured accepted packets precedes each completion event; complete native/reference colour and depth buffers, job ownership and original IRQ state are compared. Host debug background and fixed-point precision remain explicit; captured textures are transparent in this fixture. Not physical hardware pixels/timing, visible retail texture qualification, reset boot, real-time FPS or a playable port.')
exec(compile(code,str(parent),'exec'),globals())
