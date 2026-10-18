"""Host queue operations through original game status formatter and tail."""
import ctypes,hashlib,itertools,json,shlex,struct
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
PROJECT=ROOT/'outputs/sonic-adventure';SIZE=16*1024*1024;U=ctypes.c_uint32;BP=ctypes.POINTER(ctypes.c_uint8)
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,BP,U)
dll=ROOT/'work/sonic-adventure/build/native_disc_queue.dll';lib=ctypes.CDLL(str(dll))
lib.sa_disc_queue_size.restype=U
init=lib.sa_disc_queue_init;init.argtypes=[ctypes.c_void_p,Read,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,U,U]
service=lib.sa_disc_service;service.argtypes=[ctypes.c_void_p,BP,U,U,U,ctypes.POINTER(U)]
abi=[BP,ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
formatter=lib.sa_file_device_status;formatter.argtypes=abi
tail=lib.sa_file_bios_status;tail.argtypes=abi
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
manifest=json.loads((PROJECT/'disc_inventory.json').read_text())
track=next(t for t in reversed([shlex.split(s) for s in manifest['gdi'].splitlines()[1:] if s.strip()]) if t[2]=='4')
path=Path(manifest['source'])/track[4];base,stride,offset=int(track[1]),int(track[3]),int(track[5])
cases=0;errors=[];guards=0
with path.open('rb') as stream:
    for alias,scenario in itertools.product((0x0c000000,0x8c000000,0xac000000),('pending','partial','complete','cancel','partial_cancel','io_error','missing')):
        calls=[]
        @Read
        def read(user,pos,out,size):
            try:
                if scenario=='io_error' and len(calls)==1:return -17
                calls.append(pos);stream.seek(pos);data=stream.read(size)
                if len(data)!=size:return -8
                ctypes.memmove(out,data,size);return 0
            except BaseException as exc:errors.append(repr(exc));return -8
        ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary
        struct.pack_into('<I',ram,0xbc,0x8cba0000);struct.pack_into('<4I',ram,0x100,(base+16)+150,3,alias+0x800000,0)
        ram[0x800000:0x801800]=b'\xaa'*6144
        view=(ctypes.c_uint8*SIZE).from_buffer(ram);q=ctypes.create_string_buffer(lib.sa_disc_queue_size())
        assert init(q,read,None,offset,(path.stat().st_size-offset)//stride,base,stride)==0
        result=U();assert service(q,view,0,16,alias+0x100,ctypes.byref(result))==0 and result.value==1
        handle=result.value;steps={'pending':0,'partial':1,'complete':3,'cancel':0,'partial_cancel':1,'io_error':2,'missing':0}[scenario]
        for _ in range(steps):assert service(q,view,2,0,0,ctypes.byref(result))==0
        completed=1 if scenario in ('partial','partial_cancel','io_error') else 3 if scenario=='complete' else 0
        if 'cancel' in scenario:assert service(q,view,8,handle,0,ctypes.byref(result))==0 and result.value==0
        if scenario=='missing':handle=99
        transferred=completed*2048
        stream.seek(offset+16*stride+16);first=stream.read(2048)
        if completed:assert ram[0x800000:0x800800]==first
        if completed<3:assert ram[0x800000+transferred:0x801800]==b'\xaa'*(6144-transferred)
        expected_ram=bytearray(ram)
        raw=0 if scenario=='missing' else 2 if scenario=='complete' else 0xffffffff if scenario in ('cancel','partial_cancel','io_error') else 1
        extra=(8 if scenario=='io_error' else 0,0xffffffef if scenario=='io_error' else 0,transferred,0)
        expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
        expected.r[4]=handle;expected.r[5]=alias+0x900000;expected.r[15]=alias+0xf00000
        expected.sr=0x70000063;expected.pr=0x8c000100;expected.pc=0x8c6450ea
        r=(U*16)(*expected.r);sr=U(expected.sr);pr=U(expected.pr);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
        def hook(m,regs,status):
            assert regs[6]==0 and regs[7]==1
            struct.pack_into('<4I',m,regs[5]&0xffffff,*extra);regs[0]=raw;return status
        reference(expected_ram,expected,fpu=f,hooks={0x8cba0000:hook})
        callback_errors=[]
        @Call
        def call(ctx,target,m,fp,status,regs,ret):
            try:
                if target==0x8c10d94c:return tail(m,fp,status,regs,ret,ctypes.byref(fpul),ctypes.byref(macl),None,call)
                assert target==0x8cba0000 and regs[6]==0 and regs[7]==1
                value=U();rc=service(q,m,1,regs[4],regs[5],ctypes.byref(value));regs[0]=value.value
                return rc
            except BaseException as exc:callback_errors.append(repr(exc));return -9
        assert formatter(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        assert not callback_errors and not errors,(callback_errors,errors)
        assert ram==expected_ram and list(r)==list(expected.r) and sr.value==expected.sr and pr.value==expected.pr and bytes(nf)==bytes(f),scenario
        assert r[0]==(0 if scenario=='missing' else 1 if scenario=='complete' else 5 if scenario in ('cancel','partial_cancel','io_error') else 4)
        if scenario in ('complete','cancel','partial_cancel','io_error'):
            assert service(q,view,1,handle,alias+0x900100,ctypes.byref(result))==0 and result.value==0
        before=bytes(q);before_ram=bytes(ram);result=U(99)
        assert service(q,view,1,handle,alias+0xffffff,ctypes.byref(result))==-3
        assert bytes(q)==before and bytes(ram)==before_ram and result.value==99;guards+=1
        assert service(q,view,99,0,0,ctypes.byref(result))==-8 and result.value==99;guards+=1
        cases+=1
guest_read=lib.sa_disc_read_guest_sectors;guest_read.argtypes=[ctypes.c_void_p,BP,U,U,U]
@Read
def failing_guest_read(user,pos,out,size):
    if pos>=2048:return -17
    ctypes.memset(out,0x55,size);return 0
q=ctypes.create_string_buffer(lib.sa_disc_queue_size())
assert init(q,failing_guest_read,None,0,8,0,2048)==0
memory=bytearray(SIZE);view=(ctypes.c_uint8*SIZE).from_buffer(memory)
guest_cases=0
for _ in range(12):
    memory[0x1000:0x2000]=b'\xaa'*4096
    assert guest_read(q,view,0,2,0xac001000)==-17
    assert memory[0x1000:0x1800]==b'U'*2048 and memory[0x1800:0x2000]==b'\xaa'*2048
    guest_cases+=1 # More than eight failures also proves terminal slots released.
for address in (0,0x8cffff00):
    before=bytes(q);before_ram=bytes(memory)
    assert guest_read(q,view,0,1,address)==-3
    assert bytes(q)==before and bytes(memory)==before_ram;guest_cases+=1
assert guest_read(q,view,0,0,0x8c001000)==0;guest_cases+=1
initialization_cases=0
value=U(99);before=bytes(q)
assert service(q,view,3,0,0,ctypes.byref(value))==0 and value.value==0 and bytes(q)==before;initialization_cases+=1
struct.pack_into('<4I',memory,0x100,150,1,0x8c001000,0)
assert service(q,view,0,16,0x8c000100,ctypes.byref(value))==0 and value.value>0
handle=value.value;before=bytes(q);before_ram=bytes(memory);value.value=99
assert service(q,view,3,0,0,ctypes.byref(value))==-6
assert value.value==99 and bytes(q)==before and bytes(memory)==before_ram;initialization_cases+=1
assert service(q,view,8,handle,0,ctypes.byref(value))==0
before=bytes(q);value.value=99
assert service(q,view,3,0,0,ctypes.byref(value))==-6 and value.value==99 and bytes(q)==before;initialization_cases+=1
assert service(q,view,1,handle,0x8c000200,ctypes.byref(value))==0
assert service(q,view,3,0,0,ctypes.byref(value))==0 and value.value==0;initialization_cases+=1
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
report={'retail_status_cases_passed':cases,'host_guards_passed':guards,
 'synchronous_guest_read_cases_passed':guest_cases,
 'initialization_cases_passed':initialization_cases,
 'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in ('native_disc_queue.c','native_disc_queue.h','native_disc_sector_reader.c','native_disc_read_command.c','sa_file_device_status.c','sa_file_bios_status.c')},
 'dll_sha256':sha(dll),'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'scope':'Native submit/process/status/cancel with real disc sectors through retail status formatter/tail. Full RAM/GPR/SR/PR/FPU return comparison plus independent progress/cancel/error/consumption assertions.',
 'limitations':'Read commands 16/17 subset; cancellation/error mapping and terminal consumption are explicit host policies, not proven original firmware behavior. No hardware timing/DMA/streaming or complete game proof.',
 'reference':'https://kos-docs.dreamcast.wiki/syscalls_8h_source.html'}
(PROJECT/'analysis/native_disc_service_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'retail status/native service scenarios and',guards,'guard comparisons passed')
