import ctypes,hashlib,json,struct,random
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);rng=random.Random(0x56712)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_descriptor_lookup.dll')).sa_model_descriptor_lookup_8c056712;fn.restype=ctypes.c_uint32;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*3;cases=0
 for count in [1,3,8,32,128]:
  for shape in ['child','sibling','tree']:
   children=[[] for _ in range(count)]
   for i in range(1,count):children[0 if shape=='sibling' else (i-1 if shape=='child' else rng.randrange(i))].append(i)
   links=[[0,0] for _ in range(count)]
   if shape=='sibling':
    for i in range(count-1):links[i][1]=0x8ca00000+(i+1)*64
   else:
    for i,items in enumerate(children):
     if items:links[i][0]=0x8ca00000+items[0]*64
     for a,bn in zip(items,items[1:]):links[a][1]=0x8ca00000+bn*64
   for target in [0]+[0x8ca00000+i*64 for i in range(count)]+[0x8ca10000]:
    for sr0 in [0,0x700083f3]:
     for m in [old,native]:
      m[0xa00000:0xa04000]=bytes(16384)
      for i,pair in enumerate(links):struct.pack_into('<2I',m,0xa0002c+i*64,*pair)
     state=State();state.r[4]=0x8ca00000;state.r[5]=target;state.r[15]=0x8cf00000;state.r[12:15]=[0x12345678,0xabcdef12,0x31415926];state.pc=0x8c056712;state.pr=0x8c000100;state.sr=sr0;reference(old,state)
     sr=ctypes.c_uint32(sr0);result=fn(view,ctypes.byref(sr),0x8ca00000,target,0x8cf00000)
     assert result==state.r[0] and sr.value==state.sr and list(state.r[12:15])==[0x12345678,0xabcdef12,0x31415926],(count,shape,target)
     assert native[0xeffff4:0xeffffc]==old[0xeffff4:0xeffffc];cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_descriptor_lookup_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_descriptor_lookup.c').read_bytes()).hexdigest(),scope='Lookup wrapper and recursive search execute without hooks, child/sibling/random trees sizes1/3/8/32/128; every target plus missing/null, r0/SR and wrapper counter/root RAM. Original callee preservation checked. Valid acyclic trees; nested callee-save scratch outside C API.'),indent=2)+'\n');print(f'{cases} native descriptor lookup comparisons passed')
if __name__=='__main__':main()
