"""Three consecutive input polls with carried RAM/machine/FPU/MACL state."""
import ctypes,hashlib,itertools,json
from pathlib import Path
import verify_boot_input as v

def main():
 code=v.previous.compare_code
 marker='ml=ctypes.c_uint32(0x12345678)';assert code.count(marker)==1
 code=code.replace(marker,"ml=ctypes.c_uint32(c.get('macl',0x12345678))")
 exec(compile(code,'carried_input_comparison','exec'),dict(v.compare.__globals__),local:={});compare=local['compare']
 ram=v.previous.previous.previous.seed_ram();sequences=[]
 for inverse,direction in itertools.product((0,1),(0,1)):
  c=dict(entry=v.UPDATE,mode=inverse*4,inverse=inverse,features=1,capabilities=0xffffffff)
  m,s,f=v.prepare(ram,c);stages=[]
  for frame,raw in enumerate((0,128,255) if direction else (255,128,0)):
   buttons=(0xffff,0xfffe,0xffff)[frame]
   # External input changes only the explicit driver-owned controller payload.
   for i in range(24):
    p=v.PAYLOAD+i*64;m[(p+8)&0xffffff:(p+10)&0xffffff]=buttons.to_bytes(2,'little');m[(p+10)&0xffffff:(p+16)&0xffffff]=bytes([raw]*6)
   s.pc=v.UPDATE;before=v.effects.State.from_buffer_copy(s);fp=v.effects.Fpu.from_buffer_copy(f)
   record,path,a,s,f=compare(m,s,f,c);assert record['status']==0
   assert not record['queue_events'] and not record['clock_reads'] and not record['register_writes'] and not record['vram_writes'] and not record['ack_writes']
   checks=v.semantic(m,a,before,fp,s,f,record,c)
   for i in (0,6,12,18):
    value,fields,hist=v.axis_values(m,dict(c,index=i));out=v.CHANNEL+i*52
    assert v.load(a,out+8)==value and list(v.struct.unpack('<6H',v.bytes_at(a,out+24,12)))==fields
    assert all(a[address&0xffffff]==h for address,h in hist);checks+=3
   record['independent_checks']=checks;record['external_payload_bytes_changed']=24*8;stages.append(record)
   m=a;c['macl']=record['macl']
  sequences.append(stages)
 files=[Path(__file__),v.PROJECT/'verify_boot_input.py',v.PROJECT/'analysis/boot_input_translation.json',v.PROJECT/'analysis/boot_input_connection.json']
 report=dict(sequences_passed=4,stage_comparisons=12,independent_checks=sum(r['independent_checks'] for seq in sequences for r in seq),external_game_service_fixtures=0,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Four three-poll sequences carry all RAM, CPU/FPU and actual MACL results while external controller payloads change. Independent button change masks, trigger hysteresis, signed axes and four processed primary slots are checked. The updater makes no device/clock/VRAM events and does not alter GBR/MACH/inactive integer banks; the reinitialized empty bus therefore has identical state. This is not a general persistent device scheduler.',limitations=['Driver-layout RAM snapshots and valid initialized tables remain explicit fixtures. Hardware controller transport and natural startup are unproved.'],sequences=sequences)
 (v.PROJECT/'analysis/boot_input_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('sequences_passed','stage_comparisons','independent_checks')})

if __name__=='__main__':main()
