import ctypes,hashlib,json,shlex
from pathlib import Path
from verify_startup import ROOT
PROJECT=ROOT/'outputs/sonic-adventure';P=ctypes.POINTER(ctypes.c_uint8)
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,P,ctypes.c_uint32)
dll=ROOT/'work/sonic-adventure/build/native_disc_queue.dll';lib=ctypes.CDLL(str(dll))
U=ctypes.c_uint32;I=ctypes.c_int32;Q=ctypes.c_void_p
lib.sa_disc_queue_size.restype=U
specs={'init':[Q,Read,Q,ctypes.c_uint64,ctypes.c_uint64,U,U],
 'submit':[Q,U,U,P,ctypes.c_uint64,ctypes.POINTER(U)],'step':[Q],
 'status':[Q,U,ctypes.POINTER(U),ctypes.POINTER(U),ctypes.POINTER(I)],'cancel':[Q,U],'release':[Q,U]}
for name,args in specs.items():
    fn=getattr(lib,'sa_disc_queue_'+name);fn.argtypes=args;fn.restype=ctypes.c_int
init,submit,step,status,cancel,release=[getattr(lib,'sa_disc_queue_'+n) for n in ('init','submit','step','status','cancel','release')]
def queue(read,sectors=64,base=0,stride=2048,offset=0):
    q=ctypes.create_string_buffer(lib.sa_disc_queue_size())
    assert init(q,read,None,offset,sectors,base,stride)==0
    return q
def request(q,lba,count):
    out=(ctypes.c_uint8*(count*2048))(*([0xaa]*(count*2048)));handle=U()
    assert submit(q,lba,count,out,len(out),ctypes.byref(handle))==0
    return handle.value,out
def check(q,handle,want):
    state,done,error=U(),U(),I()
    assert status(q,handle,ctypes.byref(state),ctypes.byref(done),ctypes.byref(error))==0
    assert (state.value,done.value,error.value)==want
calls=[]
@Read
def read(user,pos,out,size):calls.append(pos);ctypes.memset(out,(pos//2048)&255,size);return 0
q=queue(read);a,oa=request(q,1,3);b,ob=request(q,10,2)
check(q,a,(1,0,0));check(q,b,(1,0,0));assert not calls
assert step(q)==0 and step(q)==0
check(q,a,(1,1,0));check(q,b,(1,1,0));assert calls==[2048,20480]
assert cancel(q,a)==0 and cancel(q,a)==0
check(q,a,(3,1,0));assert bytes(oa[:2048])==b'\x01'*2048 and bytes(oa[2048:])==b'\xaa'*4096
assert step(q)==0;check(q,b,(2,2,0));assert calls==[2048,20480,22528]
assert step(q)==0 and cancel(q,b)==0;check(q,b,(2,2,0))
assert release(q,a)==0;state,done,error=U(),U(),I()
assert status(q,a,ctypes.byref(state),ctypes.byref(done),ctypes.byref(error))==-7
c,oc=request(q,20,1);assert c not in (a,b)
assert cancel(q,a)==-7 and release(q,a)==-7 and release(q,c)==-6
assert step(q)==0;check(q,c,(2,1,0));assert bytes(oc)==b'\x14'*2048
fairness_cases=1

# Capacity, zero reads, failed submissions and separate queue ownership.
full=queue(read);buffers=[];handles=[]
for n in range(8):h,out=request(full,n,0);handles.append(h);buffers.append(out)
before=bytes(full);h=U(123);out=(ctypes.c_uint8*2048)()
assert submit(full,0,1,out,2048,ctypes.byref(h))==-5 and h.value==123 and bytes(full)==before
for n in range(8):assert step(full)==0;check(full,handles[n],(2,0,0))
assert submit(full,0,1,out,2048,ctypes.byref(h))==-5
assert release(full,handles[0])==0
assert submit(full,64,1,out,2048,ctypes.byref(h))==-3
assert submit(full,0,1,out,2047,ctypes.byref(h))==-3
assert submit(full,0,1,P(),2048,ctypes.byref(h))==-2
assert submit(full,0,1,out,2048,None)==-2
other=queue(read);d,od=request(other,5,1);assert d==1
assert step(other)==0;check(other,d,(2,1,0));check(full,handles[1],(2,0,0))
assert init(None,read,None,0,8,0,2048)==-2
assert init(full,Read(),None,0,8,0,2048)==-2
assert init(full,read,None,0,8,0,2448)==-2

# Per-request I/O errors and callback reentry cannot complete/cancel another job.
errors=[];reentry=[]
@Read
def failing(user,pos,out,size):
    reentry.extend((step(failed),cancel(failed,fid),release(failed,fid)))
    if pos==2048:return -17
    ctypes.memset(out,0x44,size);return 0
failed=queue(failing);fid,fo=request(failed,0,3)
assert step(failed)==0;check(failed,fid,(1,1,0))
assert step(failed)==0;check(failed,fid,(4,1,-17))
assert reentry==[-6]*6 and bytes(fo[:2048])==b'D'*2048 and bytes(fo[2048:])==b'\xaa'*4096
assert cancel(failed,fid)==0;check(failed,fid,(4,1,-17));assert release(failed,fid)==0

# Actual source-track reads: four simultaneous requests, independent hash proof.
manifest=json.loads((PROJECT/'disc_inventory.json').read_text())
track=next(t for t in reversed([shlex.split(s) for s in manifest['gdi'].splitlines()[1:] if s.strip()]) if t[2]=='4')
path=Path(manifest['source'])/track[4];base,stride,offset=int(track[1]),int(track[3]),int(track[5])
files=[next(f for f in manifest['files'] if f['path']=='1ST_READ.BIN')]
files+=sorted((f for f in manifest['files'] if not f['directory'] and 0<f['size']<65536),key=lambda f:f['path'])[:3]
proof=[];steps=0
with path.open('rb') as stream:
    @Read
    def real_read(user,pos,out,size):
        try:
            stream.seek(pos);data=stream.read(size)
            if len(data)!=size:return -8
            ctypes.memmove(out,data,size);return 0
        except BaseException as exc:errors.append(repr(exc));return -8
    real=queue(real_read,(path.stat().st_size-offset)//stride,base,stride,offset)
    jobs=[(file,*request(real,file['lba'],(file['size']+2047)//2048)) for file in files]
    for _ in range(sum((file['size']+2047)//2048 for file in files)):
        assert step(real)==0;steps+=1
    assert not errors
    for file,handle,data in jobs:
        check(real,handle,(2,(file['size']+2047)//2048,0))
        digest=hashlib.sha256(bytes(data[:file['size']])).hexdigest();assert digest==file['sha256']
        proof.append({'path':file['path'],'bytes':file['size'],'sha256':digest})
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
report={'scenario_groups_passed':['round_robin_and_partial_cancel','slot_capacity_and_release','stale_handles','zero_length','invalid_inputs','separate_queues','transport_error','callback_reentry','retail_multi_request_reads'],
 'retail_files_compared':proof,'retail_steps':steps,'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in ('native_disc_queue.c','native_disc_queue.h','native_disc_sector_reader.c','native_disc_read_command.c')},
 'dll_sha256':sha(dll),'verifier_sha256':sha(Path(__file__)),'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'limitations':'New host API, not the firmware ABI. Single-threaded; callers retain buffers until release. No game-vector binding, FAD translation, audio, DMA or timing equivalence.'}
(PROJECT/'analysis/native_disc_queue_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(report['scenario_groups_passed']),'queue scenario groups and',len(proof),'original-disc file hashes passed;',steps,'native queue steps')
