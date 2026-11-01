"""Original initializer and clock-reader opcodes using the native TMU0 core."""
import csv,ctypes,hashlib,inspect,json,random
from pathlib import Path
import tmu0_clock_native as n
from tmu0_clock_reference import Clock,ADDRESSES
import verify_startup as cpu
P=Path(__file__).resolve().parent;R=P.parents[1]
INIT,CLOCK=0x8c6025e8,0x8c60260e

# Extend only byte/word architectural MMIO routing in the retained test decoder.
# Original instructions, delay slots, guest branches and register results remain
# decoded from retail bytes; no game-helper hook is installed.
code=inspect.getsource(cpu.reference)
marker='    def read16(addr):';assert code.count(marker)==1
code=code.replace(marker,'''    def read8(addr):
        return mmio.read(addr,1) if mmio is not None and addr in mmio else ram[index(addr)]
    def write8(addr,value):
        if mmio is not None and addr in mmio:mmio.write(addr,value&255,1)
        else:ram[index(addr)]=value&255
'''+marker)
for before,after in (("return mmio[addr] & 0xffff","return mmio.read(addr,2) & 0xffff"),("mmio[addr]=value & 0xffff","mmio.write(addr,value&0xffff,2)"),("return mmio[addr]\n","return mmio.read(addr,4)\n"),("mmio[addr]=value\n","mmio.write(addr,value,4)\n"),("signed(ram[index(regs[m])], 8)","signed(read8(regs[m]),8)"),("ram[index(regs[n])] = regs[m] & 255","write8(regs[n],regs[m])"),("struct.pack_into('<H', ram, index(regs[n]), regs[m] & 65535)","write16(regs[n],regs[m])")):
 expected_count=2 if before=='signed(ram[index(regs[m])], 8)' else 1
 assert code.count(before)==expected_count,before;code=code.replace(before,after)
namespace=dict(cpu.__dict__);exec(compile(code,'TMU_byte_word_original_decoder','exec'),namespace);reference=namespace['reference']

class Fpu(ctypes.Structure):_fields_=[('banks',(n.U*16)*2),('fpscr',n.U)]
class Extra(ctypes.Structure):_fields_=[('mach',n.U),('other',n.U*8)]

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/tmu0_original_helpers_verification.json';assert not target.exists()
 original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=original.read_bytes()
 listings=[P/'analysis/movie-startup-platform/8c6025e8_instructions.tsv',P/'analysis/boot-wait/8c60260e_instructions.tsv']
 total_rows=0
 for listing in listings:
  for row in csv.DictReader(listing.open(),delimiter='\t'):
   a=int(row['address'],16);data=bytes.fromhex(row['bytes']);assert binary[a-0x8c010000:a-0x8c010000+len(data)]==data;total_rows+=1
 game_dll=R/'work/sonic-adventure/build/renderer_input_tick_mmio.dll';game=ctypes.CDLL(str(game_dll));UP=ctypes.POINTER(n.U)
 funcs={INIT:game.sa_movie_startup_8c6025e8,CLOCK:game.sa_boot_wait_8c60260e}
 for fn in funcs.values():fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),UP,UP,UP,UP,UP,ctypes.c_void_p,ctypes.c_void_p,UP,UP,ctypes.POINTER(n.Bus),ctypes.POINTER(Extra),UP];fn.restype=ctypes.c_int
 rng=random.Random(0x60260e);records=[];checks=0
 for bank,t in ((0,0),(0,1),(1,0),(1,1)):
  memory=bytearray(0x1000000);memory[0x10000:0x10000+len(binary)]=binary
  native_clock=n.initialized();wanted_clock=Clock();reference_events=[];native_events=[]
  class MMIO:
   def __contains__(self,a):return a in ADDRESSES
   def read(self,a,width):
    status,value=wanted_clock.read(a,width);assert status==0;reference_events.append(('read',a,value,width));return value
   def write(self,a,value,width):
    status=wanted_clock.write(a,value,width);assert status==0;reference_events.append(('write',a,value&((1<<(8*width))-1),width))
  mmio=MMIO()
  @n.READ
  def read(ctx,a,width,out):
   status=n.LIB.sa_tmu0_read(ctypes.byref(native_clock),a,width,out)
   if not status:native_events.append(('read',a,out[0],width))
   return status
  @n.WRITE
  def write(ctx,a,value,width):
   status=n.LIB.sa_tmu0_write(ctypes.byref(native_clock),a,value,width)
   if not status:native_events.append(('write',a,value&((1<<(8*width))-1),width))
   return status
  @n.CACHE
  def cache(ctx,a,op):return -2
  bus=n.Bus(None,read,write,cache)
  def phase(entry,elapsed):
   nonlocal checks
   assert n.LIB.sa_tmu0_advance(ctypes.byref(native_clock),elapsed)==wanted_clock.advance(elapsed)==0
   before=bytes(memory);expected=bytearray(memory);actual=bytearray(memory)
   s=cpu.State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[15]=0x8cfff000;s.sr=0x400000a0|t;s.pr=0x8cfed000;s.pc=entry
   wanted=cpu.State.from_buffer_copy(s);f=Fpu();f.banks[:]=[(n.U*16)(*[rng.getrandbits(32) for _ in range(16)]) for _ in range(2)];f.fpscr=bank<<21;f_before=bytes(f)
   fpul=n.U(0xaabbccdd);macl=n.U(0x12345678);gbr=n.U(0x8ce00000);vbr=n.U(0x8ce50000);extra=Extra();extra.mach=0x89abcdef;extra.other[:]=range(8);extra_before=bytes(extra)
   trace=[];steps=reference(expected,wanted,mmio=mmio,special={'macl':macl.value},trace=lambda pc,*a:trace.append(pc))
   sr=n.U(s.sr);pr=n.U(s.pr);stop=n.U();regs=(n.U*16)(*s.r);view=(ctypes.c_uint8*len(actual)).from_buffer(actual)
   rc=funcs[entry](view,ctypes.byref(f),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,None,ctypes.byref(stop),ctypes.byref(gbr),ctypes.byref(bus),ctypes.byref(extra),ctypes.byref(vbr))
   assert rc==0 and wanted.pc==wanted.pr==pr.value==0x8cfed000;checks+=2
   assert list(regs)==list(wanted.r) and sr.value==wanted.sr and actual==expected==before;checks+=3
   assert bytes(f)==f_before and bytes(extra)==extra_before and (fpul.value,macl.value,gbr.value,vbr.value)==(0xaabbccdd,0x12345678,0x8ce00000,0x8ce50000);checks+=3
   assert n.snapshot(native_clock)==wanted_clock.snapshot() and native_events==reference_events;checks+=2
   if entry==INIT:
    assert (native_clock.tcor,native_clock.tcnt,native_clock.tcr,native_clock.tstr,native_clock.tocr)==(0xffffffff,0xffffffff,2,1,0);checks+=1
   else:
    assert regs[0]==(0xffffffff-native_clock.tcnt)&0xffffffff;checks+=1
   records.append(dict(entry=hex(entry),elapsed_PCLK_cycles=elapsed,FPU_bank=bank,initial_T=t,steps=steps,last_native_instruction=hex(stop.value),guest_return_value=hex(regs[0]),clock_state=list(n.snapshot(native_clock)),matched_all_RAM_registers_FPU_and_ordered_peripheral_accesses=True))
   return regs[0]
  phase(INIT,0)
  for elapsed in (0,1,62,1,64,640,10000,1000000):
   value=phase(CLOCK,elapsed);assert phase(CLOCK,0)==value;checks+=1
 assert len(records)==68 and total_rows==24
 files=[Path(__file__),P/'verify_startup.py',P/'tmu0_clock_reference.py',P/'tmu0_clock_native.py',P/'source/tmu0_clock.c',P/'source/tmu0_clock.h',P/'source/sa_movie_startup.c',P/'source/sa_boot_wait.c',*listings,original,n.DLL,game_dll,P/'analysis/tmu0_clock_verification.json']
 result=dict(status='original_timer_initializer_and_clock_reader_verified_with_native_elapsed_cycle_core',original_native_comparisons=len(records),normal_returns=len(records),checks=checks,original_rows_byte_checked=total_rows,initialization_from_original_instructions=True,original_native_game_DLL_unchanged=True,finite_clock_samples_used=False,reads_advance_time=False,game_callee_outputs_supplied=0,allocator_outputs_supplied=0,private_execution_frames_supplied=0,new_original_routine_credit=0,whole_frame_integration_qualified=False,automatic_VBlank=False,physical_clock_rate_qualified=False,overall_completion_percent=10,scope='Direct existing exported C leaf-entry ABI versus independently decoded original instructions, including delay slots. Four FPU-bank/T combinations; original 6025e8 configures the native core, explicit elapsed PCLK advances produce 60260e results, repeated reads are stable. Compare all 16MiB RAM, every GPR/SR/PR, unchanged FPU/FPUL/MACL/GBR/VBR/hidden banks and exact ordered peripheral accesses. No original callee result is supplied. The native leaf ABI returns host status and last executed instruction; guest normal return is checked against original PR. This does not yet drive the full scene or schedule VBlank.',phases=records,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('VERIFIED original TMU helpers',len(records),'comparisons;',checks,'checks',flush=True)

if __name__=='__main__':main()
