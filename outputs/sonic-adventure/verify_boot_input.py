"""Original input update with explicit RAM driver snapshots, never service mocks."""
import collections,csv,ctypes,hashlib,itertools,json,struct,sys,types
from pathlib import Path
import verify_boot_interrupt as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/boot_input_translation.json';manifest=json.loads(MANIFEST.read_text())
UPDATE,CLASSIFY,ABSENT,CAPABILITIES,AXES,REMAP=0x8c601e2a,0x8c601df6,0x8c601bb2,0x8c601da6,0x8c601bf0,0x8c601d84
DESCRIPTOR,INFO,RECORD,DIRECTORY,REMAINDER,READ,LOCATE,GET=0x8c646856,0x8c646868,0x8c65f3a0,0x8c645898,0x8c10cbb8,0x8c646b72,0x8c646b42,0x8c646e5a
CHANNEL,RESULT,INVERSE,DRIVER,PAYLOAD,OUT=0x8c88e9a4,0x8c88ee84,0x8c88eee4,0x8cba0000,0x8cbd0000,0x8cbe0000
ROWS=dict(previous.ROWS);OWN={};PER_BODY={};NEW={int(r['address'],16) for r in manifest['routines']}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
exec(compile(previous.code,'input_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=PROJECT.parents[1]/'work/sonic-adventure/build/boot_input.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_input_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
effects.LIB.sa_interrupt_bus_init.argtypes=previous.effects.LIB.sa_interrupt_bus_init.argtypes
exec(compile(previous.compare_code,'input_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace),local:={});compare=local['compare']
def signed16(m,a):return struct.unpack_from('<h',m,a&0xffffff)[0]
def desc(m,i):return load(m,0x8c6468b0)+i*24
def info(m,i):return DRIVER+signed16(m,0x8c65f3ca)+i*120
def bytes_at(m,a,n):return m[a&0xffffff:(a&0xffffff)+n]

def prepare(ram,c):
 m,s,f=previous.prepare(ram,c);index=c.get('index',0);s.pc=c['entry']
 store(m,0x8c66c630,c.get('enabled',1));store(m,INVERSE,c.get('inverse',0));store(m,load(m,0x8c65f3d8),DRIVER)
 m[0xba0000:0xba2000]=bytes(0x2000)
 # The device-driver snapshot uses original table layout and packed metadata.
 for i in range(24):
  d=desc(m,i);p=PAYLOAD+i*64;ch=CHANNEL+i*52;inf=info(m,i)
  m[d&0xffffff:(d&0xffffff)+24]=bytes(24);m[(d+2)&0xffffff]=c.get('status',0);m[(d+4)&0xffffff]=c.get('type',8)&255;store(m,d+8,p)
  m[p&0xffffff:(p&0xffffff)+32]=bytes(range(32));m[(p+8)&0xffffff:(p+10)&0xffffff]=(c.get('buttons',0xffff)&65535).to_bytes(2,'little')
  m[(p+10)&0xffffff:(p+16)&0xffffff]=bytes(c.get('axes',[128]*6))
  for j in range(13):store(m,ch+j*4,(0x13579bdf+i*0x01010101+j*0x112233)&0xffffffff)
  store(m,ch+4,c.get('capabilities',0xffffffff));store(m,ch+8,c.get('old_buttons',0x12345678))
  features=c.get('features',1);store(m,inf,features)
  for j in range(features.bit_count()):m[(inf+4+j*4)&0xffffff:(inf+8+j*4)&0xffffff]=(c.get('capabilities',0xffffffff)&0xffffffff).to_bytes(4,'big')
  m[(inf+20)&0xffffff:(inf+32)&0xffffff]=b'input slot\x00\x00'
  m[(load(m,0x8c601c48)+i)&0xffffff]=c.get('hysteresis',0);m[(load(m,0x8c601d60)+i)&0xffffff]=c.get('hysteresis',0)
 m[load(m,0x8c601c58)&0xffffff]=c.get('high',192);m[load(m,0x8c601d54)&0xffffff]=c.get('low',64)
 store(m,DRIVER+signed16(m,0x8c65f518),c.get('remap',0))
 if c['entry'] in (ABSENT,CAPABILITIES,AXES):s.r[4]=CHANNEL+index*52
 if c['entry']==CAPABILITIES:
  s.r[5]=info(m,index);store(m,s.r[15]-16,c.get('scratch',0x2468ace0))
 if c['entry']==AXES:s.r[5]=desc(m,index);s.r[6]=index
 if c['entry']==CLASSIFY:s.r[6]=desc(m,index)
 if c['entry']==REMAP:s.r[4]=c.get('value',0xffffffff)
 if c['entry'] in (DESCRIPTOR,INFO,RECORD):s.r[4]=index
 if c['entry']==DIRECTORY:s.r[4]=index;s.r[5]=CHANNEL;s.r[6]=desc(m,index);s.r[7]=info(m,index)
 if c['entry']==READ:s.r[4]=OUT;s.r[5]=info(m,index)
 if c['entry']==LOCATE:s.r[4]=info(m,index);s.r[5]=c.get('bit',1)
 if c['entry']==GET:s.r[4]=c.get('selector',5)
 if c['entry']==REMAINDER:s.r[0]=c.get('divisor',6)&0xffffffff;s.r[1]=c.get('numerator',index)&0xffffffff
 if c.get('bad_stack') is not None:s.r[15]=c['bad_stack']
 return m,s,f

def remap(m,value):
 result=(value>>10)&load(m,0x8c601e3c)
 if value&load(m,0x8c601e40):result|=signed16(m,0x8c601e36)&0xffffffff
 if value&load(m,0x8c601e44):result|=15
 return result&0xffffffff

def absent(m,out,inverse):
 wanted=bytearray(bytes_at(m,out,52));wanted[0:8]=bytes(8)
 for j,value in ((8,0xffffffff if inverse else 0),(12,0 if inverse else 0xffffffff),(16,0xffffffff if inverse else 0),(20,0xffffffff if inverse else 0),(36,load(m,0x8c601c60)),(40,0)):struct.pack_into('<I',wanted,j,value)
 wanted[24:36]=bytes(12);return wanted

def axis_values(m,c):
 index=c.get('index',0);d=desc(m,index);p=load(m,d+8);value=int.from_bytes(bytes_at(m,p+8,2),'little')|load(m,0x8c601c64);hist=[]
 for raw,table,high,clear,setbit in ((m[(p+11)&0xffffff],load(m,0x8c601c48),load(m,0x8c601c58),load(m,0x8c601c68),load(m,0x8c601d58)),(m[(p+10)&0xffffff],load(m,0x8c601d60),load(m,0x8c601d64),load(m,0x8c601d68),load(m,0x8c601d6c))):
  h=m[(table+index)&0xffffff]
  if raw>=m[high&0xffffff]:value&=clear;h=1
  elif raw<=m[load(m,0x8c601d54)&0xffffff]:value|=setbit;h=0
  elif h:value&=clear
  hist.append((table+index,h))
 if not c.get('inverse',0):value^=0xffffffff
 caps=c.get('capabilities',0xffffffff);fields=[]
 for i,mask in enumerate((load(m,0x8c601d6c),load(m,0x8c601d58),load(m,0x8c601d74),load(m,0x8c601d78),load(m,0x8c601d7c),load(m,0x8c601d80))):fields.append((m[(p+10+i)&0xffffff]-(128 if i>=2 else 0))&65535 if caps&mask else 0)
 return value&0xffffffff,fields,hist

def semantic(m,a,b,fp,s,f,record,c):
 if record['status']!=0:return 0
 assert previous.retained(b,s) and bytes(fp)==bytes(f)
 entry=c['entry'];out=b.r[4]
 if entry==REMAP:assert s.r[0]==remap(m,out);return 2
 if entry==CLASSIFY:
  kind=c.get('type',8)&255;expected=load(m,0x8c601e50) if kind==signed16(m,0x8c601e38) else 0 if kind in (signed16(m,0x8c601e3a),8,5,6,0) else load(m,0x8c601e54)
  assert s.r[0]==expected and a==m;return 3
 if entry==ABSENT:assert bytes_at(a,out,52)==absent(m,out,c.get('inverse',0));return 2
 if entry==CAPABILITIES:
  expected=c.get('capabilities',0xffffffff) if c.get('features',1)&1 else c.get('scratch',0x2468ace0)
  if c.get('remap',0)==1:expected=remap(m,expected)
  assert load(a,out)==expected and load(a,out+4)==expected and load(a,out+36)==b.r[5]+20;return 3
 if entry==AXES:
  value,fields,hist=axis_values(m,c);assert load(a,out+8)==value and list(struct.unpack('<6H',bytes_at(a,out+24,12)))==fields
  assert all(a[address&0xffffff]==h for address,h in hist);return 4
 if entry in (DESCRIPTOR,INFO,RECORD):assert s.r[0]==(desc(m,c.get('index',0)) if entry==DESCRIPTOR else info(m,c.get('index',0))) and a==m if entry!=INFO else s.r[0]==info(m,c.get('index',0));return 2
 if entry==DIRECTORY:
  slot=load(m,0x8c6458f4)+c.get('index',0)*8
  assert load(a,CHANNEL)==load(a,CHANNEL+4)==0 and load(a,CHANNEL+36)==b.r[7]+20 and load(a,CHANNEL+40)==slot
  assert bytes_at(a,slot,8)==bytes_at(m,load(m,b.r[6]+8)+8,8);return 3
 if entry==REMAINDER:
  n=ctypes.c_int32(b.r[1]).value;d=ctypes.c_int32(b.r[0]).value;q=(abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1) if d else 0
  assert s.r[0]==((n-q*d)&0xffffffff if d else 0);return 2
 if entry==UPDATE:
  if not c.get('enabled',1):assert a==m;return 2
  for i in range(24):
   ch=CHANNEL+i*52;assert load(a,ch+48)==info(m,i) and load(a,ch+44)==load(m,ch+8)
   kind=c.get('type',8)&255;expected=load(m,0x8c601e50) if kind==signed16(m,0x8c601e38) else 0 if kind in (signed16(m,0x8c601e3a),8,5,6,0) else load(m,0x8c601e54)
   assert load(a,RESULT+i*4)==expected
   if i%6:assert load(a,ch+36)==info(m,i)+20
   else:
    current=load(a,ch+8);old=load(m,ch+8);inverse=c.get('inverse',0);diff=old^current
    assert load(a,ch+12)==current^0xffffffff
    assert load(a,ch+16)==(current|(diff^0xffffffff) if inverse else current&diff)
    assert load(a,ch+20)==((current^0xffffffff)|(diff^0xffffffff) if inverse else (current^0xffffffff)&diff)
  return 24*5+2
 return 1

def cases():
 for enabled,features,status,kind,inverse,mode in itertools.product((0,1),(0,1,0x21,0x40),(0,1),(8,9),(0,1),(0,4)):
  yield dict(entry=UPDATE,mode=mode,enabled=enabled,features=features,status=status,type=kind,inverse=inverse,axes=[0,255,64,128,192,255])
 for kind,mode in itertools.product((0,1,5,6,8,9,252,255),(0,4)):yield dict(entry=CLASSIFY,mode=mode,type=kind)
 for inverse,mode in itertools.product((0,1),range(8)):yield dict(entry=ABSENT,mode=mode,inverse=inverse)
 for features,mapping,mode in itertools.product((0,1,0x21),(0,1,2),(0,4)):yield dict(entry=CAPABILITIES,mode=mode,features=features,remap=mapping,capabilities=0x12345678)
 for raw,hist,inverse,caps,mode in itertools.product((0,64,65,128,191,192,255),(0,1),(0,1),(0,0xffffffff),(0,4)):
  yield dict(entry=AXES,mode=mode,axes=[raw]*6,hysteresis=hist,inverse=inverse,capabilities=caps)
 for value,mode in itertools.product((0,1,0xffffffff,0x80000000,0x12345678),(0,4)):yield dict(entry=REMAP,mode=mode,value=value)
 for entry,index in itertools.product((DESCRIPTOR,INFO,RECORD,DIRECTORY),(0,1,6,23)):yield dict(entry=entry,mode=0,index=index)
 for entry in (READ,LOCATE):
  for features,bit in itertools.product((0,1,0x21,0x40),(0,1,32,64)):yield dict(entry=entry,mode=0,features=features,bit=bit)
 for selector in range(12):yield dict(entry=GET,mode=0,selector=selector)
 for n,d in itertools.product((-2147483648,-25,-1,0,1,23,2147483647),(0,1,6,-1,-6)):yield dict(entry=REMAINDER,mode=0,numerator=n,divisor=d)
 for entry in (UPDATE,ABSENT,CAPABILITIES,AXES,INFO,READ):
  for stack in (0,0x8d000004):yield dict(entry=entry,mode=0,bad_stack=stack)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/boot_input_connection.json',PROJECT/'analysis/boot_interrupt_qualification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,mode=0) for e in (CLASSIFY,ABSENT,CAPABILITIES,AXES,REMAP,DESCRIPTOR,INFO,DIRECTORY,READ,GET,UPDATE)]
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c)
  try:record['independent_checks']=semantic(m,a,before,fp,s,f,record,c)
  except AssertionError as error:raise AssertionError((c,'independent input semantic check',str(error))) from error
  records.append(record);coverage.update(path)
  if '--smoke' in sys.argv:print(hex(c['entry']),record['status'],record['stop_pc'],len(path),flush=True)
  for j,pc in enumerate(path):
   if pc not in OWN:continue
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((pc,path[j+distance]))
  if (i+1)%64==0:print(i+1,'input comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Input smoke passed:',len(records));return
 rows=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=PER_BODY[a];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 files=[Path(__file__)]+proofs
 for p in proofs:files.extend(Path(path) for path in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=hashlib.sha256((PROJECT/manifest['source']).read_bytes()).hexdigest(),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,independent_checks=sum(r['independent_checks'] for r in records),external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original 24-slot input update, classification/disconnection/capability/analogue/remapping and original existing descriptor/driver/metadata/remainder adapters. Full machine/RAM/FPU/banked state, native boundaries and inherited peripheral state match. Driver RAM snapshots are explicit fixtures, not fabricated service returns.',limitations=['No hardware controller acquisition, Maple transactions, physical devices, timing or natural driver initialization is proved.','Immutable code/literals, distinct valid RAM/stack/data, privileged/MMU-off and inherited numeric/device domains apply.','When the original capability helper skips metadata reading, its guest stack scratch word retains the explicit prior RAM contents; it is not silently zero-initialized.','Full-game recovery, startup, gameplay, ports and requested display/texture features remain unfinished.'],cases=records)
 (PROJECT/'analysis/boot_input_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')},indent=2))

if __name__=='__main__':main()
