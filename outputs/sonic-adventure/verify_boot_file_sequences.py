"""Carry original file handles through real open/size/status/close calls."""
import hashlib,itertools,json
from pathlib import Path
import verify_boot_file as v

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 code=v.previous.compare_code;marker='ml=ctypes.c_uint32(0x12345678)';assert code.count(marker)==1
 code=code.replace(marker,"ml=ctypes.c_uint32(c.get('macl',0x12345678))")
 exec(compile(code,'file_lifecycle_comparison','exec'),dict(v.compare.__globals__,effects=v.effects,namespace=v.namespace),local:={});compare=local['compare']
 ram=v.previous.previous.previous.previous.previous.seed_ram();sequences=[]
 for mode,name in itertools.product((0,4),(b'SYSTEM.BIN',b'DATA.BIN',b'EMPTY.DAT')):
  c=dict(entry=v.VOPEN,mode=mode,request=name);m,s,f=v.prepare(ram,c);stages=[];h=None
  for entry in (v.VOPEN,v.SIZE,v.STATUS,v.CLOSE,v.CLOSE):
   c['entry']=entry;s.pc=entry
   if h is not None:s.r[4]=h;s.r[5]=v.OUT
   before=v.effects.State.from_buffer_copy(s);fp=v.effects.Fpu.from_buffer_copy(f)
   record,path,a,s,f=compare(m,s,f,c);assert record['status']==0
   record['independent_checks']=v.semantic(m,a,before,fp,s,f,record,c)
   assert not record['queue_events'] and not record['clock_reads'] and not record['mask_writes'] and not record['register_writes'] and not record['vram_writes'] and not record['ack_writes']
   if h is None:h=s.r[0];assert h==v.FD
   if entry==v.CLOSE:assert v.word16(a,h+0x4c)==0
   record['independent_checks']+=2;stages.append(record);m=a;c['macl']=record['macl']
  sequences.append(dict(mode=mode,name_hex=name.hex(),stages=stages))
 files=[Path(__file__),Path(v.__file__),v.MANIFEST,v.PROJECT/'analysis/boot_file_connection.json']
 report=dict(sequences_passed=len(sequences),stage_comparisons=sum(len(s['stages']) for s in sequences),independent_checks=sum(r['independent_checks'] for s in sequences for r in s['stages']),external_game_service_fixtures=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Six actual open/size/status/close/double-close sequences carry complete RAM/CPU/FPU and MACL between calls. Empty software peripheral state is unchanged; no disc read or firmware result is supplied. Original filesystem/directory/descriptor layouts remain explicit initialized runtime fixtures.',limitations=['This proves the handle lifecycle within initialized fixture tables, not natural filesystem initialization or successful disc I/O.','Complete reached/final machine comparisons retain the numeric, valid-memory, immutable-code and software-peripheral domains of boot_file_verification.json.'],sequences=sequences)
 def encode(value):
  if isinstance(value,(bytes,bytearray)):return {'bytes_hex':value.hex()}
  raise TypeError(type(value).__name__)
 (v.PROJECT/'analysis/boot_file_sequences_verification.json').write_text(json.dumps(report,indent=2,default=encode)+'\n')
 print({k:report[k] for k in ('sequences_passed','stage_comparisons','independent_checks')})

if __name__=='__main__':main()
