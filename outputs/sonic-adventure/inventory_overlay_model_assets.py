"""Discover self-contained basic model candidates in all byte-mapped decoded overlays."""
import collections,hashlib,json
from pathlib import Path
from model_asset_inventory import scan
from overlay_model_assets import OverlayImage,mapped_images,object_references,canonical,digest,MAPPING
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
OUT=PROJECT/'analysis/overlay-model-assets'
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'

def main():
 assert digest(BINARY)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 OUT.mkdir(exist_ok=True);binary=OverlayImage(BINARY.read_bytes(),0x8c010000)
 files=[Path(__file__),PROJECT/'model_asset_inventory.py',PROJECT/'overlay_model_assets.py',MAPPING,BINARY]
 modules=[];topologies=collections.Counter();families=collections.Counter();findings=[]
 for row in mapped_images():
  path=Path(row['decoded_path']);assert digest(path)==row['decoded_sha256'],path
  assert canonical(binary.u32(int(row['load_address_literal'],16)))==canonical(int(row['load_address'],16))
  pointer=binary.u32(int(row['filename_pointer_literal'],16));name=bytearray()
  while binary.span(pointer+len(name),1)!=b'\0':name+=binary.span(pointer+len(name),1);assert len(name)<128
  assert name.decode('ascii').lower()==row['resource'].lower()
  image=OverlayImage(path.read_bytes(),int(row['load_address'],16));models,rejected,headers=scan(image);objects=object_references(image,models)
  segments=[s for m in models for s in m['segments']];tc=collections.Counter(s['topology'] for s in segments);fc=collections.Counter(s['family'] for s in segments)
  special=[dict(module=path.name,model=m['address'],**s) for m in models for s in m['segments'] if s['topology']=='0x8000' and s['family'] in ('face','uv_face','environment_face')]
  findings.extend(special);topologies.update(tc);families.update(fc)
  report=dict(module=path.name,source_path=str(path),source_sha256=digest(path),load_address=hex(image.base),mapped_physical_load_address=row['load_address'],filename_pointer_literal=row['filename_pointer_literal'],load_address_literal=row['load_address_literal'],image_bytes=len(image.data),header_candidates=headers,validated_model_candidates=len(models),segment_records=len(segments),object_node_candidates=len(objects),node_backed_model_candidates=sum(bool(m['object_node_references']) for m in models),topology_counts=dict(tc),family_counts=dict(fc),face_nonalternate_strip_candidates=special,rejected_header_candidates=rejected,objects=objects,models=models)
  output=OUT/(path.stem+'.json');output.write_text(json.dumps(report,indent=2)+'\n')
  modules.append({k:v for k,v in report.items() if k not in ('models','objects','rejected_header_candidates','face_nonalternate_strip_candidates')}|dict(report=output.relative_to(PROJECT).as_posix(),report_sha256=digest(output),rejected_header_count=len(rejected),face_nonalternate_strip_count=len(special)))
  files.extend((path,output));print(path.name,len(models),'models,',len(segments),'segments,',len(special),'face 8000',flush=True)
 result=dict(modules_scanned=len(modules),modules_with_model_candidates=sum(bool(m['validated_model_candidates']) for m in modules),validated_model_candidates=sum(m['validated_model_candidates'] for m in modules),segment_records=sum(m['segment_records'] for m in modules),object_node_candidates=sum(m['object_node_candidates'] for m in modules),node_backed_model_candidates=sum(m['node_backed_model_candidates'] for m in modules),topology_counts=dict(topologies),family_counts=dict(families),face_nonalternate_strip_candidates=findings,modules=modules,sha256={str(p):digest(p) for p in files},newly_recovered_entries=0,scope='Self-contained basic-model candidate discovery in every existing decoded overlay mapping. Loader destination and filename literals are re-read from original main code; every decoded image hash is checked. Physical/cached/uncached RAM aliases are resolved for reads without rewriting input bytes.',limitations=['Bounded structural scan only. Models with cross-image pointers, relative/relocated pointers, alternative layouts or unsupported counts/groups can be absent. Same-address records in different modules are distinct; counts do not measure unique artwork, full asset coverage or game reachability.','Profiles use original static flags; runtime overrides and actual near-plane crossings remain separate questions. No new recovered game-code credit or completed port claim.'])
 (PROJECT/'analysis/overlay_model_assets_inventory.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('modules_scanned','modules_with_model_candidates','validated_model_candidates','segment_records','object_node_candidates','node_backed_model_candidates','topology_counts','family_counts')},indent=2))
 print('Total static face 8000 combinations:',len(findings))
if __name__=='__main__':main()
