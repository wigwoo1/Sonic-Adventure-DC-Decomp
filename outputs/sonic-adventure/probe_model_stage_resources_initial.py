"""Observe original resource I/O without injecting file bytes or callee results."""
import hashlib,json,pickle,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'outputs/sonic-adventure';W=R/'work/sonic-adventure';sys.path.insert(0,str(P))
from model_stage_resources_verified_state import normal,sha
from model_scene_verified_state import Scene

def clone(base,q):
 scene=Scene.__new__(Scene);scene.m=bytearray(base.m);scene.s=q.v.effects.State.from_buffer_copy(bytes(base.s));scene.f=q.v.effects.Fpu.from_buffer_copy(bytes(base.f));scene.c=pickle.loads(pickle.dumps(base.c,protocol=5));scene.records=[];scene.usable=True;scene.prefix=base.prefix;q.v.bind_vectors(scene.c['vbr']);return scene
def read(scene,a,n=4):return int.from_bytes(scene.m[a&0xffffff:(a&0xffffff)+n],'little')

def main():
 target=W/'model_stage_resources_initial_observation.json';assert not target.exists()
 base,q=normal();expected=(W/'decoded/SONICADV/STG00.BIN').read_bytes();main=(W/'extracted/1ST_READ.BIN').read_bytes();records=[]
 for label,entry,args in [('original_PRS_file',0x8c09da44,(0x8c15aaec,0x0c900000)),('original_stage_files',0x8c09ce90,())]:
  scene=clone(base,q);before=bytes(scene.m[0x900000:0x900000+len(expected)])
  result,path=scene.call(entry,args,compare=q.fast.compare)
  bank=bytes(scene.m[0x900000:0x900000+len(expected)])
  overlay_pcs=sorted({a for a in path if 0x900000<=(a&0xffffff)<0xa00000})
  main_mismatches=[]
  for a in set(path):
   offset=a&0xffffff
   if 0x10000<=offset<=0x10000+len(main)-2:
    original=main[offset-0x10000:offset-0x10000+2]
    if bytes(base.m[offset:offset+2])!=original or bytes(scene.m[offset:offset+2])!=original:main_mismatches.append(hex(a))
  row=dict(label=label,entry=hex(entry),arguments=list(args),status=result['status'],stop_pc=result['stop_pc'],steps=len(path),return_r0=hex(scene.s.r[0]),normal_return=scene.usable,full_state_match=result['full_state_match'],ordered_boundaries_match=result['ordered_boundaries_match'],boundaries=result['boundaries'],output_bytes=len(expected),expected_output_sha256=hashlib.sha256(expected).hexdigest(),shared_bank_before_sha256=hashlib.sha256(before).hexdigest(),shared_bank_after_sha256=hashlib.sha256(bank).hexdigest(),decoded_STG00_bytes_match=bank==expected,shared_bank_code_PCs=[hex(a) for a in overlay_pcs],main_reached_code_byte_mismatches=sorted(main_mismatches),pixel_job_summary=scene.c['pixel_job_seed'].summary(),geometry_or_global_or_file_data_injection=False,faulted_state_resumed=False)
  records.append(row);print(label,result['status'],result['stop_pc'],len(path),'STG00 match:',bank==expected,flush=True)
 report=dict(status='private_resource_I_O_observation_not_complete_game',records=records,DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),source_main_sha256=sha(W/'extracted/1ST_READ.BIN'),retail_inputs_changed=False,overall_completion_percent=10,goal_paused=False)
 target.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
