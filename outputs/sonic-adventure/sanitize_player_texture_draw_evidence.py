"""Preserve unpublished raw proofs privately; publish no retail upload trace values."""
import ast,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 result_path=P/'analysis/player_texture_draw_trace_filter_verification.json';assert not result_path.exists()
 baseline=json.loads((W/'player_texture_draw_prior_source_manifest.json').read_text());published={x['path'] for x in baseline['files']}
 script=P/'verify_player_texture_draw_commands.py';raw_report=P/'analysis/player_texture_draw_commands_verification.json';qualification=P/'analysis/player_texture_draw_qualification.json'
 originals=[script,raw_report,qualification];archive=W/'private-player-texture-draw-proof';archive.mkdir(exist_ok=True);ledger=[]
 for path in originals:
  assert path.relative_to(R).as_posix() not in published,'Never rewrite a prior published source/proof'
  dest=archive/path.name;assert not dest.exists();dest.write_bytes(path.read_bytes());assert sha(dest)==sha(path)
  ledger.append(dict(original_path=str(path),private_archive=str(dest),bytes=path.stat().st_size,sha256=sha(path)))
 ledger_path=archive/'PRESERVED_RAW_PROOF_MANIFEST.json';assert not ledger_path.exists();ledger_path.write_text(json.dumps(ledger,indent=2)+'\n')
 text=script.read_text();marker="vertex_events=result['vertex_events'],";assert text.count(marker)==1
 text=text.replace(marker,"vertex_events=result['vertex_events'] if label=='actual stored visible particle draw with loaded textures' else [],")
 marker="exec(compile(code,str(parent),'exec'),globals())";assert text.count(marker)==1
 text=text.replace(marker,"code=code.replace('persistent_graphics_device_state=True,','persistent_graphics_device_state=True,retail_pixel_trace_values_published=False,')\n"+marker)
 ast.parse(text,filename=str(script));script.write_text(text)
 original=json.loads((archive/raw_report.name).read_text());safe=json.loads(json.dumps(original));removed=0;retained=0
 for row in safe['phases']:
  if row['label']=='actual stored visible particle draw with loaded textures':
   assert {e[0] for e in row['vertex_events']}<={1,2,3,5};retained+=len(row['vertex_events'])
  else:removed+=len(row['vertex_events']);row['vertex_events']=[]
 safe['retail_pixel_trace_values_published']=False;safe['sha256'][str(script)]=sha(script)
 assert removed>0 and retained==50
 # Only report serialization changed. Keep the actual compare counts and all
 # generated geometry/descriptor/boundary/hash evidence exactly as recorded.
 normalized=json.loads(json.dumps(safe));del normalized['retail_pixel_trace_values_published'];normalized['sha256'][str(script)]=original['sha256'][str(script)]
 for old,new in zip(original['phases'],normalized['phases']):new['vertex_events']=old['vertex_events']
 assert normalized==original
 raw_report.write_text(json.dumps(safe,indent=2)+'\n')
 for path in (script,raw_report):assert sha(archive/path.name)==next(x['sha256'] for x in ledger if x['original_path']==str(path))
 # Re-issue the unpublished aggregate qualification against filtered evidence.
 # Its exact previous bytes remain in the private archive above.
 assert qualification.resolve().is_relative_to(P.resolve()) and sha(archive/qualification.name)==sha(qualification)
 qualification.unlink()
 result=dict(status='raw_unpublished_proofs_and_source_bytes_preserved_filtered_public_geometry_evidence_verified',preserved_originals=ledger,private_archive_manifest_sha256=sha(ledger_path),removed_nondraw_trace_events=removed,retained_generated_quad_trace_events=retained,retail_pixel_trace_values_published=False,game_execution_or_comparisons_changed=False,actual_scene_comparisons=safe['original_native_comparisons'],actual_scene_checks=safe['checks'],overall_completion_percent=10,sha256={str(p):sha(p) for p in (Path(__file__),script,raw_report,ledger_path)})
 result_path.write_text(json.dumps(result,indent=2)+'\n');print('Preserved3rawsource/proof files byte-for-byte privately;filtered',removed,'nondraw trace events;retained50generated quad events',flush=True)
if __name__=='__main__':main()
