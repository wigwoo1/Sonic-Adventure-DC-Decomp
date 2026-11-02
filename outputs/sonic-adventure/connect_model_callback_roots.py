"""Expose existing original spawn/scheduler callbacks in the current frame ABI."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
old=P/'source/sa_model_view_producer_connected.c';text=old.read_text()
marker='if(target==0x8c09865cu||target==0x8c098998u';assert text.count(marker)==1
roots=(0x8c07b320,0x8c044680,0x8c04481a,0x8c0448b4,0x8c09878c)
text=text.replace(marker,'if('+''.join(f'target==0x{a:x}u||' for a in roots)+'target==0x8c09865cu||target==0x8c098998u')
out=P/'source/sa_model_callback_roots_connected.c';assert not out.exists();out.write_text(text)
recipe=P/'build_model_float_color.ps1';build=P/'build_model_callback_roots.ps1';assert not build.exists()
build.write_text(recipe.read_text().replace('sa_model_view_producer_connected.c','sa_model_callback_roots_connected.c').replace('model_float_color.dll','model_callback_roots.dll'))
listing=P/'analysis/main-dispatch-context-callback/8c09878c_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));binary=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
for row in rows:
 a=int(row['address'],16);b=bytes.fromhex(row['bytes']);assert raw[a-0x8c010000:a-0x8c010000+len(b)]==b
files=[Path(__file__),old,out,recipe,build,listing,binary,P/'source/sa_draw_motion_mesh_lower_connected.c',P/'source/sa_motion_reuse_sa_task_update_list.c']
report=dict(status='existing_native_callback_roots_connected_behavior_proof_separate',new_public_roots=list(map(hex,roots)),original_function_bodies_modified=False,prior_sources_preserved=True,scheduler_flat_reference_instruction_rows=len(rows),scheduler_listing=str(listing),new_original_routine_credit=0,sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
target=P/'analysis/model_callback_roots_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
