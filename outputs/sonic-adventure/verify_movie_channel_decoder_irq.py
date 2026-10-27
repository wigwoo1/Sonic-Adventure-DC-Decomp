"""Original registration/vector/ASIC/VBlank/tick/RTE with explicit software IRQs."""
import ctypes,hashlib,itertools,json,sys
from pathlib import Path
import write_watch_channel_decoder as fast
import movie_active_input_fixtures as fixtures
from verify_sh4_exception_state import Extra,ExceptionState,U,UP
v=fast.old;P=v.P;ROOT=v.ROOT
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def carry(c,r):
 c.update(other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'],masks=r['masks'],transfer_registers=r['transfer_registers'],dma_operation=r['dma_operation'])
 c['startup_overrides']={0x5f6800+i*4:value for i,value in enumerate(r['startup_registers'])}
 for key,value in zip(v.EXCEPTION_KEYS,r['exception_state']):c[key]=value
def main():
 records=[];checks=0;comparisons=0;ram=v.seed_ram();configs=list(itertools.product((0,1,4,7),(0,0x80000000)))
 if '--smoke' in sys.argv:configs=configs[:1]
 for mode,context_option in configs:
  c=v.Input(dict(entry=v.CD_TICK,mode=mode,sr=0x40000003,countdown_value=2,game_mode=0,input_enabled=1,scheduler_counter=0,irq_status=0,masks=[0,0],vbr=0x8ce50000,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,other=[0x19283746^i*0x11111111 for i in range(8)]))
  v.bind_vectors(c['vbr']);m,s,f=fixtures.prepare(ram,c)
  # Declared existing coherent driver object; callback slots themselves are
  # populated only by the original registration API below.
  # IRQ code addresses the primary driver's static slot array directly.
  # Derive the same object's base; an arbitrary mapped object is insufficient.
  driver=(v.load(m,0x8c652508)-4-0x230)&0xffffffff
  assert driver+0x2a8==v.load(m,0x8c652504)
  at=driver&0xffffff;m[at:at+0x400]=bytes(0x400);fixtures.put(m,v.load(m,0x8c6413c4),driver)
  m[0x892840:0x893c00]=bytes(0x13c0);fixtures.literal(m,0x8c605374,0)
  for key in v.EXCEPTION_KEYS:c[key]=0
  for bank in range(2):
   for reg in range(16):f.banks[bank][reg]=((bank+1)*0x11111111^(reg+1)*0x01020304)&0xffffffff
  phases=[]
  def run(entry,args=None):
   nonlocal m,s,f,comparisons
   c['entry']=entry;s.pc=entry
   if args is not None:
    for j,value in enumerate(args):s.r[4+j]=value
   result,path,actual,s,f=fast.compare(m,s,f,c);comparisons+=1
   if result['status']:
    diagnostic=dict(entry=hex(entry),status=result['status'],stop_pc=result['stop_pc'],last_boundaries=result['boundaries'][-12:],phases=phases,irq_status=result['irq_status'],exception_state=result['exception_state'])
    (ROOT/'work/sonic-adventure/countdown_irq_failure.json').write_text(json.dumps(diagnostic,indent=2)+'\n')
   assert result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match'],(hex(entry),result['status'],result['stop_pc'])
   m=actual;carry(c,result);phases.append(dict(entry=hex(entry),status=result['status'],stop_pc=result['stop_pc'],boundaries=result['boundary_comparisons']))
   return result,path
  initialized,_=run(0x8c645de0,[context_option,0x8ce40000]);expected_context=0x8c65e958 if context_option else 0x8c65e80c
  assert v.load(m,c['vbr']+0x630)==expected_context;checks+=1
  installed,_=run(0x8c651520)
  assert v.load(m,c['vbr']+0x224)==0x8c652210;checks+=1
  slot_registered,_=run(0x8c64135a,[0x8c645b1e,0])
  assert v.load(m,driver+0x238)==0x8c645b1e and v.load(m,driver+0x23c)==0;checks+=2
  configured,_=run(0x8c051760,[2,0])
  head=v.load(m,0x8c893a80);assert head and v.load(m,head)==v.CD_TICK and v.load(m,head+4)==0;checks+=3
  assert v.load(m,v.load(m,0x8c051884))==2;checks+=1
  irq_results=[];lib=v.effects.LIB;enter=lib.sa_sh4_interrupt_enter;enter.argtypes=[ctypes.POINTER(ExceptionState),UP,UP,ctypes.POINTER(Extra),UP,U,U,ctypes.c_uint,ctypes.c_uint];enter.restype=ctypes.c_int
  for remaining in (1,0):
   before=v.effects.State.from_buffer_copy(s);before_f=bytes(f);hidden=list(c['other']);active_bank=bool(before.sr&0x40000000 and before.sr&0x20000000);physical=[None,None];physical[int(active_bank)]=list(before.r[:8]);physical[1-int(active_bank)]=hidden
   extra=Extra(c['mach'],(U*8)(*hidden));exception=ExceptionState(*[c[key] for key in v.EXCEPTION_KEYS]);sr=U(s.sr);pc=U(s.pc)
   accepted=enter(ctypes.byref(exception),ctypes.byref(sr),s.r,ctypes.byref(extra),ctypes.byref(pc),c['vbr'],0x320,6,0);assert accepted==0,('IRQ admission blocked',hex(s.sr));checks+=1
   assert list(s.r)==physical[1]+list(before.r[8:]) and list(extra.other)==physical[0];checks+=2
   assert sr.value==before.sr|0x70000000 and pc.value==c['vbr']+0x600 and bytes(f)==before_f;checks+=3
   expected=(before.sr,before.pc,before.r[15],c['expevt'],0x320,c['tra'],0);assert tuple(getattr(exception,key) for key in v.EXCEPTION_KEYS)==expected;checks+=1
   s.sr=sr.value;s.pc=pc.value;c.update(other=list(extra.other),mach=extra.mach,context_return_pc=before.pc,irq_status=8)
   for key in v.EXCEPTION_KEYS:c[key]=getattr(exception,key)
   r,path=run(s.pc)
   assert bytes(s)==bytes(before) and bytes(f)==before_f and r['integer_other']==hidden;checks+=3
   assert r['final_mach']==0x89abcdef and r['final_macl']==0x76543210 and r['final_gbr']==0x8ce01234;checks+=3
   assert v.load(m,v.load(m,0x8c051884))==remaining and r['irq_status']==0;checks+=2
   assert v.load(m,fixtures.ROOT+fixtures.fields(m)['counter'])==2-remaining;checks+=1
   assert r['exception_state']==expected[:-1]+(1,);checks+=1
   targets=[b['target'] for b in r['boundaries']];assert targets.count('0x8c645b1e')==targets.count('0x8c051810')==1 and '0x8c652210' in targets and '0x8c651bc0' in targets;checks+=4
   assert r['ack_writes']==[[12,0xa05f6900,8,4]];checks+=1
   irq_results.append(dict(counter_after=remaining,vector_entry=hex(c['vbr']+0x600),restored_interrupted_state=True,ack_writes=r['ack_writes'],exception_state=r['exception_state'],targets=targets))
  assert v.load(m,c['vbr']+0x600)==v.load(m,0x8c65e7d8);checks+=1
  records.append(dict(input_enabled=True,scheduler_counter_after=2,mode=mode,context_option=hex(context_option),context=hex(expected_context),original_native_comparisons=6,architectural_admissions=2,declared_driver_object=hex(driver),hardware_slot_registered_by_original64135a=True,callback_node=hex(head),initial_counter=2,final_counter=0,phases=phases,irqs=irq_results))
  print('COUNTDOWN IRQ',mode,hex(expected_context),'counter2->1->0/RTE',flush=True)
 if '--smoke' in sys.argv:print('Countdown IRQ smoke PASS',comparisons,checks);return
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_decoder_connection.json',v.effects.DLL,P/'source/sh4_exception_state.c',P/'source/sh4_exception_state.h',P/'analysis/countdown_hardware_path.json',P/'analysis/vblank_countdown_identity.json']
 report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,authored_irq_admission_cases=2*len(records),independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Eight explicit standalone lifecycles:real645de0 copied-vector initialization,651520 ASIC handler install,64135a original hardware-slot registration into declared coherent driver object,051760 original countdown callback registration,then two untimed softwareIRQ admissions. Actual copied vector/context,652210 handler,651bc0 acknowledgement,652554FPUwrapper,645b1e registered traversal,051810 tick and original input-active scheduler/service callees return throughRTE. Counter2->1->0 with full interrupted CPU/FPU/banked/special restoration. Six original/native ABI comparisons per lifecycle. Explicit input active/empty task pool/null interface/gameMode0/coherent driver-object fixture;actual668e60 executes perIRQ and schedulercounter0->1->2 checked. Not physical timing/frequency or automatic/uninterrupted frame-wait pumping; active input and larger handlers retain partial domains. No direct tick invocation or hand-filled callback slot substitutes for the IRQ chain.')
 (P/'analysis/movie_channel_decoder_irq_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COUNTDOWN IRQ PASS',len(records),comparisons,checks)
if __name__=='__main__':main()


