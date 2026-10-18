"""Record bounded main-library dispatcher integration without routine credit."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
expected={
 'baseline':{'instruction_cases_passed':2400,'baseline_command_cases_passed':1024,'reset_wait_cases_passed':2},
 'banks':{'bank_cases_passed':1044,'mmio_failure_cases_passed':16,'budget_cases_passed':36},
 'streams':{'stream_cases_passed':1520,'dsp_cases_passed':384,'mmio_failure_cases_passed':8,'budget_cases_passed':36},
 'stereo':{'stereo_cases_passed':264,'pool_cases_passed':88,'mmio_failure_cases_passed':8,'budget_cases_passed':48},
 'effect_flow':{'flow_cases_passed':8,'tick_cycles_passed':128,'batch_checkpoints_passed':2176},
 'positive_flow':{'flow_cases_passed':8,'tick_cycles_passed':128,'batch_checkpoints_passed':2176}}
evidence={}
for name,counts in expected.items():
 path=P/f'analysis/arm_sound_dispatcher_{name}_connected_verification.json';report=json.loads(path.read_text())
 assert report['connected'] is True
 for key,value in counts.items():assert report[key]==value,(name,key)
 for file,value in report['sha256'].items():assert sha(Path(file))==value,file
 assert str(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll') in report['sha256']
 evidence[str(path.relative_to(P))]={'sha256':sha(path),'counts':counts}
boot_path=P/'analysis/arm_sound_boot_native_verification.json';boot=json.loads(boot_path.read_text())
assert len(boot['loop_cases'])==16 and all(c['service_calls']==0 for c in boot['loop_cases'])
assert boot['dispatcher_source_sha256']==sha(P/'source/sa_arm_sound_dispatcher.c')
assert boot['dll_sha256']==sha(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll')
evidence[str(boot_path.relative_to(P))]={'sha256':sha(boot_path),'main_loop_cases_without_arm_callbacks':16}
result={'verification_scope':'Main native library dispatcher connection, finite constructed command/stream/sequence/effects paths; no hardware timing, synthesis, actual audio, all-command-path completeness or complete game proof. No additional complete-routine credit.','evidence':evidence}
(P/'analysis/arm_sound_dispatcher_connected_provenance.json').write_text(json.dumps(result,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text());status.setdefault('verified_progress',{})['arm_sound_dispatcher_connected']=result;path.write_text(json.dumps(status,indent=2)+'\n')
print('Verified main-library dispatcher connection: six suites and 16 main-loop cases without ARM callbacks; no new routine credit')
