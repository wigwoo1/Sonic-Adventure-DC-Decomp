"""Original disc initializer and ISO helpers in the retained native platform."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_rtc as old
import disc_init_platform_reference as platform_revision
P=old.P;ROOT=old.ROOT;driver=old.driver;load=old.load;store=old.store
THUNK,INIT,WRAPPER,RECORD,DIRECTORY,SECTOR,SIGNATURE,ENDIAN,FIND,ACTIVE,VERSION,PARSE,COPY=(0x8c600032,0x8c600038,0x8c601618,0x8c600dd0,0x8c6454fe,0x8c5ffc98,0x8c5ff9e6,0x8c5ff9c0,0x8c60041a,0x8c5ffcfe,0x8c64530a,0x8c645468,0x8c64535c)
OPEN,OPEN_THUNK,OPEN_INTERFACE,FIRMWARE_INIT=0x8c6002a2,0x8c600ddc,0x8c60167e,0x8c10d974
CTX,TABLE,SRC,DST,BUFFER=0x8cd00000,0x8cd40000,0x8cda0000,0x8cdb0000,0x8cd80000
MP=P/'analysis/movie_disc_init_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
exec(compile(old.code,'disc_init_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_disc_init.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
observed=effects.LIB.sa_movie_disc_init_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
platform=types.SimpleNamespace(**old.compare.__globals__['platform'].__dict__);platform.DiscReference=platform_revision.DiscReference
compare=types.FunctionType(old.compare.__code__,dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats,platform=platform),'compare')

def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])

def prepare(ram,c):
 m,s,f=old.prepare(ram,c);entry=c['entry']
 if c.get('cold_filesystem'):store(m,load(m,0x8c6000e4),0);store(m,0x8c673420,0)
 if entry in NEW:
  for base,size in ((SRC,512),(DST,512),(CTX,4096),(TABLE,4096),(BUFFER,4096)):
   m[(base-16)&0xffffff:(base&0xffffff)+size+16]=bytes([0xa5])*(size+32)
  if entry in (INIT,WRAPPER,THUNK):
   store(m,load(m,0x8c6000e4),CTX if c.get('existing_root') else 0)
   m[CTX&0xffffff:(CTX&0xffffff)+4096]=bytes(4096);m[TABLE&0xffffff:(TABLE&0xffffff)+4096]=bytes(4096)
   store(m,CTX,2)
   at=(CTX+0x38)&0xffffff;m[at:at+2]=c.get('initialized',0).to_bytes(2,'little')
   s.r[4]=c.get('slots',2)&0xffffffff;s.r[5]=CTX+c.get('misaligned',0);s.r[6]=c.get('capacity',4);s.r[7]=TABLE
   if entry==INIT:store(m,s.r[15],0x8c66c4b4)
  elif entry==ENDIAN:
   s.r[4]=c.get('source',SRC);m[SRC&0xffffff:(SRC&0xffffff)+4]=c.get('value',0x89abcdef).to_bytes(4,'little')
  elif entry==SIGNATURE:
   s.r[4]=SRC;m[(SRC+1)&0xffffff:(SRC&0xffffff)+6]=b'CD001'
   if c.get('mismatch') is not None:m[(SRC+1+c['mismatch'])&0xffffff]^=0x80
  elif entry==RECORD:s.r[4]=c.get('destination',DST);s.r[5]=c.get('value',0x89abcdef)
  elif entry==ACTIVE:s.r[4]=0 if c.get('null') else DST;store(m,DST,c.get('count',2)&0xffffffff);store(m,DST+16,c.get('value',0x89abcdef))
  elif entry==COPY:
   s.r[4]=c.get('source',SRC);s.r[5]=c.get('destination',SRC+1 if c.get('overlap') else DST);s.r[6]=c.get('count',4)&0xffffffff
   m[SRC&0xffffff:(SRC&0xffffff)+128]=bytes((i*17+3)&255 for i in range(128))
  elif entry==PARSE:
   s.r[4]=SRC;s.r[5]=DST;length=c.get('record_length',48);name=bytes(c.get('name',list(b'FOO.BIN;1')));flags=c.get('flags',0)
   m[SRC&0xffffff:(SRC&0xffffff)+256]=bytes(256);m[SRC&0xffffff]=length;store(m,SRC+2,0x12345678);store(m,SRC+10,0x87654321);m[(SRC+25)&0xffffff]=flags;m[(SRC+32)&0xffffff]=len(name);m[(SRC+33)&0xffffff:(SRC+33+len(name))&0xffffff]=name
  elif entry in (FIND,DIRECTORY,SECTOR,VERSION):
   store(m,load(m,0x8c6000e4),CTX);m[CTX&0xffffff:(CTX&0xffffff)+4096]=bytes(4096);store(m,CTX+0x10,TABLE);store(m,CTX+0x14,DST)
   if entry==FIND:
    store(m,TABLE,0 if c.get('missing_file') else 1);store(m,TABLE+16,0x123);store(m,TABLE+20,0x456);m[(TABLE+24)&0xffffff]=2
    name=b'TEST.BIN';m[SRC&0xffffff:(SRC&0xffffff)+len(name)+1]=name+b'\0';m[(TABLE+26)&0xffffff:(TABLE&0xffffff)+26+len(name)+1]=name+b'\0'
    m[(TABLE+25)&0xffffff]=((0xda+sum(name.upper()))>>2)&255
    s.r[4]=SRC;s.r[5]=DST
   elif entry==DIRECTORY:s.r[4]=BUFFER;s.r[5]=c.get('sector',0);s.r[6]=TABLE;s.r[7]=c.get('bytes',0);store(m,TABLE,0);store(m,TABLE+4,c.get('capacity',2));store(m,TABLE+8,c.get('cached_sector',0))
   elif entry==SECTOR:s.r[4]=c.get('sector',16);s.r[5]=1;s.r[6]=BUFFER
   else:s.r[4]=BUFFER
  elif entry in (OPEN,OPEN_THUNK,OPEN_INTERFACE):
   store(m,load(m,0x8c6000e4),CTX);m[CTX&0xffffff:(CTX&0xffffff)+4096]=bytes(4096);store(m,CTX+16,TABLE);store(m,TABLE,0)
   store(m,0x8c673420,0 if c.get('null_interface') else 0x8c66c4b4)
   store(m,load(m,0x8c600358),CTX+0x300);m[(CTX+0x300)&0xffffff:(CTX+0x302)&0xffffff]=b'\0\0'
   m[SRC&0xffffff:(SRC&0xffffff)+9]=b'MISSING\0\0';s.r[4]=SRC;s.r[5]=0
 return m,s,f

def cases(smoke=False):
 base=dict(entry=INIT,mode=0,rtc_samples=[0x1234,0xabcd]*32,render_seed={0x5f8000:0x17fd11db,0x5f8004:0x11},startup_overrides={0x5f689c:0xb},irq_samples=[0,8],fifo_samples=[0]*128,scan_samples=[0,1]*32)
 if smoke:items=[dict(entry=0x8c010180,cold_library=True),{},dict(entry=INIT,misaligned=1),dict(entry=INIT,existing_root=True,initialized=1),dict(entry=ENDIAN),dict(entry=PARSE)]
 else:
  items=[dict(entry=0x8c010180,cold_library=True),dict(entry=0x8c010180,cold_library=True,cold_filesystem=True)]
  items += [dict(entry=a) for a in sorted(NEW)]
  items += [dict(entry=INIT,misaligned=x) for x in (1,3,31)]+[dict(entry=INIT,existing_root=True,initialized=x) for x in (0,1,2)]
  items += [dict(entry=INIT,slots=n) for n in (0,1,3)]
  items += [dict(entry=INIT,fail_after=n) for n in (0,1)]
  items += [dict(entry=ENDIAN,value=v,mode=mode) for v,mode in itertools.product((0,0xffffffff,0x12345678,0x80000001),(0,1,4))]
  items += [dict(entry=SIGNATURE,mismatch=i) for i in range(5)]+[dict(entry=SIGNATURE,mode=4)]
  items += [dict(entry=RECORD,value=value) for value in (0,0xffffffff,0x12345678)]+[dict(entry=RECORD,destination=0x8cfffff8)]
  items += [dict(entry=ACTIVE,count=count) for count in (0,1,2,3,0xffffffff,0x80000000)]+[dict(entry=ACTIVE,null=True)]
  items += [dict(entry=OPEN_INTERFACE,null_interface=True),dict(entry=OPEN_THUNK,null_interface=True),dict(entry=FIND,missing_file=True)]
  items += [dict(entry=COPY,count=n) for n in (0,1,2,3,7,31,0xffffffff,0x80000000)]+[dict(entry=COPY,count=17,overlap=True),dict(entry=COPY,count=2,source=0x8cffffff),dict(entry=COPY,count=2,destination=0x8cffffff)]
  items += [dict(entry=PARSE,record_length=n) for n in (0,1,33,34)]
  items += [dict(entry=PARSE,flags=flags,name=list(name)) for flags,name in ((2,b'\0'),(2,b'\1'),(2,b'DIRECTORY'),(0,b'A;1'),(0,b'LONG-NAME.BIN;1'))]
 for c in items:yield base|c

def semantics(m,a,before,s,r,c):
 if r['status']:return 0
 checks=2;assert s.r[15]==before.r[15] and s.pr==before.pr
 entry=c['entry']
 if entry==ENDIAN:assert s.r[0]==int.from_bytes(sample(m,before.r[4],4),'little');checks+=1
 if entry==SIGNATURE:assert s.r[0]==int(sample(m,before.r[4]+1,5)==b'CD001');checks+=1
 if entry==RECORD:
  expected=bytearray(m);store(expected,before.r[4],0);store(expected,before.r[4]+4,before.r[5]);store(expected,before.r[4]+8,0)
  assert a==expected and s.r[0]==before.r[4];checks+=2
 if entry==ACTIVE:
  expected=bytearray(m);pointer=before.r[4]
  if pointer and ctypes.c_int32(load(m,pointer)).value>=2:store(expected,pointer+0x3c,load(m,pointer+16))
  assert a==expected;checks+=1
 if entry==COPY:
  expected=bytearray(m);n=max(0,ctypes.c_int32(before.r[6]).value)
  for i in range(n):expected[((before.r[5]+i)&0xffffff)]=expected[((before.r[4]+i)&0xffffff)]
  assert a==expected and s.r[6]==(0 if n else before.r[6]);checks+=2
 if entry==FIND:
  if c.get('missing_file'):assert s.r[0]==0xfffffff8 and sample(a,DST,32)==sample(m,DST,32);checks+=2
  else:assert s.r[0]==0 and sample(a,DST,9)==(0x123+150).to_bytes(4,'little')+(0x456).to_bytes(4,'little')+b'\2';checks+=2
 if entry==PARSE:
  expected=bytearray(m);length=m[SRC&0xffffff]
  if length>=34:
   store(expected,DST,load(m,SRC+2));store(expected,DST+4,load(m,SRC+10));flags=m[(SRC+25)&0xffffff];expected[(DST+8)&0xffffff]=flags;name=sample(m,SRC+33,m[(SRC+32)&0xffffff]);name=name if flags&2 else name[:-2]
   if flags&2 and name==b'\0':name=b'.'
   elif flags&2 and name==b'\1':name=b'..'
   # Original writes may leave bytes from the initial identifier copy beyond
   # the inserted terminator; only the defined fields are checked here.
   assert sample(a,DST,9)==sample(expected,DST,9) and sample(a,DST+10,len(name)+1)==name+b'\0';checks+=2
  else:assert sample(a,DST-16,128)==sample(m,DST-16,128) and sample(a,SRC,256)==sample(m,SRC,256);checks+=1
  assert s.r[0]==(length if length>=34 else 0);checks+=1
 return checks

def version_checks():
 service=effects.LIB.sa_disc_service;service.argtypes=[ctypes.POINTER(platform.Queue),ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32)];service.restype=ctypes.c_int
 checks=0
 for alias in (0,0x80000000,0xa0000000):
  c={};ref=platform_revision.DiscReference(ROOT,c);ram=bytearray([0xa5])*(16<<20);expected=bytearray(ram);view=effects.RAM_TYPE.from_buffer(ram);queue=platform.Queue();calls=[]
  @platform.Read
  def transport(user,pos,out,n):calls.append((pos,n));return -77
  assert effects.LIB.sa_disc_queue_init(ctypes.byref(queue),transport,None,ref.offset,ref.sectors,ref.base,ref.stride)==0;checks+=1
  parameters=alias|0x0cda0000;destination=alias|0x0cdb0000
  for memory in (ram,expected):store(memory,parameters,destination);store(memory,parameters+4,0)
  result=ctypes.c_uint32(0xdeadbeef);rc=service(ctypes.byref(queue),view,0,40,parameters,ctypes.byref(result));wanted,value=ref.service(expected,0,40,parameters)
  assert (rc,result.value)==(wanted,value)==(0,1);checks+=1
  rc=service(ctypes.byref(queue),view,2,0,0,ctypes.byref(result));wanted,value=ref.service(expected,2,0,0)
  assert (rc,result.value)==(wanted,value)==(0,0);checks+=1
  assert len(ram)==len(expected)==16<<20 and ram==expected;checks+=1
  assert sample(ram,destination,16)==platform_revision.VERSION and sample(ram,destination-16,16)==bytes([0xa5])*16 and sample(ram,destination+16,16)==bytes([0xa5])*16;checks+=3
  assert not calls and platform.snapshot(queue,view,[])==ref.snapshot();checks+=2
  for memory in (ram,expected):store(memory,parameters,0);store(memory,parameters+4,0);store(memory,parameters+8,0);store(memory,parameters+12,0)
  rc=service(ctypes.byref(queue),view,1,1,parameters,ctypes.byref(result));wanted,value=ref.service(expected,1,1,parameters)
  assert (rc,result.value)==(wanted,value)==(0,2) and ram==expected and load(ram,parameters+8)==16;checks+=2
 # Retain a concrete regression witness for the frozen historical model.
 old_ref=platform_revision.PreviousDiscReference(ROOT,{});memory=bytearray(16<<20);store(memory,0x8cda0000,0x8cdb0000);store(memory,0x8cda0004,0)
 assert old_ref.service(memory,0,40,0x8cda0000)==(0,1);old_ref.service(memory,2,0,0)
 assert len(memory)==(16<<20)+1;checks+=1
 return checks

def main():
 ram=driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=compare(m,s,f,c);r['independent_checks']=semantics(m,a,before,s,r,c);r['guest_r0']=hex(s.r[0]);root=load(a,load(m,0x8c6000e4));r['filesystem_root_before']=hex(load(m,load(m,0x8c6000e4)));r['filesystem_root_after']=hex(root);r['filesystem_initialized_flag']=int.from_bytes(sample(a,root+0x38,2),'little') if root&0x1f000000==0x0c000000 else None;records.append(r);coverage.update(path)
  for j,pc in enumerate(path):
   t=ROWS.get(pc,'');distance=2 if t.startswith(('bt/s ','bf/s ')) else 1
   if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  print(hex(c['entry']),r['status'],r['stop_pc'],r['disc_read_attempts'],flush=True)
 if '--smoke' in sys.argv:return
 rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),Path(platform_revision.__file__),P/'boot_disc_reference.py',P/'source/native_disc_queue.c',MP,P/'analysis/movie_disc_init_connection.json',P/'analysis/movie_rtc_qualification.json'];api=version_checks()
 report=dict(cases_passed=len(records),independent_checks=sum(r['independent_checks'] for r in records),version_backend_checks=api,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Original disc-library and ISO bodies execute in the existing raw-clock/native-disc platform. Every reached service executes or retains its actual missing/fault/research-wait boundary. Parent coverage and canonical helper reuse remain separate from new qualification. A versioned test model corrects the older 17-byte write to the native 16-byte identity response; native platform and older proof/source bytes are unchanged.')
 (P/'analysis/movie_disc_init_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Disc-init comparisons',len(records),report['status_counts'],report['independent_checks']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
