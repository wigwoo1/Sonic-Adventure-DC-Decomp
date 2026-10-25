"""Original controller-context construction and its actual record primitives."""
import collections,csv,ctypes,hashlib,json,sys,types
from pathlib import Path
import verify_movie_revision as old
clear=old.old;driver=old.driver;P=old.P;ROOT=P.parents[1]
CTOR,FILL,FORMAT,COPY,FLAG,STORE=(0x8c65f558,0x8c6607a8,0x8c6602d2,0x8c10fb5c,0x8c65f44a,0x8c6607c8)
REQUEST,MARK,COMMAND,PAD,MASK=(0x8c6603b0,0x8c660794,0x8c65f810,0x8c65f7cc,0x8c6607be)
CTX,SRC,DST=0x8cd80000,0x8cda0000,0x8cdb0000
FRAME=0x8cd00000
MP=P/'analysis/movie_maple_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
exec(compile(clear.code,'maple_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_maple.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
observed=effects.LIB.sa_movie_maple_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=clear.Stats()
def invoke(*args):return observed(*args,ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
exec(compile(clear.compare_code,'maple_comparison','exec'),dict(old._compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_compare=local['compare']
load=driver.load;store=driver.store
def half(m,a):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+2],'little',signed=True)
def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])
def prepare(ram,c):
 m,s,f=old.prepare(ram,c);entry=c['entry']
 if entry in NEW:
  m[CTX&0xffffff:(CTX&0xffffff)+0x2000]=bytes([0xa5])*0x2000;store(m,load(m,0x8c65f65c),0 if c.get('null_context') else CTX)
  store(m,CTX+half(m,0x8c65f644),0x8ce00000);store(m,CTX+half(m,0x8c65f648),0x8ce40000)
  source=load(m,0x8c65f664);m[source&0xffffff:(source&0xffffff)+4]=bytes(c.get('presence',[0,0,0,0]))
 elif 'presence' in c:
  source=load(m,0x8c65f664);m[source&0xffffff:(source&0xffffff)+4]=bytes(c['presence'])
 if entry==FILL:s.r[4]=c.get('destination',DST);s.r[5]=c.get('value',0x89abcdef);s.r[6]=c.get('count',0)&0xffffffff
 elif entry==FORMAT:s.r[4]=c.get('destination',DST)
 elif entry==COPY:
  s.r[0]=c.get('count',4);s.r[1]=c.get('destination',SRC+1 if c.get('overlap') else DST);s.r[2]=c.get('source',SRC)
 elif entry==FLAG:s.r[4]=c.get('value',1)
 elif entry==MASK:s.r[4]=c.get('value',0x8c123456)
 if entry in (REQUEST,MARK,COMMAND,PAD):
  m[CTX&0xffffff:(CTX&0xffffff)+0x2000]=bytes(0x2000)
  unit=c.get('unit',0);code=m[(load(m,0x8c65f960)+unit)&0xffffff];group=(code&0xc0)>>6
  m[FRAME&0xffffff:(FRAME&0xffffff)+512]=bytes([0x5a])*512
  store(m,FRAME,((group^int(c.get('different_group',False)))<<16)|0x1234)
  store(m,CTX+4044,c.get('frame_pointer',FRAME));store(m,CTX+4056,c.get('previous_words',0));store(m,CTX+4060,0x20);store(m,CTX+4064,c.get('serial',1))
  store(m,CTX+4276,c.get('buffer_index',0));store(m,CTX+4292,c.get('word_order',0));store(m,CTX+4300,c.get('padding',0))
  for bank in range(2):
   for i in range(24):store(m,CTX+4072+bank*96+i*4,0x8ce00000+bank*24576+i*1024)
  m[(CTX+0x2e+unit*44)&0xffffff]=c.get('busy',0)
  s.r[4]=unit
  if entry==PAD:s.r[4]=c.get('count',2)
  if entry==COMMAND:
   s.r[5]=c.get('command',9);s.r[6]=SRC;s.r[7]=c.get('first_words',0)
   store(m,s.r[15],SRC+64);store(m,s.r[15]+4,c.get('second_words',0))
 for base in (SRC,DST):m[(base-16)&0xffffff:(base&0xffffff)+160]=bytes((i*17+5)&255 for i in range(176))
 return m,s,f
def cases(smoke=False):
 base=dict(entry=CTOR,mode=0,render_seed={0x5f8000:0x17fd11db,0x5f8004:0x11},startup_overrides={0x5f689c:0xb},irq_samples=[0,8],fifo_samples=[0]*32,scan_samples=[0,1]*32)
 if smoke:items=[{},dict(presence=[1,0,8,15]),dict(entry=0x8c010180,cold_library=True),dict(entry=FILL,count=3),dict(entry=FORMAT),dict(entry=COPY,count=9)]
 else:
  items=[dict(presence=presence,startup_overrides={0x5f689c:revision},mode=mode) for presence,revision,mode in (([0,0,0,0],11,0),([1,0,8,15],11,0),([0,8,0,1],1,0),([15,15,15,15],0,0),([1,2,4,8],11,4),([1,0,0,0],11,1))]
  items += [dict(entry=FILL,count=n,value=value) for n,value in ((0,0),(1,0),(2,0x89abcdef),(30,0xffffffff),(-1,0x12345678),(-2147483648,0))]
  items += [dict(entry=FILL,count=2,destination=0x8cfffffc),dict(entry=FILL,count=0,destination=0x8d000000)]
  items += [dict(entry=FORMAT,mode=mode) for mode in (0,1,4)]
  items += [dict(entry=FORMAT,destination=0x8cffffc0)]
  items += [dict(entry=COPY,count=n) for n in (0,1,2,3,4,5,7,8,9,31,32)]
  items += [dict(entry=COPY,count=17,overlap=True),dict(entry=COPY,count=2,source=0x8cffffff),dict(entry=COPY,count=2,destination=0x8cffffff)]
  items += [dict(entry=FLAG,value=v) for v in (0,1,2,0xffffffff,0x80000000)]
  items += [dict(entry=STORE),dict(null_context=True)]
  items += [dict(entry=MASK,value=value) for value in (0,0x8c123456,0xacffffff,0xffffffff)]
  items += [dict(entry=MARK),dict(entry=MARK,frame_pointer=0x8d000000)]
  items += [dict(entry=PAD,count=n,previous_words=1) for n in (0,1,3,0xffffffff)]
  items += [dict(entry=REQUEST,unit=unit) for unit in (0,6,23)]
  items += [dict(entry=COMMAND,first_words=n,second_words=k,word_order=order) for n,k,order in ((0,0,0),(1,0,0),(0,2,1),(1,2,1),(2,1,0),(2,1,1),(2,2,0))]
  items += [dict(entry=COMMAND,busy=0x80),dict(entry=COMMAND,serial=0x7fffffff,word_order=1),dict(entry=COMMAND,unit=23,command=0xff,first_words=2,second_words=2,buffer_index=1)]
  items += [dict(entry=COMMAND,padding=2,previous_words=words,different_group=different) for words,different in ((0,False),(1,False),(1,True))]
  items += [dict(entry=a,bad_stack=True) for a in (CTOR,FORMAT,COPY)]
  items += [dict(entry=0x8c010180,cold_library=True),dict(entry=0x8c65ebb0,arguments=[0x8cd00000,0x8cd40000,0,0])]
  items += [dict(entry=0x8c646304,arguments=[0x8cd00000,0x8cd40000,0,0]),dict(entry=0x8c601a34,arguments=[0,0x8cd00000,0x8cd40000,0])]
  items += [dict(entry=0x8c65ebb0,arguments=[0x8cd00003,0x8cd40005,0,0],presence=[1,0,8,15]),dict(entry=0x8c65ebb0,arguments=[0x8cd00000,0x8cd40000,0,0],presence=[0]*4)]
  items += [dict(entry=0x8c65ebb0,arguments=[0x8cd00003,0,0x8cd40005,0],presence=[1,0,8,15])]
  items += [dict(entry=0x8c601a34,arguments=[0,0,0x8cd40000,0]),dict(entry=0x8c601a34,arguments=[0,0,0,0])]
  items += [dict(entry=0x8c646304,sr=0),dict(entry=0x8c65ebb0,bad_stack=True)]
  items += [dict(entry=driver.DRIVER,startup_overrides={0x5f689c:0})]
 for c in items:yield base|c
def formatted():
 out=bytearray(120);out[20:26]=b'(none)';out[26:51]=b' '*25;return bytes(out)
def packet_oracle(m,before,c):
 expected=bytearray(m);unit=before.r[4];busy=m[(CTX+0x2e+unit*44)&0xffffff]
 if busy:return expected,0xffffffff
 command=1 if c['entry']==REQUEST else before.r[5]&255;n=0 if c['entry']==REQUEST else before.r[7];k=0 if c['entry']==REQUEST else c.get('second_words',0)
 code=m[(load(m,0x8c65f960)+unit)&0xffffff];bus=code&0xc0;group=bus>>6
 pointer=load(m,CTX+4044);previous=load(m,CTX+4056);padding=load(m,CTX+4300);written=load(m,CTX+4060)
 if padding>0 and previous>0 and group==((load(m,pointer)>>16)&3):
  pointer+=previous*4
  for i in range(padding):store(expected,pointer+i*4,0x700)
  previous=padding;written+=padding*4
 pointer+=previous*4;store(expected,CTX+4044,pointer);total=n+k
 store(expected,pointer,(group<<16)|(total&255));bank=load(m,CTX+4276)
 store(expected,pointer+4,load(m,CTX+4072+bank*96+unit*4)&load(m,0x8c6607ec))
 order=load(m,CTX+4292)==1
 header=((command<<24)|(code<<16)|(bus<<8)|(total&255)) if order else ((bus<<16)|(code<<8)|((total&255)<<24)|command)
 store(expected,pointer+8,header);out=pointer+12
 def swap(v):return int.from_bytes(v.to_bytes(4,'little'),'big')
 for i in range(n):
  value=load(m,before.r[6]+i*4);store(expected,out,value if order else swap(value));out+=4
 second=load(m,before.r[15])
 for i in range(k):
  value=load(m,second+i*4);store(expected,out,swap(value) if order else value);out+=4
 serial=load(m,CTX+4064);next_serial=(serial+1)&0x7fffffff
 store(expected,CTX+4056,total+3);store(expected,CTX+4060,written+(total+3)*4);store(expected,CTX+4064,next_serial or 1)
 store(expected,CTX+0x40+unit*44,serial)
 expected[(CTX+0x2e+unit*44)&0xffffff]=command
 return expected,serial
def semantics(m,a,before,s,r,c):
 entry=c['entry'];checks=0
 if r['status']:return checks
 assert s.r[15]==before.r[15] and s.pr==before.pr;checks+=2
 if entry==0x8c65ebb0:
  root=load(a,load(m,0x8c65ecac));offset=half(m,0x8c65ec9c)
  assert root==load(m,0x8c65ecb0);checks+=1
  for delta,register in ((0,4),(4,6)):
   assert load(a,root+offset+delta)==(((before.r[register]+31)&0xffffffe0)|load(m,0x8c65ecb4));checks+=1
 if entry==FILL:
  count=ctypes.c_int32(c.get('count',0)&0xffffffff).value;count=max(count,0);dest=before.r[4];expected=before.r[5].to_bytes(4,'little')*count
  assert sample(a,dest,count*4)==expected and s.r[0]==before.r[6];checks+=2
  if dest==DST:assert sample(a,dest-16,16)==sample(m,dest-16,16) and sample(a,dest+count*4,16)==sample(m,dest+count*4,16);checks+=2
 if entry==FORMAT:assert sample(a,before.r[4],120)==formatted() and s.r[0]==0;checks+=2
 if entry==COPY:
  count=c.get('count',4);expected=bytearray(m);dest=before.r[1];source=before.r[2]
  for i in range(count):expected[(dest+i)&0xffffff]=expected[(source+i)&0xffffff]
  assert sample(a,dest-16,count+32)==sample(expected,dest-16,count+32);checks+=1
  assert sample(a,source-16,count+32)==sample(expected,source-16,count+32);checks+=1
  assert list(s.r)[2:5]==list(before.r)[2:5];checks+=1
 if entry==FLAG:assert load(a,CTX+half(m,0x8c65f518))==before.r[4]&1;checks+=1
 if entry==STORE:assert load(a,CTX+half(m,0x8c6607d6))==load(m,0x8c6607f0);checks+=1
 if entry==MASK:assert s.r[0]==before.r[4]&load(m,0x8c6607ec) and a==m;checks+=2
 if entry==MARK:
  pointer=load(m,CTX+4044);assert load(a,pointer)==load(m,pointer)|load(m,0x8c6607e8) and s.r[0]==0;checks+=2
 if entry==PAD:
  count=ctypes.c_int32(before.r[4]).value;pointer=load(m,CTX+4044)+load(m,CTX+4056)*4
  assert sample(a,pointer,max(count,0)*4)==(0x700).to_bytes(4,'little')*max(count,0);checks+=1
  assert load(a,CTX+4044)==pointer and load(a,CTX+4056)==before.r[4];checks+=2
  assert load(a,CTX+4060)==(load(m,CTX+4060)+count*4)&0xffffffff and s.r[0]==0;checks+=2
 if entry in (REQUEST,COMMAND):
  expected,serial=packet_oracle(m,before,c)
  assert sample(a,CTX,0x2000)==sample(expected,CTX,0x2000) and sample(a,FRAME,512)==sample(expected,FRAME,512);checks+=2
  assert s.r[0]==serial;checks+=1
 if entry==CTOR:
  first=load(m,CTX+half(m,0x8c65f644));second=load(m,CTX+half(m,0x8c65f648))
  for i in range(24):
   assert load(a,CTX+4072+i*4)==first+i*1024 and load(a,CTX+4168+i*4)==first+24576+i*1024;checks+=2
   assert sample(a,CTX+1100+i*120,120)==formatted();checks+=1
  expected=bytearray(24);mask=7 if c['startup_overrides'][0x5f689c]&255<2 else 15
  for i,presence in enumerate(c.get('presence',[0]*4)):
   if presence&mask:expected[i*6:i*6+6]=b'REEEEE'
  assert sample(a,CTX+3984,24)==expected;checks+=1
  assert load(a,CTX+4264)==second and load(a,CTX+4268)==second+12288;checks+=2
 return checks
def main():
 ram=driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=_compare(m,s,f,c);r['independent_checks']=semantics(m,a,before,s,r,c);r['controller_context_root']=hex(load(a,load(m,0x8c65f65c)));records.append(r);coverage.update(path)
  for j,pc in enumerate(path):
   t=ROWS.get(pc,'');distance=2 if t.startswith(('bt/s ','bf/s ')) else 1
   if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  print(hex(c['entry']),r['status'],r['stop_pc'],r['controller_context_root'],flush=True)
 if '--smoke' in sys.argv:return
 rows=[];routines=list(manifest['routines'])+[r for r in old.manifest['routines'] if int(r['address'],16) in (0x8c601a34,0x8c646304,0x8c65ebb0)]
 for routine in routines:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_maple_connection.json',P/'analysis/movie_revision_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=sum(r['independent_checks'] for r in records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Every original controller constructor/record primitive and reached lower service executes. Direct root/buffer/presence fixtures are explicit preconditions; actual startup uses its original producer. Unknown controller/clock boundaries remain stops. Full CPU/RAM/FPU/software-device state and ordered ordinary boundaries match; full-clear checking is inherited unchanged.')
 (P/'analysis/movie_maple_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Controller comparisons',len(records),report['status_counts'],report['independent_checks']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
