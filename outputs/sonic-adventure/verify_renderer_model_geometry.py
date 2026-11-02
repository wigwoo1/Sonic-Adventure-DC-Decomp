"""Reach a retail model through original renderer/matrix/geometry APIs."""
from pathlib import Path
P=Path(__file__).resolve().parent;builder=P/'verify_renderer_ta_list_reuse.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
def model_change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
code=code.replace('renderer_ta_list_reuse_verification.json','renderer_model_geometry_verification.json').replace('renderer_ta_list_reuse_frontier.json','renderer_model_geometry_frontier.json')
model_change("P/'disc_inventory.json',asset,","P/'disc_inventory.json',asset,P/'verify_renderer_model_geometry.py',")
marker=' # Attach after the real display/resource/draw producers have returned normally.'
model_change(marker,'''
 # Preserve only a matched normal public-return prefix. No failed state resumes.
 import pickle
 private=R/'work/sonic-adventure/model-geometry-prefix';private.mkdir(exist_ok=False)
 payload=pickle.dumps((bytes(base[0]),bytes(base[1]),bytes(base[2]),base[3]),protocol=5)
 (private/'normal_public_return.pkl').write_bytes(payload)
 prefix=proof('matched_normal_public_return_before_model_probe',dict(consecutive_whole_frame_calls=0,model_geometry_qualified=False,checkpoint_sha256=hashlib.sha256(payload).hexdigest(),checkpoint_pc=hex(base[1].pc),checkpoint_pr=hex(base[1].pr),prefix_boundaries=[b['target'] for p in records for b in p['boundaries']],faulted_state_saved=False))
 (private/'provenance.json').write_text(json.dumps(prefix,indent=2)+'\\n')
 root=q.get(base[0],0x8c044a00)
 exact(root&0x1f000000==0xc000000 and root&0xffffff<0xffffcc,'model root comes from original caller literal8c044a00')
 model=q.get(base[0],root+4)
 phase(0x8c0544e2,[],'original renderer initializer before retail model geometry')
 phase(0x8c63a804,[0,q.SHAPES+8],'original matrix translation using the already declared collision-shape scene position')
 geometry=phase(0x8c6215d6,[root],'original geometry wrapper with original literal-selected model')
 report=proof('original_renderer_and_retail_model_geometry_probe_returned',dict(consecutive_whole_frame_calls=0,model_geometry_qualified=False,model_root=hex(root),model_data=hex(model),model_pointer_literal='0x8c044a00',scene_position_source=hex(q.SHAPES+8),model_caller_is_external_test_input=True,renderer_context_from_original_producers=True,geometry_boundaries=[b['target'] for b in geometry['boundaries']],geometry_vertex_counts=geometry['vertex_counts'],geometry_sprite_command_buffers=geometry['sprite_command_buffers'],actual_model_rasterization_qualified=False))
 target.write_text(json.dumps(report,indent=2)+'\\n');print('RETAIL MODEL PROBE',len(records),geometry['vertex_counts'],flush=True);return
'''+marker)
exec(compile(code,str(parent),'exec'),globals())
