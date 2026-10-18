"""Native sound-bank load from actual disc through modeled audio MMIO, compared with original instructions."""
import ctypes,hashlib,itertools,json,shlex,struct
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_request_abort_8c6013d2 import Read as TimerRead,Observe,Dispatch,MMIO
from verify_interrupt_constructor import IO as InterruptIO,Read as InterruptRead,Write as InterruptWrite
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from native_disc_directory import discover_root_files
from verify_engine_cache_writeback import IO as EngineIO,Read as EngineRead,Write as EngineWrite
PROJECT=ROOT/'outputs/sonic-adventure';SIZE=16*1024*1024;U=ctypes.c_uint32;BP=ctypes.POINTER(ctypes.c_uint8)
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,BP,U)
dll=ROOT/'work/sonic-adventure/build/native_sound_bank_load.dll';lib=ctypes.CDLL(str(dll))
lib.sa_disc_queue_size.restype=U
init=lib.sa_disc_queue_init;init.argtypes=[ctypes.c_void_p,Read,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,U,U]
service=lib.sa_disc_service;service.argtypes=[ctypes.c_void_p,BP,U,U,U,ctypes.POINTER(U)]
native_entry=lib.sa_sound_bank_load_native
native_entry.argtypes=[BP,ctypes.POINTER(State),ctypes.POINTER(Fpu),P,P,ctypes.c_void_p,U,
               ctypes.c_void_p,TimerRead,ctypes.POINTER(InterruptIO),Observe,ctypes.POINTER(EngineIO),U]
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
    for discovered in found[:1]:
        assert discovered is not None
        file=dict(discovered);file['path']='SONICADV/'+file['path']
        original=next(f for f in manifest['files'] if f['path']==file['path'])
        assert file['size']==original['size'] and file['lba']==original['lba']
        file['sha256']=original['sha256'] # Comparison oracle only; extent came from disc.
        files.append(file)
    for alias,file,mode in itertools.product((0x8c000000,0xac000000),files,(0,1)):
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
        bank_data=(ROOT/'work/sonic-adventure/extracted/files'/file['path']).read_bytes()
        assert hashlib.sha256(bank_data).hexdigest()==file['sha256']
        records=[struct.unpack_from('<8I',bank_data,32+i*32) for i in range(struct.unpack_from('<I',bank_data,8)[0])]
        hardware={0xa05f688c:0,0xa05f6900:0,0xa05f6920:0xffffffff,0xa05f6924:0,MMIO:0xffffffff,
                  **{0xa05f7800+o:0 for o in range(0,0xc0,4)},**{0xa0801200+i:0x1234ab00 for i in range(0,2048,4)}}
        for record in records:
            if 0<record[5]<0x80000000:
                for at in range(0,record[5],4):hardware[0xa0800000+record[2]+at]=0
        native_hardware=dict(hardware)
        @EngineRead
        def engine_read(user,address,size):
            if address not in native_hardware or size!=4:errors.append(('read',hex(address),size));return 0
            return native_hardware[address]
        @EngineWrite
        def engine_write(user,address,value,size):
            if address not in native_hardware or size!=4:errors.append(('write',hex(address),size))
            native_hardware[address]=value
        engine=EngineIO(None,engine_read,engine_write)
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
        # Compare the complete original mount before entering the real consumer.
        for entry,args in ((0x8c6510a4,(0,0,0,0)),(0x8c10d5a8,(0,0,0,0)),(0x8c65acc0,(0,0,0,0)),
                           (0x8c601618,(2,driver,4096,alias+0x680000)),
                           (0x8c0939b0,(alias+0x770000,0,0,0))):
            expected=State();expected.r[:]=[0x22340000+i for i in range(16)]
            expected.r[4:8]=args;expected.r[15]=alias+0x7ff000
            expected.sr=0x12345678;expected.pr=0x8c74913c;expected.pc=entry
            actual=State.from_buffer_copy(expected);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
            f.fpscr=nf.fpscr=0x200000 if alias==0xac000000 else 0
            special={'macl':0}
            reference(expected_ram,expected,fpu=f,hooks={0x8c740000:hook,0x8c5fc0d8:coherent_cache,0x8c5fc336:coherent_cache},mmio=hardware,vbr=interrupt.vbr,special=special,
                      stop_pc=None)
            rc=native_entry(views[1],ctypes.byref(actual),ctypes.byref(nf),ctypes.byref(fpul),ctypes.byref(macl),queues[1],0x8c740000,None,fixed_timer,ctypes.byref(interrupt),observe,ctypes.byref(engine),500000)
            assert rc==0,(hex(entry),rc,errors)
            assert not errors,errors
            assert expected_ram==actual_ram and bytes(actual)==bytes(expected),(hex(entry),'RAM/GPR mismatch')
            assert bytes(nf)==bytes(f) and macl.value==special['macl']
            assert hardware==native_hardware
            if entry==0x8c65acc0:
                for memory in (expected_ram,actual_ram):
                    struct.pack_into('<I',memory,0x7ac874,mode)
                    struct.pack_into('<I',memory,0x8d35c0,0x1400)
            if entry==0x8c601618:
                count=struct.unpack_from('<I',actual_ram,0x680000)[0]
                assert count==directory_evidence['records']
                directory_before=bytes(actual_ram[0x680000:0x680010+count*44])
                operation_start=len(operations[1])
                assert struct.unpack_from('<I',actual_ram,0x7a0018)[0]==request
        assert actual.pc==expected.pc==expected.pr and actual.r[0]==0
        assert operations[0]==operations[1] and boundaries[0]==boundaries[1]
        assert operations[1][operation_start:][:2]==[0,2]
        assert struct.unpack_from('<HH',actual_ram,(request&0xffffff)+0x4c)==(0,0)
        # Completion leaves mode 1; original close clears ownership only.
        assert struct.unpack_from('<H',actual_ram,(request&0xffffff)+0x50)[0]==1
        assert struct.unpack_from('<I',actual_ram,0x7a001c)[0]==0
        assert struct.unpack_from('<II',actual_ram,(request&0xffffff)+8)==(file['lba']+150,file['size'])
        assert actual_ram[0x680000:0x680010+count*44]==directory_before
        offsets={0x42534d53:(0,0x1e4),0x42504d53:(0x80,0x264),0x42534f53:(0x100,0x2e4),0x52535053:(0x180,0x364),0x42504653:(0x200,0x3e4),0x424f4653:(0x280,0x464),0x57504653:(0x288,0x46c)}
        payloads=0
        for record in records:
            bus,context=offsets[record[0]];index=record[1]
            assert struct.unpack_from('<II',actual_ram,0x8d3400+context+index*8)==record[2:4]
            assert [native_hardware[0xa0801400+bus+index*8],native_hardware[0xa0801404+bus+index*8]]==list(record[2:4])
            if 0<record[5]<0x80000000:
                expected_words=struct.unpack_from('<'+'I'*(record[5]//4),bank_data,record[4])
                assert tuple(native_hardware[0xa0800000+record[2]+at] for at in range(0,record[5],4))==expected_words
                payloads+=1
        assert payloads==4
        assert struct.unpack_from('<I',actual_ram,0x8d3584)[0]==1
        assert struct.unpack_from('<I',actual_ram,0x8d39a8)[0]==0 and struct.unpack_from('<I',actual_ram,0x8d39c8)[0]==0
        digest=hashlib.sha256(actual_ram[0xd00000:0xd00000+file['size']]).hexdigest()
        assert digest==file['sha256']
        cases.append({'alias':hex(alias),'mode':mode,'path':file['path'],'bytes':file['size'],'sha256':digest,
          'returned_to':hex(expected.pc),'service_boundaries_compared':len(boundaries[0]),'file_operations':operations[1][operation_start:],
          'payloads_verified':payloads,'closed_handle':hex(request),'directory_records_preserved':count})

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
# Pin all transitive source inputs already enumerated by the lifecycle suite.
lifecycle=json.loads((PROJECT/'analysis/native_file_request_lifecycle_verification.json').read_text())
import re
names=re.findall(r"'([a-zA-Z0-9_]+\.c)'",(PROJECT/'build_native_sound_bank_load.ps1').read_text())
sources={'source/'+name:sha(PROJECT/'source'/name) for name in names}
for name in ('native_disc_queue.h','fpu_cleanup_transfer.h','interrupt_io.h','input_io.h'):
    sources['source/'+name]=sha(PROJECT/'source'/name)
for name,value in sources.items():assert sha(PROJECT/name)==value,name
report={'cases_passed':len(cases),'cases':cases,'directory_discovery':directory_evidence,'sources':sources,
 'dll_sha256':sha(dll),'builder_sha256':sha(PROJECT/'build_native_sound_bank_load.ps1'),
 'binary_sha256':sha(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'),
 'lifecycle_evidence_sha256':sha(PROJECT/'analysis/native_file_request_lifecycle_verification.json'),
 'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'directory_helper_sha256':sha(PROJECT/'native_disc_directory.py'),
 'fixture_hashes':{name:sha(PROJECT/name) for name in ('verify_interrupt_constructor.py','verify_fpu_cleanup_transfer.py','verify_tikal_controller.py','verify_request_abort_8c6013d2.py')},
 'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'scope':'Native C engine/transfer/frame initialization and disc mount -> original complete sound-bank loader -> public open/size/read/wait/close -> native allocation/configuration/multi-unit transfer/release. Complete original-instruction RAM/CPU/FPU/MMIO/MACL comparison at every return; all disc boundaries compared. Independent file hash, section allocation tables and four original audio payloads checked.',
 'limitations':'Audio MMIO readiness and writes are modeled; no ARM7 sound-driver execution or audible playback claim. Caller supplies stable memory, interrupt bootstrap, read-at/timer/MMIO callbacks, explicit engine mode/bus address configuration and coherent cache policy. No full game startup.'}
(PROJECT/'analysis/sound_bank_load_native_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'complete native sound-bank loads matched original state, disc bytes and four modeled audio payloads')
