"""Render the privately captured original quad; never publish retail pixels."""
import hashlib,json,struct
from pathlib import Path
import pvr_pixels_native as n
import pvr_pixels_reference as ref
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/render_pixel_output_verification.json';assert not target.exists()
 inputs=R/'work/sonic-adventure/pixel-inputs';metadata=json.loads((inputs/'metadata.json').read_text())
 assert sha(inputs/'original_scene_vram.bin')==metadata['vram_sha256'] and sha(inputs/'original_scene_quad.bin')==metadata['quad_sha256']
 vram=(inputs/'original_scene_vram.bin').read_bytes();quad=(inputs/'original_scene_quad.bin').read_bytes();words=list(struct.unpack('<40I',quad))
 width,height=640,480;bg=[0xff101018]*(width*height);depth=[0]*len(bg);rows=[];images=R/'work/sonic-adventure/pixel-output-qualified';images.mkdir(exist_ok=True)
 from PIL import Image
 for textures,level in [(1,i) for i in range(7)]+[(0,0)]:
  o=n.Options(width,height,width,height,textures,level,3,128)
  actual=n.draw(words,vram if textures else None,o,bg,depth);expected=ref.draw(words,vram if textures else None,o,bg,depth)
  ep=struct.pack('<'+'I'*len(bg),*expected[1]);ed=struct.pack('<'+'I'*len(bg),*expected[2])
  assert actual[0]==expected[0]==0 and actual[1]==ep and actual[2]==ed and actual[3]==expected[3]
  texture_alpha_zero=None
  if textures:
   rc,decoded,info=ref.decode(vram,words[2],words[3],level);assert rc==0
   texture_alpha_zero=all((pixel>>24)==0 for pixel in decoded)
   assert texture_alpha_zero and actual[3][3]==4
  else:assert actual[3][3]==0
  assert actual[3][1]==actual[3][2]==actual[3][4]==4
  path=images/('native_level'+str(level)+'.png' if textures else 'native_texture_off.png');assert not path.exists()
  im=Image.frombytes('RGBA',(width,height),actual[1],'raw','BGRA');im.save(path)
  if textures and level==0:
   crop=images/'native_quad_crop.png';assert not crop.exists();im.crop((312,282,328,298)).resize((256,256),Image.Resampling.NEAREST).save(crop)
  row=dict(texture_all_alpha_zero=texture_alpha_zero,textures=bool(textures),mip_level=level,complete_colour_and_depth_match=True,stats=actual[3],changed_pixels=sum(pixel!=0xff101018 for pixel in expected[1]),colour_sha256=hashlib.sha256(actual[1]).hexdigest(),depth_sha256=hashlib.sha256(actual[2]).hexdigest(),private_png_sha256=sha(path))
  rows.append(row);print(row,flush=True)
 # The captured fixture's texture is fully transparent in this host profile.
 # Require that result explicitly; it is not visible retail texture evidence.
 assert all(row['changed_pixels']==0 for row in rows[:-1]) and rows[-1]['changed_pixels']==4
 files=[Path(__file__),P/'pvr_pixels_native.py',P/'pvr_pixels_reference.py',P/'source/pvr_pixels.c',P/'source/pvr_pixels.h',n.DLL,P/'analysis/render_pixel_inputs_verification.json']
 result=dict(status='captured_quad_transparent_texture_and_vertex_colour_off_host_profile_verified',visible_retail_texture_rendering_qualified=False,initial_test_failed=True,initial_failure_reason='An unjustified visible-colour expectation failed: all decoded texture alpha values are zero. No renderer code changed to accommodate this fixture.',preserved_initial_test_sha256='31a67aaf15f68d7609f25c9a288a8bba247fc0318254a2d63e40329819ce16b6',preserved_initial_failure_log_sha256='0b82cf622bad7346b4720e26615ea46f4cde8f917ec577426db4bf7f80b44b51',cases=len(rows),complete_buffers_compared=True,rows=rows,source_vram_sha256=metadata['vram_sha256'],source_quad_sha256=metadata['quad_sha256'],retail_pixels_published=False,physical_PowerVR_pixels_matched=False,automatic_mip_LOD_qualified=False,playable_port=False,overall_completion_percent=10,new_original_routine_credit=0,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('VERIFIED original quad native pixels; private image artifacts retained',flush=True)
if __name__=='__main__':main()
