"""Validate saved full-comparison outcomes for distinct real/warm driver paths."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
EXPECTED='5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 records=[];coverage=set();edges=set();checks=0;files=[Path(__file__)];dll=ROOT/'work/sonic-adventure/build/movie_driver_control.dll'
 for cold in (False,True):
  path=ROOT/f'work/sonic-adventure/movie_driver_control_prefix_{"cold" if cold else "warm"}_capture.json';data=json.loads(path.read_text());files.append(path)
  for pinned,h in data['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
  r=data['record'];assert data['full_original_native_comparison'] and data['dll_sha256']==digest(dll);checks+=2
  assert (r['filesystem_root_before']=='0x0' and data['before_interface']=='0x0') if cold else r['filesystem_root_before']=='0x8cc80000';checks+=1
  assert r['status']==-2 and r['stop_pc']==('0x8c65a8c8' if cold else '0x8c64bddc');checks+=2
  assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000;checks+=2
  init=[b for b in r['boundaries'] if b['target']=='0x8c010180'];assert len(init)==1 and init[0]['arguments']==[1,0,1,3];checks+=2
  assert data['opened_name_hex']==b'masaru.drv\0'.hex();checks+=1
  if cold:
   assert r['filesystem_root_after']=='0x8c6946e0' and r['filesystem_initialized_flag']==1;checks+=2
   assert r['disc_read_attempts']==89 and r['masaru_payload_matches'] and r['masaru_sha256_at_stop']==EXPECTED;checks+=3
   assert data['driver_header_magic']==data['retail_expected_magic']==0x56524453 and data['driver_header_byte4']==data['driver_header_byte5']==1;checks+=2
   assert any(b['target']=='0x8c643e6c' for b in r['boundaries']) and not any(b['target']=='0x8c5ff31e' for b in r['boundaries']);checks+=2
   assert any(b['target']=='0x8c65a200' for b in r['boundaries']) and r['control_word']&1;checks+=2
   reads=[b for b in r['boundaries'] if b['target']=='0x8c600f1e'];assert reads and reads[-1]['arguments'][1:3]==[17,0x0cd00000];checks+=1
  else:
   assert r['filesystem_root_after']==r['filesystem_root_before'] and not r['disc_read_attempts'] and not r['masaru_payload_matches'];checks+=3
  records.append(r);coverage|={int(x,16) for x in data['executed_rows']};edges|={tuple(int(x,16) for x in e) for e in data['executed_edges']}
 translation=P/'analysis/movie_driver_control_translation.json';files.append(translation);routines=json.loads(translation.read_text())['routines'];rows=[]
 for routine in routines:
  body={int(r['address'],16) for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 report=dict(cases_passed=2,independent_checks=checks,cold_profile_reexecuted=True,masaru_bytes=33408,masaru_sha256=EXPECTED,coverage_by_routine=rows,dll_sha256=digest(dll),sha256={str(p):digest(p) for p in files},cases=records,scope='Two newly captured uninterrupted original/native comparisons. Real cold filesystem loads an SDRV header and takes the original driver-action path through the recovered control update to missing 65a8c8. The prepared warm missing-file control has no driver bytes and instead reaches 64bddc later. Completed comparison data is persisted before endpoint validation; no program execution or callee return is replaced. Natural reset, driver execution and complete startup remain unfinished.')
 (P/'analysis/movie_driver_control_prefix_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Validated two distinct caller paths;',checks,'checks')
if __name__=='__main__':main()
