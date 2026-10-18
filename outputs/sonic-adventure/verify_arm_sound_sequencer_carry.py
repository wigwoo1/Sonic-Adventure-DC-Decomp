"""Perturb ARMv4's unpredictable MLAS carry; test complete-sequence observability."""
from pathlib import Path
P=Path(__file__).resolve().parent
verifier=P/'verify_arm_sound_sequencer.py'
exec(compile(verifier.read_text().split('for driver,fill,flags,opcode in ')[0],str(verifier),'exec'))
reference_cpu=cpu;current_driver=0;forced_carry=0;changes=0
def perturbed_cpu(memory,hardware,state,events,trace=None):
    machine=reference_cpu(memory,hardware,state,events,trace)
    sites={a+current_driver*8 for a in (0x443c,0x4710,0x4738,0x4880)}
    def after_mlas(uc,address,size,user):
        global changes
        if address in sites:
            value=uc.reg_read(arm.UC_ARM_REG_CPSR)
            uc.reg_write(arm.UC_ARM_REG_CPSR,(value&~0x20000000)|(forced_carry<<29));changes+=1
    machine.hook_add(unicorn.UC_HOOK_CODE,after_mlas)
    return machine
cpu=perturbed_cpu
passed=[]
for driver,carry,opcode in itertools.product(range(2),range(2),range(256)):
    current_driver=driver;forced_carry=carry
    result=run(driver,0,0xf0000000,opcode)
    assert result['completed'];passed.append({'driver':driver,'forced_carry':carry,'opcode':opcode})
assert changes>0,'MLAS carry perturbation never exercised'
paths=[Path(__file__),verifier,PROJECT/'source/sa_arm_sound_sequencer.c',PROJECT/'source/sa_arm_sound_sequencer_fixture.c',dll]
report={'cases_passed':len(passed),'carry_injections':changes,'cases':passed,
 'connected':connected,
 'sha256':{str(path):sha(path) for path in paths},
 'scope':'At the instruction following each MLAS, force only C to zero or one in original ARM reference execution. All 256 constructed initial-opcode sequences per driver retain exact final active CPU/RAM/MMIO equality with the native convention. Finite carry-observability evidence, not actual ARM7/AICA hardware validation or proof for all possible sequences.'}
(P/('analysis/arm_sound_sequencer_carry_connected_verification.json' if connected else 'analysis/arm_sound_sequencer_carry_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(passed),'whole-sequencer forced-carry comparisons passed with',changes,'MLAS carry injections')
