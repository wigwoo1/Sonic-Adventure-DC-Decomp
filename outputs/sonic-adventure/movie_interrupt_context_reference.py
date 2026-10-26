"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_transfer_dispatch_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_interrupt_context_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
from verify_sh4_exception_state import ExceptionState
import interrupt_context_reference_cpu as context_cpu
EXCEPTION_KEYS=('ssr','spc','sgr','expevt','intevt','tra','returned')
namespace['reference']=context_cpu.reference
native_exception=ExceptionState()
code=old.code
marker=" path=[];events=[];";assert code.count(marker)==1
# Initialize explicit exception state independently from the native structure.
line=next(l for l in code.splitlines() if l.startswith(marker))
code=code.replace(line,line+"\n special.update({k:c.get(k,0) for k in EXCEPTION_KEYS})")
code=code.replace('console_reference.snapshot()','console_reference.snapshot(),tuple(special[k] for k in EXCEPTION_KEYS)')
marker='   if a not in queue_reference and ';assert code.count(marker)==1
code=code.replace(marker,'   if a not in (0xff000020,0xff000024,0xff000028) and a not in queue_reference and ')
marker='  assert pc in ROWS,hex(pc);path.append(pc);text=ROWS[pc]';assert code.count(marker)==1
code=code.replace(marker,marker+"\n  if pc in CONTEXT_ROWS and w>>12==0xf and sr&0x8000:stop(pc,r,sr,-4)\n  if pc in CONTEXT_ROWS and (w==0x2b or w&0xf0ff in (0x0032,0x0042,0x003a,0x403e,0x404e,0x4003,0x4023,0x4033,0x4043,0x4007,0x4027,0x4037,0x4047)) and not sr&0x40000000:stop(pc,r,sr,-4)")
code=code.replace('reference(memory,state,fpu=f,','reference(memory,state,fpu=f,stop_pc=c.get("context_return_pc"),')
marker=" return status,path,events,special['macl']";assert code.count(marker)==1
code=code.replace(marker," globals()['exception_reference_final']=tuple(special[k] for k in EXCEPTION_KEYS)\n"+marker)
namespace.update(EXCEPTION_KEYS=EXCEPTION_KEYS,CONTEXT_ROWS=set(ROWS)-set(old.ROWS))
old_dynamic=namespace['dynamic_access']
def context_access(text,r,fpscr,special):
 if text.startswith(('stc.l ','sts.l ')):
  reg=int(text.rsplit('r',1)[1]);return ((r[reg]-4)&0xffffffff,4)
 if text.startswith(('ldc.l ','lds.l ')):
  reg=int(text.split('@r',1)[1].split('+')[0]);return (r[reg],4)
 return old_dynamic(text,r,fpscr,special)
namespace['dynamic_access']=context_access
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_interrupt_context.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_interrupt_context_exception_observed;observed.argtypes=old.observed.argtypes+[ctypes.POINTER(ExceptionState)];observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff,ctypes.byref(native_exception))
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

Input=old.Input;seed_ram=old.seed_ram;sample=old.sample
VECTORS=(0x8c65e7a4,0x8c65e7d8);NORMAL,NESTED,EXIT=0x8c65e80c,0x8c65e958,0x8c65eaec
COUNTER=old.COUNTER;ASIC=old.ASIC;HANDLER=old.HANDLER;DESCRIPTOR=old.DESCRIPTOR;RECORD=old.RECORD;ROOT_CONTEXT=old.ROOT_CONTEXT

def prepare(ram,c):
 entry=c['entry'];context=Input(c|dict(entry=old.GENERIC,depth=0));m,s,f=old.prepare(ram,context);context['entry']=entry;c.update(context);s.pc=entry
 for key,value in dict(ssr=0x40000003,spc=0x8cfed000,sgr=s.r[15],expevt=0x180,intevt=0x360,tra=0,returned=0).items():c.setdefault(key,value)
 c.setdefault('context_return_pc',c['spc'])
 for i,key in enumerate(EXCEPTION_KEYS):setattr(native_exception,key,c.get(key,0))
 vbr=c.get('vbr',0x8ce50000)
 for event in (c['intevt'],c['expevt']):store(m,(vbr+0x200+((event-0x200)>>3))&0xffffffff,c.get('handler',COUNTER))
 store(m,0x8c65eae0,c.get('event',c['intevt']));store(m,0x8c65eae4,c.get('nesting',0));store(m,0x8c65eadc,c.get('interrupt_stack',0x8ce40000));store(m,0x8c65ead8,c.get('saved_stack',0x8cf00000))
 if entry in (NORMAL,NESTED):store(m,s.r[15],c.get('saved_r1',s.r[1]))
 for source,offsets in ((VECTORS[0],(0x100,0x400)),(VECTORS[1],(0x600,))):
  target=c.get('context_handler',NORMAL)
  store(m,source+48,target)
  for offset in offsets:
   origin=vbr+offset
   m[origin&0xffffff:(origin&0xffffff)+52]=m[source&0xffffff:(source&0xffffff)+52]
   aliases={}
   for pc,text in PER_BODY[source].items():
    if text.startswith(('mov.l 0x','mov.w 0x','mova 0x')):
     text=__import__('re').sub(r'0x([0-9a-f]+)',lambda hit:hex(origin+int(hit[1],16)-source),text)
    aliases[origin+pc-source]=text
   ROWS.update(aliases);namespace['CONTEXT_ROWS'].update(aliases);effects.CONNECTED.add(origin)
 return m,s,f
