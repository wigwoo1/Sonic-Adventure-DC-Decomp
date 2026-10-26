"""Append only the eight completely tested originals; preserve canonical reuse."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 qp=P/'analysis/movie_packet_flash_qualification.json';q=json.loads(qp.read_text())
 assert q['newly_registered_entries']==8 and q['newly_qualified_rows']==610 and q['qualified_conditional_outcomes']==98
 for path,h in q['sha256'].items():assert digest(path)==h,path
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for old in q['canonical_reuse']:
  now=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==old['address'])
  for key,value in old.items():assert now[key]==value,(old['address'],key)
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
  row.update(verification=qp.relative_to(P).as_posix(),status='Qualified complete original flash-packet body;610own rows/98outcomes across8new routines.1134freshoriginal/native direct and retained comparisons;real authored storage bytes/ranges/checksums/program/erase. Physical BIOS/timing and complete startup not claimed.',limitations=q['limitations'])
  prior=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if prior is None:registry['routines'].append(row)
  else:assert prior==row
 assert len(registry['routines'])==1405
 assert not any(r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c64bddc' for r in registry['routines'])
 rp.write_text(json.dumps(registry,indent=2)+'\n')
 integration={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','qualified_conditional_outcomes','executed_integration_rows','compiled_source_count','total_original_native_comparisons','authored_flash_service_cases','authored_support_recovery_credit','partial_settings','dll_sha256','limitations')}
 integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_PACKET_FLASH.txt')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status['verified_progress']['movie_packet_flash_integration']=integration
 status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1405
 sp.write_text(json.dumps(status,indent=2)+'\n')
 print('Registered8completepacketbodies;1405entries;settings113/25of28remainsunregistered')
if __name__=='__main__':main()
