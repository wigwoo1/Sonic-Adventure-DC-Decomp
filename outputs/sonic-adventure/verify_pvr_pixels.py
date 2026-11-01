"""Exhaustive colour decoding and independent complete-buffer raster checks."""
import ctypes,hashlib,json,random,struct
from pathlib import Path
import pvr_pixels_native as n
import pvr_pixels_reference as ref
P=Path(__file__).resolve().parent;R=P.parents[1]
def bits(f):return int.from_bytes(struct.pack('<f',f),'little')
def native_decode(raw,tsp,tcw,level,capacity):
 storage=(n.B*len(raw)).from_buffer_copy(raw);out=(n.U*capacity)(*[0x12345678]*capacity);info=n.Texture()
 rc=n.LIB.sa_pixel_texture_decode(storage,len(raw),tsp,tcw,level,out,capacity,ctypes.byref(info))
 return rc,bytes(out),n.snapshot(info)
def main():
 target=P/'analysis/pvr_pixels_verification.json';assert not target.exists();rng=random.Random(0x828c0028);checks=cases=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 # Every16-bit source word, independently expanded by bit strings.
 raw=b''.join(v.to_bytes(2,'little') for v in range(65536))
 for fmt in range(3):
  tsp=(5<<3)|5;tcw=(fmt<<27)|(1<<26)
  rc,pixels,info=native_decode(raw,tsp,tcw,0,65536);expected=[ref.unpack(i,fmt) for i in range(65536)]
  require(rc==0 and pixels==struct.pack('<65536I',*expected) and info[:2]==(256,256),'exhaustive direct16-bit pixel expansion')
 texture_cases=0
 for w,h in ((8,8),(8,32),(32,8),(16,64),(64,16),(64,64),(1024,8),(8,1024)):
  for fmt in range(3):
   for linear,mip in ((0,0),(1,0),(0,1)):
    if mip and w!=h:continue
    tsp=((w.bit_length()-4)<<3)|(h.bit_length()-4);tcw=(fmt<<27)|(linear<<26)|(mip<<31)
    data=bytes(rng.randrange(256) for _ in range(w*h*3+64))
    for level in range(w.bit_length()) if mip else (0,):
     rc,wanted,ti=ref.decode(data,tsp,tcw,level)
     actual,buf,info=native_decode(data,tsp,tcw,level,ti[0]*ti[1])
     require(actual==rc==0 and info==ti and buf==struct.pack('<'+'I'*len(wanted),*wanted),'recursive Morton/mipmap layout including rectangular domains')
     texture_cases+=1
 for case in range(256):
  width=rng.randrange(5,20);height=rng.randrange(5,20);o=n.Options(width,height,width,height,rng.randrange(2),0,rng.randrange(33),128)
  fmt=rng.randrange(3);tcw=(fmt<<27)|(rng.randrange(2)<<26);tsp=(rng.randrange(8)<<29)|(rng.randrange(8)<<26)|(2<<22)|(rng.randrange(2)<<20)|(rng.randrange(2)<<19)|(rng.randrange(2)<<18)|(rng.randrange(2)<<17)|(rng.randrange(2)<<16)|(rng.randrange(2)<<15)|(rng.randrange(4)<<6)
  pcw=0x80000000|(rng.choice((0,2,4))<<24)|(rng.choice((0,2))<<4)|8|(rng.randrange(2)<<1)
  isp=(rng.randrange(8)<<29)|(rng.randrange(4)<<27)|(rng.randrange(2)<<26)
  words=[pcw,isp,tsp,tcw,*[bits(rng.random()) for _ in range(4)]]
  for i in range(4):
   words += [0xf0000000 if i==3 else 0xe0000000,bits(rng.uniform(-4,width+4)),bits(rng.uniform(-4,height+4)),bits(rng.uniform(0.05,2)),bits(rng.uniform(-0.5,1.5)),bits(rng.uniform(-0.5,1.5)),bits(rng.random()) if (pcw>>4)&3==2 else rng.getrandbits(32),0]
  vram=bytes(rng.randrange(256) for _ in range(128));pixels=[rng.getrandbits(32) for _ in range(width*height)];depth=[rng.randrange(65536) for _ in pixels]
  a=n.draw(words,vram,o,pixels,depth);b=ref.draw(words,vram,o,pixels,depth)
  require(a[0]==b[0]==0 and a[1]==struct.pack('<'+'I'*len(b[1]),*b[1]) and a[2]==struct.pack('<'+'I'*len(b[2]),*b[2]) and a[3]==b[3],('complete independent raster buffers/stats',case,a[0],b[0],a[3],b[3]))
  cases+=1
 # Shared diagonal has no duplicate coverage. Off requires no VRAM pointer.
 quad=[0x82800028,7<<29,(1<<29)|(2<<22)|(1<<20),0xffffffff,*([bits(1)]*4)]
 for i,(x,y) in enumerate(((1,1),(1,7),(7,1),(7,7))):quad += [0xf0000000 if i==3 else 0xe0000000,bits(x),bits(y),bits(1),bits(0),bits(0),bits(1),0]
 o=n.Options(8,8,8,8,0,0,0,128);a=n.draw(quad,None,o)
 require(a[0]==0 and a[3]==(2,36,36,0,36) and sum(x!=0 for x in struct.unpack('<64I',a[1]))==36,'Off is vertex/material coloured and never samples, including a shared strip edge')
 bad=quad+[quad[0]|1,*quad[1:8]];saved=[0x12345678]*64;olddepth=[0x98765432]*64
 a=n.draw(bad,None,o,saved,olddepth)
 require(a[0]==-2 and a[1]==struct.pack('<64I',*saved) and a[2]==struct.pack('<64I',*olddepth),'unsupported late input cannot partially draw')
 # A long viewport exercises scaling and indexing without a huge reference image.
 o=n.Options(7680,1,640,8,0,0,0,128);a=n.draw(quad,None,o);b=ref.draw(quad,None,o)
 require(a[0]==b[0]==0 and a[1]==struct.pack('<7680I',*b[1]) and a[2]==struct.pack('<7680I',*b[2]) and a[3]==b[3],'7680-wide native addressing and projected scaling')
 files=[Path(__file__),P/'pvr_pixels_native.py',P/'pvr_pixels_reference.py',P/'source/pvr_pixels.c',P/'source/pvr_pixels.h',P/'build_pvr_pixels.ps1',n.DLL]
 report=dict(status='bounded_host_texture_and_integer_raster_profile_verified',checks=checks,exhaustive_16bit_colour_words=65536*3,texture_layout_cases=texture_cases,random_raster_cases=cases,independent_recursive_texture_oracle=True,independent_rational_raster_oracle=True,full_colour_and_depth_buffers_compared=True,texture_Off_disables_sampling=True,maximum_width_line_checked=7680,physical_PowerVR_pixel_accuracy_qualified=False,automatic_mip_LOD_qualified=False,hardware_render_timing_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('VERIFIED native pixels',checks,'checks;',cases,'raster cases;',texture_cases,'texture layout cases',flush=True)
if __name__=='__main__':main()
