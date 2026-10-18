import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_rate_table_set.dll')).sa_sound_rate_table_set;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
helpers={}
for a,dll,name in [(0x8c66b030,'sound_group_pitch_command','sa_sound_group_pitch_command'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 h=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dll}.dll')),name);h.argtypes=abi;helpers[a]=h
cases=0
table={}
for i in range(13):
 rate,pitch=struct.unpack_from('<Ih',binary,0x67c384-0x10000+i*8)
 if ctypes.c_int32(rate).value<=0:break
 table[rate]=pitch&0xffffffff
for rate,count,capacity,bank,sr0 in itertools.product(list(table)+[0,1,65535,48000+65536],[1,2],[0,256],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c1d0,0x8ca08000);ram[0xa10001]=1
 record=0xa080a8+44;ram[record:record+44]=bytes([0x55])*44;ram[record+5]=count
 ring=0xa08000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca40000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca30000,256,capacity,0,0)
 packet=0x8e9e24;template=bytes(ram[packet:packet+20])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66b030:actual.append(tuple(r[i] for i in range(4,8)))
  assert a in helpers,a
  return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca10000;s.r[5]=1;s.r[6]=rate;s.r[7]=0;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c6693ec
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 selected=table.get(rate&65535)
 code=0x03000001 if selected is None else struct.unpack_from('<I',binary,0x65a508-0x10000)[0] if capacity else 0
 assert r[0]==code and macl.value==special['macl']
 if selected is not None:assert actual==[(0x8ca10000,1,selected,0)]
 else:assert not actual
 expected_packet=bytearray(template)
 if selected is not None:
  struct.pack_into('<I',expected_packet,0,0x21);expected_packet[4]=1|(128 if count==2 else 0)
  expected_packet[5]=0x55;expected_packet[6]=selected&255;expected_packet[7]=(selected>>8)&255;expected_packet[8]=0;expected_packet[9:11]=b'\x55\x55'
 assert native[packet:packet+20]==expected_packet
 queued=int(selected is not None and not capacity)
 assert struct.unpack_from('<3I',native,ring+20)==(capacity+queued,queued,0)
 if queued:
  assert native[0xa30000:0xa30014]==expected_packet
  assert struct.unpack_from('<I',native,0xa30014)[0]==1
 else:assert native[0xa30000:0xa3001c]==bytes(28)
 if code:assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c6693ec,code)
 cases+=1
(out/'analysis/sound_rate_table_set_native_enqueue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native rate wrapper/pitch command/packet commit/copy/error recorder chain.256 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. All12 rates plus invalid/truncation,mono/stereo,empty/full ring,banks/SR. Independent exact command21 packets,queue tag/counters,helper arguments/error propagation. No modeled subordinate calls; context/ring fixtures, audio/consumer/full game unverified.'),indent=2)+'\n')
print(cases,'rate table comparisons passed')
