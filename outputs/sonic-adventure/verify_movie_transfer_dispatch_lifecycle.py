"""Original register/start/ASIC dispatch surrounding real authored G2 byte movement."""
import ctypes,hashlib,itertools,json
from pathlib import Path
import write_watch_transfer_dispatch_sound as fast
from verify_g2_transfer_host import Job,Bus,Read,Write
v=fast.old;P=v.P;ROOT=v.ROOT

class Input(v.Input):
 sound_image=None
 def get(self,key,default=None):
  if key=='sound_image':return self.sound_image
  return super().get(key,default)

def main():
 lib=v.effects.LIB;capture=lib.sa_g2_transfer_capture;capture.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(Job)];capture.restype=ctypes.c_int
 execute=lib.sa_g2_transfer_execute;execute.argtypes=[ctypes.POINTER(Job),ctypes.POINTER(Bus),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];execute.restype=ctypes.c_int
 records=[];checks=0;ram=v.seed_ram()
 for channel,mode,length in itertools.product(range(4),(0,1,4),(64,33376)):
  cpu=v.old.old;c=Input(entry=cpu.CLIENT_START,mode=mode,sdk_mode=1,current_record=0,channel=channel,length=length,source_pattern='masaru' if length==33376 else 'varying',masks=[0,0],irq_status=0,table=0xa05f7800,transfer_registers=[0]*48)
  m,s,f=cpu.prepare(ram,c);m[0x892840:0x893c00]=bytes(0x13c0)
  event=0xa60+channel*32;d=v.DESCRIPTOR
  s.pc=0x8c64594c;s.r[4:8]=[event,v.HANDLER,0x60,d];c['entry']=s.pc
  registered,path,a,s,f=fast.compare(m,s,f,c)
  assert registered['status']==0 and s.r[0]==1;checks+=2
  assert registered['masks']==[1<<(15+channel),0];checks+=1
  assert v.load(a,0x8c892850)==v.HANDLER and v.load(a,0x8c892858)==d;checks+=2
  assert v.load(a,c.get('vbr',0x8ce50000)+0x22c)==v.ASIC;checks+=1
  c['masks']=registered['masks'];c['entry']=cpu.CLIENT_START;s.pc=cpu.CLIENT_START;s.r[4]=v.RECORD;s.r[5]=0
  started,path,a,s,f=fast.compare(a,s,f,c)
  assert started['status']==0 and v.load(a,d+44)==1 and v.load(a,d+48)==0;checks+=3
  assert v.load(a,v.RECORD+4)==2 and v.load(a,v.RECORD+24)==0;checks+=2
  assert v.load(a,v.ROOT_CONTEXT+0x190)==v.RECORD;checks+=1
  regs=(ctypes.c_uint32*8)(*started['transfer_registers'][channel*8:channel*8+8]);job=Job()
  assert capture(channel,regs,ctypes.byref(job))==0;checks+=1
  assert job.channel==channel and job.root_address==0x0c900000 and job.g2_address==0x00800000 and job.direction==0 and job.length&0x7fffffff==length;checks+=5
  source=bytes(a[0x900000:0x900000+length]);unchanged=bytes(a);sound=bytearray([0xa5])*0x200000;wanted=bytearray(sound);wanted[:length]=source
  calls=[];errors=[]
  @Read
  def read(ctx,address,buffer,size):
   calls.append(('read',address,size))
   offset=address-0x0c000000
   if size!=32 or offset<0 or offset+size>len(a):errors.append(('read',address,size));return -9
   for i,value in enumerate(a[offset:offset+size]):buffer[i]=value
   return 0
  @Write
  def write(ctx,address,buffer,size):
   calls.append(('write',address,size));offset=address-0x00800000
   if size!=32 or offset<0 or offset+size>len(sound):errors.append(('write',address,size));return -9
   sound[offset:offset+size]=bytes(buffer[:size]);return 0
  bus=Bus(None,read,write);completed=ctypes.c_uint32();emitted=ctypes.c_uint32()
  assert execute(ctypes.byref(job),ctypes.byref(bus),ctypes.byref(completed),ctypes.byref(emitted))==0 and not errors;checks+=2
  assert completed.value==length and emitted.value==event and sound==wanted and bytes(a)==unchanged;checks+=4
  expected_calls=[]
  for offset in range(0,length,32):expected_calls.extend((('read',job.root_address+offset,32),('write',job.g2_address+offset,32)))
  assert calls==expected_calls;checks+=1
  # Explicit software completion input, after successful real data movement.
  # No CPU exception or physical IRQ is claimed. The original ASIC code must
  # select the registered event, then reach the original handler/callback.
  c.sound_image=bytes(sound);c['sound_image_sha256']=hashlib.sha256(sound).hexdigest();c['irq_status']=1<<(15+channel);c['transfer_registers']=started['transfer_registers'];c['entry']=v.ASIC;s.pc=v.ASIC;s.r[4]=0x360
  finished,path,a,s,f=fast.compare(a,s,f,c)
  assert finished['status']==0 and finished['irq_status']==0;checks+=2
  assert [b['arguments'][0] for b in finished['boundaries'] if int(b['target'],16)==v.GENERIC]==[event];checks+=1
  assert any(int(b['target'],16)==v.HANDLER for b in finished['boundaries']);checks+=1
  assert v.load(a,d+44)==0 and v.load(a,d+48)==1;checks+=2
  assert v.load(a,v.RECORD+4)==1 and v.load(a,v.RECORD+24)==1 and v.load(a,v.ROOT_CONTEXT+0x190)==0;checks+=3
  assert finished['sound_ram_sha256']==c['sound_image_sha256'] and not any(finished['sound_state']);checks+=2
  for r in (registered,started,finished):assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  records.append(dict(input=dict(c),original_native_comparisons=3,copied_bytes=completed.value,event=hex(emitted.value),source_preserved=True,complete_sound_ram_match=True,sound_ram_sha256=c['sound_image_sha256'],original_callback_completion=True,explicit_software_completion_input=True,boundaries=[r['boundary_comparisons'] for r in (registered,started,finished)]))
  print('Lifecycle',channel,mode,length,completed.value,flush=True)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_transfer_dispatch_connection.json',P/'source/g2_transfer_host.c',P/'source/g2_transfer_host.h',cpu.DRIVER]
 report=dict(lifecycles_passed=len(records),original_native_comparisons=3*len(records),authored_host_transfer_cases=len(records),independent_checks=checks,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original register -> original channel start -> real existing authored G2 service -> original ASIC snapshot dispatch -> original generic event traversal -> original handler -> original SDK completion. Four channels,three FPU modes,64-byte varying source and exact33376-byte local driver payload. Native host transfer has an independent whole-data oracle and is not a retail-code comparison; three guest ABI calls per lifecycle compare full state against original instructions. MMIO state/results and completed sound bytes are carried explicitly between calls. Successful transfer supplies an explicit software pending-bit input; no physical exception/IRQ timing, uninterrupted earliest startup, automatic runtime pumping, ARM execution or audio claimed.')
 (P/'analysis/movie_transfer_dispatch_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Dispatch lifecycles',len(records),'original/native',3*len(records),'checks',checks)
if __name__=='__main__':main()
