"""Record release-table verification with no new routine-completion credit."""
import hashlib,json,struct
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
verification='analysis/arm_sound_release_dispatch_verification.json';report=json.loads((P/verification).read_text())
assert [report[k] for k in ('cases_passed','direct_table_cases_passed','budget_cases_passed')]==[800,320,40]
for file,value in report['sha256'].items():assert sha(Path(file))==value,file
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
for driver,table in enumerate(report['pointer_tables']):
 path=ROOT/payloads[driver]['payload_path'];image=path.read_bytes();assert sha(path)==payloads[driver]['upload_sha256']
 pointer=struct.unpack_from('<I',image,0x8258+driver*4)[0];assert pointer==table['pointer']
 assert list(struct.unpack_from('<4I',image,pointer))==table['targets'] and image[pointer:pointer+16].hex()==table['bytes']
record={'verification':verification,'verification_sha256':sha(P/verification),'helper_cases':800,'direct_table_cases':320,'budget_cases':40,'classification':'Finite active/inactive release helper and all four internal table slots in main library, with exact original CPU/RAM and independent wrapped-arithmetic/status assertions. No new source-code recovery, exhaustive path, synthesis or game-completion claim.'}
(P/'analysis/arm_sound_release_dispatch_provenance.json').write_text(json.dumps(record,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text());status.setdefault('verified_progress',{})['arm_sound_release_dispatch_validation']=record;path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded 800 release helper, 320 direct table and 40 budget cases; no new routine credit')
