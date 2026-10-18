"""Reproduce original model discovery and validate explicit malformed-data boundaries."""
import collections,hashlib,json,runpy,struct
from pathlib import Path
from model_asset_inventory import Image,InvalidModel,PROFILES
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
INVENTORY=PROJECT/'analysis/main_model_assets_inventory.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 before=INVENTORY.read_bytes();runpy.run_path(str(PROJECT/'inventory_main_model_assets.py'),run_name='__main__');assert before==INVENTORY.read_bytes()
 inventory=json.loads(before);data=BINARY.read_bytes();base=0x8c010000;image=Image(data,base)
 assert inventory['validated_model_candidates']==860 and inventory['segment_records']==1905
 assert inventory['object_node_candidates']==858 and inventory['node_backed_model_candidates']==854
 assert inventory['topology_counts']=={'0xc000':1479,'0x0':153,'0x4000':256,'0x8000':17}
 assert inventory['face_nonalternate_strip_candidates']==[]
 for p,h in inventory['sha256'].items():assert digest(Path(p))==h,p
 # Independently read the literal comparison constants from the original bytes.
 profiles=[0]+[struct.unpack_from('<I',data,a-base)[0] for a in range(0x8c620e78,0x8c620ea4,4)]
 names=['lit','uv_lit','environment','face','uv_face','uv_constant','uv_constant','environment_face','environment_constant','environment_constant','flat','flat']
 assert dict(zip(profiles,names))==PROFILES
 for model in inventory['models']:
  a=int(model['address'],16);raw=struct.unpack_from('<5I2H',data,a-base)
  assert raw==(int(model['positions'],16),int(model['normals'],16),model['vertex_count'],int(model['segment_table'],16),int(model['material_table'],16),model['segment_count'],model['material_count'])
  for node in model['object_node_references']:assert struct.unpack_from('<I',data,int(node,16)-base+4)[0]==a
 target=int(inventory['known_particle_roots'][0]['model'],16);model=image.model(target);segment=int(model['segments'][0]['address'],16);indices=int(model['segments'][0]['index_stream'],16)
 strip=next((m,s) for m in inventory['models'] for s in m['segments'] if s['topology']=='0xc000');strip_model,strip_segment=strip
 tests=[]
 def reject(label,patches=(),address=target,truncate=None):
  changed=bytearray(data)
  for a,fmt,value in patches:struct.pack_into(fmt,changed,a-base,value)
  if truncate is not None:changed=changed[:truncate]
  try:Image(changed,base).model(address)
  except InvalidModel as exc:tests.append(dict(case=label,rejected=True,reason=str(exc)));return
  raise AssertionError('Malformed model accepted: '+label)
 reject('header outside image',address=base-4)
 reject('truncated header',address=base,truncate=39)
 reject('zero vertices',[(target+8,'<I',0)])
 reject('negative signed vertex count',[(target+8,'<I',0xffffffff)])
 reject('positions outside image',[(target,'<I',base+len(data)-4)])
 reject('unaligned positions',[(target,'<I',int(model['positions'],16)+1)])
 reject('infinite positions',[(int(model['positions'],16),'<I',0x7f800000)])
 reject('NaN normals',[(int(model['normals'],16),'<I',0x7fc12345)])
 reject('zero materials',[(target+22,'<H',0)])
 reject('negative radius',[(target+36,'<f',-1.)])
 reject('material index beyond table',[(segment,'<H',model['material_count'])])
 reject('zero primitive groups',[(segment+2,'<H',0)])
 reject('out-of-range vertex index',[(indices,'<H',model['vertex_count'])])
 reject('index stream outside image',[(segment+4,'<I',base+len(data)-2)])
 reject('UV table outside image',[(segment+20,'<I',base+len(data)-2)])
 reject('unaligned UV table',[(segment+20,'<I',base+1)])
 reject('short alternate strip',[(int(strip_segment['index_stream'],16),'<H',0x8002)],address=int(strip_model['address'],16))
 # The high bit is a winding flag, not a two's-complement negative length.
 changed=bytearray(data);stream=int(strip_segment['index_stream'],16);word=image.u16(stream)
 struct.pack_into('<H',changed,stream-base,word^0x8000)
 altered=Image(changed,base).model(int(strip_model['address'],16));old_index=strip_model['segments'].index(strip_segment)
 assert altered['segments'][old_index]['group_lengths']==strip_segment['group_lengths']
 assert altered['segments'][old_index]['group_flips'][0]!=strip_segment['group_flips'][0]
 files=[Path(__file__),PROJECT/'model_asset_inventory.py',PROJECT/'inventory_main_model_assets.py',INVENTORY,BINARY]
 result=dict(validated_model_candidates=860,segment_records=1905,object_node_candidates=858,node_backed_model_candidates=854,independent_header_checks=860,profile_literal_checks=len(profiles),malformed_cases_rejected=len(tests),alternate_winding_flag_check=True,inventory_reproduces_byte_for_byte=True,sha256={str(p):digest(p) for p in files},scope='Read-only inventory reproduction, independent original header/object-pointer/literal checks, malformed-record bounds rejection and alternate-strip high-bit winding semantics. Native instruction/asset-stream behavior is checked separately.',limitations=['Structural candidates and bounded malformed inputs only; no completeness, overlay, alternative model format or actual runtime reachability claim.'],cases=tests)
 (PROJECT/'analysis/main_model_inventory_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print('860 original headers, 12 profile literals, 17 malformed rejections and winding-bit behavior passed; inventory reproduces exactly.')
if __name__=='__main__':main()
