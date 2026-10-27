"""Register three fully exercised local bodies; keep active decoders partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_channel_decoder_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(3,158,40)
 for pin,h in q['sha256'].items():assert digest(pin)==h,pin
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1485];assert len(previous)==1485
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=path.relative_to(P).as_posix(),status='Qualified local descriptor cleanup/validation or inactive-return/active-tail gateway;158rows40outcomes across3entries. Gatewayqualification ends at real reached missing downstream dependency;active decoding andfullworker remainpartial.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1488 and registry['routines'][:1485]==previous
 for address in ('0x8c6482a0','0x8c64839a','0x8c64753c','0x8c660a7e','0x8c64b1f0','0x8c647980','0x8c6608a4','0x8c660dec'):
  assert not any(r['address']==address and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry['routines'])
 rp.write_text(json.dumps(registry,indent=2)+'\n');sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:value for k,value in q.items() if k not in ('routines','sha256','coverage_by_routine','canonical_reuse')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_CHANNEL_DECODER.txt');status['verified_progress']['movie_channel_decoder_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1488;sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1488/unique1485;decoder/worker families remainpartial')
if __name__=='__main__':main()
