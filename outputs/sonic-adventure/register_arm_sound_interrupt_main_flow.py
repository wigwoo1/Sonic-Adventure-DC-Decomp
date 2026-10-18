"""Record shared main-DLL interrupt/input/timer/main-loop state continuity."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
verification='analysis/arm_sound_interrupt_main_flow_verification.json';report=json.loads((P/verification).read_text())
assert [report[k] for k in ('flow_cases_passed','cycles_passed','phase_checkpoints_passed','commands_produced_and_consumed')]==[4,64,192,96]
assert all(f['full_tick_iterations']==3 for f in report['flows'])
for file,value in report['sha256'].items():assert sha(Path(file))==value,file
related={}
for name in ('input','exception'):
 path=P/f'analysis/arm_sound_{name}_connected_verification.json';evidence=json.loads(path.read_text());assert evidence['connected'] is True
 for file,value in evidence['sha256'].items():assert sha(Path(file))==value,file
 assert str(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll') in evidence['sha256']
 related[str(path.relative_to(P))]=sha(path)
record={'verification':verification,'verification_sha256':sha(P/verification),'connected_input_exception_evidence':related,'flows':4,'cycles':64,'checkpoints':192,'commands_produced_and_consumed':96,'classification':'Shared main DLL and RAM across explicit FIQ event entry, input parsing, timer flags, command-ring production/consumption and complete main iterations. CPU/CPSR/banks/RAM/MMIO/ordered effects match original ARM. Independently checked volume/pan state, interrupted registers, 32-interrupt counter wrap and three full-tick iterations per fixture. No automatic host interrupt scheduler, physical timing, synthesis, all command families or complete-game proof; no new complete-routine credit.'}
(P/'analysis/arm_sound_interrupt_main_flow_provenance.json').write_text(json.dumps(record,indent=2)+'\n')
path=P/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text());assert len(registry['routines'])==98
registry.setdefault('connected_flows',{})['interrupt_input_main']=record;path.write_text(json.dumps(registry,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text());status.setdefault('verified_progress',{})['arm_sound_interrupt_input_main']=record;path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded 4 connected FIQ/input/timer/main flows, 192 phase checkpoints and 96 consumed control commands; registry remains 98 routines')
