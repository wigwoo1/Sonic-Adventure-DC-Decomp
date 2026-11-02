"""Verify adapted renderer memory/stride contracts with guarded binary vectors.
Tests exact helper spans from the separately compiled complete renderer.
No whole-renderer, rasterization, texture-resolver or gameplay claim.
"""
import argparse,ctypes as C,hashlib,itertools,json,random,struct
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def f32(x):return C.c_float(x).value
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);ap.add_argument('--build',type=Path,required=True);a=ap.parse_args()
 r=a.workspace.resolve();d=a.build.resolve();build=json.loads((d/'build_verification.json').read_text())
 dll=d/'jp_renderer_helpers.dll';assert build['exit_code']==0 and build['diagnostic_bytes']==0 and sha(dll)==build['DLL_sha256']
 lib=C.CDLL(str(dll));V=C.c_void_p;U=C.c_uint32;F=C.c_float
 signatures={'jp_rh_next_vector':([V,F,V],U),'jp_rh_project':([V,F,F,V],C.c_int),
  'jp_rh_take_point':([V,V,C.c_int,V],None),'jp_rh_send':([V,U,V,F,F],U),
  'jp_rh_next_index':([V,C.POINTER(U)],C.c_int),'jp_rh_next_uv':([V,V],U),
  'jp_rh_prefetch_count':([],U),'jp_rh_prefetch_last':([],V)}
 for name,(args,ret) in signatures.items():fn=getattr(lib,name);fn.argtypes=args;fn.restype=ret
 def buffer(raw):return C.create_string_buffer(raw,len(raw))
 def guarded(length):return buffer(bytes([0xa5])*4+bytes([0xcd])*length+bytes([0x5a])*4)
 def pointer(buf,offset=0):return C.addressof(buf)+offset
 checks=0;counts=dict(next_vector=0,project_end=0,take_point=0,send_textured=0,next_index=0,next_uv=0)
 patterns=[0,0x80000000,0x3f800000,0xbf800000,1,0x80000001,0x7f800000,0xff800000,0x7fc12345,0x7fa12345,0xffffffff]
 rng=random.Random(0x1007)
 for case in range(128):
  words=[patterns[(case+i)%len(patterns)] if case<32 else rng.getrandbits(32) for i in range(16)]
  vectors=buffer(struct.pack('<6I',*words[:6]));out=guarded(16);before=bytes(vectors)
  assert lib.jp_rh_next_vector(pointer(vectors),F(-0.5),pointer(out,4))==12
  assert bytes(out)==bytes([0xa5])*4+before[:12]+struct.pack('<f',-0.5)+bytes([0x5a])*4
  assert bytes(vectors)==before;checks+=3;counts['next_vector']+=1
  points=buffer(struct.pack('<16I',*words));offsets=buffer(struct.pack('<4I',*words[8:12]));original=bytes(points),bytes(offsets)
  for index in range(4):
   out=guarded(20);lib.jp_rh_take_point(pointer(points),pointer(offsets),index,pointer(out,4))
   expected=original[0][16*index:16*index+16]+original[1][4*index:4*index+4]
   assert bytes(out)==bytes([0xa5])*4+expected+bytes([0x5a])*4
   assert (bytes(points),bytes(offsets))==original;checks+=2;counts['take_point']+=1
  payload=buffer(struct.pack('<5I',*words[:5]));saved=bytes(payload);out=guarded(32)
  control=words[5];u,v=f32(-1.25),f32(0.75);previous=lib.jp_rh_prefetch_count()
  assert lib.jp_rh_send(pointer(out,4),control,pointer(payload),u,v)==32
  expected=struct.pack('<I',control)+saved[:12]+struct.pack('<2f',u,v)+saved[12:20]
  assert bytes(out)==bytes([0xa5])*4+expected+bytes([0x5a])*4
  assert lib.jp_rh_prefetch_count()==previous+1 and lib.jp_rh_prefetch_last()==pointer(out,4)
  assert bytes(payload)==saved;checks+=4;counts['send_textured']+=1
 for case in range(128):
  x,y,rhw=[f32(rng.uniform(-4,4)) for _ in range(3)]
  cx,cy=f32(rng.uniform(-16,16)),f32(rng.uniform(-16,16))
  if case<4:x,y,rhw,cx,cy=[0.0,-0.0,1.0,-1.0][case],-0.0,0.0,-0.0,0.0
  source=buffer(struct.pack('<3f',x,y,rhw));before=bytes(source);out=guarded(16);color=bytes(out)[16:20]
  assert lib.jp_rh_project(pointer(source),cx,cy,pointer(out,4))==1
  expected=struct.pack('<3f',f32(f32(x*rhw)+cx),f32(f32(y*rhw)+cy),rhw)+color
  assert bytes(out)==bytes([0xa5])*4+expected+bytes([0x5a])*4
  assert bytes(source)==before;checks+=3;counts['project_end']+=1
 signed=[-32768,-32767,-256,-1,0,1,255,256,32766,32767]
 for value in signed:
  source=buffer(struct.pack('<3h',value,123,-456));before=bytes(source);stride=U()
  assert lib.jp_rh_next_index(pointer(source),C.byref(stride))==value and stride.value==2
  assert bytes(source)==before;checks+=2;counts['next_index']+=1
 inv=f32(float.fromhex('0x1.010106p-8'))
 for u,v in itertools.product(signed,repeat=2):
  source=buffer(struct.pack('<4h',u,v,11,22));before=bytes(source);out=guarded(8)
  assert lib.jp_rh_next_uv(pointer(source),pointer(out,4))==4
  expected=struct.pack('<2f',f32(u*inv),f32(v*inv))
  assert bytes(out)==bytes([0xa5])*4+expected+bytes([0x5a])*4
  assert bytes(source)==before;checks+=3;counts['next_uv']+=1
 renderer=(r/'work/sonic-adventure/jp-native-renderer-object-v2/external_model_native.c').read_bytes()
 assert hashlib.sha256(renderer).hexdigest()==build['source_renderer_generated_sha256']
 for span in build['exact_selected_spans']:
  assert hashlib.sha256(renderer[span['byte_start']:span['byte_end']]).hexdigest()==span['sha256']
 result=dict(status='six_actual_native_renderer_data_access_helpers_contract_verified',checks_passed=checks,cases_by_function=counts,
  arbitrary_binary32_copy_patterns_include_signed_zeros_subnormals_infinities_and_NaN_payloads=True,
  projected_arithmetic_scope='finite moderate inputs and signed zeros, separate binary32 product/add rounding',
  signed_16bit_index_and_UV_boundary_values=True,guarded_outputs_and_source_immutability=True,
  stride_and_prefetch_destination_contracts=True,exact_helper_span_count=6,
  DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
  scope='Native helper byte/stride contracts and independent finite projection/UV arithmetic. Full renderer separately compiles, but its 33 imports are not resolved by this test. No dummy graphics/texture/presentation functions.',
  limitations=['Not an original SH-4 instruction execution comparison.','No whole-renderer link, triangles/rasterization, texture-handle registry, shared Ninja/SH4ZAM state integration or gameplay verified.'],
  original_source_modified=False,game_source_artifact_publishable=False,overall_completion_percent=10,goal_paused=False,playable=False)
 target=d/'runtime_verification.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
 print(checks,'renderer data-access checks passed;',sum(counts.values()),'helper cases')
if __name__=='__main__':main()
