"""Check actual portable matrix state across native translation units.
This qualifies the selected single-game-thread backend, not SH-4 FPU equality.
"""
import argparse,ctypes as C,hashlib,json,random,struct
from pathlib import Path
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def f32(value):return C.c_float(value).value
def raw(values):return struct.pack('<'+str(len(values))+'f',*values)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);ap.add_argument('--build',type=Path,required=True);a=ap.parse_args()
 r=a.workspace.resolve();d=a.build.resolve();report=json.loads((d/'build_verification.json').read_text())
 dll=d/'jp_shz_native.dll';assert report['exit_code']==0 and report['diagnostic_bytes']==0 and sha(dll)==report['DLL_sha256']
 lib=C.CDLL(str(dll));P=C.POINTER(C.c_float)
 for name in ['jp_shz_load','jp_shz_store','jp_shz_store_from_other_unit']:
  fn=getattr(lib,name);fn.argtypes=[P];fn.restype=None
 lib.jp_shz_transform.argtypes=[P,P];lib.jp_shz_transform.restype=None
 assert lib.jp_shz_tls_model()==0 and lib.jp_shz_matrix_alignment()==8
 rng=random.Random(0x1007);checks=2;transforms=0
 for case in range(128):
  values=[f32(rng.uniform(-4,4)) for _ in range(16)]
  if case==0:values=[float(i%5==0) for i in range(16)]
  # Deliberately offset by four bytes from an eight-byte boundary.
  source=(C.c_float*18)(987,*values,654);destination=(C.c_float*18)(*[123.5]*18)
  src=C.cast(C.byref(source,4),P);out=C.cast(C.byref(destination,4),P)
  lib.jp_shz_load(src)
  lib.jp_shz_store(out);assert bytes(destination)[4:68]==raw(values)
  assert destination[0]==destination[-1]==123.5;checks+=2
  C.memset(C.addressof(destination)+4,0,64)
  lib.jp_shz_store_from_other_unit(out)
  assert bytes(destination)[4:68]==raw(values);checks+=1
  assert source[0]==987 and source[-1]==654;checks+=1
  for vector in [(1,0,0,0),(0,1,0,0),(0,0,1,0),(0,0,0,1),(1,-2,3,1),tuple(f32(rng.uniform(-2,2)) for _ in range(4))]:
   expected=[]
   for row in range(4):
    products=[f32(values[column*4+row]*vector[column]) for column in range(4)]
    expected.append(f32(f32(f32(products[0]+products[1])+products[2])+products[3]))
   input_vector=(C.c_float*4)(*vector);output_vector=(C.c_float*6)(456,*([0]*4),789)
   lib.jp_shz_transform(input_vector,C.cast(C.byref(output_vector,4),P))
   assert bytes(output_vector)[4:20]==raw(expected),(case,vector,tuple(output_vector),expected)
   assert output_vector[0]==456 and output_vector[-1]==789
   assert bytes(input_vector)==raw(vector)
   checks+=3;transforms+=1
  # In-place vectors must read the complete input before any output stores.
  vector=(C.c_float*4)(1,-2,3,1);expected=(C.c_float*4)()
  lib.jp_shz_transform(vector,expected);lib.jp_shz_transform(vector,vector)
  assert bytes(vector)==bytes(expected);checks+=1
 for name,h in report['authored_source_sha256'].items():assert sha(r/name)==h
 assert sha(r/'outputs/sonic-adventure/native/jp_sh4zam_host_config.h')==report['config_sha256']
 result=dict(status='actual_SH4ZAM_single_game_thread_native_backend_contracts_verified',checks_passed=checks,
  matrix_cases=128,independent_binary32_vector_transforms=transforms,cross_translation_unit_state_cases=128,
  unaligned_load_and_store_cases=128,in_place_vector_cases=128,DLL_sha256=sha(dll),
  build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
  limitations=['Single shared game-thread state; no thread-isolation claim.','Finite moderate inputs and zeros; portable sequential binary32 arithmetic, not SH-4 FTRV instruction rounding.','Renderer integration and game runtime remain unfinished.'],
  original_dependency_modified=False,game_source_linked=False,retail_assets_embedded=False,
  overall_completion_percent=10,goal_paused=False,playable=False)
 target=d/'runtime_verification.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
 print(checks,'portable backend checks passed;',transforms,'independent transforms; shared state verified across two callers')
if __name__=='__main__':main()
