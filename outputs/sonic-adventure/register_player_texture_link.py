"""Append three qualified texture helpers and preserve historical status."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/player_texture_link_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==3
 for path,value in q['sha256'].items():assert sha(path)==value,path
 rp=P/'analysis/recovered_routines.json';reg=json.loads(rp.read_text());prior=reg['routines'][:];assert len(prior)==1645
 for b in q['routines']:
  assert not any(x.get('module','1ST_READ.BIN')==b['module'] and x['address']==b['address'] for x in prior)
  row={k:b[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=qp.relative_to(P).as_posix(),status='Complete original texture helper software-qualified in documented domains;53group rows/12outcomes execute. Valid filename missing-file path matches;actual memory-resource loader returns error6. Full filename linker andvisible textured rendering remain unqualified.',limitations=q['limitations']);reg['routines'].append(row)
 assert len(reg['routines'])==1648 and reg['routines'][:1645]==prior and len({(x.get('module','1ST_READ.BIN'),x['address']) for x in reg['routines']})==1645
 rp.write_text(json.dumps(reg,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());assert 'player_texture_link' not in status['verified_progress'];data={k:v for k,v in q.items() if k not in ('sha256','routines')};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_TEXTURE_LINK.txt');status['verified_progress']['player_texture_link']=data
 status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1648
 sp.write_text(json.dumps(status,indent=2)+'\n')
 for name,prefix in (
  ('README.md','# Texture linker recovery and loader diagnosis: verified research update\n\nThree complete helpers (53 rows / 12 outcomes) are software-qualified; 311 fresh original/native comparisons and 1,299 checks pass. The 1,528-row filename linker and its static helper chain are recovered in a 283-source research build, with a verified missing-file error path; full linker behavior remains unqualified. Registry: 1,648 entries / 1,645 unique module/address pairs; 692 confirmed unique exact-name DLL links. Correction: filename API608c82 is not the next call after the archive loader; the static scene list has null filenames. Actual in-memory decoder608134, using original temporary-list producers, returns failure with first-descriptor error6. Next: trace conversion/graphics setup. Approximately 10% weighted milestones, not measured code coverage; full game and rendering unfinished. See [the guide](PLAYER_TEXTURE_LINK.txt).\n\n'),
  ('SOURCE_HANDOFF.txt','LATEST: texture linker recovery and actual memory-loader diagnosis, 2026-11-01\nThree qualified helpers / 53 rows / 12 outcomes. 311 comparisons / 1299 checks; 692 confirmed exact-name links; registry1648entries/1645unique pairs. New283-source incomplete research build,not playable. 22bodies/2771rows byte-checked;7canonical bodies/176rows get zero duplicate credit. Filename linker64c374is recovered but remains unqualified beyond its tested missing-file branch. Correction: original099750does not call filename API608c82afterloading;that null-filename composition was wrong. Proper original temporary-list setup for608134exposes actual memory-loader failure/error6atfirst descriptor. Next: trace6536ecconversion andreal graphics/driver initialization. See PLAYER_TEXTURE_LINK.txt andanalysis/player_texture_link_qualification.json. Approximately10%weighted milestones,not measured code coverage. Goalactive/SolHigh;earlier source/proof/build bytes preserved.\n\n')):
  path=P/name;old=path.read_bytes();assert prefix.encode() not in old;path.write_bytes(prefix.encode()+old)
 print('REGISTERED1648entries/1645unique/692confirmedlinks;historical docs preserved',flush=True)
if __name__=='__main__':main()
