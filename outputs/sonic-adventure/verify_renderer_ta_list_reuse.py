"""Twelve original/native frames using the game's own list-init reuse strobes."""
from pathlib import Path
P=Path(__file__).resolve().parent;builder=P/'verify_renderer_banked_pixel_jobs.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
def reuse_change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
code=code.replace('import renderer_banked_pixel_jobs_fixtures as q','import renderer_ta_list_reuse_fixtures as q').replace('renderer_banked_pixel_jobs_verification.json','renderer_ta_list_reuse_verification.json').replace('renderer_banked_pixel_jobs_frontier.json','renderer_ta_list_reuse_frontier.json')
reuse_change('phases=records,','phases=records,collector_reuse_from_original_list_init_strobes=True,collector_capacity=64,')
reuse_change('records.append(dict(pixel_job_summary=',"records.append(dict(ta_list_out=result['ta_list_out'],ta_current_packets=result['ta_current_packets'],ta_packet_capacity=result['ta_packet_capacity'],pixel_job_summary=")
reuse_change("  if result['pixel_job_out'] is not None:c['pixel_job_seed']=result['pixel_job_out']","  c['ta_list_seed']=result['ta_list_out']\n  if result['pixel_job_out'] is not None:c['pixel_job_seed']=result['pixel_job_out']")
reuse_change(' for frame_index in range(1,4):',' for frame_index in range(1,12):')
reuse_change("  previous_clock=dict(base[3]['frame_clock_seed'])","  previous_lists=dict(base[3]['ta_list_seed'])\n  previous_clock=dict(base[3]['frame_clock_seed'])")
marker="  exact(now['host_frames']==frame_index+1,'one explicit host advance per whole-frame call')"
reuse_change(marker,marker+'''
  lists=next_frame['ta_list_out']
  exact(lists['inits']==previous_lists['inits']+1 and lists['retired']==previous_lists['retired']+9 and lists['discarded']==0,'one actual guest list-init retires exactly the submitted nine-packet context')
  exact(next_frame['ta_current_packets']==0 and next_frame['ta_packet_capacity']==64 and next_frame['pixel_job_summary']['cursor']==0,'fixed collector and consumer cursors are rebased by native list-init, not the harness')
''')
reuse_change("final_pixels['processed']==4 and final_pixels['last_job']==4","final_pixels['processed']==12 and final_pixels['last_job']==12")
reuse_change("base[3]['render_completion_seed']['completed']==4","base[3]['render_completion_seed']['completed']==12")
code=code.replace('consecutive_whole_frame_calls=4','consecutive_whole_frame_calls=12').replace('all four actual submitted jobs complete through native pixel work','all twelve actual submitted jobs complete through native pixel work')
marker=" exact(sum(x!=0xff101018 for x in memoryview(base[3]['pixel_job_seed'].pixels).cast('I'))==2,'final actual native job produces two visible retail-textured pixels')"
reuse_change(marker,marker+'''
 expected_inits=sum((w[1]&0x1fffffff)==0x5f8144 and bool(w[2]&0x80000000) for phase in records for w in phase['pvr_register_writes'])
 expected_resets=sum((w[1]&0x1fffffff)==0x5f8008 and bool(w[2]&1) for phase in records for w in phase['pvr_register_writes'])
 exact(base[3]['ta_list_seed']['inits']==expected_inits and base[3]['ta_list_seed']['soft_resets']==expected_resets,'all context generations and reset requests come from observed original register writes')
 exact(base[3]['ta_list_seed']['retired']==108 and base[3]['ta_list_seed']['discarded']==0 and base[3]['ta_list_seed']['last_count']==9,'twelve native contexts consume 108 accepted packets without growing the 64-packet collector')
''')
code=code.replace('Not physical hardware pixels/timing, visible retail texture qualification, reset boot, real-time FPS or a playable port.','Guest TA_LIST_INIT strobes select a new collector generation while submitted jobs remain immutable; this is a bounded untimed context profile. Not physical TA parameter-RAM encoding, GPU pixels/timing, full reset boot, real-time FPS or a playable port.')
files=('verify_renderer_ta_list_reuse.py','renderer_ta_list_reuse_fixtures.py','renderer_ta_list_reuse_reference.py','renderer_ta_list_reuse_reference_code.py','renderer_ta_list_reuse_comparison.py','write_watch_renderer_ta_list_reuse_base.py','write_watch_renderer_ta_list_reuse_state.py','write_watch_renderer_ta_list_reuse_capture.py','write_watch_renderer_ta_list_reuse.py','source/ta_list_lifecycle.c','source/ta_list_lifecycle.h','build_ta_list_lifecycle.ps1','ta_list_lifecycle_native.py','ta_list_lifecycle_reference.py','verify_ta_list_lifecycle.py','verify_ta_list_partial_reset.py','analysis/ta_list_lifecycle_verification.json','analysis/ta_list_partial_reset_verification.json')
reuse_change("P/'disc_inventory.json',asset,","P/'disc_inventory.json',asset,"+','.join("P/'"+name+"'" for name in files)+",R/'work/sonic-adventure/build/ta_list_lifecycle.dll',")
exec(compile(code,str(parent),'exec'),globals())
