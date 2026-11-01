"""Append scoped texture/particle command integration without routine-count changes."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/player_texture_draw_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==0
 for path,value in q['sha256'].items():assert sha(path)==value,path
 sp=P/'project_status.json';status=json.loads(sp.read_text());assert 'player_texture_draw_integration' not in status['verified_progress']
 assert status['recovered_buildable_routines']==status['verified_progress']['recovered_buildable_routines']==1648
 data={k:v for k,v in q.items() if k!='sha256'};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_TEXTURE_DRAW.txt');status['verified_progress']['player_texture_draw_integration']=data;sp.write_text(json.dumps(status,indent=2)+'\n')
 texts={
 'README.md':'# Textured particle command path: verified software integration\n\nOriginal game code now loads all 138 EFF_REGULAR texture entries and draws an allocated particle into a 160-byte RAM command buffer. Uploaded textures persist across calls in all 8 MiB of simulated video memory. Primary evidence: 23 comparisons, including 20 scene comparisons / 472 checks / 429 independent oracles plus three continuity tests. This verifies software command staging; GPU submission and displayed pixels remain unfinished. The existing 283-source build is preserved, with zero new routine credit: 1,648 registry entries / 1,645 unique module/address pairs / 692 confirmed unique exact-name links. Retail upload trace values remain private. Approximately 10% weighted milestones, not measured code coverage. See [the guide](PLAYER_TEXTURE_DRAW.txt).\n\n',
 'SOURCE_HANDOFF.txt':'LATEST: persistent textures and original particle RAM command path, 2026-11-01\nAll138EFF_REGULARentries match independent PVMHglobal IDs with136distinct descriptors/zero errors/exact reference counts. Original graphics constructor,archive loading,task/player/collision/child/spawn/view/draw/cleanup compose with actual matched8MiBVRAM/register/storequeue/DMA carry. Entry132/global184f produces one160-byte RAM header/four-vertex buffer;physicalGPUdisplay/frame submission remains unqualified. Primary23comparisons:20scene/472checks/429oracles plus3continuity tests. Earlier20scene/468checks retained as same-domain rerun,not extra gameplay coverage. Existing283-source build/registry1648entries/1645unique pairs/692links unchanged;zero neworiginal credit. Raw retail upload traces preserved privately;public evidence filtered without changing comparisons. See PLAYER_TEXTURE_DRAW.txt andanalysis/player_texture_draw_qualification.json. Approximately10%weighted milestones,not code coverage;goalactive/SolHigh. Earlier bytes and counts remain preserved below.\n\n'}
 for name,text in texts.items():
  path=P/name;before=path.read_bytes();assert text.encode() not in before;path.write_bytes(text.encode()+before)
 print('Recorded texture/particle integration;registry1648/692links unchanged;goalactive',flush=True)
if __name__=='__main__':main()
