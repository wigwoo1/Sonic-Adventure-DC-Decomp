import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c067d84;LENGTH=20;LITERAL=0x8c067ddc;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'byte_table_bit_test_8c067d84.dll'));native=lib.sa_byte_table_bit_test_8c067d84;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);special=[0x8c780000,0x8c7ffff0,LITERAL,FUNCTION,BASE,0x8cffffff];cases=[]
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 if case<len(special):table=special[case];index=0
 else:table=0x8c740000+(case-6)*0x100;index=case%256
 address=(table+index)&0xffffffff;ai=(address&0x1fffffff)-0x0c000000;assert 0<=ai<RAM_SIZE,(case,hex(table),hex(index),hex(address),hex(ai))
 if table not in [LITERAL,FUNCTION,BASE]:ram[ai]=(case*73+0x5a)&0xff
 struct.pack_into('<I',ram,LITERAL-0x8c000000,table)
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=index;s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'table':hex(table),'index':hex(index),'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_byte_table_bit_test_8c067d84.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Exact 20-byte retail byte-table bit test; varied literal bases, offsets, byte values, code/literal aliases, and mapped RAM boundaries.','limitations':['Table contents and caller-specific index domain remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'byte_table_bit_test_8c067d84_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} byte table bit-test RAM/CPU comparisons passed')
