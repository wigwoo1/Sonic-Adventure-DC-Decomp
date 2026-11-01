"""Preserve private, normally produced texture/quad inputs for native rendering."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_render_completion.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('renderer_render_completion_verification.json','render_pixel_inputs_verification.json').replace('renderer_render_completion_frontier.json','render_pixel_inputs_frontier.json')
marker="  print(label,result['status'],result['stop_pc'],hex(s.r[0]),len(path),flush=True)";assert code.count(marker)==1
code=code.replace(marker,marker+'''
  if label=='actual stored visible particle draw with loaded textures' and result['status']==0:
   private=R/'work/sonic-adventure/pixel-inputs';private.mkdir(exist_ok=True)
   vram=private/'original_scene_vram.bin';quad=private/'original_scene_quad.bin'
   assert not vram.exists() and not quad.exists()
   words=result['sprite_command_buffers'][0]['words']
   assert len(words)==40
   vram.write_bytes(result['graphics_out'].vram);quad.write_bytes(b''.join(x.to_bytes(4,'little') for x in words))
   metadata=dict(status='private_render_inputs_from_matched_normal_original_producers',retail_assets_publishable=False,vram_bytes=vram.stat().st_size,vram_sha256=sha(vram),quad_bytes=quad.stat().st_size,quad_sha256=sha(quad),source_native_DLL_sha256=sha(R/'work/sonic-adventure/build/renderer_render_completion.dll'),overall_completion_percent=10)
   (private/'metadata.json').write_text(json.dumps(metadata,indent=2)+'\\n')
''')
marker="P/'disc_inventory.json',asset,";assert code.count(marker)==1
code=code.replace(marker,"P/'disc_inventory.json',asset,P/'verify_render_pixel_inputs.py',")
exec(compile(code,str(parent),'exec'),globals())
