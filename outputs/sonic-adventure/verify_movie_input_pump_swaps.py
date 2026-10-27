"""Independently verify byte reversal and one observed call across internal loops."""
import hashlib,itertools,json
from pathlib import Path
import movie_input_pump_fixtures as fx
v=fx.v;P=v.P;DATA=0x8cc66000

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];checks=0
 for count,(mode,sr) in itertools.product((1,2,31),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,0x8c668b24,[DATA,count,0,0],mode,sr);data=bytes((i*17+3)&255 for i in range(4*count+16));m[DATA&0xffffff:(DATA&0xffffff)+len(data)]=data
  wanted=bytearray(m)
  for i in range(count):wanted[(DATA+4*i)&0xffffff:(DATA+4*i+4)&0xffffff]=data[4*i:4*i+4][::-1]
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and s.r[4]==DATA+4*count and s.r[5]==0 and s.r[0]==0;n+=4
  assert sum(b['target']=='0x8c668b24' for b in r['boundaries'])==1 and path.count(0x8c668b24)==count;n+=2
  assert s.sr==before.sr|1 and s.pr==s.pc==before.pr and s.r[15]==before.r[15];n+=3
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==c['macl'] and r['final_gbr']==c['gbr'];n+=4
  checks+=n;records.append(dict(count=count,mode=mode,sr=hex(sr),original_loop_iterations=count,observed_calls=1,independent_checks=n))
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='IndependentfullRAMbyte-reversal/neighbors/count/register/state oracle;oneoriginal/nativefunctioncall butallinternal BF/S iterations retainedinoriginalpath. Onlylocal668b32->668b24 boundaryobservation suppressed. Canonicalhelperzero extra credit. Positive1/2/31-worddomain;originalzero-count2^32iteration behaviornotcompletedorboundedbynewsource.')
 (P/'analysis/movie_input_pump_swaps_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP SWAPS PASS',len(records),checks)
if __name__=='__main__':main()
