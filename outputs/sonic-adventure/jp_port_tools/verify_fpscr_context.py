"""Execute original startup/callback code and interrupt FPSCR-setting slices.

This is a bounded control-state qualification, not a boot-ROM or game run.
The native inverse mismatch witness diagnoses the old nearest-mode component.
External game-source bodies and binary bytes are read locally, never exported.
"""
import argparse
import ctypes as C
import hashlib
import json
import random
import struct
import sys
from pathlib import Path


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    ap=argparse.ArgumentParser()
    for name in ['workspace','upstream','binary','nearest-build','report']:
        ap.add_argument('--'+name,dest=name.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure'
    assert a.report.resolve().is_relative_to(r/'work/sonic-adventure') and not a.report.exists()
    pin_path=p/'jp_port_tools/jp_fpscr_context_external_inputs.json'
    pin=json.loads(pin_path.read_text())
    for name,row in pin['files'].items():
        raw=(a.upstream/name).read_bytes()
        assert hashlib.sha256(raw).hexdigest()==row['sha256'],name
        assert hashlib.sha1(b'blob '+str(len(raw)).encode()+b'\0'+raw).hexdigest()==row['Git_blob_sha1'],name
    assert sha(a.binary)==pin['original_binary_sha256']
    sys.path.insert(0,str(p))
    from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    binary=a.binary.read_bytes();ram=bytearray(16<<20)
    ram[0x10000:0x10000+len(binary)]=binary
    rng=random.Random(0x46505343);checks=0

    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1

    modes=[(dn<<18)|(sz<<20)|(bank<<21)|rm|flags
           for dn in [0,1] for sz in [0,1] for bank in [0,1] for rm in [0,1]
           for flags in [0,0x3f07c,0xf80,0x3fffc]]
    for mode in modes:
        s=State();s.pc=int(pin['startup_entry'],16);s.pr=0x8cfed000
        f=Fpu();f.fpscr=mode;f.fpul=rng.getrandbits(32)
        for bank in range(2):f.banks[bank][:]=[rng.getrandbits(32) for i in range(16)]
        saved=bytes(f);ccr=rng.getrandbits(32);mmio={0xff00001c:ccr}
        count=reference(ram,s,fpu=f,mmio=mmio,stop_pc=int(pin['init_entry'],16),instruction_budget=64)
        check(bytes(f)==saved,'retail entry through jump_to_init inherits all FPU state')
        check(mmio[0xff00001c]==(ccr&0x89af)|0x800,'actual cache entry effects')
        check(0<count<64,'startup bounded instruction count')

    sequence_calls=0
    # Execute the original four instructions, not a source-level replacement.
    for name,boundary in pin['interrupt_boundaries'].items():
        for text in boundary['set_default_sequences']:
            address=int(text,16)
            for mode in modes+[0x3fffff,0xffffffff]:
                s=State();s.pc=address;s.pr=0x8cfed000;s.r[0]=rng.getrandbits(32)
                f=Fpu();f.fpscr=mode
                count=reference(ram,s,fpu=f,stop_pc=address+8,instruction_budget=5)
                check(count==4 and f.fpscr==0x40001,(name,'actual default-setting slice'))
                sequence_calls+=1

    callback_calls=0
    for mode in modes:
        f=Fpu();f.fpscr=mode;f.fpul=rng.getrandbits(32)
        for bank in range(2):f.banks[bank][:]=[rng.getrandbits(32) for i in range(16)]
        before=bytes(f);bank=(mode>>21)&1;seen=[]
        s=State();s.pc=int(pin['callback_entry'],16);s.pr=0x8cfed000
        s.r[4]=0x8cfed100;s.r[5]=rng.getrandbits(32);argument=s.r[5];s.r[15]=0x8cf00000
        guard=rng.randbytes(256);ram[0xefff80:0xf00080]=guard
        def callback(memory,registers,sr):
            seen.append((f.fpscr,registers[4]))
            f.banks[bank][:]=[rng.getrandbits(32) for i in range(16)]
            # A legal callback retains SZ/FR/PR and can change its status flags.
            f.fpscr^=0x1004
            return sr
        reference(ram,s,fpu=f,hooks={0x8cfed100:callback},instruction_budget=128)
        check(seen==[(mode&~0x100000,argument)],'callback only clears SZ, inherits RM/DN/FR')
        check(bytes(f)==before,'whole callback restores FPSCR and both original float banks')
        check(s.r[15]==0x8cf00000,'callback restores SP')
        check(ram[0xefff80:0xefffb8]==guard[:56] and ram[0xf00000:0xf00080]==guard[128:],
              'callback stack writes stay within the actual 72-byte frame')
        callback_calls+=1

    # Pin source control edges without pretending the hardware interrupt ran.
    source_edges={
        'src/system/frame.c':['njSetVSyncFunction(frame_vsync);'],
        'sdk/ninja/njSystem.c':['syChainAddHandler(SYD_CHAIN_EVENT_IML6_VBLANK,'],
        'sdk/shinobi/syChain.c':['kmSetVSyncCallback(syChainMasterHandlerPVR_V, NULL);','isr->usrIsr(isr->arg);'],
        'sdk/ninja/kmcalbac_kamui_lib.c':['kmiRegisterCallback(kmiHardwareResource, VSYNC_CALLBACK, callback,'],
        'sdk/ninja/interupt_kamui_lib.c':['kmiExCallBackFunc(kmiDeviceExtension.callback[i].function,'],
        'sdk/shinobi/rfiber.c':['#define FIBER_FPSCR 0x40001','set_fpscr(FIBER_FPSCR);','set_fpscr(fpscr);']}
    for name,needles in source_edges.items():
        text=(a.upstream/name).read_text()
        for needle in needles:check(needle in text,('pinned source edge',name,needle))

    build=json.loads((a.nearest_build/'build_verification.json').read_text())
    dll=Path(build['DLL']);check(sha(dll)==build['DLL_sha256'],'preserved nearest-mode DLL')
    lib=C.CDLL(str(dll));lib.njInvertMatrix.argtypes=[C.c_void_p];lib.njInvertMatrix.restype=C.c_int
    matrix_type=C.c_uint32*16;witness=None;matrix_calls=0
    for case in range(16):
        raw=struct.pack('<16f',*[rng.uniform(-3,3) for i in range(16)])
        guest=0x8cd10000;outputs={}
        for rm in [0,1]:
            ram[0xd10000:0xd10040]=raw
            s=State();s.pc=0x8c627f9c;s.pr=0x8cfed000;s.r[4]=guest;s.r[15]=0x8cf00000
            f=Fpu();f.fpscr=0x40000|rm
            reference(ram,s,fpu=f,instruction_budget=4000)
            outputs[rm]=(s.r[0],bytes(ram[0xd10000:0xd10040]));matrix_calls+=1
        native=matrix_type.from_buffer_copy(raw);code=lib.njInvertMatrix(C.byref(native))
        check((code,bytes(native))==outputs[0],'old component retains its qualified nearest-mode contract')
        if outputs[0]!=outputs[1]:
            witness=dict(input_words=list(struct.unpack('<16I',raw)),
                         nearest_words=list(struct.unpack('<16I',outputs[0][1])),
                         toward_zero_words=list(struct.unpack('<16I',outputs[1][1])),
                         native_old_matches_nearest=True,native_old_matches_toward_zero=False)
            break
    check(witness is not None,'actual original inverse distinguishes nearest from toward-zero')
    report=dict(status='bounded_original_FPSCR_contexts_verified_RZ_integration_required',checks=checks,
        upstream_commit=pin['upstream_commit'],external_input_manifest_sha256=sha(pin_path),
        verifier_sha256=sha(Path(__file__)),executor_sha256=sha(p/'verify_startup.py'),
        original_binary_sha256=sha(a.binary),startup_entry_executions=len(modes),
        original_interrupt_default_setting_slice_executions=sequence_calls,
        complete_original_callback_executions=callback_calls,original_inverse_calls=matrix_calls,
        startup_entry_preserves_incoming_FPSCR=True,interrupt_handler_entry_mode='0x40001',
        callback_preserves_RM_DN=True,source_control_edges_checked=source_edges,
        nearest_component_DLL_sha256=sha(dll),rounding_mode_mismatch_witness=witness,
        main_boot_ROM_handoff_mode_measured=False,full_interrupt_dispatch_executed=False,
        next_action='Qualify native toward-zero/DN1 arithmetic and matrix providers; retain nearest baseline as historical proof.',
        limitations=['Startup execution stops at _INIT; it does not execute the boot ROM, hardware initialization or main.',
                     'Interrupt tests execute the actual four-instruction default-setting slices; they do not emulate complete interrupt dispatch.',
                     'The complete original callback runner executes with a diagnostic hook; source edges locate the frame-vsync callback.',
                     'Main-thread RZ is consistent with CPU reset and source intent but BIOS handoff and all subsequent contexts remain unmeasured.',
                     'The old nearest-mode matrix component is not qualified for toward-zero contexts.'],
        overall_completion_percent=10,completion_basis='weighted_milestone_estimate_not_code_coverage',goal_paused=False,playable=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(checks,'checks;',sequence_calls,'interrupt slices;',callback_calls,'complete callbacks; actual inverse rounding mismatch reproduced.')


if __name__=='__main__':main()
