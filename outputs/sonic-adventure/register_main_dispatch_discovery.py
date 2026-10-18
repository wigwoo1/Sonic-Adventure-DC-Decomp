"""Record verified switch/body discovery without native recovery credit."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
verification='analysis/main_dispatch_discovery_verification.json';report=json.loads((P/verification).read_text())
assert [report[k] for k in ('selector_cases_passed','table_entries','unique_targets','old_instruction_rows','enriched_instruction_rows','new_instruction_rows','enriched_instruction_bytes')]==[256,83,80,127,8122,7995,16244]
for file,value in report['sha256'].items():assert sha(Path(file))==value,file
diagnostics={}
for folder,body in (('main-failed-function-evidence',254),('main-dispatch-enrichment',16244)):
 path=P/'analysis'/folder/'diagnostics.tsv';rows=list(csv.DictReader(path.open(),delimiter='\t'));assert len(rows)==1
 row=rows[0];assert row['address']=='8c0a966c' and int(row['body_bytes'])==body and row['completed']=='false' and 'timeout' in row['error']
 diagnostics[str(path.relative_to(P))]={'sha256':sha(path),'body_bytes':body,'decompilation_completed':False,'diagnostic':row['error']}
record={'verification':verification,'verification_sha256':sha(P/verification),'diagnostics':diagnostics,'table_entries':83,'unique_targets':80,'additional_instruction_rows_in_function_export':7995,'enriched_instruction_bytes':16244,'classification':'Read-only Ghidra switch override expands known function body, with retail byte/target and 256 selector-prefix checks. Baseline and enriched decompilation each timed out at 60 seconds; no further decompilation retries made. Exported instructions are analysis evidence, not buildable recovered native handlers. No complete-routine, full-code-discovery or whole-project milestone credit.'}
(P/'analysis/main_dispatch_discovery_provenance.json').write_text(json.dumps(record,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text());status.setdefault('verified_progress',{})['main_dispatch_8c0a966c_discovery']=record;path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded 83-entry/80-target switch discovery and 7995 additional function instruction rows; native recovery and decompilation remain incomplete')
