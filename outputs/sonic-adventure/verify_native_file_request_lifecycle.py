"""Original handle initialization and aligned read create a native disc request."""
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
cases=[];errors=[];named_open_guards=0;close_checks=0;reuse_cycles=0
with path.open('rb') as stream:
    @Read
    def io(user,pos,out,size):
        try:
            stream.seek(pos);data=stream.read(size)
            if len(data)!=size:return -8
            ctypes.memmove(out,data,size);return 0
        except BaseException as exc:errors.append(repr(exc));return -8
    found,directory_evidence=discover_root_files(lib,io,offset,(path.stat().st_size-offset)//stride,base,stride,
        ('1st_read.bin','ADVLOGOBK.GIF','__MISSING_FILE__'))
    assert found[2] is None
    for discovered in found[:2]:
        assert discovered is not None
        file=dict(discovered)
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
        query=file['path'].lower().encode('ascii')
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
                assert target==0x8c740000
                external(1,actual_ram,r);return 0
            except BaseException as exc:errors.append(repr(exc));return -9
        # The full executable fills 0x800000..0xe6c680; keep directory storage
        # below that span and above the loaded retail code (ends 0x67c680).
        for entry,args in ((0x8c601618,(2,driver,4096,alias+0x680000)),
                           (0x8c6016d6,(alias+0x770000,0,0,0)),
                           (0x8c601084,(request,(file['size']+2047)//2048,0xac800000,0))):
            expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
            expected.r[4:8]=args;expected.r[15]=alias+0x7ff000
            expected.sr=0x12345678;expected.pr=0x8c74913c;expected.pc=entry
            r=(U*16)(*expected.r);sr=U(expected.sr);pr=U(expected.pr);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
            special={'macl':0}
            reference(expected_ram,expected,fpu=f,hooks={0x8c740000:hook,0x8c5fc0d8:coherent_cache,0x8c5fc336:coherent_cache},mmio=hardware,vbr=interrupt.vbr,special=special)
            assert functions[entry](views[1],ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
            assert not errors,errors
            assert expected_ram==actual_ram and list(expected.r)==list(r) and expected.sr==sr.value and expected.pr==pr.value and bytes(f)==bytes(nf),hex(entry)
            assert hardware==native_hardware
            assert macl.value==special['macl']
            if entry==0x8c601618:
                assert struct.unpack_from('<I',actual_ram,0x673420)[0]==0x8c66c4b4
                assert struct.unpack_from('<I',actual_ram,0x673414)[0]==driver
                assert struct.unpack_from('<I',actual_ram,0x7a0018)[0]==request
                for i in range(3):assert struct.unpack_from('<I',actual_ram,0x7a1080+i*84)[0]==driver
                assert struct.unpack_from('<I',actual_ram,0x7a000c)[0]==0x8c66d240
                assert struct.unpack_from('<I',actual_ram,0x7a003c)[0]==0x8c66c4b4
                assert struct.unpack_from('<H',actual_ram,0x7a0038)[0]==1
                assert struct.unpack_from('<I',actual_ram,0x680000)[0]==768
                directory_before=bytes(actual_ram[0x680000:0x680010+768*44])
            elif entry==0x8c6016d6:
                assert r[0]==request and struct.unpack_from('<H',actual_ram,(request&0xffffff)+0x4c)[0]==1
                assert struct.unpack_from('<II',actual_ram,(request&0xffffff)+8)==(file['lba']+150,file['size'])
                assert struct.unpack_from('<I',actual_ram,0x7a0030)[0]==0
            else:
                assert r[0]==args[1] and struct.unpack_from('<I',actual_ram,0x7a001c)[0]==request
                assert struct.unpack_from('<I',actual_ram,(request&0xffffff)+0x3c)[0]==submitted[1][-1] and struct.unpack_from('<H',actual_ram,(request&0xffffff)+0x50)[0]==2
        assert operations[0]==operations[1] and operations[1][:3]==[3,0,2] and operations[1][-2:]==[0,2]
        assert actual_ram[0x680000:0x680010+768*44]==directory_before
        assert submitted[0]==submitted[1] and all(submitted[1])
        # The original read routine already performed one pump; finish the queue.
        for _ in range((file['size']+2047)//2048-1):
            for index in range(2):
                value=U();assert service(queues[index],views[index],2,0,0,ctypes.byref(value))==0
        digest=hashlib.sha256(actual_ram[0x800000:0x800000+file['size']]).hexdigest();assert digest==file['sha256']
        # Completion consumes the actual request created by the initializer/read.
        for memory in (expected_ram,actual_ram):
            struct.pack_into('<I',memory,0x780008,request);memory[0x780002]=1
        expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
        expected.r[4]=alias+0x780000;expected.r[15]=alias+0x7ff000
        expected.sr=0x12345678;expected.pr=0x8c74913c;expected.pc=0x8c65bf60
        actual=State.from_buffer_copy(expected)
        class Timer(dict):
            def __init__(self):super().__init__({MMIO:0});self.reads=[]
            def __getitem__(self,address):
                assert address==MMIO and len(self.reads)<100
                value=(~(0x1000+len(self.reads)))&0xffffffff;self.reads.append(value);return value
        timers=[Timer(),Timer()]
        reference(expected_ram,expected,hooks={0x8c740000:hook},mmio=timers[0])
        @TimerRead
        def timer(user,address):return timers[1][address]
        assert cleanup(views[1],ctypes.byref(actual),queues[1],0x8c740000,None,timer,Observe(),500000)==0
        assert expected_ram==actual_ram and bytes(expected)==bytes(actual) and timers[0].reads==timers[1].reads
        assert struct.unpack_from('<I',actual_ram,0x7a001c)[0]==0
        assert struct.unpack_from('<H',actual_ram,(request&0xffffff)+0x50)[0]==0 and actual_ram[0x780002]==0
        assert struct.unpack_from('<I',actual_ram,(request&0xffffff)+0x18)[0]==((file['size']+2047)//2048)*2048
        # Exercise public open against the actual mounted table and handle pool.
        for name,want_handle,want_error in (('__MISSING__',0,-8),(file['path'],request+84,0),(file['path'],0,-9)):
            encoded=name.encode('ascii')
            for memory in (expected_ram,actual_ram):memory[0x770000:0x770001+len(encoded)]=encoded+b'\0'
            pool_before=bytes(actual_ram[0x7a1080:0x7a117c])
            expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
            expected.r[4]=alias+0x770000;expected.r[5]=0;expected.r[15]=alias+0x7ff000
            expected.sr=0x12345678;expected.pr=0x8c74913c;expected.pc=0x8c6016d6
            rr=(U*16)(*expected.r);ss=U(expected.sr);pp=U(expected.pr);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
            special={'macl':0};reference(expected_ram,expected,fpu=f,special=special)
            assert functions[0x8c6016d6](views[1],ctypes.byref(nf),ctypes.byref(ss),rr,ctypes.byref(pp),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
            assert not errors and expected_ram==actual_ram and list(rr)==list(expected.r)
            assert ss.value==expected.sr and pp.value==expected.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
            assert rr[0]==want_handle and struct.unpack_from('<i',actual_ram,0x7a0030)[0]==want_error,(name,hex(alias),hex(rr[0]),hex(want_handle),struct.unpack_from('<i',actual_ram,0x7a0030)[0],want_error)
            if not want_handle:assert actual_ram[0x7a1080:0x7a117c]==pool_before
            named_open_guards+=1
        assert actual_ram[0x680000:0x680010+768*44]==directory_before
        # Keep the second live handle unchanged while repeatedly releasing/reusing
        # the first. Compare every call with the original SH-4 instructions.
        other_before=bytes(actual_ram[0x7a1128:0x7a117c])
        def checked_close(handle):
            global close_checks
            expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
            expected.r[4]=handle;expected.r[15]=alias+0x7ff000
            expected.sr=0x12345678;expected.pr=0x8c74913c;expected.pc=0x8c600df4
            actual=State.from_buffer_copy(expected)
            timers[0],timers[1]=Timer(),Timer()
            reference(expected_ram,expected,hooks={0x8c740000:hook},mmio=timers[0])
            assert close(views[1],ctypes.byref(actual),queues[1],0x8c740000,None,timer,Observe(),500000)==0
            assert expected_ram==actual_ram and bytes(expected)==bytes(actual)
            assert timers[0].reads==timers[1].reads
            close_checks+=1
        checked_close(request)
        checked_close(request) # Already closed.
        checked_close(0)
        for cycle in range(3):
            for entry,args in ((0x8c6016d6,(alias+0x770000,0,0,0)),
                               (0x8c601084,(request,1,0xac800000,0))):
                expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
                expected.r[4:8]=args;expected.r[15]=alias+0x7ff000
                expected.sr=0x12345678;expected.pr=0x8c74913c;expected.pc=entry
                rr=(U*16)(*expected.r);ss=U(expected.sr);pp=U(expected.pr);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
                special={'macl':0}
                reference(expected_ram,expected,fpu=f,hooks={0x8c740000:hook},special=special)
                assert functions[entry](views[1],ctypes.byref(nf),ctypes.byref(ss),rr,ctypes.byref(pp),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
                assert not errors and expected_ram==actual_ram and list(rr)==list(expected.r)
                assert ss.value==expected.sr and pp.value==expected.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
                assert rr[0]==(request if entry==0x8c6016d6 else 1)
            checked_close(request) # Pending game request; original close invokes abort.
            assert struct.unpack_from('<H',actual_ram,(request&0xffffff)+0x4c)[0]==0
            assert actual_ram[0x7a1128:0x7a117c]==other_before
            assert actual_ram[0x680000:0x680010+768*44]==directory_before
            reuse_cycles+=1
        checked_close(request+84)
        cases.append({'alias':hex(alias),'path':file['path'],'sha256':digest,'bytes':file['size'],'directory_preserved_after_read':True})
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
report={'cases_passed':len(cases),'close_checks_passed':close_checks,'handle_reuse_cycles_passed':reuse_cycles,'named_open_guards_passed':named_open_guards,'named_open_verified':True,'cases':cases,'dll_sha256':sha(dll),'binary_sha256':hashlib.sha256(binary).hexdigest(),
 'cleanup_dll_sha256':sha(cleanup_dll),'cleanup_evidence_sha256':sha(PROJECT/'analysis/stream_cleanup_disc_native_verification.json'),
 'directory_discovery':directory_evidence,'directory_helper_sha256':sha(PROJECT/'native_disc_directory.py'),
 'interrupt_fixture_abi_sha256':sha(PROJECT/'verify_interrupt_constructor.py'),
 'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in ('native_disc_queue.c','native_disc_queue.h','native_disc_sector_reader.c','native_disc_read_command.c','sa_file_handle_open_initialize.c','sa_file_read_aligned.c','sa_file_device_read.c','sa_file_bios_request.c','sa_file_bios_execute.c','sa_callback_service_call_return_one_8c6452d6.c',
 'sa_file_directory_load.c','sa_file_directory_record_parse.c','sa_file_directory_bytes_copy.c','sa_file_name_upper.c','sa_file_name_equal.c','sa_file_name_equal_folded.c','sa_file_name_checksum.c','sa_file_filename_lookup.c',
 'sa_file_mount_sector_read.c','sa_file_reserved_handle_acquire.c','sa_file_read_sectors.c',
 'sa_file_context_initialize.c','sa_file_directory_storage_initialize.c','sa_file_device_interface_get.c','sa_file_device_initialize.c','sa_file_device_low_initialize.c',
 'sa_file_mount.c','sa_file_volume_signature_check.c','sa_file_volume_word_get.c','sa_file_directory_parent_sector_adjust.c','sa_file_directory_select.c',
 'sa_interrupt_callback_create.c','interrupt_io.h','sa_interrupt_handler_primary.c','sa_interrupt_handler_set.c','sa_mount_driver_enable.c',
 'sa_file_device_disc_request.c','sa_file_device_disc_info.c','sa_file_bios_disc_info.c','sa_file_device_version_request.c',
 'sa_file_backend_install.c','sa_file_open_dispatch.c','sa_file_backend_open.c','sa_file_error_set.c','sa_file_close.c','native_disc_service_bridge.c')},
 'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'scope':'Original backend installer/context/full mount -> public named-file open/default directory/native handle selection -> read/data/cleanup/close/reuse. Full retail RAM/GPR/SR/PR/FPU/MMIO and MACL after install/open/read; RAM/GPR/SR/PR/PC after cleanup and close. Independent mounted/table/pool/extent/data assertions, missing/full-pool errors, null/double close and preservation of another live handle.',
 'limitations':'Host supplies stable buffers/capacity/read mode, interrupt bootstrap/VBR/masks and root boot selector. Cache helpers are coherent host no-ops. Remaining file data pumped by host before cleanup. Dispatcher glue in verifier; no full engine/resource-loader startup, physical interrupts or firmware equivalence.'}
(PROJECT/'analysis/native_file_request_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'original lifecycles,',named_open_guards,'named-open checks,',close_checks,'closes,',reuse_cycles,'reuse cycles passed')
