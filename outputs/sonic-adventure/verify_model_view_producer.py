"""Original caller-selected source record, then actual retail model submission."""
import dataclasses,gc,hashlib,json,pickle,sys
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';sys.path.insert(0,str(P))
import verify_renderer_model_geometry_guarded as prefix
import model_view_producer_fixtures as q
import write_watch_model_view_producer_state as graphics
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 raw,pp,sp=prefix.checkpoint();m,s,f,c=prefix.fresh(raw)
 values={field.name:getattr(c['graphics_in'],field.name) for field in dataclasses.fields(c['graphics_in'])}
 c['graphics_in']=graphics.GraphicsState(**values)
 t=P/'analysis/model_view_producer_translation.json';translation=json.loads(t.read_text())
 for name,h in translation['sha256'].items():assert sha(name)==h,name
 assert q.get(m,0x8c8ea3e0)==0 and q.get(m,0x8c8ea460)==0
 record=q.get(m,0x8c012508);assert record==0x8c6aee28
 assert q.get(m,0x8c01250c)==0x8c63aa94
 binary=(W/'extracted/1ST_READ.BIN').read_bytes()
 assert binary[0x23f6:0x23fe]==bytes.fromhex('45d243d40b4213e5')
 phases=[]
 def call(entry,args):
  nonlocal m,s,f,c
  s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value
  result,path,m,s,f=q.fast.compare(m,s,f,c)
  assert result['full_state_match'] and result['ordered_boundaries_match']
  phases.append(dict(entry=hex(entry),arguments=args,status=result['status'],stop_pc=result['stop_pc'],steps=len(path),boundaries=result['boundaries'],write_watch=result['write_watch'],full_state_match=True,ordered_boundaries_match=True,vertex_counts=result['vertex_counts'],command_packets=len(result['command_packets']),sprite_command_buffers=result['sprite_command_buffers']))
  if result['status']==0:
   c['ta_list_seed']=result['ta_list_out'];c['frame_events_seed']=result['frame_events_state']
   for key,name in (('pixel_job_seed','pixel_job_out'),('render_completion_seed','render_completion_out'),('frame_clock_seed','frame_clock_out')):
    if result[name] is not None:c[key]=result[name]
   if result['runtime_state'] is not None:c['runtime_seed']=result['runtime_state']
   for name,value in zip(q.v.EXCEPTION_KEYS,result['exception_state']):c[name]=value
   c.update(graphics_in=result['graphics_out'],other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr'])
  return result
 result=call(0x8c63aa94,[record,0x13]);assert result['status']==0
 assert q.get(m,0x8c8ea3e0)==record and q.get(m,0x8c8ea460)==1
 targets={int(b['target'],16) for b in result['boundaries']}
 assert {0x8c63aa94,0x8c63aa04,0x8c63aa74,0x8c63a8e0}<=targets
 saved=W/'model-view-produced';saved.mkdir(exist_ok=False)
 payload=pickle.dumps((bytes(m),bytes(s),bytes(f),c),protocol=5);(saved/'normal_public_return.pkl').write_bytes(payload)
 provenance=dict(status='matched_normal_original_source_record_initializer_return',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_proof_sha256=sha(W/'model-geometry-prefix/provenance.json'),prepared_proof_sha256=sha(W/'model-geometry-prepared/provenance.json'),faulted_state_saved=False,source_record=hex(record),record_from_original_caller_literal='0x8c012508',mode_from_original_call_delay_slot='0x13',phases=phases)
 (saved/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
 files=[Path(__file__),t,q.v.effects.DLL,W/'model-view-produced/provenance.json',W/'model-geometry-prepared/provenance.json',P/'analysis/renderer_model_geometry_guarded_verification.json']
 for name in ('model_view_producer_reference','model_view_producer_fixtures','write_watch_model_view_producer','write_watch_model_view_producer_base','write_watch_model_view_producer_state','write_watch_model_view_producer_capture'):files.append(P/(name+'.py'))
 report=dict(status='original_source_record_initializer_verified_at_retail_caller_arguments',phases=phases.copy(),source_record=hex(record),source_pointer_table='0x8c8ea3e0',table_count_address='0x8c8ea460',table_count_after=1,caller_literal='0x8c012508',mode=19,full_state_and_ordered_boundaries_match=True,private_frames_injected=False,source_pointer_directly_injected=False,callee_results_fabricated=False,faulted_state_resumed=False,full_292_source_build_passed=True,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,actual_model_rasterization_qualified=False,sha256={str(f):sha(f) for f in files})
 target=P/'analysis/model_view_producer_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('ORIGINAL SOURCE RECORD PRODUCED',hex(record),len(result['boundaries']),flush=True)
 result=call(0x8c6215d6,[q.get(m,0x8c044a00)])
 report=dict(status='model_call_after_original_source_record_initialization',phases=phases,model_root_from_original_literal='0x8c044a00',model_call_status=result['status'],model_stop_pc=result['stop_pc'],full_state_and_ordered_boundaries_match=True,natural_source_record_producer_used=True,actual_model_rasterization_qualified=False,failed_state_resumed=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(target):sha(target),str(Path(__file__)):sha(Path(__file__)),str(q.v.effects.DLL):sha(q.v.effects.DLL)})
 out=P/'analysis/renderer_model_after_view_producer_verification.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n');print('RETAIL MODEL AFTER PRODUCER',result['status'],result['stop_pc'],len(result['command_packets']),flush=True)
if __name__=='__main__':main()
