"""Original copy heads: all selectors, overlap order and reached pointer guards."""
import ctypes,dataclasses,gc,hashlib,json
from pathlib import Path
from model_scene_verified_state import Scene,sha,P,W
import model_camera_copy_v2_fixtures as q
import write_watch_model_camera_copy_v2_state as graphics

def main():
 target=P/'analysis/model_camera_copy_verification.json';assert not target.exists()
 manifest=P/'analysis/model_camera_copy_v2_translation.json'
 for name,h in json.loads(manifest.read_text())['sha256'].items():assert sha(name)==h,name
 cases=[];lower_cases=[]
 specs=[dict(size=n,overlap=None,bad=None) for n in range(0,68,4)]
 specs += [dict(size=28,overlap=4,bad=None),dict(size=32,overlap=-8,bad=None),dict(size=12,overlap=None,bad='source'),dict(size=16,overlap=None,bad='destination')]
 for spec in specs:
  scene=Scene('model-view-produced')
  values={f.name:getattr(scene.c['graphics_in'],f.name) for f in dataclasses.fields(scene.c['graphics_in'])}
  scene.c['graphics_in']=graphics.GraphicsState(**values);q.v.bind_vectors(scene.c['vbr'])
  source=0x8cf10020;destination=source+spec['overlap'] if spec['overlap'] is not None else 0x8cf12020
  for i in range(128):scene.m[0xf10000+i]=(i*37+11)&255
  if spec['overlap'] is None:scene.m[0xf12000:0xf12080]=b'\xa5'*128
  size=spec['size'];expected=bytes(scene.m[(source&0xffffff):(source&0xffffff)+size])
  if spec['bad']=='source':source=0
  if spec['bad']=='destination':destination=0
  scene.s.r[0]=size;scene.s.r[1]=destination;scene.s.r[2]=source;scene.s.r[3]=0x89abcdef
  stack=scene.s.r[15];sr=scene.s.sr;entry=0x8c10cd44 if size&4 else 0x8c10cd38
  initial=(bytes(scene.m),bytes(scene.s),bytes(scene.f),dict(scene.c)) if size==32 and spec['overlap'] is None and spec['bad'] is None else None
  result,path=scene.call(entry,compare=q.fast.compare)
  assert result['full_state_match'] and result['ordered_boundaries_match']
  assert len(result['boundaries'])==1 and result['boundaries'][0]['target']==hex(entry)
  if spec['bad']:
   assert result['status']==-3 and result['stop_pc']==('0x8c10cd4e' if spec['bad']=='source' else '0x8c10cd88') and not scene.usable
  else:
   assert result['status']==0 and scene.usable and scene.s.r[3]==0x89abcdef and scene.s.r[15]==stack and scene.s.sr==sr
   if spec['overlap'] is None:
    assert bytes(scene.m[destination&0xffffff:(destination&0xffffff)+size])==expected
    assert scene.m[0xf1201f]==0xa5 and scene.m[0xf12020+size]==0xa5
  cases.append(dict(**spec,entry=hex(entry),status=result['status'],stop_pc=result['stop_pc'],steps=len(path),full_state_match=True,ordered_boundaries_match=True,write_watch=result['write_watch']))
  if initial:
   # The old lower API's documented first context field is the stop pointer.
   U=ctypes.c_uint32;B=ctypes.c_uint8;fn=q.v.effects.LIB.sa_engine_record_copy_dispatch
   fn.argtypes=[ctypes.POINTER(B),ctypes.POINTER(q.v.effects.Fpu),ctypes.POINTER(U),ctypes.POINTER(U),ctypes.POINTER(U),ctypes.POINTER(U),ctypes.POINTER(U),ctypes.c_void_p,ctypes.c_void_p];fn.restype=ctypes.c_int
   class LowerContext(ctypes.Structure):_fields_=[('published_stop',ctypes.POINTER(U))]
   for published in (False,True):
    raw,state_raw,fpu_raw,c=initial;memory=(B*len(raw)).from_buffer_copy(raw)
    state=q.v.effects.State.from_buffer_copy(state_raw);f=q.v.effects.Fpu.from_buffer_copy(fpu_raw)
    registers=(U*16)(*state.r);status=U(state.sr);pr=U(0x8cfed000);fpul=ctypes.cast(ctypes.byref(f,q.v.effects.Fpu.fpul.offset),ctypes.POINTER(U));macl=U(c['macl'])
    stop=U(0);context=LowerContext(ctypes.pointer(stop))
    rc=fn(memory,ctypes.byref(f),ctypes.byref(status),registers,ctypes.byref(pr),fpul,ctypes.byref(macl),ctypes.byref(context) if published else None,None)
    assert rc==0 and bytes(memory)==bytes(scene.m) and list(registers)==list(scene.s.r) and status.value==scene.s.sr and pr.value==scene.s.pr and bytes(f)==bytes(scene.f)
    assert macl.value==c['macl'] and (not published or stop.value==0x8c10cd96)
    lower_cases.append(dict(published_stop_pointer=published,status=rc,matched_original_CPU_FPU_registers_and_full_RAM=True))
  del scene;gc.collect()
 assert len(cases)==21 and sum(c['status']==0 for c in cases)==19 and len(lower_cases)==2
 files=[Path(__file__),manifest,q.v.effects.DLL]
 for name in ('model_camera_copy_v2_reference','model_camera_copy_v2_fixtures','write_watch_model_camera_copy_v2','write_watch_model_camera_copy_v2_base','write_watch_model_camera_copy_v2_state','write_watch_model_camera_copy_v2_capture'):files.append(P/(name+'.py'))
 report=dict(status='verified_dual_copy_heads_all_selectors_overlap_and_guards',original_native_full_comparisons=21,normal_returns=19,matched_partial_pointer_stops=2,selectors=list(range(0,68,4)),overlap_cases=2,lower_API_checks=lower_cases,cases=cases,internal_table_edges_classified_from_original_CFG=True,zero_size_original_preload_retained=True,caller_inputs_are_declared_test_records=True,game_callee_results_injected=False,faulted_state_resumed=False,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('COPY VERIFIED',len(cases),'full comparisons and',len(lower_cases),'lower API checks',flush=True)
if __name__=='__main__':main()
