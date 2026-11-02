"""Match captured banked texture data to its retail archive and native pixels."""
import ctypes,hashlib,json,struct
from pathlib import Path
import pixel_banked_native as j
import pvr_pixels_native as p
import pvr_pixels_reference as ref
import render_completion_native as r
from pixel_banked_reference import texture_view
from decompress_prs import decompress
import write_watch_renderer_render_completion as abi
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(raw):return hashlib.sha256(raw).hexdigest()
def main():
 target=P/'analysis/banked_texture_output_verification.json';assert not target.exists()
 inputs=R/'work/sonic-adventure/pixel-inputs';metadata=json.loads((inputs/'metadata.json').read_text());banks=(inputs/'original_scene_vram.bin').read_bytes();quad=(inputs/'original_scene_quad.bin').read_bytes()
 assert sha(banks)==metadata['vram_sha256'] and sha(quad)==metadata['quad_sha256'];words=list(struct.unpack('<40I',quad))
 logical=texture_view(banks);native_banks=(j.B*len(banks)).from_buffer_copy(banks);scratch=(j.B*len(banks))();assert j.LIB.sa_pixel_vram64_view(native_banks,len(banks),scratch,len(scratch))==0 and bytes(scratch)==logical
 asset=R/'work/sonic-adventure/extracted/files/SONICADV/EFF_REGULAR.PRS';compressed=asset.read_bytes();decoded,_=decompress(compressed)
 assert sha(compressed)=='2c62f8a4035181cb5366a8ea5332a793ae304b0a9546c43faa186e35c6d04096' and sha(decoded)=='686237260763d1b8a826f0949e1d3d9a2565216ab15b6418b6168e8e3e96aa0b'
 assert decoded[:4]==b'PVMH' and int.from_bytes(decoded[10:12],'little')==138
 # Walk actual chunk lengths; find a unique match for every mip texel byte.
 base=(words[3]&0x1ffffff)*8;chain=logical[base+6:base+10928];matches=[];at=0;ordinal=0
 while at<len(decoded):
  tag=decoded[at:at+4];size=int.from_bytes(decoded[at+4:at+8],'little');end=at+8+size;assert size>0 and end<=len(decoded)
  if tag==b'PVRT':
   if decoded[at+8:at+16]==bytes.fromhex('0002000040004000') and decoded[at+18:at+18+len(chain)]==chain:matches.append((ordinal,at,end))
   ordinal+=1
  else:assert tag==b'PVMH' and at==0
  at=end
 assert ordinal==138 and len(matches)==1 and len(chain)==10922
 index,start,end=matches[0];record=decoded[12+index*34:12+(index+1)*34];assert int.from_bytes(record[:2],'little')==index
 binding_info=dict(texture_ordinal=index,global_index=hex(int.from_bytes(record[30:34],'little')),archive_chunk_offset=hex(start),format_header_hex=decoded[start+8:start+16].hex(),all_mip_texel_bytes_matched=len(chain),texel_chain_sha256=sha(chain),raw_leading_padding_bytes_excluded=True)
 # Decode each archive level directly, independently of the VRAM view formula.
 pos=start+18;mips=[]
 for side in (1,2,4,8,16,32,64):
  raw=decoded[pos:pos+2*side*side];pos+=len(raw);expected=[0]*(side*side)
  for k,(x,y) in enumerate(ref.square_order(side)):expected[y*side+x]=ref.unpack(int.from_bytes(raw[k*2:k*2+2],'little'),0)
  level=6-(side.bit_length()-1);out=(j.U*(side*side))();info=p.Texture()
  assert j.LIB.sa_pixel_texture_decode(scratch,len(scratch),words[2],words[3],level,out,len(out),ctypes.byref(info))==0
  assert (info.width,info.height)==(side,side) and bytes(out)==struct.pack('<'+'I'*len(expected),*expected)
  mips.append(dict(side=side,level=level,complete_file_to_native_RGBA_match=True,opaque_texels=sum((x>>24)!=0 for x in expected),rgba_sha256=sha(bytes(out))))
 assert mips[-1]['opaque_texels']==1905
 width,height=640,480;count=width*height;background=0xff101018;rows=[];images=R/'work/sonic-adventure/banked-pixel-output';images.mkdir(exist_ok=True)
 from PIL import Image
 @r.READ
 def read(ctx,a,n,out):out[0]=0;return 0
 @r.WRITE
 def write(ctx,a,value,n):return 0
 @r.CACHE
 def cache(ctx,a,op):return 0
 for textures,level in [(1,k) for k in range(7)]+[(0,0)]:
  memory=abi.namespace['RtcMemory']();frame=memory.frame;frame.packet_count=5;frame.packet_capacity=64
  for k in range(5):frame.packets[k].words[:]=words[8*k:8*k+8]
  state=j.State();j.LIB.sa_pixel_job_init(ctypes.byref(state));params=[0]*16;params[7]=639<<16;params[8]=479<<16;render=r.initialized(params,lib=j.LIB)
  parent=r.Bus(None,read,write,cache);capture=r.Bus();binding=j.Binding();outer=r.Bus();rb=r.Binding()
  assert j.LIB.sa_pixel_job_bind(ctypes.byref(binding),ctypes.byref(state),ctypes.byref(render),ctypes.byref(frame),ctypes.byref(parent),ctypes.byref(capture))==0
  assert j.LIB.sa_render_completion_bind(ctypes.byref(rb),ctypes.byref(render),ctypes.byref(capture),ctypes.byref(outer))==0
  assert outer.write(outer.context,0xa05f8014,0xffffffff,4)==0 and state.ready==1 and render.pending==0
  pixels=(j.U*count).from_buffer_copy(struct.pack('<I',background)*count);depth=(j.U*count)();options=p.Options(width,height,width,height,textures,level,3,128)
  assert j.LIB.sa_banked_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),native_banks if textures else None,len(banks) if textures else 0,scratch if textures else None,len(banks) if textures else 0,pixels,depth,count,ctypes.byref(options))==0
  expected=ref.draw(words,logical if textures else None,options,[background]*count,[0]*count)
  assert expected[0]==0 and bytes(pixels)==struct.pack('<'+'I'*count,*expected[1]) and bytes(depth)==struct.pack('<'+'I'*count,*expected[2]) and p.snapshot(state.stats)==expected[3]
  assert state.processed==render.completed==1 and not state.ready and render.pending==7
  if not textures:assert state.stats.texture_samples==0
  path=images/('native_mip'+str(level)+'.png' if textures else 'native_off.png');assert not path.exists();im=Image.frombytes('RGBA',(width,height),bytes(pixels),'raw','BGRA');im.save(path)
  if textures and level==0:
   crop=images/'native_quad_crop.png';assert not crop.exists();im.crop((312,282,328,298)).resize((256,256),Image.Resampling.NEAREST).save(crop)
  rows.append(dict(textures=bool(textures),level=level,changed_colour_pixels=sum(x!=background for x in expected[1]),stats=p.snapshot(state.stats),colour_sha256=sha(bytes(pixels)),depth_sha256=sha(bytes(depth)),private_png_sha256=sha(path.read_bytes()),complete_native_reference_colour_and_depth_match=True,completion_follows_native_work=True))
 wrong=p.draw(words,banks,p.Options(width,height,width,height,1,0,3,128),[background]*count,[0]*count)
 wrong_count=sum(x!=background for x in struct.unpack('<'+'I'*count,wrong[1]));assert wrong[0]==0 and wrong_count==0
 assert rows[0]['changed_colour_pixels']==2 and rows[-1]['changed_colour_pixels']==4
 files=[Path(__file__),P/'pixel_banked_native.py',P/'pixel_banked_reference.py',P/'source/banked_pixel_jobs.c',P/'source/banked_pixel_jobs.h',P/'source/pvr_pixels.c',P/'source/pixel_jobs.c',P/'pvr_pixels_reference.py',P/'analysis/render_pixel_inputs_verification.json',j.DLL]
 result=dict(status='captured_retail_texture_binding_mips_and_visible_native_quad_verified_in_host_profile',asset_binding=binding_info,mips=mips,rows=rows,wrong_layout_control_changed_pixels=wrong_count,source_vram_sha256=sha(banks),source_quad_sha256=sha(quad),source_compressed_sha256=sha(compressed),source_decoded_sha256=sha(decoded),retail_images_published=False,physical_PowerVR_pixels_qualified=False,overall_completion_percent=10,new_original_routine_credit=0,sha256={str(f):sha(f.read_bytes()) for f in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('VERIFIED unique retail texture, seven complete mip decodes and eight native jobs; visible pixels:',rows[0]['changed_colour_pixels'],flush=True)
if __name__=='__main__':main()
