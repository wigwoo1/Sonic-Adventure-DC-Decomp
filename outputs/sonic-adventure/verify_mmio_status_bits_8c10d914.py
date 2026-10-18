import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c10d914;LENGTH=18;ADDRESS_LITERAL=0x8c10d934;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'mmio_status_bits_8c10d914.dll'));CALLBACK=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32);native=lib.sa_mmio_status_bits_8c10d914;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.c_void_p,CALLBACK];native.restype=None
rng=random.Random(FUNCTION);addresses=[0xff800030,0xff80002e,0xff800040,0xff800100];values=[0,1,0xffff,0x8000,0x7fff,0x0300,0x0100,0x0200,0x0300,0xff00,0x00ff];cases=[]
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 address=addresses[case%len(addresses)] if case<len(addresses) else 0xff800000+rng.randrange(0,0x400)*2;struct.pack_into('<I',ram,ADDRESS_LITERAL-0x8c000000,address)
 value=values[case%len(values)] if case<len(values) else rng.getrandbits(16);seen={'address':None,'calls':0};ctx=ctypes.c_uint32(value)
 @CALLBACK
 def read_register(c,a):seen['address']=a;seen['calls']+=1;return ctypes.cast(c,ctypes.POINTER(ctypes.c_uint32)).contents.value
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[15]=0x8cf00000;s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es,mmio={address:value})
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual),ctypes.byref(ctx),read_register)
 assert ar==er,f'RAM mismatch case={case}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}';assert seen=={'address':address,'calls':1},f'callback mismatch {case}:{seen}'
 cases.append({'case':case,'address':hex(address),'mmio_value':hex(value),'status_bits':actual.r[0],'ram_cpu_callback_match':True})
source=ROOT/'source'/'sa_mmio_status_bits_8c10d914.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state; MMIO callback and stack write checked','scope':'Exact 18-byte retail status query including nested MMIO helper call, PR save/restore, SR stack write, and masked bits 8-9.','limitations':['The device register hardware semantics and system-level meaning of its two status bits remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'mmio_status_bits_8c10d914_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} MMIO status bits RAM/CPU/callback comparisons passed')
