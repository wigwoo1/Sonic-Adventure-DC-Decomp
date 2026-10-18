from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_work_native_sizes.py').read_text()
allocation=(p/'verify_stream_native_allocation.py').read_text()
globals=allocation[allocation.index('b=binary;lower={}'):allocation.index('pool=0x8c8d61ac;size=0xac')]
globals="abi=fn.argtypes\ndef load(dll,name):\n f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);f.argtypes=abi;return f\n"+globals
globals+="helpers.update(lower)\nfor address,dll,name in [(0x8c6673b4,'sound_stream_create.dll','sa_sound_stream_create'),(0x8c667440,'sound_stream_destroy.dll','sa_sound_stream_destroy'),(0x8c10d31c,'byte_fill.dll','sa_byte_fill')]:helpers[address]=load(dll,name)\nstream_pool=0x8d61ac\n"
s=s.replace('pool=0x8a3910;cases=0',globals+'pool=0x8a3910;cases=0')
s=s.replace('[0x8ca70000]','[0x8c8d61ac]')
s=s.replace(' ram[0xa70000:0xa70100]=bytes([0x55])*256;stream_before=bytes(ram[0xa70000:0xa70100])', " ram[stream_pool:stream_pool+172*8]=bytes([0x55])*(172*8)\n for index in range(8):struct.pack_into('<H',ram,stream_pool+index*172,0 if index==0 else 1)\n stream_before=bytes(ram[stream_pool:stream_pool+172*8])\n hs=handle_pool&0xffffff;ram[hs:hs+52*8]=bytes([0x77])*(52*8)\n for index in range(8):struct.pack_into('<I',ram,hs+index*52,0 if index==0 else 1)\n handles_before=bytes(ram[hs:hs+52*8])")
s=s.replace("reference(ram,s,{a:hook(a) for a in services if a not in helpers},fpu=f,special={'macl':0})", "special={'macl':0}\n reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)")
s=s.replace(' assert native==ram', ' nf.fpul=fpul.value\n assert native==ram')
s=s.replace('assert len(actual)==(1 if slot<8 else 0)','assert actual==expected==[]')
start=s.index(' wanted_stream=bytearray(stream_before)');end=s.index(' cases+=1',start)
s=s[:start]+''' wanted_stream=bytearray(stream_before);wanted_handles=bytearray(handles_before)
 if slot<8:
  obj=bytearray(172);struct.pack_into('<H',obj,0,1);struct.pack_into('<I',obj,8,handle_pool)
  struct.pack_into('<4I',obj,0x38,channels,0x8ca40000,512,640)
  struct.pack_into('<4I',obj,0x88,0x8c65b88c,selected,0x8c6673a2,created)
  wanted_stream[:172]=obj
  obj=bytearray(52);struct.pack_into('<2I',obj,0,1,0)
  obj[48:52]=native[hs+48:hs+52]
  wanted_handles[:52]=obj
 assert native[stream_pool:stream_pool+172*8]==wanted_stream
 assert native[hs:hs+52*8]==wanted_handles
'''+s[end:]
s=s.replace('work_native_sizes_verification','work_native_allocation_verification')
start=s.index("scope='");end=s.index("'),indent=2)",start)
s=s[:start]+"scope='54 full retail RAM/CPU/FPU/SR/PR comparisons of native work constructor65b624 with stream creation6673b4,decode-handle allocation,byte fill,coefficient math,source accessors,callback registration and work reset connected; no modeled calls. Work slots0..7 and exhausted work pool,channels0/1/2,both banks,first stream/handle slots available. Independent whole1152-byte work pool,1376-byte stream pool and decode-handle pool initialization except coefficients independently covered by dedicated coefficient verifier;callback/user fields,output arguments,source object unchanged,return/stack. Other stream/handle free-slot combinations and exhaustion/null-callback path not covered here. Playback/fullgame unverified."+s[end:]
(p/'verify_work_native_allocation.py').write_text(s)
