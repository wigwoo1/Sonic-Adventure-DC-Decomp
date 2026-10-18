import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/model_descriptor_walk.dll';lib=ctypes.CDLL(str(dll));rng=random.Random(0x567b0);cases=0
 for internal in [False,True]:
  fn=getattr(lib,'sa_model_descriptor_walk_8c0567d4' if internal else 'sa_model_descriptor_collect_8c0567b0');fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*(2 if internal else 3);fn.restype=ctypes.c_uint32
  for count in [1,2,3,8,32,128]:
   for shape in ['child','sibling','random']:
    for sr0 in [0,0x700083f3]:
     nodes=[0x8ca01000+i*64 for i in range(count)];links={n:[0,0] for n in nodes}
     for i in range(1,count):
      if shape=='child':parent=i-1;slot=0
      elif shape=='sibling':parent=i-1;slot=1
      else:
       free=[(p,s) for p in range(i) for s in range(2) if not links[nodes[p]][s]];parent,slot=rng.choice(free)
      links[nodes[parent]][slot]=nodes[i]
     for mem in [old,native]:
      mem[0xa00000:0xa04000]=bytes([0xa5])*0x4000;struct.pack_into('<I',mem,0xa00000,nodes[0]);struct.pack_into('<I',mem,0xa00800,0)
      for n,(child,sibling) in links.items():struct.pack_into('<2I',mem,(n&0xffffff)+44,child,sibling)
     state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[4]=0x8ca00000 if internal else nodes[0];state.r[5]=0x8ca00800 if internal else 0x8ca00000;state.r[15]=0x8cf00000;state.pc=0x8c0567d4 if internal else 0x8c0567b0;state.pr=0x8c000100;state.sr=sr0;saved=list(state.r[8:]);reference(old,state)
     sr=ctypes.c_uint32(sr0);args=[0x8ca00000,0x8ca00800] if internal else [nodes[0],0x8ca00000,0x8cf00000];result=fn(view,ctypes.byref(sr),*args)
     assert result==state.r[0]==count and sr.value==state.sr and saved==list(state.r[8:])
     assert old[0xa00000:0xa04000]==native[0xa00000:0xa04000]
     if not internal:assert old[0xeffff4:0xeffff8]==native[0xeffff4:0xeffff8]
     cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_descriptor_walk.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),retail_span_sha256=hashlib.sha256(binary[0x467b0:0x46824]).hexdigest(),scope='Collector and recursive traversal with original recursive calls unhooked. Child chains, sibling chains and deterministic binary trees up to 128 nodes; list ordering, fixture RAM, count, r0/SR, preserved registers and collector counter scratch. Valid acyclic graphs and nonnull output lists; invalid/cyclic inputs unverified.')
 (out/'analysis/model_descriptor_walk_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} recursive descriptor comparisons passed')
if __name__=='__main__':main()
