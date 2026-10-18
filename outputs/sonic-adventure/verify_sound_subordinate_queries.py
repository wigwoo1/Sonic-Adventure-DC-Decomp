import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
specs=[('sound_subordinate_field8_get',0x8c65de00,8),('sound_subordinate_fieldc_get',0x8c65de1c,12),('sound_subordinate_volume_get',0x8c65de38,0x6c),('sound_subordinate_pan_get',0x8c65de56,None),('sound_subordinate_effect_get',0x8c65de7a,None)]
reporter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_error_report.dll')).sa_sound_error_report;reporter.argtypes=abi
def run(target=None):
 for stem,entry,offset in specs:
  if target and stem!=target:continue
  fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);fn.argtypes=abi;cases=0
  for present,value,selector,alias,bank in itertools.product([0,1],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1,2,3,0xffffffff],[0,1],[0,1]):
   ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
   words=[(value+i)&0xffffffff for i in range(32)];struct.pack_into('<32I',ram,0xa08000,*words);ram[0xa20000:0xa20010]=bytes([0x66])*16;struct.pack_into('<2I',ram,0x67c37c,0,0)
   before=bytes(ram[0xa08000:0xa08080]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
   first=0x8ca20004;second=first if alias else 0x8ca20008;pair=stem.endswith('effect_get')
   @Call
   def call(ctx,a,m,fp,srp,r,prp):
    assert a==0x8c667b7a and r[4]==0x8c67217c;errors.append(r[4]);return reporter(m,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca08000 if present else 0,first if pair else selector,second];s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x70000063
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
   reference(ram,s,{},fpu=f,special={'macl':0})
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   wanted=bytearray([0x66]*16)
   if pair:
    for pointer,result in [(first,words[30] if present else 0xffffffff),(second,words[31] if present else 0xffffffff)]:struct.pack_into('<I',wanted,(pointer&0xffffff)-0xa20000,result)
   else:
    selected=((0x70+4*selector)&0xffffffff) if offset is None else offset
    assert r[0]==(words[selected//4] if present else 0xffffffff)
   assert native[0xa20000:0xa20010]==wanted and native[0xa08000:0xa08080]==before and r[15]==0x8cf00000
   assert errors==([] if present else [0x8c67217c]);cases+=1
  scope='280 full retail RAM/CPU/FPU/SR/PR comparisons of sound-object query '+hex(entry)+'. Null/present objects,seven full-word fixtures,five channel/neighbor fixtures,both banks,two output-alias fixtures. Independent entire128-byte object preserved. Scalar getters return full word from8/c/6c;pan reads70+4*R5 modulo32 without channel validation (selectorffffffff reads6c). Effect getter writes78 then7c to output pointers;aliased outputs end with7c. Null paths report exact error string through native reporter with callback unset;scalar returnsffffffff and pair outputsffffffff. Entire16-byte output window/guards verified. No modeled calls. Invalid output pointers,application callback behavior,connected setter/query sequences/game integration/fullgame unverified.'
  (p/f'analysis/{stem}_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,stem,'comparisons passed')
if __name__=='__main__':run()
