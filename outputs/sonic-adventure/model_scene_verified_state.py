"""Carry only matched normal public-return states between original model APIs."""
import hashlib,json,pickle
from pathlib import Path
import verify_model_view_producer as inherited
P=Path(__file__).resolve().parent;W=inherited.W;q=inherited.q
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()

class Scene:
 def __init__(self,folder):
  # Verify the unchanged producer chain before loading its private test state.
  inherited.prefix.checkpoint()
  proof=json.loads((P/'analysis/model_view_producer_verification.json').read_text())
  for name,h in proof['sha256'].items():assert sha(name)==h,name
  folder=W/folder;meta=json.loads((folder/'provenance.json').read_text());raw=(folder/'normal_public_return.pkl').read_bytes()
  assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256'] and not meta['faulted_state_saved']
  if 'DLL_sha256' in meta:assert meta['DLL_sha256']==sha(q.v.effects.DLL)
  m,s,f,self.c=pickle.loads(raw);self.m=bytearray(m);self.s=q.v.effects.State.from_buffer_copy(s);self.f=q.v.effects.Fpu.from_buffer_copy(f)
  assert self.s.pc==self.s.pr==0x8cfed000
  self.records=[];self.usable=True;self.prefix=folder/'provenance.json'
 def call(self,entry,args=(),compare=None):
  assert self.usable
  self.s.pc=self.c['entry']=entry;self.s.pr=0x8cfed000
  for i,value in enumerate(args):self.s.r[4+i]=value
  # Mark unusable before execution; an exception must never resume its state.
  self.usable=False
  result,path,self.m,self.s,self.f=(compare or q.fast.compare)(self.m,self.s,self.f,self.c)
  assert result['full_state_match'] and result['ordered_boundaries_match']
  self.records.append(dict(entry=hex(entry),arguments=list(args),status=result['status'],stop_pc=result['stop_pc'],steps=len(path),boundaries=result['boundaries'],write_watch=result['write_watch'],vertex_counts=result['vertex_counts'],packets=len(result['command_packets']),full_state_match=True,ordered_boundaries_match=True))
  if result['status']==0:
   assert self.s.pc==self.s.pr==0x8cfed000
   c=self.c;c['ta_list_seed']=result['ta_list_out'];c['frame_events_seed']=result['frame_events_state']
   for key,name in (('pixel_job_seed','pixel_job_out'),('render_completion_seed','render_completion_out'),('frame_clock_seed','frame_clock_out'),('runtime_seed','runtime_state')):
    if result[name] is not None:c[key]=result[name]
   for name,value in zip(q.v.EXCEPTION_KEYS,result['exception_state']):c[name]=value
   c.update(graphics_in=result['graphics_out'],other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr'])
   self.usable=True
  return result,path
 def save(self,name):
  assert self.usable and self.records and self.records[-1]['status']==0
  assert self.s.pc==self.s.pr==0x8cfed000
  folder=W/name;folder.mkdir(exist_ok=False);raw=pickle.dumps((bytes(self.m),bytes(self.s),bytes(self.f),self.c),protocol=5)
  (folder/'normal_public_return.pkl').write_bytes(raw)
  meta=dict(status='matched_normal_original_public_return',checkpoint_sha256=hashlib.sha256(raw).hexdigest(),prefix_provenance_sha256=sha(self.prefix),prefix_provenance_path=str(self.prefix),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,records=self.records)
  (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n');return folder
