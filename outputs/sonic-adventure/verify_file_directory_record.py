import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_directory_parser.dll'))
helpers={0x8c64535c:lib.sa_file_directory_bytes_copy}
names=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll'))
for address,name in [(0x8c645442,'sa_file_name_checksum'),(0x8c645348,'sa_file_name_upper')]:helpers[address]=getattr(names,name)
for helper in helpers.values():helper.argtypes=abi
@Call
def call(ctx,target,m,f,status,regs,pr):return helpers[target](m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
fixtures=[]
for name,flags,alignment,bank in itertools.product([b'E_00B7.MLT;1',b'other.bin;1',b'\0',b'\1'],[0,2],range(4),[0,0x200000]):
 if flags==0 and len(name)<2:continue
 data=bytearray(64);data[0]=34+len(name);struct.pack_into('<I',data,2,1234);struct.pack_into('<I',data,10,13120);data[25]=flags;data[32]=len(name);data[33:33+len(name)]=name
 fixtures.append((0x8c645468,'sa_file_directory_record_parse',data,alignment,bank,data[0]))
cases=0
for pc,name,data,alignment,bank,expected in fixtures:
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 offset=0xa00000+alignment;ram[offset:offset+len(data)]=data
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8c000000+offset;s.r[5]=0x8cb00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=pc
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 fn=getattr(lib,name);fn.argtypes=abi
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert r[0]==expected
 assert struct.unpack_from('<II',native,0xb00000)==(1234,13120)
 rawname=bytes(data[33:33+data[32]])
 outname=rawname if data[25]&2 else rawname[:-2]
 if data[25]&2 and rawname==b'\0':outname=b'.'
 if data[25]&2 and rawname==b'\1':outname=b'..'
 assert native[0xb0000a:0xb0000b+len(outname)]==outname+b'\0'
 seed=struct.unpack_from('<h',binary,0x64554e-0x10000)[0]
 assert native[0xb00009]==((seed+sum(outname.upper()))&0xffffffff)>>2&255
 cases+=1
report={'cases_passed':cases,'scope':'Native record conversion, byte copy, filename checksum and uppercase against full retail RAM/CPU/FPU/SR/PR without hooks. File/directory names, suffix stripping, special dot entries, unaligned source and FPU banks; independent decoded fields, name and checksum. Directory loader compiled but not behaviorally verified.'}
(root/'analysis/file_directory_record_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'directory record comparisons passed')
