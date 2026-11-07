"""Whole-original angle_difference: every 16-bit delta, varied high words."""
import argparse,ctypes as C,hashlib,json,random,struct,sys
from pathlib import Path
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes());assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(a.binary)==json.loads((p/'jp_port_tools/jp_view_stack_external_inputs.json').read_bytes())['original_binary_sha256'];sys.path.insert(0,str(p));import verify_startup as executor
 from model_reference import make_reference
 from verify_fpu_cleanup_transfer import Fpu
 reference,reference_hashes=make_reference(executor);f=C.CDLL(b['frames_DLL']);native=bind(f,'angle_difference',C.c_int32,C.c_int32,C.c_int32);ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw;rng=random.Random(0x58cce);digest=hashlib.sha256();samples=[];checks=0
 for delta in range(65536):
  left=rng.getrandbits(32);right=(rng.getrandbits(16)<<16)|((left+delta)&65535);s=executor.State();s.pc=0x8c058cce;s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000;s.r[4]=left;s.r[5]=right;fp=Fpu();reference(ram,s,fpu=fp,special={'gbr':0,'r_bank':[0]*8},instruction_budget=1000);actual=native(C.c_int32(left).value,C.c_int32(right).value);expected=C.c_int32(s.r[0]).value
  assert s.r[15]==0x8cf00000,('whole original return',delta);checks+=1
  assert actual==expected,('same signed short-turn angle',delta,left,right,actual,expected);checks+=1;digest.update(struct.pack('<IIIi',delta,left,right,actual))
  if delta in (0,1,32767,32768,32769,65535):samples.append(dict(delta=delta,left=f'{left:08x}',right=f'{right:08x}',result=actual))
 out=dict(status='original_angle_difference_every16bit_delta_and_varied_argument_high_words_match_complete_SH4_calls',checks=checks,original_complete_calls=65536,all_16bit_differences_covered=True,high_words_varied=True,input_output_sha256=digest.hexdigest(),samples=samples,original_binary_sha256=sha(a.binary),reference_hashes=reference_hashes,original_game_function_hooks_used=False,native_CPU_registers_or_SR_compared=False,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10,goal_active=True);(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks;65536 whole original calls',flush=True)
if __name__=='__main__':main()
