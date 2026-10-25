"""Original path/directory execution through real native reads of invented media."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_disc_init as old
import disc_paths_platform_reference as media
P=old.P;ROOT=old.ROOT;driver=old.driver;load=old.load;store=old.store;sample=old.sample
STANDARD,EXTENDED,EQUAL,SPAN=0x8c64563e,0x8c64573e,0x8c645374,0x8c64541c
DIRECTORY,OPEN=old.DIRECTORY,old.OPEN
BUFFER,TABLE,ALT,SRC,OTHER,HEADER=0x8cd80000,0x8cd40000,0x8cd50000,0x8cda0000,0x8cdb0000,0x8cd0a000
MP=P/'analysis/movie_disc_paths_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
MEDIA=json.loads((P/'analysis/movie_disc_paths_media.json').read_text())['profiles']
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
exec(compile(old.old.code,'disc_paths_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_disc_paths.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
observed=effects.LIB.sa_movie_disc_paths_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
platform=types.SimpleNamespace(**old.platform.__dict__);platform.DiscReference=media.DiscReference
compare=types.FunctionType(old.compare.__code__,dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats,platform=platform),'compare')

def write_bytes(m,a,data):
 assert len(m)==16<<20 and (a&0xffffff)+len(data)<=len(m)
 m[a&0xffffff:(a&0xffffff)+len(data)]=data

def text_bytes(c,key,default):return c.get(key,default).encode('latin1')

def prepare(ram,c):
 # The retained seed is an explicitly prepared library fixture. Its linked
 # temporary descriptor is initialized here as a direct-call precondition;
 # this is distinct from the previously verified cold original producer.
 m,s,f=old.old.prepare(ram,c);entry=c['entry'];profile=MEDIA[c['media_profile']]
 root=load(m,0x8c673414);temp=load(m,root+0x14);write_bytes(m,temp,bytes(84));store(m,temp,root)
 for base,size in ((BUFFER,4352),(TABLE,4096),(ALT,4096),(SRC,1024),(OTHER,1024),(HEADER,128)):
  write_bytes(m,base-256,bytes([0xa5])*(size+512))
 path=text_bytes(c,'path','/DIR');prefix=text_bytes(c,'prefix','SPECIAL');write_bytes(m,SRC,path+b'\0');write_bytes(m,OTHER,prefix+b'\0')
 path_fad=profile['base_lba']+150+c.get('path_block',profile['path_block']);path_size=c.get('path_bytes',profile['path_bytes'])
 if entry in (STANDARD,EXTENDED):
  s.r[4]=BUFFER;s.r[5]=path_fad;s.r[6]=path_size&0xffffffff;s.r[7]=SRC
  if entry==EXTENDED:store(m,s.r[15],OTHER)
 elif entry==EQUAL:
  left=text_bytes(c,'text_left','AbC');right=text_bytes(c,'text_right','aBc');write_bytes(m,SRC,left+b'\0');write_bytes(m,OTHER,right+b'\0')
  s.r[4]=c.get('source',SRC);s.r[5]=OTHER;s.r[6]=c.get('count',3)&0xffffffff
 elif entry==SPAN:
  left=text_bytes(c,'text_left','ABC/DEF');delimiters=text_bytes(c,'delimiters','/\\');write_bytes(m,SRC,left+b'\0');write_bytes(m,OTHER,delimiters+b'\0');s.r[4]=SRC;s.r[5]=OTHER
 elif entry in (DIRECTORY,OPEN):
  table=ALT if c.get('explicit_table') else TABLE;capacity=c.get('table_capacity',8);fad=profile['base_lba']+150+c.get('directory_block',2)
  write_bytes(m,table,bytes(4096));store(m,table,c.get('initial_count',0));store(m,table+4,capacity&0xffffffff);store(m,table+8,c.get('cached_fad',0))
  if entry==DIRECTORY:s.r[4]=BUFFER;s.r[5]=fad;s.r[6]=table;s.r[7]=c.get('directory_bytes',2048)&0xffffffff
  else:
   store(m,root+4,path_fad);store(m,root+8,path_size);store(m,root+16,TABLE);store(m,0x8c88e970,HEADER)
   write_bytes(m,HEADER,c.get('filesystem_type',0).to_bytes(2,'little')+bytes(2));store(m,HEADER+4,c.get('header_fad',fad));write_bytes(m,HEADER+8,prefix+b'\0')
   s.r[4]=SRC;s.r[5]=table if c.get('explicit_table') else 0
   if not path.startswith((b'/',b'\\')):
    store(m,TABLE,1);store(m,TABLE+4,capacity);store(m,TABLE+8,c.get('cached_fad',0));record=TABLE+16
    store(m,record,45003);store(m,record+4,2048);write_bytes(m,record+8,bytes((c.get('record_flags',2),((0xda+sum(b'DIR'))>>2)&255)));write_bytes(m,record+10,b'DIR\0')
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f

def cases(smoke=False):
 base=dict(entry=STANDARD,mode=0,media_profile='standard',rtc_samples=[],fifo_samples=[0]*128,scan_samples=[0,1]*32)
 if smoke:items=[{},dict(entry=EXTENDED,media_profile='extended'),dict(entry=DIRECTORY),dict(entry=OPEN),dict(entry=EQUAL),dict(entry=SPAN)]
 else:
  items=[dict(entry=STANDARD,path=path) for path in ('/','/DIR','/DIR/CHILD','\\dir','\\dir\\child','/MISSING','/DIR/MISSING','/DIR/','//DIR','/FOO','/DIR/OTHER')]
  items += [dict(entry=EXTENDED,media_profile='extended',path=path,prefix=prefix) for path,prefix in (('/','SPECIAL'),('/DIR','SPECIAL'),('/DIR/CHILD','SPECIAL'),('\\dir','special'),('/MISSING','SPECIAL'),('/DIR/','SPECIAL'),('/','MISSING'),('/',''))]
  items += [dict(entry=entry,media_profile=profile,path=path,fail_after=failure) for entry,profile,path,failure in ((STANDARD,'crossing','/TARGETXX',-1),(STANDARD,'crossing','/TARGETXX',1),(STANDARD,'exact_path','/LAST',-1),(STANDARD,'exact_path','/LAST',1),(STANDARD,'standard','/DIR',0),(EXTENDED,'extended','/DIR',0))]
  items += [dict(entry=EXTENDED,media_profile=profile,path='/',prefix=prefix,fail_after=failure) for profile,prefix,failure in (('crossing','TARGETXX',-1),('crossing','TARGETXX',1),('exact_path','LAST',-1),('exact_path','LAST',1))]
  items += [dict(entry=STANDARD,media_profile='empty',path='/'),dict(entry=STANDARD,path_block=16),dict(entry=STANDARD,path_bytes=0),dict(entry=STANDARD,path_bytes=10)]
  items += [dict(entry=EQUAL,text_left=a,text_right=b,count=n,mode=mode) for a,b,n,mode in (('','',3,0),('','X',3,0),('ABC','abc',3,0),('ABC','abd',3,0),('ABC','abd',2,4),('ABC','abcd',5,0),('ab','AB',8,1),('X','Y',0,0),('X','Y',-1,0),('\x80','\x80',1,0),('\xff','\x80',1,0))]
  items += [dict(entry=EQUAL,count=0,source=0x8d000000),dict(entry=EQUAL,count=1,source=0x8d000000)]
  items += [dict(entry=SPAN,text_left=a,delimiters=b) for a,b in (('','/\\'),('ABC',''),('ABC','X'),('/ABC','/\\'),('AB\\CD','/\\'),('ABC/DEF','/\\'),('A','AX'),('XA','AX'),('\x80Z','\x80'))]
  items += [dict(entry=DIRECTORY,media_profile=profile,table_capacity=cap,directory_bytes=n) for profile,cap,n in (('standard',8,2048),('standard',1,2048),('standard',0,2048),('empty',0,2048),('empty',8,2048),('single',1,2048),('two',2,2048),('short_size',2,2048),('short_size',8,2048),('multi',8,2048),('multi',8,4096),('multi',8,0),('exact_directory',9,2048),('standard',8,-1),('standard',8,0),('standard',0,0),('standard',-1,2048))]
  items += [dict(entry=DIRECTORY,media_profile='multi',fail_after=n) for n in (0,1)]
  items += [dict(entry=DIRECTORY,cached_fad=45152,initial_count=7),dict(entry=DIRECTORY,media_profile='multi',table_capacity=1)]
  items += [dict(entry=OPEN,path=path,filesystem_type=kind,media_profile='extended' if kind==1 else 'standard',explicit_table=explicit) for path,kind,explicit in (('/DIR',0,False),('/DIR',0,True),('\\DIR',0,False),('/',0,False),('/MISSING',0,False),('/DIR',1,False),('/',1,False),('/DIR',1,True),('DIR',0,False),('DIR',0,True),('MISSING',0,False))]
  items += [dict(entry=OPEN,path='DIR',record_flags=0)]
  items += [dict(entry=OPEN,path='/DIR',filesystem_type=1,media_profile='extended',**extra) for extra in (dict(table_capacity=2,header_fad=45153),dict(table_capacity=1,header_fad=45153),dict(cached_fad=45153,initial_count=2,header_fad=45153),dict(header_fad=45154),dict(fail_after=0,header_fad=45153),dict(fail_after=1,header_fad=45153))]
  items += [dict(entry=entry,bad_stack=True) for entry in (STANDARD,EXTENDED,EQUAL,SPAN)]
 for c in items:yield base|c

def upper(value):return value-32 if 97<=value<=122 else value

def directory_oracle(m,a,s,r,c,block,table):
 profile=MEDIA[c['media_profile']];checks=0;fad=profile['base_lba']+150+block
 if load(m,table+8)==fad:
  assert sample(a,table,4096)==sample(m,table,4096);return 1
 # Failed reads leave the cache invalid, while any prior output remains part
 # of the complete machine comparison. No successful directory is invented.
 if s.r[0] not in (0,0xfffffff4):assert load(a,table+8)==0;return 1
 capacity=ctypes.c_int32(load(m,table+4)).value;requested=c.get('directory_bytes',2048)
 records=[]
 if requested>0 and capacity>0:
  first=profile['directories'].get(str(block),[]);size=first[0]['size'] if first else requested
  for number in range((size+2047)//2048):records.extend(profile['directories'].get(str(block+number),[]))
 expected=records[:max(0,capacity)];assert load(a,table)==len(expected);checks+=1
 for i,record in enumerate(expected):
  at=table+16+i*44;name=bytes(record['identifier'])
  if record['flags']&2:name=b'.' if name==b'\0' else b'..' if name==b'\1' else name
  else:name=name[:-2]
  fields=record['extent'].to_bytes(4,'little')+record['size'].to_bytes(4,'little')+bytes((record['flags'],))
  assert sample(a,at,9)==fields;checks+=1
  assert a[(at+9)&0xffffff]==((0xda+sum(upper(x) for x in name))>>2)&255;checks+=1
  assert sample(a,at+10,len(name)+1)==name+b'\0';checks+=1
 assert load(a,table+8)==(fad if s.r[0]==0 else 0);checks+=1
 assert sample(a,table-16,16)==sample(m,table-16,16);checks+=1
 return checks

def semantics(m,a,before,s,r,c):
 checks=2;assert len(m)==len(a)==16<<20 and r['full_state_match']
 if r['status']:return checks
 assert s.r[15]==before.r[15] and s.pr==before.pr;checks+=2
 if c['entry'] in (STANDARD,EXTENDED):
  path=c.get('path','/DIR').replace('\\','/').upper();prefix=c.get('prefix','SPECIAL').upper();profile=MEDIA[c['media_profile']]
  expected=profile['namespace'].get(path,0) if c['entry']==STANDARD else profile['prefix_namespaces'].get(prefix,{}).get(path,0)
  if c.get('fail_after',-1)>=0 or c.get('path_block',profile['path_block'])>=16:expected=0
  if c.get('path_bytes') in (0,10) and path!='/':expected=0
  assert s.r[0]==expected,(c,'abstract namespace',hex(s.r[0]),hex(expected));checks+=1
 if c['entry']==EQUAL:
  left=text_bytes(c,'text_left','AbC')+b'\0';right=text_bytes(c,'text_right','aBc')+b'\0';n=max(0,ctypes.c_int32(before.r[6]).value);equal=True
  for i in range(n):
   x=left[i] if i<len(left) else 0;y=right[i] if i<len(right) else 0
   if upper(x)!=upper(y):equal=False;break
   if not x:break
  assert s.r[0]==int(equal);checks+=1
 if c['entry']==SPAN:
  text=text_bytes(c,'text_left','ABC/DEF');delimiters=text_bytes(c,'delimiters','/\\');wanted=next((i for i,ch in enumerate(text) if ch in delimiters),len(text));assert s.r[0]==wanted;checks+=1
 if c['entry']==DIRECTORY:checks+=directory_oracle(m,a,s,r,c,c.get('directory_block',2),TABLE)
 if c['entry']==OPEN:
  path=c.get('path','/DIR').replace('\\','/').upper();profile=MEDIA[c['media_profile']];table=ALT if c.get('explicit_table') else TABLE
  extent=profile['namespace'].get(path,0) if path.startswith('/') else 45003 if path=='DIR' else 0
  if not extent or (c.get('fail_after')==0 and path.startswith('/')):assert s.r[0]==0xfffffff8;checks+=1
  elif not path.startswith('/') and not c.get('record_flags',2)&2:assert s.r[0]==0xfffffff5;checks+=1
  else:
   # A type-one post-processing step may copy the first extent into the
   # second entry. Check that original call and its narrow mutation separately.
   post=c.get('filesystem_type',0)==1 and c.get('header_fad',45152)==extent+150 and s.r[0] in (0,0xfffffff4)
   calls=sum(b['target']==hex(old.ACTIVE) for b in r['boundaries']);assert calls==int(post);checks+=1
   if post and load(a,table)>=2:assert load(a,table+0x3c)==load(a,table+0x10);checks+=1
 return checks

def main():
 ram=driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=compare(m,s,f,c);r['independent_checks']=semantics(m,a,before,s,r,c);r['guest_r0']=hex(s.r[0]);table=ALT if c.get('explicit_table') else TABLE;r['table_header']=[load(a,table+i*4) for i in range(3)];records.append(r);coverage.update(path)
  for i,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  print(hex(c['entry']),r['status'],r['guest_r0'],r['disc_read_attempts'],flush=True)
 if '--smoke' in sys.argv:return
 routines=list(manifest['routines'])+[r for r in old.manifest['routines'] if int(r['address'],16) in (DIRECTORY,OPEN)];rows=[]
 for routine in routines:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),Path(media.__file__),MP,P/'analysis/movie_disc_paths_connection.json',P/'analysis/movie_disc_paths_media.json',P/'analysis/movie_disc_init_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=sum(r['independent_checks'] for r in records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,cases=records,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original paths and directory loader execute through native sector reads of explicit invented media. No helper, media-read status or lookup return is substituted. The existing library/linked temporary descriptor is a declared direct-call fixture; the retained cold-start proof is separate. Complete machine/memory/device state and ordered service boundaries match.')
 (P/'analysis/movie_disc_paths_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Path comparisons',len(records),report['status_counts'],report['independent_checks']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
