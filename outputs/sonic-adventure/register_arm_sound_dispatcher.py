"""Record bounded dispatcher baseline evidence without claiming completed integration."""
import csv,hashlib,json,re,struct
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
verification='analysis/arm_sound_dispatcher_baseline_verification.json';report=json.loads((P/verification).read_text())
assert [report[k] for k in ('instruction_cases_passed','baseline_command_cases_passed','reset_wait_cases_passed')]==[2400,1024,2]
for path,value in report['sha256'].items():assert sha(Path(path))==value,path
bank_path='analysis/arm_sound_dispatcher_banks_verification.json';banks=json.loads((P/bank_path).read_text())
assert [banks[k] for k in ('bank_cases_passed','mmio_failure_cases_passed','budget_cases_passed')]==[1044,16,36]
assert banks['reference_alignment_corrections']>0
for path,value in banks['sha256'].items():assert sha(Path(path))==value,path
stream_path='analysis/arm_sound_dispatcher_streams_verification.json';streams=json.loads((P/stream_path).read_text())
assert [streams[k] for k in ('stream_cases_passed','dsp_cases_passed','mmio_failure_cases_passed','budget_cases_passed')]==[1520,384,8,36]
for path,value in streams['sha256'].items():assert sha(Path(path))==value,path
translation=json.loads((P/'analysis/arm_sound_dispatcher_translation.json').read_text())
flow_path='analysis/arm_sound_dispatcher_effect_flow_verification.json';flow=json.loads((P/flow_path).read_text())
positive_path='analysis/arm_sound_dispatcher_positive_flow_verification.json';positive=json.loads((P/positive_path).read_text())
assert [positive[k] for k in ('flow_cases_passed','tick_cycles_passed','batch_checkpoints_passed')]==[8,128,2176]
for path,value in positive['sha256'].items():assert sha(Path(path))==value,path
assert [flow[k] for k in ('flow_cases_passed','tick_cycles_passed','batch_checkpoints_passed')]==[8,128,2176]
for path,value in flow['sha256'].items():assert sha(Path(path))==value,path
stereo_path='analysis/arm_sound_dispatcher_stereo_verification.json';stereo=json.loads((P/stereo_path).read_text())
assert [stereo[k] for k in ('stereo_cases_passed','pool_cases_passed','mmio_failure_cases_passed','budget_cases_passed')]==[264,88,8,48]
for path,value in stereo['sha256'].items():assert sha(Path(path))==value,path
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
builder=(P/'build_arm_sound_dispatcher.ps1').read_text();names=re.findall(r"'(sa_arm_sound[^']+\.c)'",builder)
known=set()
for name in names:
    text=(P/'source'/name).read_text()
    if name=='sa_arm_sound_dispatcher_fixture.c':text=text.split('            switch(r[15]){')[0]
    known.update(int(a,16) for a in re.findall(r'case (0x[0-9a-f]+)u:',text))
branches={'b','bl','beq','bne','bcc','bcs','bhi','bmi','bpl','bgt','blt','ble','bge','blne','bleq'}
tables=[]
for driver,rows in enumerate(translation['drivers']):
    path=ROOT/payloads[driver]['payload_path'];image=path.read_bytes()
    assert sha(path)==payloads[driver]['upload_sha256'];assert len(rows)==(2346,2345)[driver]
    directory=P/'analysis/arm-sequencer-enrichment'/(Path(payloads[driver]['path']).name+'.arm.bin')
    with (directory/'instructions.tsv').open() as f:enriched={int(r['address'],16):r for r in csv.DictReader(f,delimiter='\t')}
    for row in rows:
        address=int(row['address'],16);assert enriched[address]==row
        assert image[address:address+int(row['length'])].hex()==row['bytes']
        mnemonic,operand=row['instruction'].split(' ',1)
        if mnemonic in branches:assert int(operand,16) in known,(driver,row)
    assert 0x17b0+driver*12 in {int(r['address'],16) for r in rows},'stop-channel dependency missing'
    for base,count in ((0x49e4+driver*8,64),(0x4ae4+driver*8,16)):
        targets=[]
        for address in range(base,base+count*4,4):
            word=struct.unpack_from('<I',image,address)[0];assert word>>24==0xea and address in known
            delta=word&0xffffff
            if delta&0x800000:delta-=1<<24
            target=address+8+4*delta;assert target in known;targets.append(target)
        tables.append({'driver':driver,'base':base,'count':count,'targets':targets,'bytes_sha256':hashlib.sha256(image[base:base+count*4]).hexdigest()})
(P/'analysis/arm_sound_dispatcher_baseline_provenance.json').write_text(json.dumps({'verification':verification,'verification_sha256':sha(P/verification),'bank_verification':bank_path,'bank_verification_sha256':sha(P/bank_path),'stream_verification':stream_path,'stream_verification_sha256':sha(P/stream_path),'branch_tables':tables,
 'scope':'Original selected rows/bytes and branch targets have native cases, existing helpers reused. Structural evidence and zero-argument baselines do not validate all positive bank/active command paths.'},indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_dispatcher_baseline']={'verification':verification,'instruction_cases':2400,'zero_argument_command_cases':1024,'reset_wait_cases':2,
 'classification':'Separate dispatcher overlay baseline; see SMSB/SOSB/SFpB evidence for tested positive paths. See stereo/pool and connected dispatcher records for later coverage; other command paths and physical audio unfinished; no complete-routine or whole-project milestone credit'}
status['verified_progress']['arm_sound_dispatcher_banks']={'verification':bank_path,'bank_priority_cases':1044,'mmio_failure_cases':16,'budget_cases':36,'reference_alignment_corrections':banks['reference_alignment_corrections'],
 'classification':'Constructed SMSB successful starts/replacements, priority and cleanup validated in overlay; additional command paths remain unproven; main-library coverage is recorded separately; no complete-routine or whole-project milestone credit'}
status['verified_progress']['arm_sound_dispatcher_streams_dsp']={'verification':stream_path,'mono_sosb_cases':1520,'dsp_configuration_cases':384,'mmio_failure_cases':8,'budget_cases':36,
 'classification':'Constructed mono SOSB starts/replacements and SFpB DSP configuration through overlay; stereo/preemption and main-library coverage are recorded separately; other commands and physical sound execution unfinished; no complete-routine or whole-project milestone credit'}
path.write_text(json.dumps(status,indent=2)+'\n')
status['verified_progress']['arm_sound_dispatcher_stereo_pool']={'verification':stereo_path,'stereo_buffer_cases':264,'allocation_cases':88,'mmio_failure_cases':8,'budget_cases':48,'classification':'Separate overlay; original driver length-storage differences and exhausted-scan index 255 retained. No synthesis, physical timing, all-command or integration proof; no milestone credit'}
status['verified_progress']['arm_sound_dispatcher_effect_flow']={'verification':flow_path,'flows':8,'cycles':128,'checkpoints':2176,'classification':'Contiguous dispatcher/stream/sequencer/effects overlay with constructed invalid-command and missing-DSP-bank packets; shared counter consumption verified. Positive command integration, timing and audio synthesis unfinished; no milestone credit'}
path.write_text(json.dumps(status,indent=2)+'\n')
status['verified_progress']['arm_sound_dispatcher_positive_flow']={'verification':positive_path,'flows':8,'cycles':128,'checkpoints':2176,'classification':'Successful SOSB sample starts and repeated replacements through contiguous dispatcher/stream/sequencer/effects overlay; independent active/sample/ownership/mailbox checks. Other command families, main-driver integration, timing and synthesis unfinished; no milestone credit'}
path.write_text(json.dumps(status,indent=2)+'\n')
provenance_path=P/'analysis/arm_sound_dispatcher_baseline_provenance.json'
provenance=json.loads(provenance_path.read_text());provenance.update({'stereo_verification':stereo_path,'stereo_verification_sha256':sha(P/stereo_path)})
provenance.update({'effect_flow_verification':flow_path,'effect_flow_verification_sha256':sha(P/flow_path)})
provenance.update({'positive_flow_verification':positive_path,'positive_flow_verification_sha256':sha(P/positive_path)})
provenance_path.write_text(json.dumps(provenance,indent=2)+'\n')
print('Recorded dispatcher overlay: baseline, 1044 SMSB, 1520 mono SOSB and 384 DSP cases; focused guards passed; no connected registry credit')
