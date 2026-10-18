"""Deterministic overlay discovery and independent raw-header/alias checks."""
import hashlib,json,runpy,struct
from pathlib import Path
from overlay_model_assets import OverlayImage,canonical,digest
PROJECT=Path(__file__).resolve().parent
INVENTORY=PROJECT/'analysis/overlay_model_assets_inventory.json'

def main():
 before=INVENTORY.read_bytes();runpy.run_path(str(PROJECT/'inventory_overlay_model_assets.py'),run_name='__main__');assert before==INVENTORY.read_bytes()
 report=json.loads(before)
 for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 assert (report['modules_scanned'],report['modules_with_model_candidates'],report['validated_model_candidates'],report['segment_records'])==(42,39,15984,45425)
 assert report['node_backed_model_candidates']==15975 and len(report['face_nonalternate_strip_candidates'])==2
 headers=nodes=aliases=0;special=[]
 for module in report['modules']:
  detail=json.loads((PROJECT/module['report']).read_text());data=Path(detail['source_path']).read_bytes();image=OverlayImage(data,int(detail['load_address'],16))
  for model in detail['models']:
   a=int(model['address'],16);raw=struct.unpack_from('<5I2H',data,a-image.base)
   assert raw==(int(model['positions'],16),int(model['normals'],16),model['vertex_count'],int(model['segment_table'],16),int(model['material_table'],16),model['segment_count'],model['material_count'])
   headers+=1
   for node in model['object_node_references']:
    pointer=struct.unpack_from('<I',data,int(node,16)-image.base+4)[0];assert canonical(pointer)==a;nodes+=1
  for alias in (image.base&0x1fffffff,image.base,(image.base&0x1fffffff)|0xa0000000):
   assert image.span(alias,len(data))==data and image.contains(alias,len(data))
   assert not image.contains(alias-1,len(data)+1) and not image.contains(alias,len(data)+1) and not image.contains(alias,-1)
   aliases+=1
  assert not image.contains(0x4c000000|(image.base&0xffffff),1) and not image.contains(0xe0000000,4)
  for finding in detail['face_nonalternate_strip_candidates']:
   model=next(m for m in detail['models'] if m['address']==finding['model'])
   assert model['object_node_references'] and finding['family']=='environment_face' and finding['group_lengths']==[5] and finding['index_min']==3 and finding['index_max']==7
   special.append(dict(module=detail['module'],model=model['address'],positions_sha256=model['positions_sha256'],normals_sha256=model['normals_sha256'],strip_index_sha256=finding['index_sha256']))
 assert headers==15984 and nodes==22401 and aliases==126
 assert {x['module'] for x in special}=={'AL_MAIN.BIN','AL_RACE.BIN'}
 assert len({x['strip_index_sha256'] for x in special})==1
 files=[Path(__file__),PROJECT/'overlay_model_assets.py',PROJECT/'model_asset_inventory.py',INVENTORY]
 files.extend(Path(p) for p in report['sha256'])
 result=dict(modules_verified=42,independent_header_checks=headers,raw_object_pointer_checks=nodes,ram_alias_checks=aliases,inventory_reproduces_byte_for_byte=True,special_models=special,sha256={str(p):digest(p) for p in files},newly_recovered_entries=0,scope='All mapped overlay hashes/load/filename literals, deterministic structural discovery, raw original model/object fields and physical/cached/uncached image bounds. Two face-strip occurrences verified without inventing asset names.',limitations=['Structural discovery and shared-index occurrence evidence only, not uniqueness of artwork, complete file-format discovery or scene/gameplay execution.'])
 (PROJECT/'analysis/overlay_model_inventory_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print('15,984 headers, 22,401 object pointers and 126 image-alias checks passed; overlay inventory reproduces exactly.')
if __name__=='__main__':main()
