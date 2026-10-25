"""Original initializer records, request pools and actual driver-file loader."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_record_constructors as old
import movie_diagnostics_reference_cpu as cpu
import sh4_widen_reference as widen_model
import console_service_reference as console_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_diagnostics_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
assert old.namespace['reference'] is cpu.base.cpu_ns['reference']
namespace['reference']=cpu.reference
old_probe=old.namespace['numeric_probe']
def numeric_probe(w,f,sr):
 if f.fpscr&0x80000 and w&0xf00f in (0xf004,0xf005):
  probe=old.effects.Fpu.from_buffer_copy(f);rc,new_sr=widen_model.compare(probe,sr,w>>8&15,w>>4&15,w&15==5);return rc,probe,new_sr
 return old_probe(w,f,sr)
namespace['numeric_probe']=numeric_probe
marker='  scalar=w>>12==15';assert code.count(marker)==1
code=code.replace(marker,"  if w&0xf0ff==0xf0ad:\n   probe=Fpu.from_buffer_copy(f);rc=widen_model.widen(probe,f.fpul,w>>8&15)\n   if rc:stop(pc,r,sr,rc,probe)\n"+marker)
namespace['widen_model']=widen_model
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_diagnostics.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_diagnostics_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});compare=local['compare']

LOG,OUTPUT,PRINTF,FORMAT,CHECK,INTEGER,LENGTH,FLOATING,APPEND,TERMINATE,FILE_QUERY,MOVE,DECIMAL,DIGITS,ROUND=(0x8c65a2fc,0x8c65a33e,0x8c10d400,0x8c10dbc4,0x8c10e576,0x8c10e304,0x8c10d3f0,0x8c10f1a4,0x8c10e50e,0x8c10e566,0x8c600ec6,0x8c10d25c,0x8c10fedc,0x8c10fdac,0x8c10f8a2)
DEST,FMT,VA,TEXT,CONTEXT,FILE=0x8cda0000,0x8cdb0000,0x8cdc0000,0x8cdd0000,0x8cde0000,0x8cdf0000
CALLBACK=0x8c5ff2cc

def seed_ram():return seed.seed_ram()
def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])
def write(m,a,data):m[a&0xffffff:(a&0xffffff)+len(data)]=data
def prepare(ram,c):
 m,s,f=old.old.prepare(ram,c);entry=c['entry']
 if entry not in NEW:return m,s,f
 write(m,DEST-16,bytes([0xa5])*0x1010);write(m,FMT,c.get('diagnostic_format','hello').encode('latin1')+b'\0');write(m,TEXT,c.get('diagnostic_text','ABC').encode('latin1')+b'\0')
 for i,x in enumerate(c.get('args',[42])):store(m,VA+i*4,x&0xffffffff)
 store(m,0x8c67c1dc,c.get('callback',CALLBACK));store(m,0x8c67c1d8,c.get('enabled',1));store(m,0x8c67340c,0x12345678)
 if entry==LOG:
  store(m,s.r[15],FMT)
  for i,x in enumerate(c.get('args',[42])):store(m,s.r[15]+4+i*4,x&0xffffffff)
 elif entry==OUTPUT:s.r[4]=TEXT
 elif entry==PRINTF:s.r[4]=DEST;s.r[5]=FMT;s.r[6]=VA
 elif entry==FORMAT:
  s.r[4]=c.get('format_kind',1);s.r[5]=c.get('writer',CALLBACK);s.r[6]=c.get('stream',DEST);s.r[7]=FMT;store(m,s.r[15],VA)
 elif entry in (CHECK,TERMINATE):
  store(m,CONTEXT,c.get('format_kind',1));store(m,CONTEXT+4,DEST);s.r[4]=CONTEXT;write(m,DEST+0x10,bytes([c.get('file_flags',0)]))
 elif entry==FILE_QUERY:
  s.r[4]=0 if c.get('null_file') else FILE;write(m,FILE+0x4c,(c.get('file_active',1)&0xffff).to_bytes(2,'little'));store(m,FILE+0x14,c.get('file_value',0x12345678))
 elif entry==LENGTH:s.r[4]=TEXT
 elif entry==APPEND:
  s.r[4]=TEXT;s.r[5]=c.get('count',3)&0xffffffff;s.r[6]=CONTEXT
  for i,x in enumerate((c.get('format_kind',1),DEST,7,c.get('writer',CALLBACK))):store(m,CONTEXT+4*i,x)
 elif entry==MOVE:
  src=TEXT+c.get('src_offset',0);dst=TEXT+c.get('dst_offset',0x200);write(m,TEXT,bytes(range(256))*4);s.r[4]=dst;s.r[5]=src;s.r[6]=c.get('count',16)
 elif entry==ROUND:
  write(m,DEST,c.get('digits','099995').encode('latin1')+b'\0');s.r[4]=DEST;s.r[5]=c.get('digit_index',5)
 elif entry==INTEGER:
  s.r[4]=c.get('value',42)&0xffffffff;s.r[5]=DEST;s.r[6]=c.get('specifier',100);s.r[7]=CONTEXT;write(m,CONTEXT,bytes(48));write(m,CONTEXT,bytes([c.get('number_flags',0),c.get('number_padding',32)]));store(m,CONTEXT+4,c.get('number_width',0));store(m,CONTEXT+8,c.get('number_precision',0xffffffff));store(m,CONTEXT+12,32)
 elif entry==FLOATING:
  store(m,CONTEXT,VA);s.r[4]=CONTEXT;s.r[5]=DEST;s.r[6]=ord('f');s.r[7]=CONTEXT+0x100;write(m,CONTEXT+0x100,bytes(48));store(m,CONTEXT+0x108,c.get('number_precision',0xffffffff))
 if c.get('conversion_dn0'):f.fpscr&=~0x40000
 if c.get('conversion_invalid_trap'):f.fpscr|=0x800
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f

def cases(smoke=False):
 base=next(old.old.cases())
 if smoke:
  for entry in (LOG,OUTPUT,PRINTF,FORMAT,CHECK,INTEGER,LENGTH,FLOATING,APPEND,TERMINATE,FILE_QUERY,MOVE,ROUND):yield base|dict(entry=entry)
  yield base|dict(entry=0x8c65acc0)
  return
 for entry,mode in itertools.product((LOG,OUTPUT,PRINTF,FORMAT,CHECK,INTEGER,LENGTH,APPEND,TERMINATE,FILE_QUERY,MOVE,ROUND),(0,1,4)):yield base|dict(entry=entry,mode=mode)
 for entry in (LOG,OUTPUT):
  for cb,flag in itertools.product((0,CALLBACK,0x8cfef000),(0,1,2,0xffffffff)):yield base|dict(entry=entry,callback=cb,enabled=flag)
 for entry in (PRINTF,FORMAT,LOG):
  for fmt,args,expected in (('',[],''),('hello',[],'hello'),('%%',[],'%'),('%d',[0],'0'),('%d',[0xffffffff],'-1'),('%u',[0xffffffff],'4294967295'),('%x',[0xdeadbeef],'deadbeef'),('%08X',[0x1a2b],'00001A2B'),('%#x',[0x123],'0x123'),('%s',[TEXT],'ABC'),('%c',[65],'A'),('%d/%o/%u',[12,9,7],'12/11/7')):
   yield base|dict(entry=entry,diagnostic_format=fmt,args=args,expected=expected)
 for entry in (CHECK,TERMINATE):
  for kind,flag in itertools.product((0,1,2,0xffffffff),(0,0x40,0x80,0xff)):yield base|dict(entry=entry,format_kind=kind,file_flags=flag)
 for active in (0,1,0xffff):yield base|dict(entry=FILE_QUERY,file_active=active)
 yield base|dict(entry=FILE_QUERY,null_file=True)
 for text in ('','a','a'*33,'ABC\x80\xff'):yield base|dict(entry=LENGTH,diagnostic_text=text)
 for kind,n,writer in itertools.product((0,1),(0,1,3,16),(CALLBACK,0x8cfef000)):
  yield base|dict(entry=APPEND,format_kind=kind,count=n,writer=writer,diagnostic_text=('ABC'*6)[:16])
 yield base|dict(entry=APPEND,format_kind=0,count=3,diagnostic_text='\0AB')
 for count,src,dst in itertools.product((0,1,2,3,4,8,16,31),(0,1,2),(0,1,2,4,0x200)):
  yield base|dict(entry=MOVE,count=count,src_offset=src,dst_offset=dst)
 for digits,index in (('05',1),('012344',5),('012345',5),('099995',5),('000005',5),('099999',3)):
  yield base|dict(entry=ROUND,digits=digits,digit_index=index)
 for value,specifier in itertools.product((0,1,0xffffffff,0x80000000,0x7fffffff),(100,105,117,111,120,88)):yield base|dict(entry=INTEGER,value=value,specifier=specifier)
 for raw,mode in itertools.product((0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0x7f800001,0x7fc00001),(0,4)):
  yield base|dict(entry=FLOATING,args=[raw],mode=mode)
 yield base|dict(entry=FLOATING,args=[1],conversion_dn0=True)
 yield base|dict(entry=FLOATING,args=[0x7fc00001],conversion_invalid_trap=True)
 selection=P/'analysis/movie_diagnostics_integer_case_selection.json'
 if selection.exists():
  for selected in json.loads(selection.read_text())['cases']:yield base|dict(entry=INTEGER)|selected
 yield base|dict(entry=0x8c65acc0)

def main():
 ram=seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);r,path,a,s,f=compare(m,s,f,c);assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2;r['guest_r0_at_stop']=hex(s.r[0]);records.append(r);coverage.update(path)
  if 'expected' in c and r['status']==0:
   output=load(m,0x8c65a370) if c['entry']==LOG else DEST;expected=c['expected'].encode('latin1');assert sample(a,output,len(expected)+1)==expected+b'\0',(c,sample(a,output,64));checks+=1
  for i,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  print(hex(c['entry']),r['status'],r['stop_pc'],hex(s.r[0]),flush=True)
 if '--smoke' in sys.argv:return
 rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[P/'analysis/movie_diagnostics_integer_case_selection.json',Path(__file__),Path(old.__file__),MP,P/'analysis/movie_diagnostics_connection.json',P/'analysis/movie_record_constructors_qualification.json',P/'sh4_widen_reference.py',P/'movie_diagnostics_reference_cpu.py']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Original diagnostic/formatting bodies with explicit RAM/vararg/callback fixtures. All machine/RAM/device state and ordered boundaries compare. Selected formatted output strings have independent literal oracles. Callback fixtures invoke actual existing retail code; unknown callbacks/callees remain stops. Complete formatter/physical output/driver execution and natural startup are not claimed.')
 (P/'analysis/movie_diagnostics_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Diagnostic comparisons',len(records),checks,report['status_counts']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
