import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/heap_allocate_ram.dll')).sa_heap_allocate_ram_8c098a80;fn.restype=ctypes.c_uint32;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];rng=random.Random(0x98a80);cases=0
 for case in range(2048):
  count=rng.randrange(1,12);blocks=[0x8ca00000]
  for i in range(count):blocks.append(blocks[-1]+rng.randrange(1,32)*16)
  free=rng.sample(blocks[:-1],rng.randrange(count+1));free.append(blocks[-1])
  size=rng.choice([0,1,15,16,17,32,255,4096,0x7fffffff,0xffffffe0,0xffffffe1,0xffffffff,rng.randrange(1024)])
  sr0=rng.getrandbits(32)
  for mem in [old,native]:
   mem[0xa00000:0xa04000]=bytes([0xa5])*16384
   for i,b in enumerate(blocks):struct.pack_into('<4I',mem,b&0xffffff,0,0,blocks[i+1] if i+1<len(blocks) else 0,blocks[i-1] if i else 0)
   for i,b in enumerate(free):struct.pack_into('<2I',mem,b&0xffffff,free[i+1] if i+1<len(free) else 0,free[i-1] if i else 0)
   struct.pack_into('<I',mem,0x79f03c,free[0])
  state=State();state.pc=0x8c098a80;state.pr=0x8c000100;state.r[4]=size;state.sr=sr0;reference(old,state)
  sr=ctypes.c_uint32(sr0);result=fn(view,ctypes.byref(sr),size)
  assert result==state.r[0] and sr.value==state.sr,(case,size,result,state.r[0],hex(sr.value),hex(state.sr))
  assert native[0xa00000:0xa04000]==old[0xa00000:0xa04000] and native[0x79f03c:0x79f040]==old[0x79f03c:0x79f040],(case,size)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/heap_allocate_ram_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/heap_allocate_ram.c').read_bytes()).hexdigest(),scope='Retail RAM allocator through return without hooks: randomized physical block sizes and free-list ordering, exact/split/exhausted/sentinel paths, zero and wrapping requests; r0/SR and modified heap metadata compared.'),indent=2)+'\n');print(f'{cases} RAM allocator comparisons passed')
if __name__=='__main__':main()
