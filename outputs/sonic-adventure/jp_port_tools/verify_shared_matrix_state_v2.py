"""Verify one active matrix across real Ninja and SH4ZAM APIs in one runtime."""
import argparse,ctypes as C,hashlib,json,random,struct
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);ap.add_argument('--build',type=Path,required=True);a=ap.parse_args()
 r=a.workspace.resolve();d=a.build.resolve();build=json.loads((d/'build_verification.json').read_text());dll=Path(build['DLL'])
 assert build['exit_code']==0 and build['diagnostic_bytes']==0 and sha(dll)==build['DLL_sha256']
 lib=C.CDLL(str(dll));P=C.c_void_p;I=C.c_int32;U=C.c_uint32
 signatures={
  'njInitMatrix':([P,I,I],None),'njGetMatrix':([P],None),'njSetMatrix':([P,P],None),'njUnitMatrix':([P],None),
  'njPushMatrix':([P],C.c_int),'njPopMatrix':([U],C.c_int),'njTranslateV':([P,P],None),'njCalcVector':([P,P,P],None),
  'jp_ninja_current_matrix_storage':([],P),'shz_xmtrx_state_':([],P),
  'jp_shz_load':([P],None),'jp_shz_store':([P],None),'jp_shz_store_from_other_unit':([P],None),
  'jp_shz_transform':([P,P],None)}
 for n,(args,ret) in signatures.items():fn=getattr(lib,n);fn.argtypes=args;fn.restype=ret
 Matrix=C.c_float*16;Vector=C.c_float*3;Vec4=C.c_float*4
 counts={};checks=0
 def verify(condition,message):
  nonlocal checks
  assert condition,message
  checks+=1
 def get_ninja():
  out=Matrix();lib.njGetMatrix(C.byref(out));return bytes(out)
 def get_shz(other=False):
  out=Matrix();getattr(lib,'jp_shz_store_from_other_unit' if other else 'jp_shz_store')(C.byref(out));return bytes(out)
 def both(expected,label):
  verify(get_ninja()==bytes(expected),label+' Ninja')
  verify(get_shz()==bytes(expected),label+' renderer')
  verify(get_shz(True)==bytes(expected),label+' second renderer unit')
 backing=(C.c_uint32*(16*8))(*([0xa5a5a5a5]*(16*8)))
 stack=(Matrix*6).from_buffer(backing,64);stack_address=C.addressof(stack)
 guard_before=bytes(backing)[:64];guard_after=bytes(backing)[-64:]
 lib.njInitMatrix(stack_address,6,0)
 pointer=lib.jp_ninja_current_matrix_storage()
 verify(pointer==lib.shz_xmtrx_state_(),'same actual storage pointer')
 verify(pointer%64==0,'explicit 64-byte alignment')
 verify(pointer>0xffffffff,'native pointer above 32-bit address range')
 verify(pointer!=stack_address,'active state is separate from deferred stack')
 reflection=Matrix(-1,0,0,0,0,-1,0,0,0,0,1,0,0,0,0,1)
 both(reflection,'initial reflected matrix');verify(bytes(stack[0])==bytes(reflection),'initial saved stack slot')
 counts['initialization_and_pointer_ABI']=checks
 rng=random.Random(0x1007);start=checks
 for case in range(128):
  # Copy-only paths must retain arbitrary float words, including NaN payloads.
  words=[rng.getrandbits(32) for _ in range(16)];matrix=Matrix.from_buffer_copy(struct.pack('<16I',*words))
  lib.njSetMatrix(None,C.byref(matrix));both(matrix,'Ninja-written state')
  words[case%16]^=0x80000000;matrix=Matrix.from_buffer_copy(struct.pack('<16I',*words))
  lib.jp_shz_load(C.byref(matrix));both(matrix,'renderer-written state')
  verify(lib.jp_ninja_current_matrix_storage()==lib.shz_xmtrx_state_()==pointer,'storage identity remains stable')
 counts['bidirectional_matrix_copy']=checks-start
 start=checks
 identity=Matrix(*[float(i%5==0) for i in range(16)])
 lib.njSetMatrix(None,C.byref(identity));translation=Vector(2,3,4)
 lib.njTranslateV(None,C.byref(translation))
 vertex=Vec4(1,2,3,1);out=Vec4();lib.jp_shz_transform(C.byref(vertex),C.byref(out))
 verify(tuple(out)==(3,5,7,1),'Ninja translation immediately affects renderer transform')
 rotation=Matrix(0,1,0,0,-1,0,0,0,0,0,1,0,0,0,0,1)
 lib.jp_shz_load(C.byref(rotation));vector=Vector(2,3,4);result=Vector()
 lib.njCalcVector(None,C.byref(vector),C.byref(result))
 verify(tuple(result)==(-3,2,4),'renderer-written rotation immediately affects Ninja vector transform')
 external=Matrix();before=get_ninja();lib.njSetMatrix(C.byref(external),C.byref(identity))
 verify(bytes(external)==bytes(identity) and get_ninja()==get_shz()==before,'explicit output matrix does not replace active state')
 counts['cross_API_geometry']=checks-start
 start=checks
 a=Matrix(*[float(i+1)/4 for i in range(16)]);b=Matrix(*[float(16-i)/8 for i in range(16)]);c=Matrix(*[float(i-8)/2 for i in range(16)])
 lib.njSetMatrix(None,C.byref(a));verify(lib.njPushMatrix(None)==1,'push saves and duplicates active matrix')
 both(a,'null-push state');verify(bytes(stack[0])==bytes(stack[1])==bytes(a),'null push saves both slots')
 lib.jp_shz_load(C.byref(b));stale=bytes(stack[2])
 verify(lib.njPushMatrix(C.byref(c))==1,'replacement push succeeds');both(c,'replacement-push state')
 verify(bytes(stack[1])==bytes(b) and bytes(stack[2])==stale,'replacement push saves renderer state and leaves new slot deferred')
 verify(lib.njPopMatrix(0)==1,'zero pop reloads saved slot');both(stale,'zero-pop deferred-slot reload')
 lib.jp_shz_load(C.byref(c));verify(lib.njPopMatrix(1)==1,'pop renderer-modified active state');both(b,'first restored shared matrix')
 verify(lib.njPopMatrix(1)==1,'pop to base');both(a,'base restored shared matrix')
 for invalid in [1,2,0xffffffff]:
  before_stack=bytes(backing);verify(lib.njPopMatrix(invalid)==0,'invalid pop rejected')
  both(a,'rejected-pop state');verify(bytes(backing)==before_stack,'invalid pop does not alter stack memory')
 for _ in range(5):verify(lib.njPushMatrix(None)==1,'fill stack within capacity')
 lib.jp_shz_load(C.byref(b));before_stack=bytes(backing)
 verify(lib.njPushMatrix(C.byref(c))==0,'full stack rejects replacement push');both(b,'rejected full push retains unsaved renderer state')
 verify(bytes(backing)==before_stack,'full push does not alter stack memory')
 verify(lib.njPopMatrix(5)==1,'multi-pop to base');both(a,'multi-pop shared restoration')
 verify(C.c_int32.in_dll(lib,'_stack_size_').value==1,'stack depth restored')
 verify(C.c_void_p.in_dll(lib,'_nj_current_matrix_ptr_').value==stack_address,'saved-slot pointer restored')
 verify(bytes(backing)[:64]==guard_before and bytes(backing)[-64:]==guard_after,'stack guards retained')
 counts['shared_deferred_stack_and_failure_paths']=checks-start
 for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h
 report=dict(status='Ninja_and_renderer_single_active_matrix_state_contracts_verified',checks_passed=checks,sections=counts,
  bidirectional_arbitrary_word_matrix_cases=128,shared_pointer_alignment=64,native_pointer_above_32bits_tested=True,
  shader_and_Ninja_geometry_observe_each_other=True,deferred_stack_semantics_preserved=True,
  DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
  limitations=['One game thread owns one runtime instance; independently built DLLs are separate instances.','Copy paths preserve arbitrary words; geometric examples are finite exact cases, not equivalence of all portable/SH-4 vector arithmetic.','Complete renderer, graphics submission, texture registry and gameplay remain unfinished.'],
  game_source_linked=build.get('game_source_linked',False),retail_assets_embedded=False,overall_completion_percent=10,goal_paused=False,playable=False)
 target=d/'shared_state_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
 print(checks,'shared Ninja/renderer state checks passed; alignment, bidirectional visibility and deferred stack verified')
if __name__=='__main__':main()
