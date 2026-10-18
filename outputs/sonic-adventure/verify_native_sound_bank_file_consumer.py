"""Original sound-bank file consumer connected to actual native disc I/O. Stops before audio download."""
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
dll=ROOT/'work/sonic-adventure/build/native_disc_queue.dll';lib=ctypes.CDLL(str(dll))
lib.sa_disc_queue_size.restype=U
init=lib.sa_disc_queue_init;init.argtypes=[ctypes.c_void_p,Read,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,U,U]
service=lib.sa_disc_service;service.argtypes=[ctypes.c_void_p,BP,U,U,U,ctypes.POINTER(U)]
cleanup_dll=ROOT/'work/sonic-adventure/build/stream_cleanup_disc_native.dll'
cleanup=ctypes.CDLL(str(cleanup_dll)).sa_stream_cleanup_disc_native
cleanup.argtypes=[BP,ctypes.POINTER(State),ctypes.c_void_p,U,ctypes.c_void_p,TimerRead,Observe,U]
close=ctypes.CDLL(str(cleanup_dll)).sa_file_close_disc_native
close.argtypes=cleanup.argtypes
abi=[BP,ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
functions={0x8c600464:lib.sa_file_handle_open_initialize,0x8c601084:lib.sa_file_read_aligned,
           0x8c644ff6:lib.sa_file_device_read,0x8c10d938:lib.sa_file_bios_request,0x8c10d960:lib.sa_file_bios_execute}
functions.update({0x8c600038:lib.sa_file_context_initialize,0x8c600dd0:lib.sa_file_directory_storage_initialize,
 0x8c645328:lib.sa_file_device_interface_get,0x8c644fe8:lib.sa_file_device_initialize,0x8c10d974:lib.sa_file_device_low_initialize})
functions.update({0x8c601618:lib.sa_file_backend_install,0x8c6016d6:lib.sa_file_open_dispatch,
 0x8c6004d4:lib.sa_file_backend_open,0x8c5ffa08:lib.sa_file_error_set,0x8c6455ce:lib.sa_file_filename_lookup,
 0x8c6453c0:lib.sa_file_name_equal})
for address,name in {0x8c5ffd28:'sa_file_mount',0x8c5ff9e6:'sa_file_volume_signature_check',0x8c5ff9c0:'sa_file_volume_word_get',
 0x8c5ffcfe:'sa_file_directory_parent_sector_adjust',0x8c60041a:'sa_file_directory_select',0x8c64135a:'sa_interrupt_handler_primary',
 0x8c6412c2:'sa_interrupt_handler_set',0x8c658c20:'sa_mount_driver_enable',0x8c6451c8:'sa_file_device_disc_request',
 0x8c6451fc:'sa_file_device_disc_info',0x8c10d988:'sa_file_bios_disc_info',0x8c64530a:'sa_file_device_version_request',
 0x8c5ffc98:'sa_file_mount_sector_read',0x8c600d82:'sa_file_reserved_handle_acquire',0x8c600f1e:'sa_file_read_sectors',
 0x8c6454fe:'sa_file_directory_load',0x8c645468:'sa_file_directory_record_parse',0x8c64535c:'sa_file_directory_bytes_copy',
 0x8c645348:'sa_file_name_upper',0x8c645442:'sa_file_name_checksum',0x8c6453e2:'sa_file_name_equal_folded'}.items():
    functions[address]=getattr(lib,name)
consumer_dll=ROOT/'work/sonic-adventure/build/sound_bank_file_consumer.dll'
consumer=ctypes.CDLL(str(consumer_dll))
functions.update({0x8c0939b0:consumer.sa_sound_bank_load,0x8c600e2c:consumer.sa_file_size_get,0x8c600df4:lib.sa_file_close})
for fn in functions.values():fn.argtypes=abi
constructor=lib.sa_interrupt_callback_create;constructor.argtypes=abi[:7]+[ctypes.POINTER(InterruptIO)]+abi[7:]
finalizer=ctypes.CDLL(str(cleanup_dll)).sa_request_finalize_native
finalizer.argtypes=[BP,ctypes.POINTER(State),ctypes.c_void_p,TimerRead,Dispatch,Observe,U]
Pump=ctypes.CFUNCTYPE(None,BP,ctypes.POINTER(State))
pump=lib.sa_callback_service_call_return_one_8c6452d6;pump.argtypes=[BP,ctypes.POINTER(State),Pump];pump.restype=None
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
        operations=[[],[]];submitted=[[],[]]
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
        def hook(m,r,sr):external(0,m,r);return sr
        def coherent_cache(m,r,sr):r[0]=0;return sr
        @TimerRead
        def fixed_timer(user,address):assert address==MMIO;return 0xffffffff
        @Dispatch
        def finalizer_device(memory,state,target,user):
            if target!=0x8c740000 or state.contents.r[6]!=0:return -8
            external(1,actual_ram,state.contents.r);state.contents.pc=state.contents.pr;return 0
        @Call
        def call(ctx,target,m,f,sr,r,pr):
            try:
                if target in functions:return functions[target](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
                if target==0x8c64594c:return constructor(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(interrupt),None,call)
                if target in (0x8c5fc0d8,0x8c5fc336):r[0]=0;return 0
                if target==0x8c600cf0:
                    state=State();state.r[:]=[r[i] for i in range(16)];state.sr,state.pr,state.pc=sr[0],pr[0],target
                    rc=finalizer(m,ctypes.byref(state),None,fixed_timer,finalizer_device,Observe(),100000)
                    for i in range(16):r[i]=state.r[i]
                    sr[0],pr[0]=state.sr,state.pr;return rc
                if target==0x8c6452d6:
                    state=State();state.r[:]=[r[i] for i in range(16)];state.sr,state.pr,state.pc=sr[0],pr[0],target
                    @Pump
                    def lower(memory,s):
                        if s.contents.pc!=0x8c10d960:raise AssertionError(hex(s.contents.pc))
                        rr=(U*16)(*s.contents.r);ss=U(s.contents.sr);pp=U(s.contents.pr)
                        assert functions[0x8c10d960](m,f,ctypes.byref(ss),rr,ctypes.byref(pp),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
                        s.contents.r[:]=rr;s.contents.sr,s.contents.pr,s.contents.pc=ss.value,pp.value,pp.value
                    pump(m,ctypes.byref(state),lower)
                    for i in range(16):r[i]=state.r[i]
                    sr[0],pr[0]=state.sr,state.pr;return 0
                if target==0x8c093ce0:
                    assert r[4]==0x0cd00000
                    download_calls.append(tuple(r[i] for i in range(4,8)))
                    assert hashlib.sha256(actual_ram[0xd00000:0xd00000+file['size']]).hexdigest()==file['sha256']
                    return -100 # Deliberate stop before audio processing.
                assert target==0x8c740000
                external(1,actual_ram,r);return 0
            except BaseException as exc:errors.append(repr(exc));return -9
        # Compare the complete original mount before entering the real consumer.
        download_calls=[]
        for entry,args in ((0x8c601618,(2,driver,4096,alias+0x680000)),
                           (0x8c0939b0,(alias+0x770000,0,0,0))):
            expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
            expected.r[4:8]=args;expected.r[15]=alias+0x7ff000
            expected.sr=0x12345678;expected.pr=0x8c74913c;expected.pc=entry
            rr=(U*16)(*expected.r);ss=U(expected.sr);pp=U(expected.pr);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
            f.fpscr=nf.fpscr=0x200000 if alias==0xac000000 else 0
            special={'macl':0}
            reference(expected_ram,expected,fpu=f,hooks={0x8c740000:hook,0x8c5fc0d8:coherent_cache,0x8c5fc336:coherent_cache},mmio=hardware,vbr=interrupt.vbr,special=special,
                      stop_pc=0x8c093ce0 if entry==0x8c0939b0 else None)
            rc=functions[entry](views[1],ctypes.byref(nf),ctypes.byref(ss),rr,ctypes.byref(pp),ctypes.byref(fpul),ctypes.byref(macl),None,call)
            assert rc==(-100 if entry==0x8c0939b0 else 0),(hex(entry),rc,errors)
            assert not errors,errors
            assert expected_ram==actual_ram and list(rr)==list(expected.r),(hex(entry),'RAM/GPR mismatch')
            assert ss.value==expected.sr and pp.value==expected.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
            assert hardware==native_hardware
            if entry==0x8c601618:
                count=struct.unpack_from('<I',actual_ram,0x680000)[0]
                assert count==directory_evidence['records']
                directory_before=bytes(actual_ram[0x680000:0x680010+count*44])
                operation_start=len(operations[1])
                assert struct.unpack_from('<I',actual_ram,0x7a0018)[0]==request
        assert len(download_calls)==1 and expected.pc==0x8c093ce0
        assert expected.r[4]==download_calls[0][0]==0x0cd00000
        assert operations[0]==operations[1] and submitted[0]==submitted[1]
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
          'download_boundary':hex(expected.pc),'file_operations':operations[1][operation_start:],
          'closed_handle':hex(request),'directory_records_preserved':count})

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
# Pin all transitive source inputs already enumerated by the lifecycle suite.
lifecycle=json.loads((PROJECT/'analysis/native_file_request_lifecycle_verification.json').read_text())
sources=dict(lifecycle['sources'])
sources.update(json.loads((PROJECT/'analysis/stream_cleanup_disc_native_verification.json').read_text())['sources'])
for name in ('sa_sound_bank_load.c','sa_file_size_get.c','fpu_cleanup_transfer.h'):
    sources['source/'+name]=sha(PROJECT/'source'/name)
for name,value in sources.items():assert sha(PROJECT/name)==value,name
report={'cases_passed':len(cases),'cases':cases,'directory_discovery':directory_evidence,'sources':sources,
 'dll_sha256':sha(dll),'consumer_dll_sha256':sha(consumer_dll),'cleanup_dll_sha256':sha(cleanup_dll),
 'binary_sha256':sha(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'),
 'lifecycle_evidence_sha256':sha(PROJECT/'analysis/native_file_request_lifecycle_verification.json'),
 'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'directory_helper_sha256':sha(PROJECT/'native_disc_directory.py'),
 'fixture_hashes':{name:sha(PROJECT/name) for name in ('verify_interrupt_constructor.py','verify_fpu_cleanup_transfer.py','verify_tikal_controller.py','verify_request_abort_8c6013d2.py')},
 'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'scope':'Original backend install and SONICADV mount, then sound-bank load 8c0939b0 -> public named open -> size -> synchronous sector read/native queue/completion -> close. Full guest RAM/GPR/SR/PR/FPU/MMIO/MACL compared with original instructions. Real file bytes independently checked against extraction hashes.',
 'limitations':'Stops deliberately at 8c093ce0 before audio download; no playback or full sound-loader completion claim. Host supplies interrupt bootstrap/coherent cache policy and stable storage. Dispatcher is Python verifier glue; no full engine startup.'}
(PROJECT/'analysis/native_sound_bank_file_consumer_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'original sound-bank file consumers reached audio-download boundary with matching retail state/data')
