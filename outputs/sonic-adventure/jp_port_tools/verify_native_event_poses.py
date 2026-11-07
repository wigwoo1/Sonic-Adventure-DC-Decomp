"""Compare genuine Sonic pose trees/getters with complete original SH4 calls.

This qualifies sampled POS/ANG pose paths, not player welding/event execution.
"""
import argparse,ctypes as C,hashlib,json,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from verify_native_motion_data import Entry
from verify_native_motion_store import View
from verify_native_action_poses import Action,Motion
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','build','binary','numeric-dll','motion-inputs','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=read(a.build/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256']
 pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json');assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256'];iv=read(a.motion_inputs/'inputs.json');bank=a.motion_inputs/'s_mot/content.bin';assert sha(bank)==iv['files']['s_mot/bin']['sha256']
 sys.path.insert(0,str(p));import verify_startup as executor
 from model_reference import make_reference
 from verify_fpu_cleanup_transfer import Fpu
 reference,reference_hashes=make_reference(executor);U,I,P,F,Q,S=C.c_uint32,C.c_int32,C.c_void_p,C.c_float,C.c_uint64,C.c_size_t;numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',I,U,P,U)
 def service(op,state):assert numeric(op,C.byref(state),state.fpul)==0
 f=C.CDLL(b['frames_DLL']);motions=C.CDLL(b['motion_DLL']);checks=0;records=[];calls={'pose_action':0,'pose_get_count':0,'pose_get_matrix':0}
 def check(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 check(bind(motions,'jp_motions_initialize',I,S,U)(8<<20,16)==0 and bind(motions,'jp_motions_select',I,I)(0)==0,'actual existing Motion owner initialized and Sonic bank selected')
 views=[]
 for bank_id,name in ((0,'sonic_updraft_action'),(7,'sonic_full_speed_action')):
  v=View();check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(bank_id,name.encode(),C.byref(v))==0,'genuine action retained');action=Action.from_address(v.data);motion=Motion.from_address(action.motion);check(motion.frames==33 and motion.type==3 and motion.inputs==2,'genuine POS/ANG33-frame contract');views.append((name,v,action))
 entries=[]
 for name in ('s_mot','models'):
  get=bind(motions,'jp_motion_'+name+'_entry',I,U,P)
  for index in range(bind(motions,'jp_motion_'+name+'_count',U)()):e=Entry();assert get(index,C.byref(e))==0;entries.append(e)
 def original_address(ptr):
  e=next(e for e in entries if e.data<=ptr<e.data+e.host_bytes);assert (ptr-e.data)%e.host_element==0;return e.address+(ptr-e.data)//e.host_element*e.original_element
 table={name:int(at,16) for at,name in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)};initial=bytearray(16<<20);raw=a.binary.read_bytes();initial[0x10000:0x10000+len(raw)]=raw;raw=bank.read_bytes();initial[0xc00000:0xc00000+len(raw)]=raw
 def iw(ram,address,values):struct.pack_into('<'+'I'*len(values),ram,address&0xffffff,*values)
 def invoke(ram,name,bankmode,matrix,registers,frame=0.):
  fp=Fpu();fp.fpscr=0x40001|(bankmode<<21);fp.banks[bankmode^1][:]=list(matrix);fp.banks[bankmode][4]=struct.unpack('<I',struct.pack('<f',frame))[0];s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000
  for index,value in registers.items():s.r[index]=value
  reference(ram,s,fpu=fp,vector_instruction=service,special={'gbr':0,'r_bank':[0]*8},instruction_budget=3000000);calls[name]+=1;check(s.r[15]==0x8cf00000,('complete original stack returns',name));return s,fp
 stack=(C.c_uint8*(128*64))();identity=(U*16)(*[0x3f800000 if i%5==0 else 0 for i in range(16)]);rotated=(U*16)(*struct.unpack('<16I',struct.pack('<16f',0,1,0,0,-1,0,0,0,0,0,1,0,17,-3,4,1)));draw=bind(f,'pose_action',I,P,P,F,U,P);count=bind(f,'pose_get_count',U);get=bind(f,'pose_get_matrix',I,U,P)
 for name,v,action in views:
  for bankmode in (0,1):
   for parent_id,parent in enumerate((identity,rotated)):
    for frame in (0.,8.5,16.,32.):
     for node,out_enabled in [(n,e) for n in (0,8,72,127) for e in (0,1)]:
      ram=bytearray(initial);iw(ram,table['_stack_size_'],[1]);iw(ram,table['_max_stack_size_'],[128]);iw(ram,table['_nj_matrix_flag_'],[0]);iw(ram,table['_nj_base_matrix_ptr_'],[0x8ce00000]);iw(ram,table['_nj_current_matrix_ptr_'],[0x8ce00000]);iw(ram,0x8cd00000,list(parent));iw(ram,0x8cd01000,[0xa5a5a5a5]*16);out=(U*20)(*[0xa5a5a5a5]*20);baseptr=None if parent_id==0 else parent;original_base=0 if parent_id==0 else 0x8cd00000
      s,fp=invoke(ram,'pose_action',bankmode,identity,{4:original_base,5:original_address(v.data),6:node,7:0x8cd01000 if out_enabled else 0},frame);bind(f,'njInitMatrix',None,P,I,I)(stack,128,0);bind(f,'njSetMatrix',None,P,P)(None,identity);native=draw(baseptr,v.data,frame,node,C.byref(out,8) if out_enabled else None);original_count=struct.unpack_from('<I',ram,table['pose_count']&0xffffff)[0]
      check(native==s.r[0]==1 and count()==original_count==min(node+1,73),'same original pose result and traversal limit/count')
      expected=bytes(ram[0xd01000:0xd01040]);actual=bytes(out)[8:72]
      if actual!=expected:(d/'last_case.json').write_text(json.dumps(dict(action=name,bank=bankmode,parent=parent_id,frame=frame,node=node,original_count=original_count,expected=expected.hex(),actual=actual.hex()),indent=2)+'\n')
      check(actual==expected and list(out[:2])+list(out[18:])==[0xa5a5a5a5]*4,('original requested pose matrix and output guards match',name,bankmode,parent_id,frame,node))
      combined=hashlib.sha256()
      for index in range(original_count):
       buf=(U*18)(*[0x5a5a5a5a]*18);check(get(index,C.byref(buf,4))==1,'native getter accepts each retained original node');original_matrix=bytes(ram[(table['pose_matrices']&0xffffff)+64*index:(table['pose_matrices']&0xffffff)+64*(index+1)]);check(bytes(buf)[4:68]==original_matrix and buf[0]==buf[17]==0x5a5a5a5a,('all genuine node matrices match whole original pose',name,frame,node,index));combined.update(original_matrix)
      # Exercise both getter entrypoints as full original calls as well.
      sc,_=invoke(ram,'pose_get_count',bankmode,identity,{});check(sc.r[0]==count(),'complete original getter count')
      for index in (0,original_count-1,original_count,0xffffffff):
       iw(ram,0x8cd02000,[0x6b6b6b6b]*16);buf=(U*18)(*[0x6b6b6b6b]*18);sg,_=invoke(ram,'pose_get_matrix',bankmode,identity,{4:index,5:0x8cd02000});got=get(index,C.byref(buf,4));check(got==sg.r[0] and bytes(buf)[4:68]==bytes(ram[0xd02000:0xd02040]) and buf[0]==buf[17]==0x6b6b6b6b,('whole original getter accepts/rejects bounds without guard changes',index))
      now=(U*16)();bind(f,'njGetMatrix',None,P)(now);check(bytes(now)==bytes(identity) and I.in_dll(f,'_stack_size_').value==1,'native pose and getters restore complete caller matrix/stack');check(list(fp.banks[bankmode^1])==list(identity),'complete original pose restores caller XMTRX');records.append(dict(action=name,bank=bankmode,parent=parent_id,frame=frame,node=node,out_enabled=bool(out_enabled),retained_nodes=original_count,matrix_sha256=combined.hexdigest()))
 for name,v,action in views:check(bind(motions,'jp_motions_release',I,Q)(v.token)==0,'retained action reference retires')
 check(bind(motions,'jp_motions_shutdown',I)()==0,'same actual Motion owner shuts down')
 out=dict(status='sampled_genuine_Sonic_POS_ANG_pose_trees_and_getters_match_complete_original_SH4_calls',checks=checks,original_calls=calls,records=records,source_actions=['sonic_updraft_action','sonic_full_speed_action'],sampled_frames=[0,8.5,16,32],node_limits=[0,8,72,127],basis_modes=['inherited_identity','explicit_rotated_translated'],optional_output_pointer_and_NULL_both_verified=True,full_tree_nodes=73,all_retained_node_matrix_words_compared=True,whole_original_getter_bounds_calls=True,original_binary_sha256=sha(a.binary),reference_hashes=reference_hashes,original_game_function_hooks_used=False,three_factor_scale_pose_path_verified=False,player_welding_event_motion_and_gameplay_behavior_verified=False,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10,goal_active=True)
 (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks',calls,flush=True)
if __name__=='__main__':main()
