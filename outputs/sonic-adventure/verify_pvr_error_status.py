"""Independent clear-on-one controls and retained raw access trace for ISTERR."""
import ctypes,hashlib,json,struct
from pathlib import Path
import pvr_dma_reference as model
from verify_vram_aperture_bus import Bus,Read,Write,Cache,U
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 dll=ROOT/'work/sonic-adventure/build/movie_buffer_initialize.dll';lib=ctypes.CDLL(str(dll));State=model.state_type(Bus);model.declare(lib,Bus,State)
 ram=(ctypes.c_uint8*0x1000000)();parent_events=[]
 @Read
 def read(ctx,a,n,out):parent_events.append(('read',a,n));out[0]=0xfeed;return 0
 @Write
 def write(ctx,a,value,n):parent_events.append(('write',a,value,n));return 0
 @Cache
 def cache(ctx,a,op):return 0
 parent=Bus(None,read,write,cache);records=[];checks=0
 for address in (0x005f6908,0x805f6908,0xa05f6908):
  s=State();bus=Bus();assert lib.sa_pvr_dma_bus_init(ctypes.byref(s),ram,ctypes.byref(parent),ctypes.byref(bus),None,None,1)==0
  # Host hardware error input, never a game completion record.
  s.error_status=0xa55affff;ref=model.Reference({});ref.error_status=0xa55affff;wanted=0xa55affff
  for i,mask in enumerate((0,1,0xaaaaff00,0xffffffff)*80):
   out=U();assert bus.read(bus.context,address,4,ctypes.byref(out))==0 and out.value==wanted;ref.error_access(address,0,1);checks+=2
   assert bus.write(bus.context,address,mask,4)==0;ref.error_access(address,mask,2);wanted&=~mask;checks+=1
   assert s.error_status==wanted and model.snapshot(s)==ref.snapshot();checks+=2
  for offset,width in ((0,1),(0,2),(1,4),(2,4),(3,4),(0,8)):
   before=model.snapshot(s);out=U(0x1234)
   assert bus.read(bus.context,address+offset,width,ctypes.byref(out))==-3 and out.value==0x1234;checks+=2
   assert bus.write(bus.context,address+offset,0xffffffff,width)==-3 and model.snapshot(s)==before;checks+=2
  assert not parent_events and s.error_count==256 and s.error_dropped==384;checks+=3
  raw=ctypes.string_at(ctypes.addressof(s)+State.error_trace.offset,s.error_count*12);assert raw==b''.join(struct.pack('<III',*e) for e in ref.error_events);checks+=1
  records.append(dict(address=hex(address),valid_accesses=640,invalid_accesses=12,retained=s.error_count,dropped=s.error_dropped,trace_sha256=hashlib.sha256(raw).hexdigest()))
 files=[Path(__file__),Path(model.__file__),P/'source/pvr_dma_bus.c',P/'source/pvr_dma_bus.h',dll]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,parent_trace_untouched=True,original_recovery_credit=0,dll_sha256=digest(dll),sha256={str(p):digest(p) for p in files},scope='Authored ISTERR W1C platformpolicy,threealiases,zero/selective/allbitclears,width/alignmentrejection,readvaluepreservation,exactrawtrace/hash/capacity/dropcontrols. No IRQerrorhandlercoverage orhardwaretimingclaim.')
 (P/'analysis/pvr_error_status_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('PVR ERROR',len(records),checks)

if __name__=='__main__':main()
