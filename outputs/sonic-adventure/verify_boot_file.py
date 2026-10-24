"""Original file-handle and lookup code; unresolved firmware stops stay explicit."""
import collections,csv,ctypes,hashlib,itertools,json,struct,sys,types
from pathlib import Path
import verify_boot_event as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/boot_file_translation.json';manifest=json.loads(MANIFEST.read_text())
THUNK,LOAD,WORKER,VOPEN,SIZE,DIV,READ,CLOSE=0x8c09da40,0x8c09cc80,0x8c09ce40,0x8c6016d6,0x8c600e2c,0x8c10ca5c,0x8c600f1e,0x8c600df4
VSTATUS,VPOLL,OPEN,INIT,TEMP,STATUS,ERROR,FIND,HASH,UPPER,COMPARE,COMPARE_I=0x8c60178c,0x8c6017a4,0x8c6004d4,0x8c600464,0x8c600d82,0x8c6013a4,0x8c5ffa08,0x8c6455ce,0x8c645442,0x8c645348,0x8c6453cc,0x8c6453e2
RAW_READ,UNALIGNED,WAIT,CANCEL,PUMP,GDREAD,GDSTATUS=0x8c601084,0x8c600fac,0x8c600cf0,0x8c6013d2,0x8c6005c2,0x8c644ff6,0x8c6451fc
FS,DIRECTORY,NAME,FD,DEST,OUT,OTHER=0x8cc80000,0x8cca0000,0x8ccb0000,0x8cc90000,0x8ccc0000,0x8ccd0000,0x8cce0000
OPS,GDOPS,BIOS=0x8c66c4b4,0x8c66d240,0x8c00f100
ROWS=dict(previous.ROWS);OWN={};PER_BODY={};NEW={int(r['address'],16) for r in manifest['routines']};GUARDS=dict(previous.namespace['GUARDS']);GUARD_ENTRY=dict(previous.namespace['GUARD_ENTRY'])
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
 guards=routine.get('bounded_research_waits',[])
 if guards:GUARD_ENTRY[int(routine['address'],16)]=[int(g['pc'],16) for g in guards]
 for g in guards:GUARDS[int(g['pc'],16)]=(g['continue_when_t'],g['max_continue_tests'])
namespace=dict(previous.namespace,ROWS=ROWS,GUARDS=GUARDS,GUARD_ENTRY=GUARD_ENTRY,COMPARISON_ROWS=set(PER_BODY[COMPARE]),RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
code=previous.code;marker='  if pc in tracked:events.append(event(pc,r,sr,pr))';assert code.count(marker)==1
# This entry is also its own loop header. A fallthrough/backedge inside that
# leaf body is not a second service/function dispatch. Calls/tails from outside
# its byte-checked body, including the root invocation, remain observed.
code=code.replace(marker,"  if pc in tracked and not (pc==0x8c6453cc and len(path)>1 and path[-2] in COMPARISON_ROWS):events.append(event(pc,r,sr,pr))")
exec(compile(code,'file_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=PROJECT.parents[1]/'work/sonic-adventure/build/boot_file.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_file_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
effects.LIB.sa_event_bus_init.argtypes=previous.effects.LIB.sa_event_bus_init.argtypes
exec(compile(previous.compare_code,'file_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace),local:={});compare=local['compare']
def word16(m,a):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+2],'little')
def put16(m,a,v):m[a&0xffffff:(a&0xffffff)+2]=(v&65535).to_bytes(2,'little')
def bytes_at(m,a,n):return m[a&0xffffff:(a&0xffffff)+n]
def upper(c):return c-32 if 97<=c<=122 else c
def name_hash(m,value):return ((sum(upper(x) for x in value)+(word16(m,0x8c64554e)&255))>>2)&255
def handle(i):return FD+i*0x54
def text(m,a):
 p=a&0xffffff;e=m.index(0,p);return bytes(m[p:e])

def prepare(ram,c):
 m,s,f=previous.prepare(ram,c);entry=c['entry'];s.pc=entry
 m[FS&0xffffff:(FS&0xffffff)+0x1000]=bytes(0x1000);m[FD&0xffffff:(FD&0xffffff)+0x800]=bytes(0x800);m[DIRECTORY&0xffffff:(DIRECTORY&0xffffff)+0x1000]=bytes(0x1000)
 store(m,0x8c673420,OPS if c.get('driver',True) else 0);store(m,0x8c673414,FS);store(m,load(m,0x8c5ffc0c),FS)
 slots=c.get('slots',3);store(m,FS,slots);store(m,FS+0xc,GDOPS);store(m,FS+0x10,DIRECTORY);store(m,FS+0x14,FD+0x300);store(m,FS+0x18,FD);store(m,FS+0x1c,handle(1) if c.get('busy',False) else 0);store(m,FS+0x3c,OPS);put16(m,FS+0x38,1);store(m,0x8c0000bc,BIOS)
 mode_ptr=load(m,0x8c600598);put16(m,mode_ptr,c.get('dma_mode',0))
 names=c.get('names',[b'SYSTEM.BIN',b'DATA.BIN',b'EMPTY.DAT']);sizes=c.get('sizes',[4097,2048,0]);store(m,DIRECTORY,c.get('directory_count',len(names)))
 for i,name in enumerate(names):
  assert len(name)<34 and 0 not in name;r=DIRECTORY+16+i*44;store(m,r,300+i*20);store(m,r+4,sizes[i] if i<len(sizes) else 0);m[(r+9)&0xffffff]=name_hash(m,name)^(1 if c.get('bad_hash',False) else 0)
  p=(r+10)&0xffffff;m[p:p+len(name)+1]=name+b'\0'
 request=c.get('request',b'SYSTEM.BIN');p=NAME&0xffffff;m[p:p+len(request)+1]=request+b'\0';other=c.get('other_name',request);p=OTHER&0xffffff;m[p:p+len(other)+1]=other+b'\0'
 opening=entry in (LOAD,THUNK,OPEN,VOPEN);free=c.get('free',0)
 for i in range(5):
  h=handle(i)
  for j in range(21):store(m,h+j*4,0xa1b20000+i*0x100+j)
  store(m,h,FS);store(m,h+4,1);store(m,h+8,450);store(m,h+0xc,c.get('size',4097));store(m,h+0x10,c.get('sectors',3));store(m,h+0x14,c.get('position',0));store(m,h+0x18,0);store(m,h+0x24,0);store(m,h+0x34,0);store(m,h+0x40,0)
  active=int(i!=free) if opening else c.get('active',0 if entry==INIT else 1)
  put16(m,h+0x4c,active);put16(m,h+0x4e,c.get('dma_mode',0));put16(m,h+0x50,c.get('state',1));put16(m,h+0x52,c.get('error',0))
 store(m,handle(0)+0x3c,1);store(m,handle(0)+0x44,0);store(m,OUT,0xdeadbeef);store(m,FS+0x30,0xabcdef01)
 m[DEST&0xffffff:(DEST&0xffffff)+0x4000]=bytes([0xa5])*0x4000
 store(m,load(m,0x8c09cec0),c.get('loading',0));store(m,load(m,0x8c09cec4),c.get('abort',0))
 h=0 if c.get('null_handle',False) else handle(c.get('index',0))
 if entry in (THUNK,LOAD):s.r[4]=NAME;s.r[5]=DEST
 elif entry in (OPEN,VOPEN):s.r[4]=NAME;s.r[5]=DIRECTORY if c.get('explicit_directory') else 0
 elif entry in (SIZE,STATUS,CLOSE,READ,RAW_READ,UNALIGNED,WAIT,CANCEL):
  s.r[4]=h;s.r[5]=OUT if entry==SIZE else c.get('count',1);s.r[6]=c.get('buffer',DEST)
 elif entry in (INIT,TEMP):s.r[4]=h;s.r[5]=c.get('file_id',7);s.r[6]=c.get('sector_start',450);s.r[7]=c.get('size',4097)
 elif entry==FIND:s.r[4]=NAME;s.r[5]=DIRECTORY;s.r[6]=c.get('case_sensitive',0)
 elif entry==HASH:s.r[4]=NAME
 elif entry in (COMPARE,COMPARE_I,0x8c6453c0):s.r[4]=NAME;s.r[5]=OTHER
 elif entry==UPPER:s.r[4]=c.get('character',97)&0xffffffff
 elif entry==DIV:s.r[0]=c.get('divisor',2048)&0xffffffff;s.r[1]=c.get('numerator',4097)&0xffffffff
 elif entry==ERROR:s.r[4]=c.get('error_value',0xfffffff8)
 elif entry==GDREAD:
  s.r[4]=c.get('sector_start',450);s.r[5]=c.get('count',1);s.r[6]=c.get('buffer',DEST);s.r[7]=c.get('dma_mode',0)
 if c.get('bad_stack') is not None:s.r[15]=c['bad_stack']
 return m,s,f

def descriptor_expected(m,h,file_id,start,size,temporary=False):
 w=bytearray(bytes_at(m,h,0x54));put=lambda offset,value:struct.pack_into('<I',w,offset,value&0xffffffff)
 for offset,value in ((4,file_id),(8,start),(12,size),(16,(ctypes.c_int32((size+2047)&0xffffffff).value>>11)&0xffffffff)):put(offset,value)
 offsets=(0x14,0x18,0x1c,0x20,0x3c,0x44) if temporary else (0x14,0x18,0x1c,0x20,0x24,0x2c,0x34,0x3c,0x40,0x44)
 for offset in offsets:put(offset,0)
 for offset,value in ((0x48,0),(0x4a,0),(0x4c,1),(0x50,0),(0x52,0)):struct.pack_into('<H',w,offset,value)
 if not temporary:struct.pack_into('<H',w,0x4e,word16(m,load(m,0x8c600598)))
 return w

def semantic(m,a,b,fp,s,f,record,c):
 if record['status']!=0:
  if record['stop_pc']==hex(BIOS):
   assert record['status']==-2 and any(int(x['target'],16)==BIOS for x in record['boundaries'])
   assert bytes_at(a,DEST,0x4000)==bytes_at(m,DEST,0x4000)
   return 2
  return 0
 assert previous.previous.previous.retained(b,s) and bytes(fp)==bytes(f)
 entry=c['entry'];h=b.r[4]
 if entry==UPPER:assert s.r[0]==(upper(ctypes.c_int32(h).value)&0xffffffff) and a==m;return 3
 if entry==HASH:assert s.r[0]==name_hash(m,c.get('request',b'SYSTEM.BIN'));return 2
 if entry in (COMPARE,COMPARE_I,0x8c6453c0):
  left=text(m,NAME);right=text(m,OTHER)
  if entry==COMPARE_I:left=bytes(upper(x) for x in left);right=bytes(upper(x) for x in right)
  assert s.r[0]==int(left==right)
  if entry==COMPARE_I:assert bytes_at(a,NAME,len(left)+1)==bytes_at(m,NAME,len(left)+1) and bytes_at(a,OTHER,len(right)+1)==bytes_at(m,OTHER,len(right)+1)
  else:assert a==m
  return 3
 if entry==FIND:
  left=text(m,NAME);found=0;count=max(ctypes.c_int32(load(m,DIRECTORY)).value,0)
  for i in range(count):
   right=text(m,DIRECTORY+16+i*44+10)
   if c.get('case_sensitive',0)!=1:right=bytes(upper(x) for x in right);candidate=bytes(upper(x) for x in left)
   else:candidate=left
   if m[(DIRECTORY+16+i*44+9)&0xffffff]==name_hash(m,left) and candidate==right:found=i+1;break
  assert s.r[0]==found;return 2
 if entry in (INIT,TEMP):
  allowed=bool(h) and (load(m,FS+0x1c)!=h if entry==TEMP else word16(m,h+0x4c)==0)
  assert s.r[0]==(h if allowed else 0)
  if allowed:assert bytes_at(a,h,0x54)==descriptor_expected(m,h,b.r[5],b.r[6],b.r[7],entry==TEMP)
  return 3
 if entry in (OPEN,VOPEN):
  if entry==VOPEN and not c.get('driver',True):assert s.r[0]==0;return 2
  wanted=text(m,NAME);names=c.get('names',[b'SYSTEM.BIN',b'DATA.BIN',b'EMPTY.DAT']);match=next((i for i,name in enumerate(names) if bytes(upper(x) for x in name)==bytes(upper(x) for x in wanted) and not c.get('bad_hash') and i<max(c.get('directory_count',len(names)),0)),None)
  free=next((i for i in range(max(c.get('slots',3),0)) if word16(m,handle(i)+0x4c)==0),None)
  if match is None:assert s.r[0]==0 and load(a,FS+0x30)==0xfffffff8
  elif free is None:assert s.r[0]==0 and load(a,FS+0x30)==0xfffffff7
  else:
   sizes=c.get('sizes',[4097,2048,0]);size=sizes[match] if match<len(sizes) else 0
   assert s.r[0]==handle(free) and load(a,FS+0x30)==0
   assert bytes_at(a,handle(free),0x54)==descriptor_expected(m,handle(free),match+1,450+match*20,size)
  return 4
 if entry==SIZE:
  valid=bool(h) and word16(m,h+0x4c)!=0;assert s.r[0]==int(valid) and load(a,OUT)==(load(m,h+0xc) if valid else 0xdeadbeef);return 3
 if entry==STATUS:
  valid=bool(h) and word16(m,h+0x4c)!=0;expected=ctypes.c_int16(word16(m,h+0x50)).value if valid else -10;assert s.r[0]==expected&0xffffffff and a==m;return 3
 if entry==CLOSE:
  if h:assert word16(a,h+0x4c)==0
  return 2
 if entry==ERROR:assert s.r[0]==1 and load(a,FS+0x30)==b.r[4];return 2
 if entry==DIV:
  n,d=ctypes.c_int32(b.r[1]).value,ctypes.c_int32(b.r[0]).value;q=(abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1) if d else 0;assert s.r[0]==q&0xffffffff;return 2
 return 1

def cases():
 for entry,request,free,mode in itertools.product((OPEN,VOPEN),(b'SYSTEM.BIN',b'system.bin',b'DATA.BIN',b'EMPTY.DAT',b'MISSING.BIN'),(0,1,2,-1),(0,4)):
  yield dict(entry=entry,mode=mode,request=request,free=free)
 for entry,slots,explicit in itertools.product((OPEN,VOPEN),(0,1,3),(0,1)):yield dict(entry=entry,mode=0,slots=slots,explicit_directory=explicit,bad_hash=bool(explicit))
 yield dict(entry=VOPEN,mode=0,driver=False)
 for count in (0,-1):yield dict(entry=OPEN,mode=0,directory_count=count)
 for entry,active,size,mode in itertools.product((INIT,TEMP),(0,1),(0,1,2047,2048,2049,0xffffffff),(0,4)):
  yield dict(entry=entry,mode=mode,active=active,size=size)
 yield dict(entry=TEMP,mode=0,busy=True,index=1)
 for entry,null,active,mode in itertools.product((SIZE,STATUS,CLOSE),(0,1),(0,1),(0,4)):yield dict(entry=entry,mode=mode,null_handle=null,active=active)
 for state in (0,1,2,3,4,5,0xffff):yield dict(entry=CLOSE,mode=0,state=state)
 for request,other,sensitive,entry in itertools.product((b'',b'ABC',b'abc',b'ABCD',b'\x80abc'),(b'',b'ABC',b'abc'),(0,1),(FIND,COMPARE,COMPARE_I,0x8c6453c0)):
  yield dict(entry=entry,mode=0,request=request,other_name=other,case_sensitive=sensitive,names=[b'',b'ABC',b'ABCD',b'\x80abc'],sizes=[0]*4)
 for character in (-1,0,64,65,90,96,97,122,123,127,128,255):yield dict(entry=UPPER,mode=0,character=character)
 for request in (b'',b'ABC',b'abc',b'\x80\xff',b'LONG_FILENAME.PRS'):yield dict(entry=HASH,mode=0,request=request)
 for n,d in itertools.product((-2147483648,-4097,-1,0,1,4097,2147483647),(0,1,2048,-1,-2048)):yield dict(entry=DIV,mode=0,numerator=n,divisor=d)
 for entry,size,request in itertools.product((LOAD,THUNK),(0,1,2048,2049),(b'SYSTEM.BIN',b'MISSING.BIN')):yield dict(entry=entry,mode=0,request=request,sizes=[size,0,0])
 for entry in (READ,RAW_READ,UNALIGNED,WAIT,CANCEL,GDREAD,PUMP,VSTATUS,VPOLL,WORKER):
  yield dict(entry=entry,mode=0)
 for entry in (VSTATUS,VPOLL,WORKER):yield dict(entry=entry,mode=0,driver=False)
 yield dict(entry=READ,mode=0,busy=True)
 for entry in (LOAD,OPEN,VOPEN,INIT,TEMP,SIZE,READ):
  for stack in (0,0x8d000004):yield dict(entry=entry,mode=0,bad_stack=stack)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/boot_file_connection.json',PROJECT/'analysis/boot_event_qualification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,mode=0) for e in (UPPER,HASH,FIND,OPEN,VOPEN,INIT,TEMP,SIZE,STATUS,CLOSE,DIV,LOAD,READ,WORKER)]
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c)
  try:record['independent_checks']=semantic(m,a,before,fp,s,f,record,c)
  except AssertionError as error:raise AssertionError((c,'independent file semantics',str(error))) from error
  records.append(record);coverage.update(path)
  if '--smoke' in sys.argv:print(hex(c['entry']),record['status'],record['stop_pc'],len(path),flush=True)
  for j,pc in enumerate(path):
   if pc not in OWN:continue
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((pc,path[j+distance]))
  if (i+1)%128==0:print(i+1,'file comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('File smoke passed:',len(records));return
 rows=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=PER_BODY[a];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 files=[Path(__file__),PROJECT/'verify_boot_event.py']+proofs
 for p in proofs:files.extend(Path(path) for path in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=hashlib.sha256((PROJECT/manifest['source']).read_bytes()).hexdigest(),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,independent_checks=sum(r['independent_checks'] for r in records),external_game_service_fixtures=0,unresolved_firmware_vector=hex(BIOS),newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original directory/name/handle/open/size/temporary/status/close and file/read/driver proposals. Complete reached/final RAM/machine/FPU/banks/MACH/MACL/GBR/VBR and software peripheral state match. Driver/context/directory snapshots use original table layouts. Real byte code executes until missing firmware/cache/game dependencies; no file or firmware return is supplied.',limitations=['Firmware vector contents and driver/filesystem/directory/handle tables are explicit runtime fixtures. No BIOS ROM code, firmware service result, physical disc read, natural filesystem initialization or completion is fabricated.','File loader, read/wait/cancellation/service and callback parents remain partial at their real platform/game boundaries unless separately qualified. No full-parent credit is inferred from missing-file or zero-byte cases.','Known cache routines with unexported uncached alias blocks and large discontiguous initialization are retained as unverified research exports, not connected as completed helpers.','The finite 50000-iteration retry delay is bounded only in research builds: -4 stops on the second continued loop test. Without SA_BOUND_RETAIL_STALLS the original finite delay/retry loops remain.','Immutable code/literals, distinct initialized terminated names/directories/handles/stack, valid qualified CPU/MMU-off and inherited numeric/device domains apply. Gameplay, ports/demakes and display/texture features remain unfinished.'],cases=records)
 def evidence_value(value):
  if isinstance(value,(bytes,bytearray)):return {'bytes_hex':value.hex()}
  raise TypeError(type(value).__name__)
 (PROJECT/'analysis/boot_file_verification.json').write_text(json.dumps(report,indent=2,default=evidence_value)+'\n');print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')},indent=2))

if __name__=='__main__':main()
