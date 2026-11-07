"""Original sine/finite unit-vector execution and shared sound forwarder proof."""
import argparse,ctypes as C,hashlib,json,random,re,struct,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def bits(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','build','upstream','binary','numeric-dll','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(a.build/'build_verification.json');pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json')
 assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 for key in ('frames_DLL','game_sound_DLL'):assert sha(b[key])==b[key+'_sha256']
 sys.path.insert(0,str(p));from verify_startup import State,reference
 from verify_fpu_cleanup_transfer import Fpu
 lib=C.CDLL(b['frames_DLL']);sound=C.CDLL(b['game_sound_DLL']);numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',C.c_int,C.c_uint32,C.c_void_p,C.c_uint32)
 sine=bind(lib,'njSin',C.c_float,C.c_int32);unit=bind(lib,'njUnitVector',C.c_float,C.c_void_p);checks=0;calls=0
 def check(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 check(C.cast(unit,C.c_void_p).value==C.cast(sound.njUnitVector,C.c_void_p).value,'sound PE forwarder resolves exact same normalization function address')
 def service(op,f):assert numeric(op,C.byref(f),f.fpul)==0
 for angle in range(65536):
  fp=Fpu();fp.fpscr=0x40001;fp.fpul=angle;service(0xf0fd,fp);check(bits(sine(angle))==fp.banks[0][0],('all sixteen-bit sine inputs agree with qualified FSCA',angle))
 table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)};ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw;rng=random.Random(0x4d4f5250);arena=(C.c_ubyte*64)()
 for bank in range(2):
  for i in range(512):
   fp=Fpu();fp.fpscr=0x40001|(bank<<21);s=State();s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000;s.pc=table['njSin'];angle=C.c_int32(rng.getrandbits(32)).value;s.r[4]=angle&0xffffffff
   reference(ram,s,fpu=fp,vector_instruction=service,instruction_budget=1000);check(s.r[15]==0x8cf00000 and bits(sine(angle))==fp.banks[bank][0],('complete original sine including wrapped signed/high-bit angle',bank,angle));calls+=1
   words=[(rng.getrandbits(1)<<31)|(rng.randrange(110,140)<<23)|rng.getrandbits(23) for _ in range(3)]
   if i<8:words=[bits(float((i+1)*(j+1))*(-1 if (i+j)%2 else 1)) for j in range(3)]
   rawarena=bytearray(rng.randbytes(64));struct.pack_into('<3I',rawarena,16,*words);ram[0xd00000:0xd00040]=rawarena;C.memmove(arena,bytes(rawarena),64)
   fp=Fpu();fp.fpscr=0x40001|(bank<<21);s=State();s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000;s.pc=table['njUnitVector'];s.r[4]=0x8cd00010
   reference(ram,s,fpu=fp,vector_instruction=service,instruction_budget=1000);value=unit(C.addressof(arena)+16);check(bytes(arena)==bytes(ram[0xd00000:0xd00040]) and bits(value)==fp.banks[bank][0] and s.r[15]==0x8cf00000,('complete original normalization, returned length and byte guards',bank,i));calls+=1
 reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
 def imports(path):
  run=subprocess.run([str(reader),'--coff-imports',str(path)],capture_output=True);assert not run.returncode and not run.stderr;return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
 graph={path.name.lower():[n.lower() for n in imports(path) if (a.build/n).exists()] for path in a.build.glob('*.dll')};active=set();done=set()
 def visit(n):
  assert n not in active;active.add(n)
  for child in graph[n]:
   if child not in done:visit(child)
  active.remove(n);done.add(n)
 for n in graph:
  if n not in done:visit(n)
 check('jp_native_game_sound.dll' not in graph['jp_native_frames.dll'] and 'jp_native_frames.dll' in graph['jp_native_game_sound.dll'],'same downward sound-to-Frame dependency; no reverse import/cycle')
 d.mkdir();out=dict(status='all_sine_angles_complete_original_math_samples_and_exact_sound_forwarder_verified',checks=checks,all_sixteen_bit_sine_inputs=65536,original_complete_machine_calls=calls,original_register_banks=2,normalization_scope='finite_nonzero_vectors_with_finite_nonzero_squared_length',nonfinite_zero_extreme_normalization_new_machine_proof=False,sound_and_Frame_normalizer_function_address_identical=True,actual_local_PE_import_graph=graph,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),console_hardware_validation=False,morph_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
 (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks;',calls,'original calls',flush=True)
if __name__=='__main__':main()
