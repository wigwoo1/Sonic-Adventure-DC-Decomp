"""Angle wrap and signed conversion boundaries within the masked-trap domain."""
import collections,json
from pathlib import Path
import verify_draw_angle_curve as e
PROJECT=Path(__file__).resolve().parent
PROFILES=(
 ((65535,32767,-32768),(0,-32768,32767)),
 ((2147483647,-2147483648,16777217),(-2147483648,2147483647,-16777217)),
 ((0,0,0),(2147483520,-2147483648,1)),
 ((0,0,0),(1,2147483520,1)),
 ((0,0,0),(1,1,2147483520)),
)
def main():
 proofs=[PROJECT/'analysis'/n for n in ('draw_angle_curve_connection.json','draw_angle_curve_verification.json')]
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert e.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,off in ((e.SEED.state.gate.support.morph.MAIN,0x10000),(e.SEED.state.gate.support.morph.MODULE,0x900000)):
  b=p.read_bytes();ram[off:off+len(b)]=b
 ram=e.caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]
 records=[];boundaries=0;trap_sites=collections.Counter()
 for payloads in PROFILES:
  e.sample=lambda i,values=payloads:values[i]
  for mode in (0,4):
   for enable in (0,):
    for frame in (-4,0,1.5,2,4,8):
     c=dict(entry=e.ENTRY,mode=mode,enable=enable,labels=(0,4),frame=frame,index=0,semantic=True,integer_key_payloads=payloads)
     r,path,b=e.compare(*e.prepare(ram,c),c);records.append(r);boundaries+=b
     if r['status']==1:trap_sites[r['stop_pc']]+=1
 assert len(records)==60 and not trap_sites,trap_sites
 assert not any(r['status'] in (-1,-2,-3) for r in records)
 files=[Path(__file__),PROJECT/'verify_draw_angle_curve.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(cases_passed=len(records),independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),status_counts=dict(collections.Counter(r['status'] for r in records)),enabled_invalid_ftrc_traps_tested=0,boundary_comparisons=boundaries,queue_events=sum(r['queue_events'] for r in records),native_original_renderer_initializer_stages=len(records),dll_sha256=e.digest(e.DLL),sha256={str(p):e.digest(p) for p in files},scope='Supplemental explicit integer-angle payload boundaries: 65535/0, signed 16-bit crossing, signed 32-bit limits, FLOAT rounding and positive/negative saturation; Invalid exceptions are masked; enabled-FTRC paths are outside the instruction-model domain and are not claimed. Both FPU banks, fractional/interior/extrapolated frames. Complete reached/final state and RAM; finite returning cases also use independent staged exact-rational arithmetic. Same inherited valid-RAM/numerical qualifications as the main helper proof.',newly_registered_entries=0,cases=records)
 (PROJECT/'analysis/draw_angle_boundaries_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:v for k,v in report.items() if k not in ('sha256','cases','scope')},indent=2))
if __name__=='__main__':main()
