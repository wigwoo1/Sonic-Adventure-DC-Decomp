"""Read-only PAL/USA source identities; no upstream implementations imported."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';b=(W/'extracted/1ST_READ.BIN').read_bytes();base=0x8c010000
registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
specs=[('cross',0x8c027360,0x42,'ab64ed43a74ef8ae8bc802e9a03ac1ff70c19370a74186906ecf5878a3149dd8'),('length',0x8c63a69c,0x10,'184ec57b105022cf5a5df589f52fed8dc017109b7c0ff5626bfaf6c31c5a39dd'),('normalize',0x8c63a88c,0x20,'91bc28ff6fe7b04c8d5178dd3b7e8ee411224556da83d770225895326e61376b'),('movement_body',0x8c0730a0,0x1174,'a6ea591ff19b73ed4f2124d643c6f94b9651cef5ec9f88cdb7d42cf29eb3e001')]
results=[]
for name,pal,size,expected in specs:
 matches=[]
 for a in range(max(base,pal-0x2000),min(base+len(b)-size,pal+0x2000)+1,2):
  if hashlib.sha256(b[a-base:a-base+size]).hexdigest()==expected:
   matches.append(dict(usa_address=hex(a),registered=[dict(name=r['name'],source=r['source']) for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==a]))
 results.append(dict(name=name,pal_address=hex(pal),bytes=size,upstream_sha256=expected,search_domain='PAL_address +/-0x2000,evenaddresses; bounded exacthashsearch only;not exhaustive region mapping',exact_matches=matches))
report=dict(registry_entries_at_assessment=len(registry),registry_snapshot_sha256=hashlib.sha256((P/'analysis/recovered_routines.json').read_bytes()).hexdigest(),registry_baseline_commit='d54a1fb449e9d5eaa918342bba5c7846d7802413',upstream_commit='73e9e854cde6fe8b158779c0c0a70c6cec4828ff',results=results,new_routine_credit=0,runtime_code_imported=False,scope='Read-only original-byte hash comparison;absence within boundeddomain does not prove absence or incompatibility. Positivebytes do not prove nativeimplementation or ABI adaptation.',overall_completion_percent=10)
report['sha256']={str(Path(__file__)):hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),str(W/'extracted/1ST_READ.BIN'):hashlib.sha256(b).hexdigest()};report['sources']=['https://github.com/sonicfreak1337/SARecomp/blob/73e9e854cde6fe8b158779c0c0a70c6cec4828ff/src/sonic_collision_math.hpp','https://github.com/sonicfreak1337/SARecomp/blob/73e9e854cde6fe8b158779c0c0a70c6cec4828ff/tools/prepare-movement-resolver.py'];(P/'analysis/sarecomp_gameplay_compatibility_probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(results,indent=2))
