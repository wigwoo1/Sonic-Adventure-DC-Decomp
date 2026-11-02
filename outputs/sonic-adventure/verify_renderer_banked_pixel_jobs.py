"""Qualify visible retail texture work and Texture Off in the complete scene."""
from pathlib import Path
P=Path(__file__).resolve().parent;builder=P/'verify_renderer_pixel_jobs.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
def bank_change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
code=code.replace('import renderer_pixel_jobs_fixtures as q','import renderer_banked_pixel_jobs_fixtures as q').replace('import pixel_jobs_native as job_model','import pixel_banked_native as job_model').replace('renderer_pixel_jobs_verification.json','renderer_banked_pixel_jobs_verification.json').replace('renderer_pixel_jobs_frontier.json','renderer_banked_pixel_jobs_frontier.json')
bank_change('visible_retail_texture_rendering_qualified=False,','visible_retail_texture_rendering_qualified=True,bank_linear_to_texture_view_adapter_used=True,')
bank_change('captured textures are transparent in this fixture.','the bank-linear game storage is converted to the texture address view, whose mip texels match a unique retail archive entry. This fixture produces two textured pixels and four vertex/material pixels with sampling Off.')
marker=' for frame_index in range(1,4):'
bank_change(marker,'''
 # A normal original/native call with the external host Texture Off option.
 om=bytearray(base[0]);os=q.v.effects.State.from_buffer_copy(base[1]);of=q.v.effects.Fpu.from_buffer_copy(base[2]);oc=dict(base[3])
 os.pc=oc['entry']=q.get(om,child+0x14);os.pr=0x8cfed000;os.r[4]=child;oc['backend_completed_job']=0
 seed=oc['pixel_job_seed'];oc['pixel_job_seed']=job_model.Seed(seed.state,seed.pixels,seed.depth,seed.options[:4]+(0,)+seed.options[5:])
 off,off_path,om,os,of=q.fast.compare(om,os,of,oc)
 exact(off['status']==0 and off['full_state_match'] and off['ordered_boundaries_match'],'Texture Off call retains complete original/native state and ordered equality')
 off_summary=off['pixel_job_summary'];off_pixels=memoryview(off['pixel_job_out'].pixels).cast('I')
 exact(off_summary['processed']==1 and off_summary['stats'][3]==0 and sum(x!=0xff101018 for x in off_pixels)==4,'Texture Off actually produces four vertex/material-coloured pixels without sampling')
 backend_controls.append(dict(kind='Texture_Off_vertex_material_colours',status=off['status'],steps=len(off_path),full_state_match=True,ordered_boundaries_match=True,matched_return_state_discarded=True,pixel_job_summary=off_summary))
 print('Texture Off matched control',off['status'],off_summary['stats'],flush=True)
'''+marker)
marker=" exact(final_pixels['stats'][3]==4 and final_pixels['stats'][4]==4,'final native job samples four textured fragments')"
bank_change(marker,marker+"\n exact(sum(x!=0xff101018 for x in memoryview(base[3]['pixel_job_seed'].pixels).cast('I'))==2,'final actual native job produces two visible retail-textured pixels')")
files=('verify_renderer_banked_pixel_jobs.py','renderer_banked_pixel_jobs_fixtures.py','renderer_banked_pixel_jobs_reference.py','renderer_banked_pixel_jobs_comparison.py','renderer_banked_jobs_state.py','write_watch_renderer_banked_pixel_jobs_base.py','write_watch_renderer_banked_pixel_jobs_state.py','write_watch_renderer_banked_pixel_jobs_capture.py','write_watch_renderer_banked_pixel_jobs.py','source/banked_pixel_jobs.c','source/banked_pixel_jobs.h','build_banked_pixel_jobs.ps1','pixel_banked_native.py','pixel_banked_reference.py','analysis/banked_vram_view_verification.json','analysis/banked_pixel_jobs_unit_verification.json','analysis/banked_texture_output_verification.json')
bank_change("P/'disc_inventory.json',asset,","P/'disc_inventory.json',asset,"+','.join("P/'"+name+"'" for name in files)+",R/'work/sonic-adventure/build/banked_pixel_jobs.dll',")
exec(compile(code,str(parent),'exec'),globals())
