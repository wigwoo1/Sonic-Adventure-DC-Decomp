"""Roundtrip canonical toe data and execute real original toe-model morphs."""
import argparse,bisect,ctypes as C,hashlib,json,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from verify_native_motion_data import Entry,Field
from verify_native_motion_store import View
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','build','binary','numeric-dll','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(a.build/'build_verification.json');pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json');assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 lib=C.CDLL(b['motion_DLL']);frame=C.CDLL(b['frames_DLL']);getter=bind(lib,'jp_motion_models_entry',C.c_int,C.c_uint32,C.c_void_p);count=bind(lib,'jp_motion_models_count',C.c_uint32)();assert count==2849;entries=[];checks=0
 def check(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 for i in range(count):
  e=Entry();check(getter(i,C.byref(e))==0,'actual canonical model record');entries.append(e)
 check([e.name.decode() for e in entries[:2793]]==b['toe_models_old_record_names'],'all previous canonical record identities/indices preserved')
 byname={e.name.decode():e for e in entries};intervals=sorted((e.data,e.data+e.host_bytes,e) for e in entries);starts=[x[0] for x in intervals];check(all(intervals[i-1][1]<=intervals[i][0] for i in range(1,len(intervals))),'all canonical definitions remain disjoint')
 layout=bind(lib,'jp_motion_layout',C.c_uint32,C.c_uint32,C.c_uint32,C.c_void_p);layouts={}
 for kind in {e.kind for e in entries if e.kind}:
  fields=[]
  for i in range(layout(kind,0,None)):
   f=Field();layout(kind,i,C.byref(f));fields.append(f)
  layouts[kind]=fields
 def pointer_word(pointer):
  if not pointer:return 0
  index=bisect.bisect_right(starts,pointer)-1;assert index>=0 and pointer<intervals[index][1],('unowned model pointer',hex(pointer));start,end,e=intervals[index];offset=pointer-start
  if not e.kind:return e.address+offset
  element,inner=divmod(offset,e.host_element)
  if not inner:return e.address+element*e.original_element
  for f in layouts[e.kind]:
   if f.host<=inner<f.host+(8 if f.pointer else f.bytes):assert not f.pointer or inner==f.host;return e.address+element*e.original_element+f.original+inner-f.host
  raise AssertionError(('native padding pointer',e.name,inner))
 def packed(e):
  raw=C.string_at(e.data,e.host_bytes)
  if not e.kind:return raw
  out=bytearray(e.original_bytes)
  for i in range(e.original_bytes//e.original_element):
   for f in layouts[e.kind]:
    old=i*e.original_element+f.original;new=i*e.host_element+f.host
    if f.pointer:struct.pack_into('<I',out,old,pointer_word(struct.unpack_from('<Q',raw,new)[0]))
    else:out[old:old+f.bytes]=raw[new:new+f.bytes]
  return bytes(out)
 initial=bytearray(16<<20);raw=a.binary.read_bytes();initial[0x10000:0x10000+len(raw)]=raw
 def compare(ram):
  for e in entries:check(packed(e)==bytes(ram[e.address&0xffffff:(e.address&0xffffff)+e.original_bytes]),('canonical complete original bytes',e.name))
 compare(initial);before={e.name:C.string_at(e.data,e.host_bytes) for e in entries}
 sys.path.insert(0,str(p));import verify_startup as executor
 from model_reference import make_reference
 from verify_fpu_cleanup_transfer import Fpu
 reference,reference_hashes=make_reference(executor);numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',C.c_int,C.c_uint32,C.c_void_p,C.c_uint32)
 def service(op,f):assert numeric(op,C.byref(f),f.fpul)==0
 table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)};records=[];acquire=bind(lib,'jp_motions_acquire',C.c_int,C.c_uint32,C.c_char_p,C.c_void_p);release=bind(lib,'jp_motions_release',C.c_int,C.c_uint64);shutdown=bind(lib,'jp_motions_shutdown',C.c_int);blend_fn=bind(frame,'model_blend',None,C.c_void_p,C.c_void_p,C.c_void_p,C.c_float)
 for bank in range(2):
  for side in ('plus','minus'):
   for blend in (0.,0.5,1.):
    check(bind(lib,'jp_motions_initialize',C.c_int,C.c_size_t,C.c_uint32)(8<<20,16)==0,'actual extended model-data owner initialized');views=[]
    for suffix in ('shape_a','shape_b','blend','model'):
     v=View();name='sonic_toe_'+side+'_z_'+suffix;check(acquire(7,name.encode(),C.byref(v))==0,'genuine toe model consumer acquired');views.append(v)
    v=View();check(acquire(7,('sonic_toe_'+side+'_z_face_normals').encode(),C.byref(v))==0,'genuine writable normal-buffer consumer acquired');views.append(v)
    check(shutdown()==7,'live actual toe views block owner shutdown')
    ram=bytearray(initial);f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][4]=struct.unpack('<I',struct.pack('<f',blend))[0];s=executor.State();s.pc=table['model_blend'];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000;s.r[4:7]=[byname['sonic_toe_'+side+'_z_'+n].address for n in ('shape_a','shape_b','blend')]
    reference(ram,s,fpu=f,vector_instruction=service,special={'gbr':0,'r_bank':[0]*8},instruction_budget=2000000);blend_fn(views[0].data,views[1].data,views[2].data,blend);check(s.r[15]==0x8cf00000,'complete original genuine toe model blend returns');compare(ram)
    records.append(dict(bank=bank,side=side,blend=blend,output_sha256=hashlib.sha256(b''.join(packed(e) for e in entries)).hexdigest()))
    for v in views:check(release(v.token)==0,'real toe view retired')
    check(shutdown()==0,'actual owner restores extended canonical data');check(all(C.string_at(e.data,e.host_bytes)==before[e.name] for e in entries),'all2849 canonical records restored byte-for-byte including both normal buffers')
 out=dict(status='genuine_canonical_toe_shapes_full_original_model_morph_and_owner_restoration_verified',checks=checks,canonical_model_records=count,prior_indices_preserved=2793,added_records=56,original_complete_genuine_toe_morph_calls=len(records),records=records,whole_canonical_model_pack_compared_after_each_morph=True,genuine_upstream_GCC_BSS_normal_buffers=True,actual_model_view_shutdown_protection_and_restoration_verified=True,reference_hashes=reference_hashes,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_binary_sha256=sha(a.binary),original_sonic_blend_toes_control_function_or_player_gameplay_verified=False,playable=False,overall_completion_percent=10)
 (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks;',len(records),'complete genuine toe morph calls',flush=True)
if __name__=='__main__':main()
