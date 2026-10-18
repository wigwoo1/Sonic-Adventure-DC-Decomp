import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_aiff_parse.dll')).sa_sound_aiff_parse;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
cases=0
for variant,channels,frames,bits,rate,bank in itertools.product(['valid','odd_chunk','bad_form','bad_type','short_comm'],[1,2],[0,3,0xffffffff],[8,16],[22050,44100],[0,0x200000]):
 sr0=0x70000063
 def chunk(tag,data):return tag+struct.pack('>I',len(data))+data+(b'\x00' if len(data)&1 else b'')
 comm=struct.pack('>HIH',channels,frames,bits)+struct.pack('>H',0x400e if rate==44100 else 0x400d)+bytes.fromhex('ac44000000000000')
 if variant=='short_comm':comm=comm[:17]
 prefix=chunk(b'JUNK',b'abc') if variant=='odd_chunk' else b''
 body=b'AIFF'+prefix+chunk(b'COMM',comm)+chunk(b'SSND',struct.pack('>II',3,0)+bytes(range(20)))
 data=b'FORM'+struct.pack('>I',len(body))+body
 if variant=='bad_form':data=b'XXXX'+data[4:]
 if variant=='bad_type':data=data[:8]+b'XXXX'+data[12:]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa10000:0xa10000+len(data)]=data;struct.pack_into('<4I',ram,0xa20000,*([0x55555555]*4));struct.pack_into('<I',ram,0xf00000,0x8ca2000c)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):raise AssertionError(('unexpected service',a))
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca10000;s.r[5]=0x8ca20000;s.r[6]=0x8ca20004;s.r[7]=0x8ca20008;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c669528
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 valid=variant in ('valid','odd_chunk')
 expected_pointer=0x8ca10000+12+len(prefix)+len(chunk(b'COMM',comm))+8+4+3 if valid else 0
 assert r[0]==expected_pointer,(variant,hex(r[0]),hex(expected_pointer),struct.unpack_from('<4I',native,0xa20000))
 assert struct.unpack_from('<4I',native,0xa20000)==((rate,channels,bits,frames) if valid else (0x55555555,)*4)
 assert native[0xa10000:0xa10000+len(data)]==data
 cases+=1
(out/'analysis/sound_aiff_parse_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native356-instruction AIFF parser669528.240 full retail RAM/CPU/FPU/SR/PR comparisons and independent extracted rate/channels/sample width/frame count/retail pointer computed SSND body+4+offset (four bytes before conventional sample start). Constructed FORM/AIFF COMM/SSND,odd JUNK padding,bad FORM/type,short COMM; channels1/2,frames0/3/FFFFFFFF,bits8/16,rates22050/44100,both banks. Input preservation. Bounds safety,truncated buffers,all chunk permutations and real game integration unverified.'),indent=2)+'\n')
print(cases,'AIFF parser comparisons passed')
