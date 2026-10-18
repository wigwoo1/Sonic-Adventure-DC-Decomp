"""Fully native sound-bank file dispatch compared with original instructions; audio boundary external."""
import ctypes,hashlib,itertools,json,shlex,struct
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_request_abort_8c6013d2 import Read as TimerRead,Observe,Dispatch,MMIO
from verify_interrupt_constructor import IO as InterruptIO,Read as InterruptRead,Write as InterruptWrite
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from native_disc_directory import discover_root_files
PROJECT=ROOT/'outputs/sonic-adventure';SIZE=16*1024*1024;U=ctypes.c_uint32;BP=ctypes.POINTER(ctypes.c_uint8)
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,BP,U)
dll=ROOT/'work/sonic-adventure/build/native_sound_bank_file_consumer.dll';lib=ctypes.CDLL(str(dll))
lib.sa_disc_queue_size.restype=U
init=lib.sa_disc_queue_init;init.argtypes=[ctypes.c_void_p,Read,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,U,U]
service=lib.sa_disc_service;service.argtypes=[ctypes.c_void_p,BP,U,U,U,ctypes.POINTER(U)]
native_entry=lib.sa_sound_bank_file_consumer_native
native_entry.argtypes=[BP,ctypes.POINTER(State),ctypes.POINTER(Fpu),P,P,ctypes.c_void_p,U,
               ctypes.c_void_p,TimerRead,ctypes.POINTER(InterruptIO),Observe,Dispatch,U]
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
manifest=json.loads((PROJECT/'disc_inventory.json').read_text())
track=next(t for t in reversed([shlex.split(s) for s in manifest['gdi'].splitlines()[1:] if s.strip()]) if t[2]=='4')
path=Path(manifest['source'])/track[4];base,stride,offset=int(track[1]),int(track[3]),int(track[5])
files=[]
cases=[];errors=[]
with path.open('rb') as stream:
    @Read
    def io(user,pos,out,size):
        try:
            stream.seek(pos);data=stream.read(size)
            if len(data)!=size:return -8
            ctypes.memmove(out,data,size);return 0
        except BaseException as exc:errors.append(repr(exc));return -8
    found,directory_evidence=discover_root_files(lib,io,offset,(path.stat().st_size-offset)//stride,base,stride,
        ('E_00B7.MLT','E_0001.MLT','__MISSING_FILE__'),boot_directory='SONICADV')
    assert found[2] is None
    for discovered in found[:2]:
        assert discovered is not None
        file=dict(discovered);file['path']='SONICADV/'+file['path']
        original=next(f for f in manifest['files'] if f['path']==file['path'])
        assert file['size']==original['size'] and file['lba']==original['lba']
        file['sha256']=original['sha256'] # Comparison oracle only; extent came from disc.
        files.append(file)
    for alias,file in itertools.product((0x8c000000,0xac000000),files):
        ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary
        driver=alias+0x7a0000;request=driver+0x10d4
        mode_address=struct.unpack_from('<I',binary,0x8c600598-0x8c010000)[0]&0xffffff
        struct.pack_into('<H',ram,mode_address,0) # Explicit platform read-mode configuration.
        for addr,value in ((0xbc,0x8c740000),(0x673414,0),(0x673420,0),(0x67c004,0x8c7d0000)):
            struct.pack_into('<I',ram,addr,value)
        ram[0x80f0:0x80fe]=b'SONICADV    '+bytes([1,0])
        query=file['path'].split('/')[-1].lower().encode('ascii')
        ram[0x770000:0x770001+len(query)]=query+b'\0'
        expected_ram,actual_ram=bytearray(ram),bytearray(ram)
        queues=[ctypes.create_string_buffer(lib.sa_disc_queue_size()) for _ in range(2)]
        views=[(ctypes.c_uint8*SIZE).from_buffer(m) for m in (expected_ram,actual_ram)]
        for q in queues:assert init(q,io,None,offset,(path.stat().st_size-offset)//stride,base,stride)==0
        operations=[[],[]];submitted=[[],[]];boundaries=[[],[]]
        hardware={0xa05f6920:0,0xa05f6924:0,MMIO:0xffffffff};native_hardware=dict(hardware)
        @InterruptRead
        def interrupt_read(user,address,value):value[0]=native_hardware[address];return 0
        @InterruptWrite
        def interrupt_write(user,address,value):native_hardware[address]=value;return 0
        interrupt=InterruptIO(0x8c7e0000,0,None,interrupt_read,interrupt_write)
        def external(index,m,regs):
            value=U();operations[index].append(regs[7])
            assert regs[6]==0
            assert service(queues[index],views[index],regs[7],regs[4],regs[5],ctypes.byref(value))==0
            if regs[7]==0:submitted[index].append(value.value)
            regs[0]=value.value
        def hook(m,r,sr):
            boundaries[0].append((hashlib.sha256(m).hexdigest(),tuple(r),sr,expected.pr))
            external(0,m,r);return sr
        def coherent_cache(m,r,sr):r[0]=0;return sr
        @TimerRead
        def fixed_timer(user,address):assert address==MMIO;return 0xffffffff
        @Observe
        def observe(memory,state):
            if state.contents.pc!=0x8c740000:return
            try:
                r=state.contents.r
                operations[1].append(r[7])
                boundaries[1].append((hashlib.sha256(actual_ram).hexdigest(),tuple(r),state.contents.sr,state.contents.pr))
            except BaseException as exc:errors.append(repr(exc))
        @Dispatch
        def boundary(memory,state,target,user):
            try:
                assert target==state.contents.pc==0x8c093ce0 and state.contents.r[4]==0x0cd00000
                download_calls.append(tuple(state.contents.r[i] for i in range(4,8)))
                assert hashlib.sha256(actual_ram[0xd00000:0xd00000+file['size']]).hexdigest()==file['sha256']
                return -100
            except BaseException as exc:errors.append(repr(exc));return -9
        # Compare the complete original mount before entering the real consumer.
        download_calls=[]
        for entry,args in ((0x8c601618,(2,driver,4096,alias+0x680000)),
                           (0x8c0939b0,(alias+0x770000,0,0,0))):
            expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
            expected.r[4:8]=args;expected.r[15]=alias+0x7ff000
            expected.sr=0x12345678;expected.pr=0x8c74913c;expected.pc=entry
            actual=State.from_buffer_copy(expected);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
            f.fpscr=nf.fpscr=0x200000 if alias==0xac000000 else 0
            special={'macl':0}
            reference(expected_ram,expected,fpu=f,hooks={0x8c740000:hook,0x8c5fc0d8:coherent_cache,0x8c5fc336:coherent_cache},mmio=hardware,vbr=interrupt.vbr,special=special,
                      stop_pc=0x8c093ce0 if entry==0x8c0939b0 else None)
            rc=native_entry(views[1],ctypes.byref(actual),ctypes.byref(nf),ctypes.byref(fpul),ctypes.byref(macl),queues[1],0x8c740000,None,fixed_timer,ctypes.byref(interrupt),observe,boundary,500000)
            assert rc==(-100 if entry==0x8c0939b0 else 0),(hex(entry),rc,errors)
            assert not errors,errors
            assert expected_ram==actual_ram and bytes(actual)==bytes(expected),(hex(entry),'RAM/GPR mismatch')
            assert bytes(nf)==bytes(f) and macl.value==special['macl']
            assert hardware==native_hardware
            if entry==0x8c601618:
                count=struct.unpack_from('<I',actual_ram,0x680000)[0]
                assert count==directory_evidence['records']
                directory_before=bytes(actual_ram[0x680000:0x680010+count*44])
                operation_start=len(operations[1])
                assert struct.unpack_from('<I',actual_ram,0x7a0018)[0]==request
        assert len(download_calls)==1 and expected.pc==0x8c093ce0
        assert expected.r[4]==download_calls[0][0]==0x0cd00000
        assert operations[0]==operations[1] and boundaries[0]==boundaries[1]
        assert operations[1][operation_start:][:2]==[0,2]
        assert struct.unpack_from('<HH',actual_ram,(request&0xffffff)+0x4c)==(0,0)
        # Completion leaves mode 1; original close clears ownership only.
        assert struct.unpack_from('<H',actual_ram,(request&0xffffff)+0x50)[0]==1
        assert struct.unpack_from('<I',actual_ram,0x7a001c)[0]==0
        assert struct.unpack_from('<II',actual_ram,(request&0xffffff)+8)==(file['lba']+150,file['size'])
        assert actual_ram[0x680000:0x680010+count*44]==directory_before
        digest=hashlib.sha256(actual_ram[0xd00000:0xd00000+file['size']]).hexdigest()
        assert digest==file['sha256']
        cases.append({'alias':hex(alias),'path':file['path'],'bytes':file['size'],'sha256':digest,
          'download_boundary':hex(expected.pc),'service_boundaries_compared':len(boundaries[0]),'file_operations':operations[1][operation_start:],
          'closed_handle':hex(request),'directory_records_preserved':count})

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
# Pin all transitive source inputs already enumerated by the lifecycle suite.
lifecycle=json.loads((PROJECT/'analysis/native_file_request_lifecycle_verification.json').read_text())
import re
names=re.findall(r"'([a-zA-Z0-9_]+\.c)'",(PROJECT/'build_native_sound_bank_file_consumer.ps1').read_text())
sources={'source/'+name:sha(PROJECT/'source'/name) for name in names}
for name in ('native_disc_queue.h','fpu_cleanup_transfer.h','interrupt_io.h'):
    sources['source/'+name]=sha(PROJECT/'source'/name)
for name,value in sources.items():assert sha(PROJECT/name)==value,name
report={'cases_passed':len(cases),'cases':cases,'directory_discovery':directory_evidence,'sources':sources,
 'dll_sha256':sha(dll),'builder_sha256':sha(PROJECT/'build_native_sound_bank_file_consumer.ps1'),
 'binary_sha256':sha(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'),
 'lifecycle_evidence_sha256':sha(PROJECT/'analysis/native_file_request_lifecycle_verification.json'),
 'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'directory_helper_sha256':sha(PROJECT/'native_disc_directory.py'),
 'fixture_hashes':{name:sha(PROJECT/name) for name in ('verify_interrupt_constructor.py','verify_fpu_cleanup_transfer.py','verify_tikal_controller.py','verify_request_abort_8c6013d2.py')},
 'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'scope':'Native C dispatcher; original backend install and SONICADV mount, then sound-bank load 8c0939b0 -> public named open -> size -> synchronous sector read/native queue/completion -> close. Full guest RAM/GPR/SR/PR/FPU/MMIO/MACL compared with original instructions. Real file bytes independently checked against extraction hashes.',
 'limitations':'Stops deliberately at 8c093ce0 before audio download; no playback or complete sound-loader execution claim. Host supplies stable buffers, read-at, timer/MMIO/bootstrap callbacks and coherent cache policy. File dispatch and queue/completion execute in C; no full engine startup.'}
(PROJECT/'analysis/sound_bank_file_consumer_native_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'original sound-bank file consumers reached audio-download boundary with matching retail state/data')
