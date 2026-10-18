"""Read-only bounded parser for the recovered 40/24/20-byte basic model layout.
Names are inferred. Structural candidates do not prove runtime reachability.
"""
import hashlib,math,struct
PROFILE_MASK=0x03600000
PROFILES={0:'lit',0x00200000:'uv_lit',0x00600000:'environment',0x01000000:'face',0x01200000:'uv_face',0x02200000:'uv_constant',0x03200000:'uv_constant',0x01600000:'environment_face',0x02600000:'environment_constant',0x03600000:'environment_constant',0x02000000:'flat',0x03000000:'flat'}
class InvalidModel(ValueError):pass
class Image:
 def __init__(self,data,base):self.data=data;self.base=base;self.end=base+len(data)
 def contains(self,a,n=1):return isinstance(a,int) and n>=0 and self.base<=a<=self.end and n<=self.end-a
 def span(self,a,n):
  if not self.contains(a,n):raise InvalidModel(f'outside image: {a:#x}+{n:#x}')
  return self.data[a-self.base:a-self.base+n]
 def u16(self,a):return struct.unpack('<H',self.span(a,2))[0]
 def u32(self,a):return struct.unpack('<I',self.span(a,4))[0]
 def vectors(self,a,n):
  raw=self.span(a,n*12);values=struct.unpack('<'+'f'*(n*3),raw)
  if not all(math.isfinite(v) and abs(v)<=1e8 for v in values):raise InvalidModel('nonfinite or implausible vector data')
  return hashlib.sha256(raw).hexdigest()
 def model(self,a):
  words=struct.unpack('<5I2H4f',self.span(a,40));positions,normals,count,sets,materials,nsets,nmaterials,*bounds=words
  if not (1<=count<=65535 and 1<=nsets<=4096 and 1<=nmaterials<=4096):raise InvalidModel('invalid bounded counts')
  if any(v and v&3 for v in (positions,normals,sets,materials)):raise InvalidModel('unaligned pointer')
  if not all(math.isfinite(v) and abs(v)<=1e8 for v in bounds) or bounds[3]<0:raise InvalidModel('invalid bounds')
  positions_hash=self.vectors(positions,count)
  normal_hash=self.vectors(normals,count) if normals else None
  self.span(sets,nsets*24);self.span(materials,nmaterials*20)
  segments=[]
  for i in range(nsets):
   s=sets+i*24;kind,groups=struct.unpack('<2H',self.span(s,4));material=kind&0x3fff;topology=kind&0xc000
   if material>=nmaterials:raise InvalidModel('material index outside table')
   if groups==0:raise InvalidModel('zero primitive group count outside inventory contract')
   stream=self.u32(s+4);at=stream;lengths=[];flips=[];maximum=-1;minimum=count;corners=0
   if not stream or stream&1:raise InvalidModel('missing or unaligned index stream')
   for group in range(groups):
    if topology in (0,0x4000):length=3 if topology==0 else 4;flip=False
    else:
     word=self.u16(at);at+=2;length=word&0x7fff if topology==0xc000 else word;flip=bool(word&0x8000) if topology==0xc000 else False
     if length<3 or length>65535:raise InvalidModel('group length outside bounded inventory contract')
    raw=self.span(at,length*2);indices=struct.unpack('<'+'H'*length,raw);at+=length*2
    if max(indices)>=count:raise InvalidModel('vertex index outside table')
    minimum=min(minimum,min(indices));maximum=max(maximum,max(indices));corners+=length;lengths.append(length);flips.append(flip)
   m=materials+material*20;flags=self.u32(m+16);uv=self.u32(s+20)
   if uv&1:raise InvalidModel('unaligned UV pointer')
   if uv:self.span(uv,corners*4)
   segments.append(dict(address=hex(s),topology=hex(topology),material_index=material,material_address=hex(m),material_flags=hex(flags),profile=hex(flags&PROFILE_MASK),family=PROFILES.get(flags&PROFILE_MASK,'unrecognized'),group_count=groups,group_lengths=lengths,group_flips=flips,corner_count=corners,triangle_count=sum(n-2 for n in lengths),index_stream=hex(stream),index_bytes=at-stream,index_sha256=hashlib.sha256(self.span(stream,at-stream)).hexdigest(),index_min=minimum,index_max=maximum,uv_pointer=hex(uv)))
  return dict(address=hex(a),positions=hex(positions),normals=hex(normals),vertex_count=count,segment_table=hex(sets),material_table=hex(materials),segment_count=nsets,material_count=nmaterials,bounds=bounds,positions_sha256=positions_hash,normals_sha256=normal_hash,segments=segments)

def scan(image):
 records=[];rejected=[];headers=0
 for off in range(0,len(image.data)-39,4):
  positions,normals,count,sets,materials=struct.unpack_from('<5I',image.data,off)
  if not (1<=count<=65535 and image.contains(positions,count*12) and (not normals or image.contains(normals,count*12)) and image.contains(sets,24) and image.contains(materials,20)):continue
  nsets,nmaterials=struct.unpack_from('<2H',image.data,off+20)
  if not (1<=nsets<=4096 and 1<=nmaterials<=4096 and image.contains(sets,nsets*24) and image.contains(materials,nmaterials*20)):continue
  headers+=1
  try:records.append(image.model(image.base+off))
  except InvalidModel as exc:rejected.append(dict(address=hex(image.base+off),reason=str(exc)))
 return records,rejected,headers


def object_references(image,models):
 by_address={int(m['address'],16):[] for m in models};objects=[]
 for off in range(0,len(image.data)-51,4):
  flags,model=struct.unpack_from('<2I',image.data,off)
  if model not in by_address or flags>0x7ff:continue
  child,sibling=struct.unpack_from('<2I',image.data,off+44)
  if any(a and (a&3 or not image.contains(a,52)) for a in (child,sibling)):continue
  xyz=struct.unpack_from('<3f',image.data,off+8);scale=struct.unpack_from('<3f',image.data,off+32)
  if not all(math.isfinite(v) and abs(v)<=1e8 for v in xyz+scale):continue
  a=image.base+off;by_address[model].append(hex(a))
  objects.append(dict(address=hex(a),model=hex(model),flags=hex(flags),child=hex(child),sibling=hex(sibling),record_sha256=hashlib.sha256(image.span(a,52)).hexdigest()))
 for m in models:m['object_node_references']=by_address[int(m['address'],16)]
 return objects
