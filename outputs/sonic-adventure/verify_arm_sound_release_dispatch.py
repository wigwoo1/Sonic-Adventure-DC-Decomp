"""Active release-mode computed dispatch and original-table arithmetic."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
cases=[];budgets=[]
def run(driver,slot,mode,parameter,release,active,flags,budget=1000,raw=False):
 image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray(SIZE);memory[:len(image)]=image
 base=struct.unpack_from('<I',memory,0x7804+driver*8)[0];record=base+slot*0x58
 memory[record]=active;memory[record+0x3c]=release
 struct.pack_into('<I',memory,record+0x38,0x11223344);struct.pack_into('<I',memory,record+0x34,0x55667788)
 initial=State();initial.r[:]=[0x12340000+i for i in range(16)];initial.r[0]=slot;initial.r[1]=mode;initial.r[2]=parameter;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0x81d0+driver*4;initial.cpsr=flags|0x13
 if raw:initial.r[15]=0x8204+driver*4;initial.r[12]=record
 struct.pack_into('<I',memory,initial.r[13],STOP)
 native=bytearray(memory);hardware=bytearray(MMIO_SIZE);events=[]
 machine=cpu(memory,hardware,initial,events);machine.emu_start(initial.r[15],STOP,count=budget);expected=state_of(machine)
 actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
 @Read
 def read(user,address,size,out):errors.append(('unexpected read',address));return -9
 @Write
 def write(user,address,value,size):errors.append(('unexpected write',address));return -9
 rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,budget)
 completed=expected.r[15]==STOP
 assert rc==(0 if completed else -3) and not errors
 assert bytes(actual)==bytes(expected) and native==machine.mem_read(0,SIZE) and not events,'original CPU/RAM/no MMIO'
 out=lambda a:struct.unpack_from('<I',native,a)[0]
 if completed:
  if raw:
   wanted=(release,(release+parameter)&0xffffffff,((release*parameter)&0xffffffff)>>8,parameter)[mode]
   assert out(record+0x38)==wanted and out(record+0x34)==0x55667788,'direct table arithmetic preserves parameter'
  elif mode<=2:
   assert native[record+0x23]==mode+1 and out(record+0x34)==parameter,'mode/parameter saved'
   wanted=((release+parameter)&0xffffffff,((release*parameter)&0xffffffff)>>8,parameter)[mode]
   assert out(record+0x38)==(wanted if active else 0x11223344),'independent active release arithmetic'
  else:
   assert native[record+1]&0x20 and out(record+0x34)==0x55667788 and out(record+0x38)==0x11223344,'invalid mode status and preserved fields'
 return {'driver':driver,'slot':slot,'mode':mode,'parameter':parameter,'release':release,'active':active,'flags':flags,'budget':budget,'completed':completed,'raw_table_entry':raw}
for driver,slot,mode,parameter,release,active,flags in itertools.product(range(2),(0,7),(0,1,2,3,0xffffffff),(0,1,0x100,0x80000000,0xffffffff),(0,255),(0,1),(0,0xf0000000)):
 cases.append(run(driver,slot,mode,parameter,release,active,flags))
for driver,mode,budget in itertools.product(range(2),(0,1,2,3),(1,4,8,12,16)):budgets.append(run(driver,7,mode,0xffffffff,255,1,0xf0000000,budget))
# The internal helper also dispatches mode zero when called directly. Its target
# is a simple return-value load; its callers must initialize r12 explicitly.
direct_cases=[]
for driver,slot,mode,parameter,release,flags in itertools.product(range(2),(0,7),range(4),(0,1,0x100,0x80000000,0xffffffff),(0,255),(0,0xf0000000)):
 direct_cases.append(run(driver,slot,mode,parameter,release,1,flags,raw=True))
direct=[]
for driver in range(2):
 image=(ROOT/payloads[driver]['payload_path']).read_bytes();pointer=struct.unpack_from('<I',image,0x8258+driver*4)[0]
 targets=struct.unpack_from('<4I',image,pointer);assert targets==tuple(a+driver*4 for a in (0x823c,0x8220,0x822c,0x8240))
 direct.append({'driver':driver,'pointer':pointer,'targets':targets,'bytes':image[pointer:pointer+16].hex()})
import re
paths=[Path(__file__),support,dll,P/'build_arm_sound_boot_native.ps1',P/'source/arm_sound_memory.h']+[P/'source'/name for name in re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_boot_native.ps1').read_text())]
report={'cases_passed':len(cases),'direct_table_cases_passed':len(direct_cases),'direct_table_cases':direct_cases,'budget_cases_passed':len(budgets),'cases':cases,'budgets':budgets,'pointer_tables':direct,'sha256':{str(p):sha(p) for p in paths},'scope':'Original active/inactive release-mode helper with boundary parameters and wrapped 32-bit multiply/add arithmetic; exact CPU/CPSR/RAM, independent fields/status, no MMIO or ARM callback. Original four-entry pointer-table bytes/targets verified structurally; all four internal table targets additionally execute with explicitly initialized caller registers. No hardware timing/audio or full-game proof.'}
(P/'analysis/arm_sound_release_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'release dispatch cases,',len(direct_cases),'direct table cases and',len(budgets),'budgets passed')
