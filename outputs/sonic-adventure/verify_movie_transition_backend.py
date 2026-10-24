"""Check borrowed timing samples and expose the historical mask mismatch."""
import ctypes,hashlib,json
from pathlib import Path
import verify_movie_transition as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=bytearray(16<<20);view=v.effects.RAM_TYPE.from_buffer(ram);m=v.TimingMemory();bus=v.effects.Bus();out=ctypes.c_uint32();checks=0
 init=v.effects.LIB.sa_display_timing_bus_init;configure=v.effects.LIB.sa_display_timing_set_samples
 init(ctypes.byref(m),view,ctypes.byref(bus),0xa35a)
 assert not m.samples and not m.sample_count and not m.sample_cursor and m.port_data==0xa35a;checks+=1
 out.value=0x12345678;before=bytes(m);assert bus.read(bus.context,v.SCAN,4,ctypes.byref(out))==-4 and bytes(m)==before and out.value==0x12345678;checks+=1
 values=[i*0x10203041&0xffffffff for i in range(5000)];storage=(ctypes.c_uint32*len(values))(*values)
 assert configure(ctypes.byref(m),storage,len(values))==0;checks+=1
 for i,value in enumerate(values):
  address=(0x005f810c,0x805f810c,0xa05f810c)[i%3]
  assert bus.read(bus.context,address,4,ctypes.byref(out))==0 and out.value==value and m.sample_cursor==i+1 and m.registers[0x10c//4]==value
 assert list(storage)==values and m.vertex.trace_count==4096 and m.vertex.trace_dropped==904;checks+=1
 assert [(e.kind,e.address,e.value,e.width) for e in m.vertex.trace[:4096]]==[(8,(0x005f810c,0x805f810c,0xa05f810c)[i%3],values[i],4) for i in range(4096)];checks+=1
 out.value=0xabcdef01;before=bytes(m);assert bus.read(bus.context,v.SCAN,4,ctypes.byref(out))==-4 and out.value==0xabcdef01 and bytes(m)==before;checks+=1
 for width in (1,2,8):
  assert bus.read(bus.context,v.SCAN,width,ctypes.byref(out))==-3 and out.value==0xabcdef01 and bytes(m)==before;checks+=1
 for width in (1,2,4,8):assert bus.write(bus.context,v.SCAN,0,width)==-3 and bytes(m)==before;checks+=1
 assert bus.read(bus.context,v.SCAN,4,None)==-3 and bytes(m)==before;checks+=1
 assert configure(ctypes.byref(m),None,1)==-3 and bytes(m)==before and configure(None,None,0)==-3;checks+=1
 assert configure(ctypes.byref(m),None,0)==0 and not m.samples and not m.sample_cursor and not m.sample_count;checks+=1
 assert bus.read(bus.context,0xff800030,2,ctypes.byref(out))==0 and out.value==0xa35a;checks+=1
 assert bus.write(bus.context,0xff00001c,0x905,4)==0 and bus.read(bus.context,0xff00001c,4,ctypes.byref(out))==0 and out.value==0x105 and m.ic_requests==1;checks+=1
 assert bus.write(bus.context,0xa05f6920,0x13579bdf,4)==0 and m.masks[0]==0x13579bdf;checks+=1
 assert bus.write(bus.context,0xa05f8040,0x12345678,4)==0 and bus.read(bus.context,0xa05f8040,4,ctypes.byref(out))==0 and out.value==0x12345678;checks+=1
 assert bus.cache(bus.context,0x8c001000,3)==0 and m.vertex.invalidates==1;checks+=1
 assert configure(ctypes.byref(m),storage,len(values))==0 and not m.sample_cursor and m.sample_count==len(values);checks+=1
 init(ctypes.byref(m),view,ctypes.byref(bus),0x200);assert not m.samples and not m.sample_cursor and not m.sample_count and m.port_data==0x200 and not m.ccr and not m.vertex.trace_count and not any(m.masks) and not any(ram);checks+=1
 # Historical evidence stays immutable. Compare its actual compiled helper's
 # read count against newly byte-checked original-instruction cases.
 old_path=v.ROOT/'work/sonic-adventure/build/packet_mmio.dll';old=ctypes.CDLL(str(old_path));callback=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32);legacy=old.sa_mmio_transition_wait_8c604594;legacy.argtypes=[callback,ctypes.c_void_p];legacy.restype=None
 direct_path=v.P/'analysis/movie_transition_verification.json';direct=json.loads(direct_path.read_text());regressions=[]
 for bit in range(9,32):
  samples=[1<<bit,1,0,1];record=next(r for r in direct['cases'] if r['input'].get('mode')==0 and r['input']['scan_samples']==samples)
  assert record['status']==0 and record['full_state_match'] and record['scan_cursor']==2
  reads=[]
  def reader(context,address):
   assert address==v.SCAN;reads.append(address);return samples[len(reads)-1]
  reader_fn=callback(reader);legacy(reader_fn,None)
  assert len(reads)==(4 if (1<<bit)&0xff0f01ff else 2)
  regressions.append(dict(bit=bit,original_and_new_native_reads=2,legacy_reads=len(reads),legacy_differs=len(reads)!=2))
 assert sum(r['legacy_differs'] for r in regressions)==12
 binary=v.ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();assert raw[0x604618-0x10000:0x60461a-0x10000]==b'\xff\x01'
 files=[Path(__file__),Path(v.__file__),v.P/'source/display_timing_bus.c',v.P/'source/display_timing_bus.h',direct_path,v.P/'source/packet_mmio.c',v.P/'analysis/packet_mmio_verification.json',binary]
 report=dict(sampled_words_passed=len(values),backend_checks=checks,legacy_outside_field_cases=23,legacy_mask_counterexamples=12,original_mask='0x000001ff',legacy_mask='0xff0f01ff',legacy_dll_sha256=digest(old_path),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},legacy_regressions=regressions,scope='Borrowed input ownership and immutability, 5000 raw 32-bit reads across three address aliases, trace/drop count, exact exhaustion, rejection without mutation, reconfiguration/reset, retained RAM, inherited A/V/CCR/mask/render/cache delegation. Twenty-three outside-nine-bit cases use complete original-instruction comparisons; twelve expose extra reads in the preserved old helper. No physical scan generation or display output is claimed.')
 (v.P/'analysis/movie_transition_backend_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Backend checks',checks,'raw samples',len(values),'historical mask counterexamples',12)
if __name__=='__main__':main()
