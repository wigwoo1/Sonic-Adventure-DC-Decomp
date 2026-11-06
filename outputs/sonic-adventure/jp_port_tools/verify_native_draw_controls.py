"""Mixed original SH-4 draw attribute/control executions vs real native state."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from build_native_draw_controls import NAMES
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
 b=json.loads((a.build/'build_verification.json').read_bytes());pin=json.loads((p/'jp_port_tools/jp_view_stack_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256'] and sha(b['frames_DLL'])==b['frames_DLL_sha256']
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 sys.path.insert(0,str(p));from verify_startup import State,reference
 table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)};ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw
 lib=C.CDLL(b['frames_DLL']);fields=('_nj_constant_attr_and_','_nj_constant_attr_or_','_nj_control_3d_flag_');native=[C.c_uint32.in_dll(lib,n) for n in fields];rng=random.Random(0x44524157);checks=calls=0;counts={n:0 for n in NAMES}
 for n in fields+('saved_attr_and','saved_attr_or','saved_control_3d'):struct.pack_into('<I',ram,table[n]&0xffffff,0)
 for v in native:v.value=0
 for i in range(4104):
  if i%16==0:
   for n,v in zip(fields,native):v.value=rng.getrandbits(32);struct.pack_into('<I',ram,table[n]&0xffffff,v.value)
  name=NAMES[i%8] if i<64 or i>=4096 else rng.choice(NAMES);argc=0 if name.endswith(('save','restore')) else 2 if name.startswith('draw_attr') else 1
  args=[rng.choice((0,0xffffffff,1<<rng.randrange(32),rng.getrandbits(32))) for _ in range(argc)];s=State();s.pc=table[name];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.r[4:4+argc]=args;s.sr=0x70000000
  reference(ram,s,instruction_budget=1000);bind(lib,name,None,*([C.c_uint32]*argc))(*args);calls+=1;counts[name]+=1
  expected=[struct.unpack_from('<I',ram,table[n]&0xffffff)[0] for n in fields];assert [v.value for v in native]==expected,(i,name,args,expected,[v.value for v in native]);checks+=1
  assert s.r[15]==0x8cf00000;checks+=1
 d.mkdir();out=dict(status='eight_original_draw_controls_mixed_sequences_match_complete_JP_SH4_execution',checks=checks,original_complete_calls=calls,function_calls=counts,original_binary_sha256=sha(a.binary),build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),real_existing_renderer_state=True,original_single_slot_save_semantics=True,original_player_callback_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
 (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks;',calls,'original calls',flush=True)
if __name__=='__main__':main()
