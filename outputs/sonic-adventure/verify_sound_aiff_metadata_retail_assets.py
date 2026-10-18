import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_aiff_metadata.dll')).sa_sound_aiff_metadata;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
cases=0
parser=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_aiff_parse.dll')).sa_sound_aiff_parse;parser.argtypes=abi
assets=json.loads((out/'analysis/aiff_asset_inventory.json').read_text())['assets'];records=[]
for asset,bank,force_size in itertools.product(assets,[0,0x200000],[False,True]):
 sr0=0x70000063;path=ROOT/asset['path'];data=path.read_bytes();chunks={};cursor=12
 while cursor+8<=len(data):
  tag=data[cursor:cursor+4];size=struct.unpack_from('>I',data,cursor+4)[0]
  assert cursor+8+size<=len(data),(path,tag,size)
  chunks.setdefault(tag,(cursor+8,data[cursor+8:cursor+8+size]));cursor+=8+size+(size&1)
 _,comm=chunks[b'COMM'];ssnd_start,ssnd=chunks[b'SSND']
 channels,frames,bits=struct.unpack_from('>HIH',comm)
 exponent=struct.unpack_from('>H',comm,8)[0]-16383;mantissa=int.from_bytes(comm[10:18],'big')
 rate=mantissa>>(63-exponent) if exponent<=63 else mantissa<<(exponent-63)
 offset=struct.unpack_from('>I',ssnd)[0]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa10000:0xa10000+len(data)]=data;ram[0xa20000:0xa20020]=bytes([0x55])*32;struct.pack_into('<6I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24,28)])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c669528,a
  return parser(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca10000;s.r[5]=max(4096,len(data)) if force_size else len(data);s.r[6]=0x8ca20000;s.r[7]=0x8ca20004;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c66983e
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 accepted=force_size or len(data)>=4096
 expected=bytearray([0x55])*32
 struct.pack_into('<H',expected,0,ssnd_start+4+offset if accepted else 0)
 if accepted:
  expected[4]=255;expected[8]=bits&255;expected[12]=(channels*bits//8)&255;expected[16]=channels&255
  struct.pack_into('<3I',expected,20,rate,frames,1)
 assert r[0]==(0 if accepted else 0xffffffff)
 assert native[0xa20000:0xa20020]==expected,(path,force_size,bytes(native[0xa20000:0xa20020]),expected)
 assert native[0xa10000:0xa10000+len(data)]==data
 records.append(dict(path=asset['path'],sha256=hashlib.sha256(data).hexdigest(),fpscr_bank=bank,input_size=max(4096,len(data)) if force_size else len(data),accepted=accepted,retail_data_offset=ssnd_start+4+offset))
 cases+=1
(out/'analysis/sound_aiff_metadata_retail_assets_verification.json').write_text(json.dumps(dict(cases_passed=cases,assets=records,scope='Native76-instruction AIFF metadata wrapper66983e with native parser.40 full retail RAM/CPU/FPU/SR/PR/MACL cases across10 original AIFF files,both banks,actual size or4096 minimum fixture size. Independent4096-byte threshold,16-bit returned offset,rate/channels/width/frames,byte stride,type1/FF marker and neighbor preservation. Original files unchanged. Wrapper propagates SSND body+4+offset without correction; playback/full game unverified.'),indent=2)+'\n')
print(cases,'retail AIFF metadata comparisons passed')
