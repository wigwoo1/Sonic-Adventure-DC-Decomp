"""Full VRAM conversion against a word oracle and the established aperture map."""
import array,ctypes,hashlib,json,random
from pathlib import Path
import pixel_banked_native as n
from pixel_banked_reference import texture_view
from vram_aperture_reference import mapping
P=Path(__file__).resolve().parent;R=P.parents[1]
def main():
 target=P/'analysis/banked_vram_view_verification.json';assert not target.exists()
 words=array.array('I',(((i*0x9e3779b1)^0x5a5a5a5a)&0xffffffff for i in range(0x200000)));raw=words.tobytes();source=(n.B*len(raw)).from_buffer_copy(raw);out=(n.B*len(raw))()
 fn=n.LIB.sa_pixel_vram64_view
 assert fn(source,len(raw),out,len(out))==0
 actual=bytes(out);assert actual==texture_view(raw) and bytes(source)==raw
 rng=random.Random(0x04000000);probes=[0,4,8,0x3ffff8,0x400000,0x7ffffc]+[rng.randrange(0x200000)*4 for _ in range(4096)]
 for offset in probes:
  bank_offset=mapping(0xa4000000+offset)-0xa5000000
  assert actual[offset:offset+4]==raw[bank_offset:bank_offset+4]
 before=bytes(out)
 for size,cap in ((len(raw)-4,len(raw)),(len(raw),len(raw)-4),(0,len(raw))):
  assert fn(source,size,out,cap)==-3 and bytes(out)==before
 assert fn(None,len(raw),out,len(raw))==-6 and bytes(out)==before
 assert fn(source,len(raw),source,len(raw))==-3 and bytes(source)==raw
 overlap=ctypes.cast(ctypes.byref(source,4),ctypes.POINTER(n.B))
 assert fn(source,len(raw),overlap,len(raw))==-3 and bytes(source)==raw
 files=[Path(__file__),P/'source/banked_pixel_jobs.c',P/'source/banked_pixel_jobs.h',P/'build_banked_pixel_jobs.ps1',P/'pixel_banked_native.py',P/'pixel_banked_reference.py',P/'vram_aperture_reference.py',n.DLL]
 result=dict(status='bank_linear_to_texture_view_full_VRAM_conversion_verified',full_vram_bytes_compared=len(raw),full_conversion_words=len(words),legacy_aperture_mapping_probes=len(probes),invalid_range_null_and_overlap_controls=6,input_bytes_unchanged=True,overall_completion_percent=10,physical_GPU_pixels_qualified=False,sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('VERIFIED full 8MiB view,',len(probes),'aperture probes and six rejection controls',flush=True)
if __name__=='__main__':main()
