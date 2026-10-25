"""Invented volume/directory inputs for original initializer comparisons."""
import hashlib,json
from pathlib import Path
from prepare_movie_disc_paths_media import directory_record,path_record
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
OUT=ROOT/'work/sonic-adventure/fixtures/disc-mount'

def main():
 OUT.mkdir(parents=True,exist_ok=True);profiles={}
 def add(name,*,special=False,prefix=b'SPECIAL',identifier=b'SPECIAL',child_count=3,empty=False,signature=b'CD001',capacity=8,fail_after=-1):
  raw=bytearray(32*2048);volume=16*2048
  raw[volume+1:volume+6]=signature
  paths=path_record(b'\0',45002,1)+path_record(identifier,45003,1)
  fields={140:45008,132:len(paths),158:45002,166:2048}
  for offset,value in fields.items():raw[volume+offset:volume+offset+4]=value.to_bytes(4,'little')
  raw[8*2048:8*2048+len(paths)]=paths
  root=[] if empty else [dict(name=b'\0'),dict(name=b'\1'),dict(name=identifier,extent=45003)]
  child=[dict(name=b'\0',extent=45003),dict(name=b'\1')]+[dict(name=('FILE'+str(i)+';1').encode(),extent=45006+i,size=17+i,flags=0) for i in range(max(0,child_count-2))]
  if not child_count:child=[]
  records={}
  for block,items in ((2,root),(3,child)):
   data=b''.join(directory_record(**r)[0] for r in items);assert len(data)<=2048
   raw[block*2048:block*2048+len(data)]=data;records[str(block)]=[directory_record(**r)[1] for r in items]
  target=OUT/(name+'.bin');target.write_bytes(raw)
  profiles[name]=dict(path=target.relative_to(ROOT).as_posix(),bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),base_lba=45000,stride=2048,volume_fad=45166,volume_fields={str(k):v for k,v in fields.items()},signature=list(signature),special=special,prefix=list(prefix),capacity=capacity,fail_after=fail_after,directories=records)
 add('normal');add('bad_signature',signature=b'BAD00');add('empty_root',empty=True);add('root_full',capacity=1)
 add('special',special=True);add('special_twelve',special=True,prefix=b'ABCDEFGHIJKL',identifier=b'ABCDEFGHIJKL')
 add('special_missing',special=True,prefix=b'MISSING');add('special_empty_prefix',special=True,prefix=b'')
 add('special_empty_child',special=True,child_count=0);add('special_child_full',special=True,capacity=3,child_count=4)
 for n in (0,1,2):add('special_fail_'+str(n),special=True,fail_after=n)
 report=dict(profiles=profiles,retail_bytes_included=False,whole_disc_images=False,scope='Invented 32-sector raw streams; declared volume fields and directory records. Header bytes are explicit direct-call inputs, not recovered physical media observations.',sha256={str(Path(__file__)):hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),str(P/'prepare_movie_disc_paths_media.py'):hashlib.sha256((P/'prepare_movie_disc_paths_media.py').read_bytes()).hexdigest()})
 (P/'analysis/movie_mount_media.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated',len(profiles),'invented initializer media profiles')
if __name__=='__main__':main()
