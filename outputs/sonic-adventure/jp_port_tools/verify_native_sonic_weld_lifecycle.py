"""Private complete original Sonic weld make/update/free comparison.

Actual Frame heap and canonical model owner; original function calls are not
hooked. This is a valid-record diagnostic,not original Sonic task gameplay.
"""
import argparse,bisect,ctypes as C,faulthandler,hashlib,json,re,struct,sys
from pathlib import Path
ap=argparse.ArgumentParser()
for n in ('workspace','upstream','build','binary','numeric-dll','motion-inputs','module-inputs','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
args=ap.parse_args();r=args.workspace.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure';d=args.output.resolve();assert not d.exists() and d.is_relative_to(w);d.mkdir();sys.path.insert(0,str(p/'jp_port_tools'));sys.path.insert(0,str(p))
import verify_startup as executor
from model_reference import make_reference
from verify_fpu_cleanup_transfer import Fpu
from native_packet_test_support import bind
from verify_native_motion_data import Entry,Field
from verify_native_motion_store import View
from verify_native_character_tree import Node
from verify_native_model_preflight import Model
from verify_native_action_poses import Action,Motion
def sha(x):return hashlib.sha256(Path(x).read_bytes()).hexdigest()
def read(x):return json.loads(Path(x).read_bytes())
b=read(args.build/'build_verification.json');binary=args.binary;numeric_dll=args.numeric_dll.resolve();pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json');assert sha(binary)==pin['original_binary_sha256'] and sha(numeric_dll)==pin['numerical_model_DLL_sha256']
for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
module_manifest=read(args.module_inputs/'inputs.json');assert module_manifest['actual_supplied_JP_disc_module_verified']
for n,row in module_manifest['files'].items():assert sha(args.module_inputs/n)==row['sha256']
assert b['sonic_weld_data_module_inputs_sha256']==sha(args.module_inputs/'inputs.json')
assert sha(args.motion_inputs/'s_mot/content.bin')==read(args.motion_inputs/'inputs.json')['files']['s_mot/bin']['sha256']
fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True);U,I,P,F,Q,S=C.c_uint32,C.c_int32,C.c_void_p,C.c_float,C.c_uint64,C.c_size_t;f=C.CDLL(b['frames_DLL']);m=C.CDLL(b['motion_DLL']);numeric=bind(C.CDLL(str(numeric_dll)),'sa_sh4_vector_numeric',I,U,P,U);reference,reference_hashes=make_reference(executor)
def service(op,state):assert numeric(op,C.byref(state),state.fpul)==0
class Weld(C.Structure):_fields_=[('base',P),('object_a',P),('object_b',P),('count',C.c_uint8),('kind',C.c_int8),('node_a',C.c_uint8),('node_b',C.c_uint8),('vertices',P),('indices',P)]
class PlayerAction(C.Structure):_fields_=[('action',P),('nodes',C.c_uint8),('mode',C.c_int8),('next',C.c_uint16),('blend',F),('speed',F)]
layouts=b['player_weld_recipe']['layout_values'];nl,ol=layouts['native'],layouts['original_LP32'];assert C.sizeof(Weld)==nl['weld.sizeof']==48 and C.sizeof(PlayerAction)==nl['player_action.sizeof']==24
for record,name in ((Weld,'weld'),(PlayerAction,'player_action')):
 for field,*_ in record._fields_:assert getattr(record,field).offset==nl[name+'.'+field]
entries=[]
for i in range(bind(m,'jp_motion_models_count',U)()):e=Entry();assert bind(m,'jp_motion_models_entry',I,U,P)(i,C.byref(e))==0;entries.append(e)
assert len(entries)==len(b['sonic_weld_data_old_record_names'])+len(b['sonic_weld_data_added_registry']);intervals=sorted((e.data,e.data+e.host_bytes,e) for e in entries);starts=[x[0] for x in intervals];assert all(intervals[i-1][1]<=intervals[i][0] for i in range(1,len(intervals)));fields={}
for kind in {e.kind for e in entries if e.kind}:
 rows=[];layout=bind(m,'jp_motion_layout',U,U,U,P)
 for i in range(layout(kind,0,None)):out=Field();assert layout(kind,i,C.byref(out));rows.append(out)
 fields[kind]=rows
def ptr_word(ptr):
 if not ptr:return 0
 at=bisect.bisect_right(starts,ptr)-1;assert at>=0 and ptr<intervals[at][1],('unowned canonical pointer',hex(ptr));lo,hi,e=intervals[at];offset=ptr-lo
 if not e.kind:return e.address+offset
 element,inner=divmod(offset,e.host_element)
 if not inner:return e.address+element*e.original_element
 for field in fields[e.kind]:
  if field.host<=inner<field.host+(8 if field.pointer else field.bytes):assert not field.pointer or inner==field.host;return e.address+element*e.original_element+field.original+inner-field.host
 raise AssertionError(('pointer into native padding',e.name,inner))
def native_ptr(address):
 if not address:return None
 choices=[e for e in entries if e.address<=address<e.address+e.original_bytes]
 assert len(choices)==1,(hex(address),[e.name for e in choices]);e=choices[0];offset=address-e.address
 if not e.kind:return e.data+offset
 element,inner=divmod(offset,e.original_element);assert not inner;return e.data+element*e.host_element
def packed(e):
 raw=C.string_at(e.data,e.host_bytes)
 if not e.kind:return raw
 out=bytearray(e.original_bytes)
 for index in range(e.original_bytes//e.original_element):
  for field in fields[e.kind]:
   old=index*e.original_element+field.original;new=index*e.host_element+field.host
   if field.pointer:struct.pack_into('<I',out,old,ptr_word(struct.unpack_from('<Q',raw,new)[0]))
   else:out[old:old+field.bytes]=raw[new:new+field.bytes]
 return bytes(out)
initial=bytearray(16<<20);raw=binary.read_bytes();initial[0x10000:0x10000+len(raw)]=raw;raw=(args.module_inputs/'B_CHAOS7.BIN').read_bytes();initial[0x900000:0x900000+len(raw)]=raw;raw=(args.motion_inputs/'s_mot/content.bin').read_bytes();initial[0xc00000:0xc00000+len(raw)]=raw;checks=0
def check(ok,label):
 global checks
 assert ok,label;checks+=1
def compare_models(ram,phase):
 for e in entries:
  expected=bytes(ram[e.address&0xffffff:(e.address&0xffffff)+e.original_bytes]);actual=packed(e)
  if expected!=actual:
   (d/'first_model_difference.json').write_text(json.dumps(dict(phase=phase,name=e.name.decode(),address=f'{e.address:08x}',expected=expected.hex(),actual=actual.hex(),first_difference=next(i for i,(a,b) in enumerate(zip(expected,actual)) if a!=b)),indent=2)+'\n')
  check(expected==actual,('complete canonical model bytes',phase,e.name))
compare_models(initial,'initial_roundtrip');canonical_before={e.name:C.string_at(e.data,e.host_bytes) for e in entries};table={name:int(at,16) for at,name in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(args.upstream/'config/1st_read/symbols.txt').read_text(),re.M)};original_table=bytes(initial[0x1dc28c:0x1dc28c+33*24]);raw_rows=[struct.unpack_from('<IIIBbBBII',original_table,i*24) for i in range(32)];assert not struct.unpack_from('<I',original_table,32*24)[0]
index_arrays={};index_before={}
for base,a,other,count,kind,na,nb,vertices,indices in raw_rows:
 if indices:
  data=bytes(initial[indices&0xffffff:(indices&0xffffff)+count*4]);assert not index_arrays.get(indices) or index_before[indices]==data
  if indices not in index_arrays:buf=C.create_string_buffer(32+len(data));C.memset(buf,0xa5,C.sizeof(buf));C.memmove(C.addressof(buf)+16,data,len(data));index_arrays[indices]=buf;index_before[indices]=data
 for ptr in (base,a,other):
  if ptr:check(ptr_word(native_ptr(ptr))==ptr,'every original ordinary/light/Super weld object resolves to same canonical native owner')
 if kind in (1,2,3):
  indices_words=struct.unpack('<'+'H'*(count*2),data)
  for ptr,which in ((a,0),(other,1)):
   model=Model.from_address(Node.from_address(native_ptr(ptr)).model);check(bool(model.points) and bool(model.normals) and all(i<model.point_count for i in indices_words[which::2]),'real weld vertices and normals bounded by actual canonical model')
identity=(U*16)(*[0x3f800000 if i%5==0 else 0 for i in range(16)]);matrix_stack=(C.c_uint8*(128*64))();native_update=bind(f,'player_update_welds',None,P,P,P);native_direct=bind(f,'player_weld',None,I,P,P,F);native_heap_init=bind(f,'heap_init',None);heap_stats=bind(f,'jp_heap_statistics',None,P,P);player_records=(P*8).in_dll(f,'player_records');saved_players=list(player_records);records=[];original_calls=[]
def iw(ram,address,values):struct.pack_into('<'+'I'*len(values),ram,address&0xffffff,*values)
def invoke(ram,name,bank,regs,frame=0.):
 fp=Fpu();fp.fpscr=0x40001|(bank<<21);fp.banks[bank^1][:]=list(identity);fp.banks[bank][4]=struct.unpack('<I',struct.pack('<f',frame))[0];s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000
 for key,value in regs.items():s.r[key]=value
 reference(ram,s,fpu=fp,vector_instruction=service,special={'gbr':0,'r_bank':[0]*8},instruction_budget=12000000);check(s.r[15]==0x8cf00000,('complete original routine returns',name));original_calls.append(name);return s
def stats():
 total=I();largest=I();heap_stats(C.byref(total),C.byref(largest));return total.value,largest.value
def guards(buf,payload):return C.string_at(C.addressof(buf),16)==b'\xa5'*16 and C.string_at(C.addressof(buf)+16+payload,16)==b'\xa5'*16
def pointer_from(buf):return C.addressof(buf)+16
for super_active,variant in [(s,v) for s in (False,True) for v in ('genuine','both','to_b')]:
 for bank in (0,1):
  check(bind(m,'jp_motions_initialize',I,S,U)(8<<20,16)==0 and bind(m,'jp_motions_select',I,I)(0)==0,'actual canonical Motion owner initializes for lifecycle');views=[]
  for bank_id,name in ((7,'sonic_object'),(7,'super_sonic_object'),(7,'sonic_stand_action'),(7,'super_sonic_stand_action'),(7,'sonic_full_speed_action')):
   v=View();check(bind(m,'jp_motions_acquire',I,U,C.c_char_p,P)(bank_id,name.encode(),C.byref(v))==0,'genuine root/action owner reference held through make/update/free');views.append(v)
  action=Action.from_address(views[2].data);check(action.object==views[0].data and Motion.from_address(action.motion).frames>9,'actual first stand action and Sonic tree match');ram=bytearray(initial);iw(ram,table['_stack_size_'],[1]);iw(ram,table['_max_stack_size_'],[128]);iw(ram,table['_nj_matrix_flag_'],[0]);iw(ram,table['_nj_base_matrix_ptr_'],[0x8ce00000]);iw(ram,table['_nj_current_matrix_ptr_'],[0x8ce00000]);native_heap_init();invoke(ram,'heap_init',bank,{});check(stats()==(0x90000,0x90000),'actual native task heap initially coalesced')
  bind(f,'njInitMatrix',None,P,I,I)(matrix_stack,128,0);bind(f,'njSetMatrix',None,P,P)(None,identity);player_buf=C.create_string_buffer(nl['player.sizeof']+32);C.memset(player_buf,0xa5,C.sizeof(player_buf));player=pointer_from(player_buf);C.memset(player,0,nl['player.sizeof']);weld_buf=C.create_string_buffer(33*C.sizeof(Weld)+32);C.memset(weld_buf,0xa5,C.sizeof(weld_buf));weld_ptr=pointer_from(weld_buf);C.memset(weld_ptr,0,33*C.sizeof(Weld));welds=(Weld*33).from_address(weld_ptr);action_bytes=131*C.sizeof(PlayerAction);actions_buf=C.create_string_buffer(action_bytes+32);C.memset(actions_buf,0xa5,C.sizeof(actions_buf));actions_ptr=pointer_from(actions_buf);C.memset(actions_ptr,0,action_bytes);ram[0xd00000:0xd00000+512]=b'\x00'*512;ram[0xd01000:0xd01000+len(original_table)]=original_table;ram[0xd02000:0xd02000+131*16]=b'\x00'*(131*16)
  for action_index,view_index in ((0,2),(130,3)):
   first=initial[0x1db888+16*action_index:0x1db888+16*(action_index+1)];pa=PlayerAction.from_address(actions_ptr+action_index*C.sizeof(PlayerAction));pa.action=views[view_index].data;pa.nodes=first[4];pa.mode=struct.unpack_from('<b',first,5)[0];pa.next=struct.unpack_from('<H',first,6)[0];C.memmove(C.addressof(pa)+12,bytes(first[8:16]),8);ram[0xd02000+16*action_index:0xd02000+16*(action_index+1)]=first;check(ptr_word(pa.action)==struct.unpack_from('<I',first)[0] and pa.nodes==73,'exact genuine ordinary/Super action table row and retained action pointer')
  mutable=set(range(nl['player.anim.weld_mode'],nl['player.anim.weld_mode']+2))
  for index in range(6):
   for field in ('weld_point','weld_axis'):mutable.update(range(nl['player.'+field+str(index)],nl['player.'+field+str(index)]+12))
  def native_checked(function,*args):
   before=C.string_at(player,nl['player.sizeof']);function(*args);after=C.string_at(player,nl['player.sizeof']);check(all(before[i]==after[i] for i in range(nl['player.sizeof']) if i not in mutable),'all other native player record bytes unchanged by original weld routines')
  for index,row in enumerate(raw_rows):
   base,a,other,count,kind,na,nb,vertices,indices=row
   if base!=ptr_word(views[0].data) and not super_active:kind=0;ram[0xd01000+index*24+13]=0
   # Valid generic API cases on the same real geometry. Keep the genuine
   # table case separate; these are not claims about Sonic's default config.
   if variant!='genuine' and index==0:kind=1 if variant=='both' else 3;ram[0xd01000+index*24+13]=kind
   if variant!='genuine' and index==31:kind=9;ram[0xd01000+index*24+13]=kind
   welds[index]=Weld(native_ptr(base),native_ptr(a),native_ptr(other),count,kind,na,nb,None,pointer_from(index_arrays[indices]) if indices else None)
  C.c_int16.from_address(player+nl['player.anim.weld_mode']).value=0;C.c_int16.from_address(player+nl['player.anim']+nl['player_anim.state']).value=3;P.from_address(player+nl['player.anim.actions']).value=actions_ptr;P.from_address(player+nl['player.anim.welds']).value=weld_ptr;struct.pack_into('<h',ram,0xd00000+ol['player.anim.weld_mode'],0);struct.pack_into('<h',ram,0xd00000+ol['player.anim']+ol['player_anim.state'],3);iw(ram,0x8cd00000+ol['player.anim.actions'],[0x8cd02000]);iw(ram,0x8cd00000+ol['player.anim.welds'],[0x8cd01000]);player_records[0]=player;iw(ram,table['player_records'],[0x8cd00000]);heap_map={}
  def check_welds(phase):
   for index,weld in enumerate(welds):
    old=struct.unpack_from('<IIIBbBBII',ram,0xd01000+index*24)
    if weld.vertices:
     check(bool(old[7]),'original and native allocation both exist');heap_map[weld.vertices]=(old[7],weld.count*48);check(C.string_at(weld.vertices,weld.count*48)==bytes(ram[old[7]&0xffffff:(old[7]&0xffffff)+weld.count*48]),('real weld allocation payload matches',phase,index))
    def convert(ptr,kind):
     if not ptr:return 0
     if kind=='vertices':return heap_map[ptr][0]
     if kind=='indices':return next(at for at,buf in index_arrays.items() if pointer_from(buf)==ptr)
     return ptr_word(ptr)
    packed_row=struct.pack('<IIIBbBBII',convert(weld.base,'model'),convert(weld.object_a,'model'),convert(weld.object_b,'model'),weld.count,weld.kind,weld.node_a,weld.node_b,convert(weld.vertices,'vertices'),convert(weld.indices,'indices'));check(packed_row==bytes(ram[0xd01000+index*24:0xd01000+(index+1)*24]),('all weld record fields',phase,index))
   for index in range(6):
    for field in ('weld_point','weld_axis'):check(C.string_at(player+nl['player.'+field+str(index)],12)==bytes(ram[0xd00000+ol['player.'+field+str(index)]:0xd00000+ol['player.'+field+str(index)]+12]),('all player weld markers',phase,field,index))
   check(C.c_int16.from_address(player+nl['player.anim.weld_mode']).value==struct.unpack_from('<h',ram,0xd00000+ol['player.anim.weld_mode'])[0],'same weld lifecycle mode');check(guards(player_buf,nl['player.sizeof']) and guards(weld_buf,33*C.sizeof(Weld)) and guards(actions_buf,action_bytes),'native caller/output guards remain intact')
  invoke(ram,'player_update_welds',bank,{4:0,5:0,6:0x8cd00000});native_checked(native_update,None,None,player);check_welds('make');compare_models(ram,'make');live=sum(bool(x.vertices) for x in welds);check(live==(27 if super_active else 15),'genuine ordinary/boss initialization branch allocation count');check(bind(m,'jp_motions_shutdown',I)()==7,'live real model/action references prevent owner shutdown while welds hold pointers')
  for action_index,view_index in (((0,2),(130,3)) if super_active else ((0,2),)):
   C.c_uint16.from_address(player+nl['player.anim']+nl['player_anim.action']).value=action_index;struct.pack_into('<H',ram,0xd00000+ol['player.anim']+ol['player_anim.action'],action_index);active=Action.from_address(views[view_index].data);check(Motion.from_address(active.motion).frames>9,'genuine active stand motion extent')
   for frame in (0.,4.,8.5):
    C.c_float.from_address(player+nl['player.anim.frame']).value=frame;struct.pack_into('<f',ram,0xd00000+ol['player.anim.frame'],frame);invoke(ram,'player_update_welds',bank,{4:0,5:0,6:0x8cd00000});native_checked(native_update,None,None,player);check_welds('update_'+str(action_index)+'_'+str(frame));compare_models(ram,'update_'+str(action_index)+'_'+str(frame))
    invoke(ram,'player_weld',bank,{4:0,5:ptr_word(active.object),6:ptr_word(active.motion)},frame);native_checked(native_direct,0,active.object,active.motion,frame);check_welds('direct_'+str(action_index)+'_'+str(frame));compare_models(ram,'direct_'+str(action_index)+'_'+str(frame));print('update/direct',super_active,variant,bank,action_index,frame,'match',flush=True)
  C.c_int16.from_address(player+nl['player.anim.weld_mode']).value=2;struct.pack_into('<h',ram,0xd00000+ol['player.anim.weld_mode'],2);invoke(ram,'player_update_welds',bank,{4:0,5:0,6:0x8cd00000});native_checked(native_update,None,None,player);check_welds('free');compare_models(ram,'free');check(all(not x.vertices for x in welds),'all actual per-weld heap allocations released');check(stats()==(0x90000,0x90000),'actual heap fully coalesces after free');check(all(C.string_at(e.data,e.host_bytes)==canonical_before[e.name] for e in entries),'FREE itself restores all canonical bytes before Motion shutdown')
  for address,buf in index_arrays.items():check(guards(buf,len(index_before[address])) and C.string_at(pointer_from(buf),len(index_before[address]))==index_before[address],'shared original index buffers and guards unchanged')
  player_records[:]=saved_players
  for v in views:check(bind(m,'jp_motions_release',I,Q)(v.token)==0,'actual retained model/action view retires after free')
  check(bind(m,'jp_motions_shutdown',I)()==0,'actual Motion owner retires');records.append(dict(super_active=super_active,variant=variant,bank=bank,live_weld_allocations=live,frames=[0,4,8.5],all_canonical_records_restored_before_owner_shutdown=True))
faulthandler.disable();fault.close();out=dict(status='genuine_Sonic_and_Super_Sonic_weld_lifecycle_and_direct_weld_all32_rows_match_complete_original_calls',checks=checks,records=records,original_calls=original_calls,canonical_records=len(entries),prior_record_indices=2876,actual_task_heap_allocations_used=True,original_game_function_hooks_used=False,genuine_original_table_rows=32,ordinary_and_boss_Super_weld_modes=True,valid_generic_kind_variations_on_real_geometry=True,generic_variations_are_not_default_Sonic_configuration=True,all_weld_kinds_OFF_BOTH_TO_A_TO_B_and_six_MARK_outputs_exercised=True,original_action_rows_used=[0,130],other_player_record_bytes_preserved=True,blended_player_anim_state_branch_or_heap_exhaustion_verified=False,build_verification_sha256=sha(args.build/'build_verification.json'),verifier_sha256=sha(__file__),module_inputs_manifest_sha256=sha(args.module_inputs/'inputs.json'),reference_hashes=reference_hashes,player_weld_direct_pose_verified=True,event_motions_draw_and_gameplay_verified=False,whole_project_completion_percent=10,goal_active=True,playable=False);(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks',len(original_calls),'whole original calls')
