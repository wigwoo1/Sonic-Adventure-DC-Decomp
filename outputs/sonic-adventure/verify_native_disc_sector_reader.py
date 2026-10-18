"""Native platform sector reader: synthetic layouts and original user data."""
import ctypes,hashlib,itertools,json,random,shlex
from pathlib import Path
from verify_startup import ROOT
PROJECT=ROOT/'outputs/sonic-adventure'
P=ctypes.POINTER(ctypes.c_uint8)
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,P,ctypes.c_uint32)
dll=ROOT/'work/sonic-adventure/build/native_disc_sector_reader.dll'
fn=ctypes.CDLL(str(dll)).sa_disc_read_sectors
fn.argtypes=[Read,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,ctypes.c_uint32,ctypes.c_uint32,
             ctypes.c_uint32,ctypes.c_uint32,P,ctypes.c_uint64,ctypes.POINTER(ctypes.c_uint32)]
fn.restype=ctypes.c_int
synthetic=guards=0
for stride,prefix,first,count in itertools.product((2048,2352),(0,17),(100,102,107),(0,1)):
    rng=random.Random(stride+prefix)
    payload=[rng.randbytes(2048) for _ in range(8)]
    track=bytearray(prefix+8*stride)
    offset=16 if stride==2352 else 0
    for i,data in enumerate(payload):track[prefix+i*stride+offset:prefix+i*stride+offset+2048]=data
    calls=[]
    @Read
    def read(user,pos,out,size):
        assert user==123 and size==2048
        calls.append(pos);ctypes.memmove(out,bytes(track[pos:pos+size]),size);return 0
    output=(ctypes.c_uint8*(count*2048+16))(*([0xaa]*(count*2048+16)))
    dest=ctypes.cast(ctypes.byref(output,8),P);done=ctypes.c_uint32(99)
    assert fn(read,123,prefix,8,100,stride,first,count,dest,count*2048,ctypes.byref(done))==0
    assert done.value==count and bytes(output[:8])==b'\xaa'*8 and bytes(output[-8:])==b'\xaa'*8
    assert bytes(output[8:-8])==b''.join(payload[first-100:first-100+count])
    assert calls==[prefix+(first-100+i)*stride+offset for i in range(count)]
    synthetic+=1

# Rejected ranges and unavailable callbacks perform no I/O and preserve output.
calls=[]
@Read
def unexpected(user,pos,out,size):calls.append(pos);return -9
for prefix,sectors,base,stride,first,count,capacity in (
 (0,8,100,2352,99,1,2048),(0,8,100,2352,108,1,2048),
 (0,8,100,2352,107,2,4096),(0,8,100,2352,100,1,2047),
 (0,8,100,2448,100,1,2048),((1<<64)-1,8,100,2352,100,1,2048),
 (0,8,100,2048,109,0,0)):
    output=(ctypes.c_uint8*4096)(*([0xaa]*4096));done=ctypes.c_uint32(99)
    assert fn(unexpected,None,prefix,sectors,base,stride,first,count,output,capacity,ctypes.byref(done))<0
    assert done.value==0 and not calls and bytes(output)==b'\xaa'*4096;guards+=1
for read,out,completed in ((Read(),output,ctypes.byref(done)),(unexpected,P(),ctypes.byref(done)),(unexpected,output,None)):
    assert fn(read,None,0,8,100,2048,100,1,out,2048,completed)==-2
    assert not calls;guards+=1
# Host I/O failure exposes completed sectors and preserves its error code.
@Read
def failing(user,pos,out,size):
    if pos>=4096:return -17
    ctypes.memset(out,0x42,size);return 0
output=(ctypes.c_uint8*8192)(*([0xaa]*8192));done=ctypes.c_uint32()
assert fn(failing,None,0,8,0,2048,0,4,output,8192,ctypes.byref(done))==-17
assert done.value==2 and bytes(output[:4096])==b'B'*4096 and bytes(output[4096:])==b'\xaa'*4096;guards+=1
# 64-bit file offsets reach the I/O service without truncation.
positions=[]
@Read
def large(user,pos,out,size):positions.append(pos);ctypes.memset(out,0x43,size);return 0
assert fn(large,None,1<<40,8,0,2048,1,1,output,8192,ctypes.byref(done))==0
assert positions==[(1<<40)+2048] and done.value==1;synthetic+=1

manifest=json.loads((PROJECT/'disc_inventory.json').read_text())
tracks=[shlex.split(line) for line in manifest['gdi'].splitlines()[1:] if line.strip()]
track=next(t for t in reversed(tracks) if t[2]=='4')
base,stride,prefix=int(track[1]),int(track[3]),int(track[5])
path=Path(manifest['source'])/track[4]
sectors=(path.stat().st_size-prefix)//stride
real=[];io_errors=[]
with path.open('rb') as stream:
    @Read
    def real_read(user,pos,out,size):
        try:
            stream.seek(pos);data=stream.read(size)
            if len(data)!=size:return -8
            ctypes.memmove(out,data,size);return 0
        except BaseException as exc:io_errors.append(repr(exc));return -8
    files=[next(f for f in manifest['files'] if f['path']=='1ST_READ.BIN')]
    files+=sorted((f for f in manifest['files'] if not f['directory'] and 0<f['size']<65536),key=lambda f:f['path'])[:3]
    for file in files:
        count=(file['size']+2047)//2048;output=(ctypes.c_uint8*(count*2048))();done=ctypes.c_uint32()
        assert fn(real_read,None,prefix,sectors,base,stride,file['lba'],count,output,len(output),ctypes.byref(done))==0
        assert done.value==count and not io_errors
        digest=hashlib.sha256(bytes(output[:file['size']])).hexdigest()
        assert digest==file['sha256'],file['path']
        real.append({'path':file['path'],'bytes':file['size'],'sectors':count,'sha256':digest})
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
report={'synthetic_cases_passed':synthetic,'host_guard_cases_passed':guards,'retail_files_compared':real,
 'source_sha256':sha(PROJECT/'source/native_disc_sector_reader.c'),'dll_sha256':sha(dll),
 'verifier_sha256':sha(Path(__file__)),'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'track':{'path':str(path),'base_lba':base,'stride':stride,'file_offset':prefix,'sectors':sectors},
 'scope':'New native platform support, not recovered game code. Reads actual user track bytes through an explicit host read-at service; matches existing extraction hashes.',
 'limitations':'2048-byte cooked and 2352-byte MODE1 payload layouts only. No BIOS command queue, timing, DMA, audio, MODE2 or whole-game integration.'}
(PROJECT/'analysis/native_disc_sector_reader_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(synthetic,'synthetic cases,',guards,'host guards and',len(real),'original-disc files passed')
