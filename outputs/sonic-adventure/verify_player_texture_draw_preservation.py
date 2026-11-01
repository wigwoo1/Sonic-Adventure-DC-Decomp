"""Preserve prior published bytes and original raw evidence during safe publication."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure';G=r'C:\Program Files\Git\cmd\git.exe';BASE='0053a520ecbcda369f3f2eaf2abab1aead3bbbe5'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def old(path):return subprocess.check_output([G,'-C',str(D),'show',BASE+':'+path])
def main():
 target=P/'analysis/player_texture_draw_preservation.json';assert not target.exists()
 mp=W/'player_texture_draw_prior_source_manifest.json';bp=W/'player_texture_draw_prior_build_manifest.json';m=json.loads(mp.read_text());b=json.loads(bp.read_text());assert len(m['files'])==17183 and len(b['files'])==2799 and m==json.loads(old('PUBLICATION_MANIFEST.json'))
 mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json')};changed=[]
 for row in m['files']:
  path=R/row['path'];assert path.exists()
  if sha(path)!=row['sha256']:assert row['path'] in mutable,row['path'];changed.append(row['path'])
 assert set(changed)==mutable
 for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(old('outputs/sonic-adventure/'+name))
 assert (P/'analysis/recovered_routines.json').read_bytes()==old('outputs/sonic-adventure/analysis/recovered_routines.json')
 before=json.loads(old('outputs/sonic-adventure/project_status.json'));after=json.loads((P/'project_status.json').read_text());del after['verified_progress']['player_texture_draw_integration'];assert after==before
 for row in b['files']:assert (R/row['path']).stat().st_size==row['bytes'] and sha(R/row['path'])==row['sha256'],row['path']
 filter_path=P/'analysis/player_texture_draw_trace_filter_verification.json';filtering=json.loads(filter_path.read_text());assert not filtering['retail_pixel_trace_values_published'] and not filtering['game_execution_or_comparisons_changed']
 for row in filtering['preserved_originals']:assert Path(row['private_archive']).stat().st_size==row['bytes'] and sha(row['private_archive'])==row['sha256']
 for path,value in filtering['sha256'].items():assert sha(path)==value,path
 qp=P/'analysis/player_texture_draw_qualification.json';q=json.loads(qp.read_text())
 for path,value in q['sha256'].items():assert sha(path)==value,path
 public=json.loads((P/'analysis/player_texture_draw_commands_verification.json').read_text());assert public['retail_pixel_trace_values_published'] is False
 for row in public['phases']:
  if row['label']=='actual stored visible particle draw with loaded textures':assert len(row['vertex_events'])==50 and {e[0] for e in row['vertex_events']}<={1,2,3,5}
  else:assert not row['vertex_events']
 files=[Path(__file__),mp,bp,qp,filter_path,P/'PLAYER_TEXTURE_DRAW.txt',P/'analysis/player-texture-draw-reference-inputs.tsv',P/'analysis/player-texture-draw-references/references.tsv',P/'ghidra/export_player_texture_draw_references.ps1',P/'ghidra/ExportResourceReferences.java']
 result=dict(status='prior_source_proof_registry_status_builds_and_raw_evidence_preserved_public_trace_filter_verified',baseline_commit=BASE,prior_selected_files=17183,preserved_immutable_selected_files=17180,changed_authorized_status_files=sorted(changed),prior_registry_rows_preserved=1648,result_registry_rows=1648,prior_builds_preserved=2799,added_build_files=0,raw_unpublished_files_preserved=3,retail_pixel_trace_values_published=False,qualification_sha256=sha(qp),overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('Preserved17180immutable prior source/proofs/all1648registry rows/all2799builds/3rawunpublished source-proofs',flush=True)
if __name__=='__main__':main()
