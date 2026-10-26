"""Exercise the retained seventeen-argument observation ABI in the new DLL."""
import ctypes,hashlib,itertools,json
from pathlib import Path
import write_watch_transfer_runtime as fast
import verify_movie_transfer_dispatch as dispatch
import verify_movie_transfer_interrupt as irq
import verify_movie_cpu_transfer as cpu
v=fast.old;P=v.P;ROOT=v.ROOT

def main():
 legacy=v.effects.LIB.sa_movie_transfer_runtime_observed;legacy.argtypes=v.ordinary.argtypes[:-1];legacy.restype=ctypes.c_int
 new=v.observed;v.observed=lambda *args:legacy(*args[:-1])
 records=[];ram=v.seed_ram()
 try:
  for family,module in (('dispatch',dispatch),('handler',irq),('cpu',cpu)):
   for c in itertools.islice(module.cases(),2):
    for key in v.EXCEPTION_KEYS:c[key]=0
    m,s,f=module.v.prepare(ram,c);r,path,a,s,f=fast.compare(m,s,f,c)
    assert r['full_state_match'] and r['ordered_boundaries_match'] and not any(r['exception_state'])
    records.append(dict(family=family,entry=hex(c['entry']),status=r['status'],stop_pc=r['stop_pc']))
 finally:v.observed=new
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'analysis/movie_transfer_runtime_connection.json']
 report=dict(legacy_abi_comparisons=len(records),cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Six representative old paths execute the retained seventeen-argument observation export. Exception state is absent, remains zero and receives no fabricated return. Full original/native state and boundaries match. Existing CPU/Extra layouts unchanged.')
 (P/'analysis/movie_transfer_runtime_compatibility_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Legacy ABI comparisons',len(records))
if __name__=='__main__':main()
