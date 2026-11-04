"""Build real clock/tick services and adapted complete external frame source.

Game GCC branches are selected locally, without enabling SH-4 SDK inlines.
Original sources/proofs remain unchanged. The diagnostic DLL has trace fixtures
and stays private; production objects have no fixture providers.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','presentation','gpu','production-audit','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    pb=json.loads((a.presentation/'build_verification.json').read_text());gb=json.loads((a.gpu/'build_verification.json').read_text());audit=json.loads(a.production_audit.read_text())
    for b in [pb,gb]:
        assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    raw=(u/'src/system/frame.c').read_bytes();assert hashlib.sha256(raw).hexdigest()=='54e27d894217925eb9a5194df014e03f8a99a6e6dfd10ef00bb692f06b951150'
    text=raw.decode().replace('\r\n','\n');changes={
        '#ifdef GCC\nvolatile int frame_countdown;\n#else\nint frame_countdown;\n#endif':'int frame_countdown;',
        '    while (frame_countdown > 0) {\n    }':'    if (jp_wait_until_le(&frame_countdown, 0)) return;',
        '    while (frame_countdown > left) {\n    }':'    if (jp_wait_until_le(&frame_countdown, left)) return;',
        '    if (draw_off == TRUE || present_hold()) {':'    int native_hold = draw_off == TRUE ? FALSE : present_hold();\n    if (jp_wait_status()) return;\n    if (draw_off == TRUE || native_hold) {',
        'void frame_wait(void)\n{':'void frame_wait(void)\n{\n    if (jp_wait_sync()) return;',
        'void frame_wait_step(int left)\n{':'void frame_wait_step(int left)\n{\n    if (jp_wait_sync()) return;',
    }
    for old,new in changes.items():assert text.count(old)==1,old;text=text.replace(old,new)
    gcc_branches=text.count('#ifdef GCC');assert gcc_branches==6,gcc_branches
    text=text.replace('#ifdef GCC','#if defined(GCC) || defined(JP_NATIVE_FRAME_WAIT)')
    # Forced native compatibility headers already expose port declarations.
    # The existing frame.h's plain int matches serialized callback ownership.
    frame=b'#include "jp_native_frame_wait.h"\n_Static_assert(sizeof(int)==4,"frame counter ABI");\n'+text.encode()
    present=(a.presentation/'external_present_native.c').read_bytes();assert hashlib.sha256(present).hexdigest()==pb['generated_source_sha256']
    hold=function(present,'present_hold');old=b'    while (frame_countdown == frame_interval) {\n    }'
    normalized=hold.replace(b'\r\n',b'\n');assert normalized.count(old)==1
    newhold=normalized.replace(old,b'    if (jp_wait_while_equal(&frame_countdown, frame_interval)) return TRUE;')
    assert present.count(hold)==1;present=b'#include "jp_native_frame_wait.h"\n'+present.replace(hold,newhold)
    d.mkdir();frame_path=d/'external_frame_native.c';frame_path.write_bytes(frame)
    present_path=d/'external_present_native.c';present_path.write_bytes(present)
    fixture=p/'jp_port_tools/frame_wait_diagnostic_fixture.c';diag=d/'external_frame_diagnostic.c'
    diag.write_bytes(frame+b'\nstatic int holding;\n'+newhold+b'\n'+fixture.read_bytes())
    cmd=list(pb['command']);common=cmd[:cmd.index('-c')]
    common+=['-DJP_NATIVE_FRAME_WAIT=1']
    commands=[common+['-c',str(frame_path),'-o',str(d/'frame_pacing.obj')],common+['-c',str(present_path),'-o',str(d/'present.obj')],common+['-c',str(diag),'-o',str(d/'diagnostic.obj')]]
    authored=[p/'native/jp_native_frame_wait.c',p/'native/jp_native_frame_wait_windows.c']
    for source,name in zip(authored,['wait.obj','windows_idle.obj']):
        commands.append([str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-c',str(source),'-o',str(d/name)])
    clocks=[]
    for name in ['clock.obj','sdk_clock.obj']:
        paths=[Path(n) for n in audit['linked_objects_sha256'] if Path(n).name==name];assert len(paths)==1;path=paths[0];assert sha(path)==audit['linked_objects_sha256'][str(path)];clocks.append(path)
    exports=['frame_pacing_init','frame_set_interval','frame_wait','frame_wait_step','frame_vsync','present_hold','njSetVSyncFunction','syTmrGetCount']
    dll=d/'jp_native_frame_wait.dll';commands.append([str(a.compiler),'-shared','-fuse-ld=lld',str(d/'diagnostic.obj'),str(d/'wait.obj'),str(d/'windows_idle.obj'),*map(str,clocks),
        str(a.gpu/'jp_native_window.lib'),*[('-Wl,/export:'+n) for n in exports],'-Wl,kernel32.lib','-Wl,user32.lib','-o',str(dll)])
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    inputs=[u/'src/system/frame.c',u/'src/system/frame.h',u/'gcc/src/system/present.c',u/'gcc/src/system/port.h',u/'sdk/include/machine.h']
    authored += [p/'native/jp_native_frame_wait.h',fixture,Path(__file__).resolve()]
    report=dict(status='native_frame_wait_and_original_source_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),
        authored_source_sha256={n.relative_to(r).as_posix():sha(n) for n in authored},external_source_sha256={n.relative_to(u).as_posix():sha(n) for n in inputs},
        frame_adaptations=changes,locally_selected_GCC_game_branches=gcc_branches,SDK_GCC_hardware_inlines_enabled=False,frame_countdown_owner='original int, serialized game-thread callback; volatile reads in native wait service',
        generated_source_sha256={n.name:sha(n) for n in [frame_path,present_path,diag]},production_objects_sha256={n:sha(d/n) for n in ['frame_pacing.obj','present.obj','wait.obj','windows_idle.obj']},
        frame_pacing_undefined_symbols=sorted(s['name'] for s in coff_symbols(d/'frame_pacing.obj') if s['section']==0 and s['value']==0),
        existing_clock_objects_sha256={str(n):sha(n) for n in clocks},GPU_DLL=str(a.gpu.resolve()/'jp_native_window.dll'),GPU_DLL_sha256=sha(a.gpu/'jp_native_window.dll'),
        frames_DLL=gb['frames_DLL'],frames_DLL_sha256=gb['frames_DLL_sha256'],DLL=str(dll),DLL_sha256=sha(dll),diagnostic_fixture_in_DLL=True,diagnostic_DLL_publishable=False,
        original_frame_functions_retained=True,original_proofs_and_source_modified=False,full_game_frame_loop_executed=False,Kamui_render_flip_implemented=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
