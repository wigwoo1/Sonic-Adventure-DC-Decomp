"""Bounded overlay image aliases and source mapping, without relocating file bytes."""
import hashlib,json,math,struct
from pathlib import Path
from model_asset_inventory import Image,InvalidModel
PROJECT=Path(__file__).resolve().parent
MAPPING=PROJECT/'analysis/overlay_load_map.json'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def canonical(a):
 if isinstance(a,int) and a&0x1f000000==0x0c000000 and a&0xe0000000 in (0,0x80000000,0xa0000000):return (a&0x1fffffff)|0x80000000
 return a

class OverlayImage(Image):
 def __init__(self,data,base):super().__init__(data,canonical(base))
 def contains(self,a,n=1):return super().contains(canonical(a),n)
 def span(self,a,n):return super().span(canonical(a),n)


def mapped_images():
 mapping=json.loads(MAPPING.read_text());unique={}
 for row in mapping['modules']:
  key=(str(Path(row['decoded_path']).resolve()),canonical(int(row['load_address'],16)))
  if key in unique:assert row['decoded_sha256']==unique[key]['decoded_sha256']
  unique[key]=row
 return [unique[k] for k in sorted(unique,key=lambda k:(Path(k[0]).name,k[1]))]


def object_references(image,models):
 by_address={canonical(int(m['address'],16)):[] for m in models};objects=[]
 for off in range(0,len(image.data)-51,4):
  flags,raw_model=struct.unpack_from('<2I',image.data,off);model=canonical(raw_model)
  if model not in by_address or flags>0x7ff:continue
  child,sibling=struct.unpack_from('<2I',image.data,off+44)
  if any(a and (a&3 or not image.contains(a,52)) for a in (child,sibling)):continue
  xyz=struct.unpack_from('<3f',image.data,off+8);scale=struct.unpack_from('<3f',image.data,off+32)
  if not all(math.isfinite(v) and abs(v)<=1e8 for v in xyz+scale):continue
  a=image.base+off;by_address[model].append(hex(a))
  objects.append(dict(address=hex(a),model=hex(model),raw_model_pointer=hex(raw_model),flags=hex(flags),child=hex(child),sibling=hex(sibling),record_sha256=hashlib.sha256(image.span(a,52)).hexdigest()))
 for m in models:m['object_node_references']=by_address[canonical(int(m['address'],16))]
 return objects
