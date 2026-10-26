"""Original register/start/ASIC dispatch surrounding real authored G2 byte movement."""
import ctypes,hashlib,itertools,json
from pathlib import Path
import write_watch_interrupt_context_sound as fast
from verify_g2_transfer_host import Job,Bus,Read,Write
from verify_sh4_exception_state import Extra,ExceptionState,U,UP
v=fast.old;P=v.P;ROOT=v.ROOT

class Input(v.Input):
 sound_image=None
 def get(self,key,default=None):
  if key=='sound_image':return self.sound_image
  return super().get(key,default)

def carry(c,result):
 c.update(other=result['integer_other'],mach=result['final_mach'],macl=result['macl'],gbr=result['final_gbr'],vbr=result['vbr'])
 for key,value in zip(v.EXCEPTION_KEYS,result['exception_state']):c[key]=value

def main():
 lib=v.effects.LIB;capture=lib.sa_g2_transfer_capture;capture.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(Job)];capture.restype=ctypes.c_int
 execute=lib.sa_g2_transfer_execute;execute.argtypes=[ctypes.POINTER(Job),ctypes.POINTER(Bus),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];execute.restype=ctypes.c_int
 records=[];checks=0;ram=v.seed_ram()
 for channel,mode,length,context in [(ch,0,33376,cx) for ch in range(4) for cx in (v.NORMAL,v.NESTED)]+[(ch,mode,64,cx) for ch,mode,cx in itertools.product((0,3),(1,4),(v.NORMAL,v.NESTED))]:
  cpu=v.old.old.old;c=Input(entry=cpu.CLIENT_START,mode=mode,sdk_mode=1,current_record=0,channel=channel,length=length,source_pattern='masaru' if length==33376 else 'varying',masks=[0,0],irq_status=0,table=0xa05f7800,transfer_registers=[0]*48)
  m,s,f=cpu.prepare(ram,c)
  alias_input=v.Input(entry=0x8ce50600);v.prepare(ram,alias_input) # Bind metadata only; discard the fixture RAM.
  for key in v.EXCEPTION_KEYS:c[key]=0
  s.pc=0x8c645de0;s.r[4]=0x80000000 if context==v.NESTED else 0;s.r[5]=0x8ce40000;c['entry']=s.pc
  initialized,path,a,s,f=fast.compare(m,s,f,c)
  assert initialized['status']==0 and v.load(a,0x8ce50630)==context
  for offset,source in ((0x100,v.VECTORS[0]),(0x400,v.VECTORS[0]),(0x600,v.VECTORS[1])):
   assert a[0xe50000+offset:0xe50000+offset+52]==a[(source&0xffffff):(source&0xffffff)+52]
  carry(c,initialized);m=a
  m[0x892840:0x893c00]=bytes(0x13c0)
  event=0xa60+channel*32;d=v.DESCRIPTOR
  s.pc=0x8c64594c;s.r[4:8]=[event,v.HANDLER,0x60,d];c['entry']=s.pc
  registered,path,a,s,f=fast.compare(m,s,f,c)
  assert registered['status']==0 and s.r[0]==1;checks+=2
  assert registered['masks']==[1<<(15+channel),0];checks+=1
  assert v.load(a,0x8c892850)==v.HANDLER and v.load(a,0x8c892858)==d;checks+=2
  assert v.load(a,c.get('vbr',0x8ce50000)+0x22c)==v.ASIC;checks+=1
  carry(c,registered);c['masks']=registered['masks'];c['entry']=cpu.CLIENT_START;s.pc=cpu.CLIENT_START;s.r[4]=v.RECORD;s.r[5]=0
  started,path,a,s,f=fast.compare(a,s,f,c)
  assert started['status']==0 and v.load(a,d+44)==1 and v.load(a,d+48)==0;checks+=3
  assert v.load(a,v.RECORD+4)==2 and v.load(a,v.RECORD+24)==0;checks+=2
  assert v.load(a,v.ROOT_CONTEXT+0x190)==v.RECORD;checks+=1
  regs=(ctypes.c_uint32*8)(*started['transfer_registers'][channel*8:channel*8+8]);job=Job()
  assert capture(channel,regs,ctypes.byref(job))==0;checks+=1
  assert job.channel==channel and job.root_address==0x0c900000 and job.g2_address==0x00800000 and job.direction==0 and job.length&0x7fffffff==length;checks+=5
  source=bytes(a[0x900000:0x900000+length]);unchanged=bytes(a);sound=bytearray([0xa5])*0x200000;wanted=bytearray(sound);wanted[:length]=source
  calls=[];errors=[]
  @Read
  def read(ctx,address,buffer,size):
   calls.append(('read',address,size))
   offset=address-0x0c000000
   if size!=32 or offset<0 or offset+size>len(a):errors.append(('read',address,size));return -9
   for i,value in enumerate(a[offset:offset+size]):buffer[i]=value
   return 0
  @Write
  def write(ctx,address,buffer,size):
   calls.append(('write',address,size));offset=address-0x00800000
   if size!=32 or offset<0 or offset+size>len(sound):errors.append(('write',address,size));return -9
   sound[offset:offset+size]=bytes(buffer[:size]);return 0
  bus=Bus(None,read,write);completed=ctypes.c_uint32();emitted=ctypes.c_uint32()
  assert execute(ctypes.byref(job),ctypes.byref(bus),ctypes.byref(completed),ctypes.byref(emitted))==0 and not errors;checks+=2
  assert completed.value==length and emitted.value==event and sound==wanted and bytes(a)==unchanged;checks+=4
  expected_calls=[]
  for offset in range(0,length,32):expected_calls.extend((('read',job.root_address+offset,32),('write',job.g2_address+offset,32)))
  assert calls==expected_calls;checks+=1
  # Accept an explicitly supplied untimed software IRQ after the real byte copy.
  # Independent canonical-bank assertions cover the complete admission state.
  before=v.effects.State.from_buffer_copy(s);before_f=bytes(f);hidden=list(started['integer_other'])
  prior_bank=1 if before.sr&0x40000000 and before.sr&0x20000000 else 0
  physical=[None,None];physical[prior_bank]=list(before.r[:8]);physical[1-prior_bank]=hidden
  extra=Extra(started['final_mach'],(U*8)(*hidden));e=ExceptionState(*[c[k] for k in v.EXCEPTION_KEYS]);sr=U(s.sr);pc=U(s.pc)
  enter=lib.sa_sh4_interrupt_enter;enter.argtypes=[ctypes.POINTER(ExceptionState),UP,UP,ctypes.POINTER(Extra),UP,U,U,ctypes.c_uint,ctypes.c_uint];enter.restype=ctypes.c_int
  assert enter(ctypes.byref(e),ctypes.byref(sr),s.r,ctypes.byref(extra),ctypes.byref(pc),c.get('vbr',0x8ce50000),0x360,6,0)==0
  assert list(s.r)==physical[1]+list(before.r[8:]) and list(extra.other)==physical[0]
  assert sr.value==before.sr|0x70000000 and pc.value==0x8ce50600 and bytes(f)==before_f
  assert tuple(getattr(e,k) for k in v.EXCEPTION_KEYS)==(before.sr,before.pc,before.r[15],c['expevt'],0x360,c['tra'],0)
  s.sr=sr.value;s.pc=pc.value;c['other']=list(extra.other)
  c['mach']=extra.mach;c['macl']=started['macl'];c['gbr']=started['final_gbr'];c['vbr']=started['vbr']
  for key in v.EXCEPTION_KEYS:c[key]=getattr(e,key)
  c['context_return_pc']=before.pc
  c.sound_image=bytes(sound);c['sound_image_sha256']=hashlib.sha256(sound).hexdigest();c['irq_status']=1<<(15+channel);c['transfer_registers']=started['transfer_registers'];c['entry']=s.pc
  finished,path,a,s,f=fast.compare(a,s,f,c)
  assert bytes(s)==bytes(before) and bytes(f)==before_f and finished['integer_other']==hidden
  assert finished['final_mach']==started['final_mach'] and finished['macl']==started['macl'] and finished['final_gbr']==started['final_gbr']
  assert finished['exception_state']==tuple(getattr(e,k) for k in v.EXCEPTION_KEYS[:-1])+(1,)
  assert finished['status']==0 and finished['irq_status']==0;checks+=2
  assert [b['arguments'][0] for b in finished['boundaries'] if int(b['target'],16)==v.old.GENERIC]==[event];checks+=1
  assert any(int(b['target'],16)==v.HANDLER for b in finished['boundaries']);checks+=1
  assert v.load(a,d+44)==0 and v.load(a,d+48)==1;checks+=2
  assert v.load(a,v.RECORD+4)==1 and v.load(a,v.RECORD+24)==1 and v.load(a,v.ROOT_CONTEXT+0x190)==0;checks+=3
  assert finished['sound_ram_sha256']==c['sound_image_sha256'] and not any(finished['sound_state']);checks+=2
  for r in (initialized,registered,started,finished):assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  checks+=12 # Vector copying, admission state and complete interrupted-state contracts above.
  records.append(dict(input=dict(c),original_native_comparisons=4,context=hex(context),architectural_entry_verified=True,complete_interrupted_state_restored=True,copied_bytes=completed.value,event=hex(emitted.value),source_preserved=True,complete_sound_ram_match=True,sound_ram_sha256=c['sound_image_sha256'],original_callback_completion=True,explicit_software_completion_input=True,boundaries=[r['boundary_comparisons'] for r in (initialized,registered,started,finished)]))
  print('Lifecycle',channel,mode,length,completed.value,flush=True)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_interrupt_context_connection.json',P/'source/g2_transfer_host.c',P/'source/g2_transfer_host.h',cpu.DRIVER]
 report=dict(lifecycles_passed=len(records),original_native_comparisons=4*len(records),authored_irq_admission_cases=len(records),authored_host_transfer_cases=len(records),independent_checks=checks,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original645de0 initialization copies actual vectors and selects normal or nested context; original registration and start, real authored G2 copy, explicit untimed architecture IRQ admission, actual copied vector, full original context, ASIC, generic, registered handler, SDK callback and RTE. Four separate original/native ABI comparisons per lifecycle. Entire interrupted CPU/FPU/banked/special state restored independently, whole source and2MiB sound data preserved. Admission and transfer are authored support with zero retail-code credit. No physical timing, automatic pump, uninterrupted early caller, guest checkpoint, ARM execution or audio is claimed.')
 (P/'analysis/movie_interrupt_context_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Context lifecycles',len(records),'original/native',4*len(records),'checks',checks)
if __name__=='__main__':main()
