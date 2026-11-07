"""Private full-original tree-index proof for every genuine Sonic sub-root."""
import argparse,ctypes as C,hashlib,json,sys
from pathlib import Path
ap=argparse.ArgumentParser()
for n in ('workspace','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
a=ap.parse_args();r=a.workspace.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(w);d.mkdir();sys.path.insert(0,str(p/'jp_port_tools'));sys.path.insert(0,str(p))
import verify_startup as executor
from model_reference import make_reference
from verify_fpu_cleanup_transfer import Fpu
from native_packet_test_support import bind
from verify_native_motion_data import Entry
from verify_native_motion_store import View
from verify_native_character_tree import Node
def sha(x):return hashlib.sha256(Path(x).read_bytes()).hexdigest()
b=json.loads((a.build/'build_verification.json').read_bytes());pin=json.loads((p/'jp_port_tools/jp_view_stack_external_inputs.json').read_bytes());binary=a.binary;assert sha(binary)==pin['original_binary_sha256'] and sha(b['frames_DLL'])==b['frames_DLL_sha256'];reference,reference_hashes=make_reference(executor);f=C.CDLL(b['frames_DLL']);motions=C.CDLL(b['motion_DLL']);I,U,P,Q,S=C.c_int32,C.c_uint32,C.c_void_p,C.c_uint64,C.c_size_t
assert bind(motions,'jp_motions_initialize',I,S,U)(8<<20,16)==0 and bind(motions,'jp_motions_select',I,I)(0)==0;v=View();assert bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(7,b'sonic_object',C.byref(v))==0;entries=[]
for i in range(bind(motions,'jp_motion_models_count',U)()):e=Entry();assert bind(motions,'jp_motion_models_entry',I,U,P)(i,C.byref(e))==0;entries.append(e)
def original(ptr):
 e=next(e for e in entries if e.data<=ptr<e.data+e.host_bytes);assert (ptr-e.data)%e.host_element==0;return e.address+(ptr-e.data)//e.host_element*e.original_element
def digest():
 h=hashlib.sha256()
 for e in entries:h.update(C.string_at(e.data,e.host_bytes))
 return h.hexdigest()
before=digest();nodes=[];seen=set()
def visit(ptr):
 assert ptr not in seen;seen.add(ptr);nodes.append(ptr);n=Node.from_address(ptr)
 if n.child:visit(n.child)
 if n.sibling:visit(n.sibling)
visit(v.data);assert len(nodes)==73;ram=bytearray(16<<20);raw=binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw;native=bind(f,'tree_node_index',I,P,P);other=Node();targets=[(x,original(x)) for x in nodes]+[(None,0),(C.addressof(other),0x8cd0f000)];checks=0;calls=0;output=hashlib.sha256()
for root in nodes:
 for target,original_target in targets:
  s=executor.State();s.pc=0x8c059700;s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000;s.r[4]=original(root);s.r[5]=original_target;reference(ram,s,fpu=Fpu(),special={'gbr':0,'r_bank':[0]*8},instruction_budget=100000);actual=native(root,target);expected=C.c_int32(s.r[0]).value;assert s.r[15]==0x8cf00000;checks+=1;assert actual==expected,(hex(original(root)),hex(original_target),actual,expected);checks+=1;calls+=1;output.update(f'{original(root):08x}:{original_target:08x}:{actual}\n'.encode())
assert digest()==before;checks+=1;assert bind(motions,'jp_motions_release',I,Q)(v.token)==0 and bind(motions,'jp_motions_shutdown',I)()==0;checks+=1
out=dict(status='genuine_Sonic_tree_index_all73_subroots_targets_and_missing_targets_match_whole_original_calls',checks=checks,original_complete_calls=calls,genuine_nodes=73,targets_per_root=75,all_canonical_model_bytes_unchanged=True,output_sha256=output.hexdigest(),build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_binary_sha256=sha(binary),reference_hashes=reference_hashes,original_game_function_hooks_used=False,player_weld_lifecycle_allocation_mutation_free_still_unverified=True,overall_completion_percent=10,goal_active=True,playable=False);(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks;',calls,'whole original calls')
