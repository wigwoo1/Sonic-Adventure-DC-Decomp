"""Capture completed full comparisons before separate endpoint assertions."""
import hashlib,json,time
from pathlib import Path
import write_watch_driver_control as fast
v=fast.old
P=v.P;ROOT=v.ROOT;DRIVER=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 driver=DRIVER.read_bytes();ram=v.seed_ram();base=next(c for c in v.old.old.old.old.old.cases() if c['entry']==0x8c0546c0)
 files=[Path(__file__),Path(fast.__file__),P/'windows_write_watch.py',P/'analysis/windows_write_watch_verification.json',Path(v.__file__),v.MP,P/'analysis/movie_driver_control_connection.json',P/'analysis/movie_request_fields_qualification.json',DRIVER]
 for label in ('dual','fast'):
  for cold in (False,True):
   c=base|dict(cold_filesystem=cold,progress=True,write_watch_legacy_hashes=label=='dual');m,s,f=v.old.old.old.old.prepare(ram,c)
   before_root=v.load(m,0x8c673414);before_interface=v.load(m,0x8c673420)
   print('START',label,cold,flush=True);started=time.perf_counter();record,path,a,s,f=fast.compare(m,s,f,c);elapsed=time.perf_counter()-started
   # compare() has already asserted full CPU/RAM/FPU/device and ordered trace equality.
   root=v.load(a,0x8c673414);payload=v.sample(a,0x0cd00000,len(driver));opened=[b for b in record['boundaries'] if b['target']=='0x8c6016d6'];edges=set()
   for i,pc in enumerate(path):
    text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
    if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
   record.update(filesystem_root_before=hex(before_root),filesystem_root_after=hex(root),filesystem_initialized_flag=int.from_bytes(v.sample(a,root+0x38,2),'little'),masaru_sha256_at_stop=hashlib.sha256(payload).hexdigest(),masaru_original_sha256=hashlib.sha256(driver).hexdigest(),masaru_payload_matches=payload==driver,masaru_bytes=len(driver))
   capture=dict(record=record,mode=label,elapsed_seconds=elapsed,before_interface=hex(before_interface),opened_name_hex=v.sample(m,opened[-1]['arguments'][0],11).hex() if opened else None,driver_header_magic=int.from_bytes(payload[:4],'little'),retail_expected_magic=v.load(m,0x8c643f28),driver_header_byte4=payload[4],driver_header_byte5=payload[5],executed_rows=[hex(x) for x in sorted(set(path))],executed_edges=[[hex(a),hex(b)] for a,b in sorted(edges)],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},full_original_native_comparison=True)
   destination=ROOT/f'work/sonic-adventure/write_watch_prefix_{label}_{"cold" if cold else "warm"}_capture.json';destination.write_text(json.dumps(capture,indent=2)+'\n')
   print('CAPTURED',label,cold,record['status'],record['stop_pc'],record['boundary_comparisons'],record['disc_read_attempts'],flush=True)
if __name__=='__main__':main()
