"""Compare genuine native SDK floorf return bits/SDK errno with whole SH4 calls.

Fixed RM=1,DN=1 value policy. FPSCR flags/traps and arbitrary host rounding
modes are explicitly outside this native value API's contract.
"""
import argparse,ctypes as C,hashlib,json,random,struct,sys
from pathlib import Path
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','build','binary','numeric-dll','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=read(a.build/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 assert sha(b['frames_DLL'])==b['frames_DLL_sha256'];pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json');assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
 sys.path.insert(0,str(p));import verify_startup as executor
 from model_reference import make_reference
 from verify_fpu_cleanup_transfer import Fpu
 reference,reference_hashes=make_reference(executor);numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',C.c_int32,C.c_uint32,C.c_void_p,C.c_uint32)
 def service(op,state):assert numeric(op,C.byref(state),state.fpul)==0
 f=C.CDLL(b['frames_DLL']);native=bind(f,'jp_runtime_floor_word',C.c_uint32,C.c_uint32);sdk_errno=C.c_int32.in_dll(f,'jp_sdk_errno');old_errno=sdk_errno.value;ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw
 inputs=set()
 for sign in (0,0x80000000):
  for exponent in range(256):
   for fraction in (0,1,0x3fffff,0x400000,0x7ffffe,0x7fffff):inputs.add(sign|(exponent<<23)|fraction)
  for integer in range(257):
   word=struct.unpack('<I',struct.pack('<f',float(integer)))[0]
   for delta in (-1,0,1):
    if 0<=word+delta<=0x7fffffff:inputs.add(sign|(word+delta))
 rng=random.Random(0x1238b4)
 for i in range(2048):inputs.add(rng.getrandbits(32))
 inputs=sorted(inputs);checks=0;records=[];seen_values=set();seen_errors=set()
 def check(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 check(b['runtime_floor_recipe']['existing_SDK_print_errno_owner_reused'],'same pre-existing SDK print errno owner reused,not duplicate storage')
 try:
  for bank in (0,1):
   for index,bits in enumerate(inputs):
    before=(0x12345678^(index*0x9e3779b9))&0xffffffff;struct.pack_into('<I',ram,0x7c0c40,before);fp=Fpu();fp.fpscr=0x40001|(bank<<21);fp.banks[bank][4]=bits;s=executor.State();s.pc=0x8c11c1d8;s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000;entered=[]
    def trace(pc,word,regs,sr,pr,fpu):
     if pc==0x8c1238b4:entered.append(pc)
    reference(ram,s,fpu=fp,vector_instruction=service,special={'gbr':0,'r_bank':[0]*8},trace=trace,instruction_budget=1000000);expected=fp.banks[bank][0];error=struct.unpack_from('<I',ram,0x7c0c40)[0];sdk_errno.value=C.c_int32(before).value;actual=native(bits);actual_error=sdk_errno.value&0xffffffff
    (d/'last_case.json').write_text(json.dumps(dict(input=f'{bits:08x}',bank=bank,expected=f'{expected:08x}',actual=f'{actual:08x}',original_errno=error,native_errno=actual_error),indent=2)+'\n') if actual!=expected or error!=actual_error else None
    check(s.r[15]==0x8cf00000 and entered==[0x8c1238b4],('whole genuine floorf and _in_floors return safely',bits,bank))
    check(actual==expected,('exact floor return bits',hex(bits),bank,hex(actual),hex(expected)))
    check(actual_error==error,('same original SDK errno mutation/preservation',hex(bits),bank,actual_error,error));seen_values.add(actual);seen_errors.add(error if error in (1100,1101) else 'unchanged');records.append(dict(input=f'{bits:08x}',result=f'{expected:08x}',errno_before=f'{before:08x}',errno_after=f'{error:08x}',bank=bank))
 finally:sdk_errno.value=old_errno
 half=len(inputs);check([dict(x,bank=0) for x in records[:half]]==[dict(x,bank=0) for x in records[half:]],'native/original output and error state independent of active SH4 register bank')
 check({1100,1101,'unchanged'}<=seen_errors,'infinity,NaN and ordinary SDK errno policies exercised')
 out=dict(status='genuine_runtime_floor_return_bits_and_shared_SDK_errno_match_complete_original_SH4_calls',checks=checks,unique_inputs=len(inputs),original_complete_floor_calls=len(records),both_SH4_FPU_banks=True,all_binary32_sign_exponent_classes=True,input_fractions_per_exponent=6,near_integer_inputs='0..256 and adjacent binary32 values,both signs',random_input_draws=2048,input_word_sha256=hashlib.sha256(struct.pack('<'+'I'*len(inputs),*inputs)).hexdigest(),output_record_sha256=hashlib.sha256(json.dumps(records,separators=(',',':')).encode()).hexdigest(),records=records,original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),reference_hashes=reference_hashes,original_game_function_hooks_used=False,entire_RAM_or_native_CPU_state_compared=False,FPSCR_flags_traps_and_arbitrary_host_FP_modes_verified=False,fixed_numerical_policy='RM=1,DN=1;return values and shared SDK errno',SDK_errno_reuses_same_existing_print_owner=True,SDK_errno_does_not_alias_host_CRT_errno_function=True,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10,goal_active=True)
 (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks;',len(records),'whole original calls',flush=True)
if __name__=='__main__':main()
