"""Deterministic invented media bytes for original path/directory verification.

These small raw sector streams are test inputs, not retail data or full discs.
The abstract records and expected namespaces are declared separately from the
game implementation; native transport still reads each requested sector.
"""
import hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];OUT=ROOT/'work/sonic-adventure/fixtures/disc-paths'
BASE=45000;SECTORS=16;PATH_BLOCK=8

def path_record(name,extent,parent):
 name=bytes(name);return bytes((len(name),0))+struct.pack('<IH',extent,parent)+name+bytes(len(name)&1)

def directory_record(name,extent=45002,size=2048,flags=2,length=None):
 name=bytes(name);n=length or (33+len(name)+(not len(name)&1));assert 34<=n<=255 and 33+len(name)<=n
 raw=bytearray(n);raw[0]=n;struct.pack_into('<I',raw,2,extent);struct.pack_into('>I',raw,6,extent);struct.pack_into('<I',raw,10,size);struct.pack_into('>I',raw,14,size);raw[25]=flags;raw[28:32]=b'\1\0\0\1';raw[32]=len(name);raw[33:33+len(name)]=name
 return bytes(raw),dict(identifier=list(name),extent=extent,size=size,flags=flags,length=n)

def directory(records):
 raw=bytearray();notes=[]
 for record in records:
  data,note=directory_record(**record);raw.extend(data);notes.append(note)
 assert len(raw)<=2048
 return bytes(raw)+bytes(2048-len(raw)),notes

def main():
 OUT.mkdir(parents=True,exist_ok=True);profiles={}
 normal=[dict(name=b'\0'),dict(name=b'\1'),dict(name=b'DIR',extent=45003),dict(name=b'FOO.BIN;1',extent=45006,size=17,flags=0)]
 standard=[(b'\0',45002,1),(b'DIR',45003,1),(b'CHILD',45004,2)]
 extended=[(b'\0',45002,1),(b'SPECIAL',45002,1),(b'DIR',45003,2),(b'CHILD',45004,3)]
 namespace={'/':45002,'/DIR':45003,'/DIR/CHILD':45004}
 def add(name,paths=standard,dirs=None,namespace_map=None,path_override=None,prefix_namespaces=None):
  raw=bytearray(SECTORS*2048);abstract={}
  if dirs is None:dirs={2:normal,3:[dict(name=b'\0',extent=45003),dict(name=b'\1'),dict(name=b'CHILD',extent=45004),dict(name=b'INNER;1',extent=45006,size=17,flags=0)],4:[dict(name=b'\0',extent=45004),dict(name=b'\1',extent=45003),dict(name=b'LEAF;1',extent=45006,size=17,flags=0)]}
  for block,records in dirs.items():
   data,notes=directory(records);raw[block*2048:(block+1)*2048]=data;abstract[str(block)]=notes
  encoded=path_override if path_override is not None else b''.join(path_record(*r) for r in paths)
  assert PATH_BLOCK*2048+len(encoded)<=len(raw);raw[PATH_BLOCK*2048:PATH_BLOCK*2048+len(encoded)]=encoded
  target=OUT/(name+'.bin');target.write_bytes(raw)
  profiles[name]=dict(path=target.relative_to(ROOT).as_posix(),bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),base_lba=BASE,stride=2048,path_block=PATH_BLOCK,path_bytes=len(encoded),namespace=namespace if namespace_map is None else namespace_map,prefix_namespaces=prefix_namespaces or {},directories=abstract)
 add('standard');add('extended',paths=extended,prefix_namespaces={'SPECIAL':namespace})
 add('empty',dirs={2:[]},namespace_map={},path_override=bytes(40))
 add('single',dirs={2:[dict(name=b'\0')]})
 add('two',dirs={2:[dict(name=b'\0'),dict(name=b'A;1',extent=45006,size=17,flags=0)]})
 add('short_size',dirs={2:[dict(name=b'\0',size=1024),dict(name=b'A;1',extent=45006,size=17,flags=0)]})
 add('multi',dirs={2:[dict(name=b'\0',size=4096),dict(name=b'FIRST;1',extent=45006,size=17,flags=0)],3:[dict(name=b'SECOND;1',extent=45007,size=31,flags=0)]})
 exact=[dict(name=b'\0',length=248)]+[dict(name=('FILE'+str(i)+';1').encode(),extent=45006+i,size=i+1,flags=0,length=248) for i in range(7)]+[dict(name=b'LAST;1',extent=45015,size=1,flags=0,length=64)]
 add('exact_directory',dirs={2:exact})
 crossing=[standard[0]]+[(b'X',45002,999)]*203+[(b'TARGETXX',45005,1)]
 add('crossing',paths=crossing,namespace_map={'/':45002,'/TARGETXX':45005},prefix_namespaces={'TARGETXX':{'/':45005}})
 exact_paths=[standard[0]]+[(b'X',45002,999)]*201+[(b'IGNORED',45002,999),(b'LAST',45005,1)]
 assert len(b''.join(path_record(*r) for r in exact_paths))==2048
 add('exact_path',paths=exact_paths,namespace_map={'/':45002,'/LAST':45005},prefix_namespaces={'LAST':{'/':45005}})
 report=dict(profiles=profiles,retail_bytes_included=False,whole_disc_images=False,scope='Generated explicit raw 2048-byte sector test streams with abstract path/record declarations. They do not establish physical drive timing, natural mount ordering or original media coverage.',sha256={str(Path(__file__)):hashlib.sha256(Path(__file__).read_bytes()).hexdigest()})
 (P/'analysis/movie_disc_paths_media.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated',len(profiles),'invented media fixtures; no retail bytes')
if __name__=='__main__':main()
