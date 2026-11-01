"""Append three qualified texture initializers and preserve historical status."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/player_texture_resource_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==3
 for path,value in q['sha256'].items():assert sha(path)==value,path
 rp=P/'analysis/recovered_routines.json';reg=json.loads(rp.read_text());prior=reg['routines'][:];assert len(prior)==1642
 for b in q['routines']:
  assert not any(x.get('module','1ST_READ.BIN')==b['module'] and x['address']==b['address'] for x in prior)
  row={k:b[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=qp.relative_to(P).as_posix(),status='Complete original texture initializer software-qualified in documented domains;64group rows/6outcomes execute. Actual retail resource read/decode matches,while texture-ID linking reaches unrecovered64c374. Visible textured rendering remains unqualified.',limitations=q['limitations']);reg['routines'].append(row)
 assert len(reg['routines'])==1645 and reg['routines'][:1642]==prior and len({(x.get('module','1ST_READ.BIN'),x['address']) for x in reg['routines']})==1642
 rp.write_text(json.dumps(reg,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());assert 'player_texture_resource' not in status['verified_progress'];data={k:v for k,v in q.items() if k not in ('sha256','routines')};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_TEXTURE_RESOURCE.txt');status['verified_progress']['player_texture_resource']=data
 status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1645
 sp.write_text(json.dumps(status,indent=2)+'\n')
 for name,prefix in (
  ('README.md','# Texture resource read and decode: verified research update\n\nThree complete original initializers (64 rows / 6 conditional outcomes) are now software-qualified. 66 fresh original/native comparisons and 357 checks pass, including actual local EFF_REGULAR.PRS read and decompression with exact complete input/output hashes. Four additive builds lead to 282 sources; all final sources pass strict compilation checks. Registry: 1,645 entries / 1,642 unique module/address pairs; 689 confirmed exact-name unique DLL links. The decoder stages numeric texture IDs; original linking reaches missing 0x8c64c374. Visible rendering and full game remain unfinished. Approximately 10% weighted milestones, not measured code coverage. See [the guide](PLAYER_TEXTURE_RESOURCE.txt).\n\n'),
  ('SOURCE_HANDOFF.txt','LATEST: original texture resource read/decode, 2026-11-01\nThree qualified initializers / 64 rows / 6 outcomes; 66 comparisons / 357 checks / 155 independent exact oracles. Four additive research builds, final282sources; strict compile passes. All50recovered bodies/2813rows byte-checked,including127canonical reused rows with zero duplicate credit. Registry1645entries/1642unique pairs/689confirmed exact-name links. Actual124234-byte EFF_REGULAR.PRS read and358304-byte decode match preserved hashes; numeric texture IDs await original linking,which reaches unrecovered64c374. See PLAYER_TEXTURE_RESOURCE.txt and analysis/player_texture_resource_qualification.json. Approximately10%weighted milestones,not measured code coverage;incomplete research artifacts,not playable. Goal active;continueSolHigh. Earlier source/proof/build bytes preserved.\n\n')):
  path=P/name;old=path.read_bytes();assert prefix.encode() not in old;path.write_bytes(prefix.encode()+old)
 print('REGISTERED1645entries/1642unique/689confirmedlinks;historical docs preserved',flush=True)
if __name__=='__main__':main()
