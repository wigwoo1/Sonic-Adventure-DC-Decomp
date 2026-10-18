"""Retail read formatter -> native submission bridge -> real sector queue."""
import ctypes,hashlib,itertools,json,shlex,struct
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
PROJECT=ROOT/'outputs/sonic-adventure';SIZE=16*1024*1024
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ROOT/'work/sonic-adventure/build/native_disc_queue.dll';lib=ctypes.CDLL(str(dll))
ByteP=ctypes.POINTER(ctypes.c_uint8);U=ctypes.c_uint32
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,ByteP,U)
lib.sa_disc_queue_size.restype=U
init=lib.sa_disc_queue_init;init.argtypes=[ctypes.c_void_p,Read,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,U,U]
step=lib.sa_disc_queue_step;step.argtypes=[ctypes.c_void_p]
submit=lib.sa_disc_submit_read_command;submit.argtypes=[ctypes.c_void_p,ByteP,U,U,ctypes.POINTER(U)]
abi=[ByteP,ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
native=lib.sa_file_device_read;native.argtypes=abi
request=lib.sa_file_bios_request;request.argtypes=abi
manifest=json.loads((PROJECT/'disc_inventory.json').read_text())
track=next(t for t in reversed([shlex.split(s) for s in manifest['gdi'].splitlines()[1:] if s.strip()]) if t[2]=='4')
path=Path(manifest['source'])/track[4];base,stride,offset=int(track[1]),int(track[3]),int(track[5])
files=[next(f for f in manifest['files'] if f['path']=='1ST_READ.BIN'),next(f for f in manifest['files'] if not f['directory'] and 0<f['size']<65536)]
proof=[];cases=0;guards=0;io_errors=[]
with path.open('rb') as stream:
    @Read
    def read(user,pos,out,size):
        try:
            stream.seek(pos);data=stream.read(size)
            if len(data)!=size:return -8
            ctypes.memmove(out,data,size);return 0
        except BaseException as exc:io_errors.append(repr(exc));return -8
    for file,mode,alias,alignment in itertools.product(files,(0,1),(0x0c000000,0x8c000000,0xac000000),(0,1)):
        ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xbc,0x8cba0000)
        actual_ram=bytearray(ram);view=(ctypes.c_uint8*SIZE).from_buffer(actual_ram)
        q=ctypes.create_string_buffer(lib.sa_disc_queue_size())
        assert init(q,read,None,offset,(path.stat().st_size-offset)//stride,base,stride)==0
        count=(file['size']+2047)//2048;dest=alias+0x800000+alignment
        s=State();s.r[:]=[0x22340000+i for i in range(16)]
        s.r[4]=file['lba']+150;s.r[5]=count;s.r[6]=dest;s.r[7]=mode;s.r[15]=0x8cf00000
        s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c644ff6
        events=[];actual=[];errors=[]
        def hook(target):
            def invoke(m,r,sr):
                if target==0x8cba0000:
                    words=struct.unpack_from('<4I',m,r[5]&0xffffff);events.append((r[4],words));r[0]=1
                else:r[0]=0
                return sr
            return invoke
        @Call
        def call(ctx,target,m,f,sr,r,pr):
            try:
                if target==0x8c10d938:return request(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
                if target==0x8cba0000:
                    assert r[6]==0 and r[7]==0
                    actual.append((r[4],struct.unpack_from('<4I',actual_ram,r[5]&0xffffff)))
                    handle=U();result=submit(q,m,r[4],r[5],ctypes.byref(handle))
                    assert result==0;r[0]=handle.value
                else:assert target in (0x8c5fc0d8,0x8c5fc336);r[0]=0
                return 0
            except BaseException as exc:errors.append(repr(exc));return -9
        r=(U*16)(*s.r);sr=U(s.sr);pr=U(s.pr);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
        reference(ram,s,fpu=f,hooks={t:hook(t) for t in (0x8cba0000,0x8c5fc0d8,0x8c5fc336)})
        assert native(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
        assert ram==actual_ram and list(s.r)==list(r) and s.sr==sr.value and s.pr==pr.value and bytes(f)==bytes(nf)
        assert actual==events and actual[0][0]==16+mode
        for _ in range(count):assert step(q)==0
        digest=hashlib.sha256(actual_ram[0x800000+alignment:0x800000+alignment+file['size']]).hexdigest()
        assert digest==file['sha256'] and not io_errors
        cases+=1
        if file['path'] not in [p['path'] for p in proof]:proof.append({'path':file['path'],'sha256':digest,'bytes':file['size']})
    # Malformed guest spans/parameters leave the queue and guest RAM unchanged.
    q=ctypes.create_string_buffer(lib.sa_disc_queue_size());assert init(q,read,None,offset,32,base,stride)==0
    memory=bytearray(SIZE);view=(ctypes.c_uint8*SIZE).from_buffer(memory)
    for command,param,words in ((15,0x8c000100,(base+150,1,0x8c001000,0)),
        (16,0x8cffffff,(base+150,1,0x8c001000,0)),(16,0x8c000100,(149,1,0x8c001000,0)),
        (16,0x8c000100,(base+150,1,0x8cffff00,0)),(17,0x8c000100,(base+150,1,0,0)),
        (17,0x8c000100,(base+150,1,0x8c001000,1)),(16,0x8c000100,(base+150,1,0x4c001000,0))):
        struct.pack_into('<4I',memory,0x100,*words);before=bytes(q);before_ram=bytes(memory);handle=U(99)
        assert submit(q,view,command,param,ctypes.byref(handle))<0
        assert bytes(q)==before and bytes(memory)==before_ram and handle.value==99;guards+=1
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
report={'retail_formatter_cases_passed':cases,'host_guard_cases_passed':guards,'retail_files_compared':proof,
 'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'dll_sha256':sha(dll),'verifier_sha256':sha(Path(__file__)),'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in ('native_disc_queue.c','native_disc_queue.h','native_disc_sector_reader.c','native_disc_read_command.c','sa_file_device_read.c','sa_file_bios_request.c')},
 'scope':'Retail formatter and request tail match RAM/GPR/SR/PR/FPU at return; native operation-0 read submission queues real disc reads. Both commands, three aliases and aligned/unaligned buffers. No whole firmware ABI claim.',
 'limitations':'Completion/status/cancel vector adapters, DMA/cache hardware semantics and full-game execution remain unfinished.'}
(PROJECT/'analysis/native_disc_read_command_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'retail formatter/native queue cases,',guards,'invalid-command guards and',len(proof),'original file hashes passed')
