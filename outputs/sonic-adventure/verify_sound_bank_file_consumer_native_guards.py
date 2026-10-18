"""Missing platform services must reject before touching guest execution state."""
import ctypes,hashlib,json
from pathlib import Path
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_request_abort_8c6013d2 import Read as TimerRead,Observe,Dispatch
from verify_interrupt_constructor import IO,Read,Write
project=ROOT/'outputs/sonic-adventure';U=ctypes.c_uint32;P=ctypes.POINTER(U)
BP=ctypes.POINTER(ctypes.c_uint8)
dll=ROOT/'work/sonic-adventure/build/native_sound_bank_file_consumer.dll'
fn=ctypes.CDLL(str(dll)).sa_sound_bank_file_consumer_native
fn.argtypes=[BP,ctypes.POINTER(State),ctypes.POINTER(Fpu),P,P,ctypes.c_void_p,U,
             ctypes.c_void_p,TimerRead,ctypes.POINTER(IO),Observe,Dispatch,U]
ram=(ctypes.c_uint8*(16*1024*1024))();state=State();state.pc=0x8c601618
fpu=Fpu();fpul=U(123);macl=U(456);queue=ctypes.create_string_buffer(1)
callback_calls=[]
def unexpected(*args):callback_calls.append(True);return -99
timer=TimerRead(unexpected);read=Read(unexpected);write=Write(unexpected)
boundary=Dispatch(unexpected);interrupt=IO(0,0,None,read,write)
args=[ram,ctypes.byref(state),ctypes.byref(fpu),ctypes.byref(fpul),ctypes.byref(macl),
      queue,0x8c740000,None,timer,ctypes.byref(interrupt),Observe(),boundary,100]
def snapshot():
    return (hashlib.sha256(ram).digest(),bytes(state),bytes(fpu),fpul.value,macl.value,
            bytes(queue),bytes(interrupt),tuple(callback_calls))
checks=[]
for index,value in [(0,None),(1,None),(2,None),(3,None),(4,None),(5,None),
                    (8,TimerRead()),(9,None),(11,Dispatch()),
                    (9,ctypes.byref(IO(0,0,None,Read(),write))),
                    (9,ctypes.byref(IO(0,0,None,read,Write())))]:
    supplied=list(args);supplied[index]=value;before=snapshot()
    assert fn(*supplied)==-2 and snapshot()==before
    checks.append('missing_service_'+str(len(checks)))
state.pc=0xdeadbeef;before=snapshot()
assert fn(*args)==-8 and snapshot()==before
checks.append('unsupported_entry')
state.pc=0x8c601618;supplied=list(args);supplied[-1]=0;before=snapshot()
assert fn(*supplied)==-3 and snapshot()==before
checks.append('zero_dispatch_budget')
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
report={'checks_passed':len(checks),'checks':checks,'dll_sha256':sha(dll),
 'verifier_sha256':sha(Path(__file__)),
 'native_evidence_sha256':sha(project/'analysis/sound_bank_file_consumer_native_verification.json'),
 'scope':'Eleven missing-service cases, unsupported root entry and zero dispatch budget reject with unchanged RAM/CPU/FPU/special registers/queue/interrupt state.'}
(project/'analysis/sound_bank_file_consumer_native_guards_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(checks),'native consumer service/entry/budget guards passed')
