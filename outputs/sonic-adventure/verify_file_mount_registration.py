"""Original mount through real callback construction, stopping before device init."""
import ctypes,hashlib,itertools,json,struct
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_interrupt_constructor import IO,Read,Write
PROJECT=ROOT/'outputs/sonic-adventure';SIZE=16*1024*1024;U=ctypes.c_uint32
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ROOT/'work/sonic-adventure/build/file_mount_registration.dll';lib=ctypes.CDLL(str(dll))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
mount=lib.sa_file_mount;mount.argtypes=abi
constructor=lib.sa_interrupt_callback_create;constructor.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
native_helpers={0x8c64135a:lib.sa_interrupt_handler_primary,0x8c6412c2:lib.sa_interrupt_handler_set,0x8c658c20:lib.sa_mount_driver_enable}
for fn in native_helpers.values():fn.argtypes=abi
assert binary[0x648c20:0x648c28]==bytes.fromhex('0b0001e00b0001e0')
def literal(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]
globals_=[literal(0x8c5ffe30),literal(0x8c5ffe40)]
callbacks=[literal(0x8c5ffe3c),literal(0x8c5ffe44)]
pool=0x8c892840;events=(0x900,0xa40);STOP=0x8c600d82
cases=[];service_entries=0
for alias,mask,bank,cached,full in itertools.product((0x8c000000,0xac000000),(0,15),(0,0x200000),(False,True),(False,True)):
    ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary
    def put(a,v):struct.pack_into('<I',ram,a&0xffffff,v)
    put(0x8c673414,alias+0x7a0000);put(alias+0x7a0014,alias+0x790000)
    put(0x8c67c004,0x8ca00100)
    for i in range(256):put(pool+16*i+12,0x80000000 if full or i==0 else 0)
    for index,(event,callback,global_word) in enumerate(zip(events,callbacks,globals_)):
        put(global_word,index+7 if cached else 0)
        if cached:
            slot=index+7;node=pool+16*slot
            struct.pack_into('<4I',ram,node&0xffffff,callback,0,0x1234,0x80000050)
            put(0x8c893840+(event>>4)*4,node)
    native_ram=bytearray(ram);view=(ctypes.c_uint8*SIZE).from_buffer(native_ram)
    hardware={0xa05f6920:0x10203040,0xa05f6924:0x50607080};native_hardware=dict(hardware)
    logs=[[],[]];errors=[];installs=[[],[]]
    @Read
    def read(ctx,address,value):
        if address not in native_hardware:errors.append(hex(address));return -3
        value[0]=native_hardware[address];return 0
    @Write
    def write(ctx,address,value):
        if address not in native_hardware:errors.append(hex(address));return -3
        native_hardware[address]=value;return 0
    io=IO(0x8ca08000,0,None,read,write)
    initial=State();initial.r[:]=[0x22340000+i for i in range(16)];initial.r[15]=alias+0x7ff000
    initial.sr=0x70000003|(mask<<4);initial.pr=0x8c74913c;initial.pc=0x8c5ffd28
    expected=State.from_buffer_copy(initial);captured=State()
    f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
    r=(U*16)(*initial.r);sr=U(initial.sr);pr=U(initial.pr);fpul=U();macl=U()
    def trace(pc,word,regs,status,ret,fp):
        if pc==0x8c64594c:logs[0].append((tuple(regs),status,ret,hashlib.sha256(ram).hexdigest()))
        if pc==0x8c64135a:installs[0].append((pc,tuple(regs[4:8]),status))
    @Call
    def call(ctx,target,m,fp,status,regs,ret):
        try:
            if target==0x8c64594c:
                logs[1].append((tuple(regs[i] for i in range(16)),status[0],ret[0],hashlib.sha256(native_ram).hexdigest()))
                return constructor(m,fp,status,regs,ret,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
            if target==STOP:
                captured.r[:]=[regs[i] for i in range(16)];captured.sr,captured.pr,captured.pc=status[0],ret[0],target
                return -100 # Deliberate, observed boundary; not a completed mount.
            if target==0x8c64135a:installs[1].append((target,tuple(regs[i] for i in range(4,8)),status[0]))
            assert target in native_helpers,hex(target)
            return native_helpers[target](m,fp,status,regs,ret,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        except BaseException as exc:errors.append(repr(exc));return -9
    reference(ram,expected,fpu=f,mmio=hardware,vbr=io.vbr,trace=trace,stop_pc=STOP)
    assert mount(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==-100
    assert not errors,errors
    assert ram==native_ram and bytes(expected)==bytes(captured) and bytes(f)==bytes(nf)
    assert logs[0]==logs[1] and installs[0]==installs[1] and hardware==native_hardware
    handles=[struct.unpack_from('<I',native_ram,a&0xffffff)[0] for a in globals_]
    assert handles==([7,8] if cached else [0,0] if full else [1,2])
    if not cached and not full:
        assert len(installs[1])==1
        assert struct.unpack_from('<II',native_ram,0xa00338)==(0x8c645b1e,0)
        for event,callback,handle in zip(events,callbacks,handles):
            node=pool+handle*16
            assert struct.unpack_from('<I',native_ram,node&0xffffff)[0]==callback
            assert struct.unpack_from('<I',native_ram,(0x8c893840+(event>>4)*4)&0xffffff)[0]==node
        assert native_hardware[0xa05f6920]==hardware[0xa05f6920] and native_hardware[0xa05f6920]&0x4000
    assert len(logs[1])==(0 if cached else 2)
    cases.append({'alias':hex(alias),'mask':mask,'bank':bank,'cached':cached,'pool_full':full,'handles':handles,'constructors':len(logs[1])})
    service_entries+=len(logs[1])
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
report={'cases_passed':len(cases),'constructor_entries_compared':service_entries,'native_primary_installation':True,'cases':cases,
 'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in ('sa_file_mount.c','sa_interrupt_callback_create.c','interrupt_io.h','sa_interrupt_handler_primary.c','sa_interrupt_handler_set.c','sa_mount_driver_enable.c')},
 'dll_sha256':sha(dll),'binary_sha256':hashlib.sha256(binary).hexdigest(),'verifier_sha256':sha(Path(__file__)),
 'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'scope':'Original mount -> native interrupt constructor with full retail RAM/GPR/SR/PR/PC/FPU and MMIO comparison at the reserved-handle boundary; independent handles/callback nodes/heads/cache/pool-exhaustion checks.',
 'limitations':'Deliberate stop before reserved-handle/device initialization. Primary handler installation now executes natively; secondary installation is not exercised by these two mount events. VBR/mask registers and driver storage supplied. No completed mount or physical interrupts claimed.'}
(PROJECT/'analysis/file_mount_registration_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'mount registration cases and',service_entries,'constructor entries passed; stopped before device initialization')
