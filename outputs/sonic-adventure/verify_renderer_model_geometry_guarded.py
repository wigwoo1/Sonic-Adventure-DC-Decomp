"""Strict cold-view failures from verified normal public-return checkpoints."""
import copy,dataclasses,gc,hashlib,json,pickle,sys
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
sys.path.insert(0,str(P))
import renderer_ta_list_reuse_fixtures as old_q
import renderer_model_geometry_guarded_fixtures as q
import write_watch_renderer_model_geometry_guarded_state as graphics
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def checkpoint():
 prefix=W/'model-geometry-prefix';prepared=W/'model-geometry-prepared'
 pp=json.loads((prefix/'provenance.json').read_text());sp=json.loads((prepared/'provenance.json').read_text())
 assert sp['prefix_proof_sha256']==sha(prefix/'provenance.json') and not sp['faulted_state_saved'] and not pp['faulted_state_saved']
 assert all(p['status']==0 and p['full_state_match'] and p['ordered_boundaries_match'] for p in pp['phases'])
 for f,h in pp['sha256'].items():assert sha(f)==h,f
 for folder,proof in ((prefix,pp),(prepared,sp)):assert sha(folder/'normal_public_return.pkl')==proof['checkpoint_sha256']
 translation=P/'analysis/renderer_model_geometry_guarded_translation.json';t=json.loads(translation.read_text())
 for f,h in t['sha256'].items():assert sha(f)==h,f
 raw=(prepared/'normal_public_return.pkl').read_bytes()
 return raw,pp,sp

def fresh(raw):
 m,s,f,c=pickle.loads(raw)
 s=q.v.effects.State.from_buffer_copy(s);f=q.v.effects.Fpu.from_buffer_copy(f)
 assert s.pc==s.pr==0x8cfed000
 prior=c['graphics_in'];values={f.name:getattr(prior,f.name) for f in dataclasses.fields(prior)}
 c['graphics_in']=graphics.GraphicsState(**values)
 assert all(getattr(c['graphics_in'],name)==value for name,value in values.items())
 return bytearray(m),s,f,c

def main():
 target=P/'analysis/renderer_model_geometry_guarded_verification.json';assert not target.exists()
 raw,pp,sp=checkpoint();records=[]
 cases=[dict(label='natural cold-view original model wrapper',entry=0x8c6215d6,pointer=None,bits=None)]
 cases += [dict(label='declared invalid-view unit input',entry=0x8c6215d6,pointer=ptr,bits=bits) for ptr in (0,0xfffffff8,0x10000000,0x8cfffffc) for bits in (0,0x100000,0x200000,0x300000)]
 for case in cases:
  m,s,f,c=fresh(raw);assert q.get(m,0x8c8ea3e0)==0
  if case['pointer'] is not None:q.put(m,0x8c8ea3e0,case['pointer'])
  if case['bits'] is not None:f.fpscr=(f.fpscr&~0x300000)|case['bits']
  s.pc=c['entry']=case['entry'];s.pr=0x8cfed000;s.r[4]=q.get(m,0x8c044a00)
  result,path,discarded_m,discarded_s,discarded_f=q.fast.compare(m,s,f,c)
  assert result['status']==-3 and result['stop_pc']=='0x8c620f60'
  assert result['full_state_match'] and result['ordered_boundaries_match']
  assert result['boundary_comparisons']==(3 if case['entry']==0x8c6215d6 else 1)
  records.append(dict(case,status=result['status'],stop_pc=result['stop_pc'],steps=len(path),boundary_comparisons=result['boundary_comparisons'],full_state_match=True,ordered_boundaries_match=True,write_watch=result['write_watch'],full_vram_bytes_compared=result['full_vram_bytes_compared']))
  print('MATCHED COLD GUARD',len(records),case,result['stop_pc'],flush=True)
  del m,s,f,c,result,path,discarded_m,discarded_s,discarded_f;gc.collect()
 files=[Path(__file__),P/'analysis/renderer_model_geometry_guarded_translation.json',q.v.effects.DLL, W/'model-geometry-prefix/provenance.json',W/'model-geometry-prepared/provenance.json']
 for name in ('renderer_model_geometry_guarded_reference','renderer_model_geometry_guarded_fixtures','write_watch_renderer_model_geometry_guarded','write_watch_renderer_model_geometry_guarded_base','write_watch_renderer_model_geometry_guarded_state','write_watch_renderer_model_geometry_guarded_capture'):files.append(P/(name+'.py'))
 report=dict(status='verified_cold_invalid_view_guard_only',comparisons=len(records),cases=records,full_291_source_link_passed=True,full_guest_partial_state_and_ordered_boundaries_match=True,normal_checkpoint_reused=True,failed_state_resumed=False,original_guards_weakened=False,camera_pointer_fabricated_for_natural_case=False,positive_model_geometry_qualified=False,actual_model_rasterization_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('COLD GUARDS VERIFIED',len(records),flush=True)
if __name__=='__main__':main()
