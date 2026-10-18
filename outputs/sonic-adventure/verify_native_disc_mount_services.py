"""Original drive-init/status wrappers connected to the image platform."""
import ctypes,hashlib,itertools,json,struct
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
functions={0x8c6451c8:lib.sa_file_device_disc_request,0x8c6451fc:lib.sa_file_device_disc_info,
           0x8c64530a:lib.sa_file_device_version_request,0x8c10d938:lib.sa_file_bios_request,0x8c10d988:lib.sa_file_bios_disc_info}
for fn in functions.values():fn.argtypes=abi
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
io_calls=[];errors=[]
@Read
def read(user,pos,out,size):io_calls.append(pos);return -9
comparisons=guards=0
for alias,present in itertools.product((0x0c000000,0x8c000000,0xac000000),(False,True)):
    ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xbc,0x8c740000)
    ram[0x800000:0x800010]=b'\xaa'*16
    view=(ctypes.c_uint8*SIZE).from_buffer(ram);q=ctypes.create_string_buffer(lib.sa_disc_queue_size())
    assert init(q,read,None,0,8 if present else 0,45000,2352)==0
    for entry in (0x8c6451c8,0x8c6451fc,0x8c64530a):
        expected_ram=bytearray(ram);state=State();state.r[:]=[0x22340000+i for i in range(16)]
        state.r[4]=alias+0x800000;state.r[15]=alias+0xf00000
        state.sr=0x70000063;state.pr=0x8c000100;state.pc=entry
        r=(U*16)(*state.r);sr=U(state.sr);pr=U(state.pr);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
        expected_ops=[];actual_ops=[]
        def hook(m,regs,status):
            assert regs[6]==0;expected_ops.append(regs[7])
            if entry==0x8c6451c8:
                assert regs[7]==0 and regs[4]==24 and regs[5]==0;regs[0]=1
            elif entry==0x8c64530a:
                assert regs[7]==0 and regs[4]==40
                assert struct.unpack_from('<II',m,regs[5]&0xffffff)==(alias+0x800000,0)
                regs[0]=2
            else:
                assert regs[7]==4
                struct.pack_into('<II',m,regs[4]&0xffffff,2 if present else 7,0x80 if present else 0);regs[0]=0
            return status
        @Call
        def call(ctx,target,m,fp,status,regs,ret):
            try:
                if target in functions:return functions[target](m,fp,status,regs,ret,ctypes.byref(fpul),ctypes.byref(macl),None,call)
                assert target==0x8c740000 and regs[6]==0
                actual_ops.append(regs[7]);value=U()
                rc=service(q,m,regs[7],regs[4],regs[5],ctypes.byref(value));regs[0]=value.value;return rc
            except BaseException as exc:errors.append(repr(exc));return -9
        reference(expected_ram,state,fpu=f,hooks={0x8c740000:hook})
        assert functions[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        assert not errors and actual_ops==expected_ops,errors
        assert expected_ram==ram and list(state.r)==list(r) and state.sr==sr.value and state.pr==pr.value and bytes(f)==bytes(nf)
        assert r[0]==(2 if entry==0x8c64530a else 1) and not io_calls
        comparisons+=1
    # INIT is queued, requires processing, transfers no data, then consumes status.
    value=U();assert service(q,view,1,1,alias+0x800100,ctypes.byref(value))==0 and value.value==1
    assert service(q,view,2,0,0,ctypes.byref(value))==0
    assert service(q,view,1,1,alias+0x800100,ctypes.byref(value))==0 and value.value==2
    assert ram[0x800100:0x800110]==bytes(16)
    assert service(q,view,1,1,alias+0x800100,ctypes.byref(value))==0 and value.value==0
    assert not io_calls and ram[0x800008:0x800010]==b'\xaa'*8
    version_before=bytes(ram[0x800000:0x800010])
    assert service(q,view,1,2,alias+0x800100,ctypes.byref(value))==0 and value.value==1
    assert bytes(ram[0x800000:0x800010])==version_before
    assert service(q,view,2,0,0,ctypes.byref(value))==0
    assert ram[0x800000:0x800010]==b'SA-NATIVE v 1.00'
    assert service(q,view,1,2,alias+0x800100,ctypes.byref(value))==0 and value.value==2
    assert struct.unpack_from('<4I',ram,0x800100)==(0,0,16,0)
    assert service(q,view,1,2,alias+0x800100,ctypes.byref(value))==0 and value.value==0
    # Cancelled metadata never overwrites its destination or reports bytes.
    struct.pack_into('<II',ram,0x800200,alias+0x800300,0);ram[0x800300:0x800310]=b'\xaa'*16
    assert service(q,view,0,40,alias+0x800200,ctypes.byref(value))==0 and value.value>0
    cancelled=value.value
    assert service(q,view,8,cancelled,0,ctypes.byref(value))==0 and value.value==0
    assert service(q,view,2,0,0,ctypes.byref(value))==0
    assert service(q,view,1,cancelled,alias+0x800100,ctypes.byref(value))==0 and value.value==0xffffffff
    assert ram[0x800300:0x800310]==b'\xaa'*16 and struct.unpack_from('<4I',ram,0x800100)==(0,0,0,0)
    for bad in (0,alias+0xfffffc):
        before=bytes(q);before_ram=bytes(ram);value.value=99
        assert service(q,view,4,bad,0,ctypes.byref(value))==-3
        assert value.value==99 and bytes(q)==before and bytes(ram)==before_ram;guards+=1
    before=bytes(q);value.value=99
    assert service(q,view,0,24,alias+0x100,ctypes.byref(value))==0 and value.value==0
    assert bytes(q)==before;guards+=1
    for parameter,destination,reserved in ((alias+0xffffff,alias+0x800300,0),(alias+0x800200,alias+0xfffff8,0),
        (alias+0x800200,alias+0x800300,1),(alias+0x800200,0,0)):
        struct.pack_into('<II',ram,0x800200,destination,reserved)
        before=bytes(q);before_ram=bytes(ram)
        assert service(q,view,0,40,parameter,ctypes.byref(value))==0 and value.value==0
        assert bytes(q)==before and ram==before_ram;guards+=1
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
report={'retail_wrapper_cases_passed':comparisons,'guard_cases_passed':guards,
 'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in ('native_disc_queue.c','native_disc_queue.h','native_disc_sector_reader.c','native_disc_read_command.c','sa_file_device_disc_request.c','sa_file_device_disc_info.c','sa_file_device_version_request.c','sa_file_bios_request.c','sa_file_bios_disc_info.c')},
 'dll_sha256':sha(dll),'binary_sha256':hashlib.sha256(binary).hexdigest(),'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'scope':'Retail INIT/check-drive/version wrappers vs native wrappers calling image platform; full RAM/GPR/SR/PR/FPU, three aliases, image present/absent, queued zero-I/O completion, cancellation and status byte-count assertions.',
 'limitations':'Host image policies and native version identity SA-NATIVE v 1.00, not original firmware. Full mount/event registration remain unfinished.',
 'reference':'https://kos-docs.dreamcast.wiki/syscalls_8h_source.html'}
(PROJECT/'analysis/native_disc_mount_services_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(comparisons,'mount-service wrapper cases and',guards,'guard cases passed')
