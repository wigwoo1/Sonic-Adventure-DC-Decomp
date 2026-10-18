"""Inventory structurally validated model records inside original 1ST_READ.BIN."""
import collections,hashlib,json,struct
from pathlib import Path
from model_asset_inventory import Image,scan,object_references,PROFILE_MASK
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';BASE=0x8c010000

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert digest(BINARY)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 image=Image(BINARY.read_bytes(),BASE)
 assert image.u32(0x8c620cb0)==PROFILE_MASK and image.u16(0x8c620b62)==0x3fff
 models,rejected,headers=scan(image);objects=object_references(image,models);by_address={int(m['address'],16):m for m in models}
 roots=[]
 for literal in (0x8c0449fc,0x8c044a00):
  node=image.u32(literal);model=image.u32(node+4);assert model in by_address and hex(node) in by_address[model]['object_node_references']
  roots.append(dict(literal=hex(literal),node=hex(node),model=hex(model),node_sha256=hashlib.sha256(image.span(node,52)).hexdigest()))
 segments=[s for m in models for s in m['segments']]
 findings=[dict(model=m['address'],**s) for m in models for s in m['segments'] if s['topology']=='0x8000' and s['family'] in ('face','uv_face','environment_face')]
 files=[Path(__file__),PROJECT/'model_asset_inventory.py',BINARY,PROJECT/'source/sa_main_dispatch_mesh.c',PROJECT/'source/sa_main_dispatch_primitives.c',PROJECT/'source/sa_main_dispatch_alternate_strips.c']
 result=dict(binary_sha256=digest(BINARY),module='1ST_READ.BIN',load_address=hex(BASE),image_bytes=len(image.data),header_candidates=headers,validated_model_candidates=len(models),rejected_header_candidates=rejected,segment_records=len(segments),object_node_candidates=len(objects),node_backed_model_candidates=sum(bool(m['object_node_references']) for m in models),objects=objects,topology_counts=dict(collections.Counter(s['topology'] for s in segments)),family_counts=dict(collections.Counter(s['family'] for s in segments)),face_nonalternate_strip_candidates=findings,known_particle_roots=roots,models=models,sha256={str(p):digest(p) for p in files},newly_recovered_entries=0,scope='Read-only structural inventory of aligned basic model candidates in the original main executable, with in-image vector/material/segment/index/UV bounds, finite vector data and index-range checks. Two particle roots are confirmed by existing code literals; remaining candidates require caller/reference evidence.',limitations=['This is not exhaustive discovery: runtime-allocated/relocated structures, alternate model formats, overlays and external assets are outside this scan. Count/length bounds and nonzero valid groups restrict accepted candidates. Rejections are recorded explicitly.','Static material profiles use original unmodified flags and default dispatch mask; runtime overrides and geometric near-plane crossings are not proved. Structural validity alone does not establish asset identity or actual game reachability.'])
 (PROJECT/'analysis/main_model_assets_inventory.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('header_candidates','validated_model_candidates','segment_records','object_node_candidates','node_backed_model_candidates','topology_counts','family_counts')},indent=2))
 print('Face-lighted 8000 candidates:',len(findings),'rejected headers:',len(rejected))
if __name__=='__main__':main()
