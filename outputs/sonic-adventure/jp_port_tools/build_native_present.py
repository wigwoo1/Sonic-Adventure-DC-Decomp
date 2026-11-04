"""Build native render/flip services with actual shared SDK device ownership.

Relink existing frame-owner objects to export their data address only. Preserve
all original objects and source. Diagnostic game/trace bodies remain private.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','wait','frames','gpu','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    wb=json.loads((a.wait/'build_verification.json').read_text());fb=json.loads((a.frames/'build_verification.json').read_text());gb=json.loads((a.gpu/'build_verification.json').read_text())
    for b in [wb,fb,gb]:
        assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir();(d/'frames').mkdir();commands=[]
    relink=list(fb['commands'][-1]['command']);assert '-shared' in relink
    original_frame_objects={str((r/v).resolve()):sha((r/v).resolve()) for v in relink if v.endswith('.obj')}
    relink[relink.index('-o')+1]=str(d/'frames/jp_native_frames.dll');at=relink.index('-o');relink[at:at]=['-Xlinker','/export:kmiDeviceExtension,DATA'];commands.append(relink)
    authored=[p/'native'/n for n in ['jp_native_present.c','jp_native_present_sdk.c','jp_native_present.h','jp_native_frame_service_extension.c']]
    wait_source=p/'native/jp_native_frame_wait.c';raw=wait_source.read_bytes();assert sha(wait_source)==wb['authored_source_sha256'][wait_source.relative_to(r).as_posix()]
    service=d/'authored_wait_service.c';service.write_bytes(raw+b'\n'+(p/'native/jp_native_frame_service_extension.c').read_bytes())
    simple=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native')]
    sdk_path=r/'work/sonic-adventure/reshaped-sonic-adventure-review/sdk/include';assert sha(sdk_path/'kamui.h')=='c87ce01634acc830b434a1efc706c2982b2a5bb3c1f5ee1a0a76b3b58998057c'
    for source,name in [(service,'wait_service.obj'),(p/'native/jp_native_present.c','kamui_present.obj'),(p/'native/jp_native_present_sdk.c','present_sdk.obj')]:
        commands.append(simple+['-I'+str(sdk_path),'-c',str(source),'-o',str(d/name)])
    base_frame=a.wait/'external_frame_native.c';base_present=a.wait/'external_present_native.c'
    for path in [base_frame,base_present]:assert sha(path)==wb['generated_source_sha256'][path.name]
    frame=base_frame.read_bytes().replace(b'\r\n',b'\n');present=base_present.read_bytes().replace(b'\r\n',b'\n');changes=[]
    def patch(raw,name,old,new):
        before=function(raw,name);assert before.count(old)==1,(name,old);after=before.replace(old,new)
        changes.append(dict(function=name,before_sha256=hashlib.sha256(before).hexdigest(),after_sha256=hashlib.sha256(after).hexdigest()))
        return raw.replace(before,after)
    for call in ['kmRender();','kmFlipFrameBuffer();']:
        frame=patch(frame,'frame_wait',call.encode(),(call+'\n        if (jp_present_status()) return;').encode())
        present=patch(present,'show_first',call.encode(),(call+'\n    if (jp_present_status()) { jp_clock_write_tstr(run); return; }').encode())
    present=patch(present,'pvr_fits',b'static int pvr_fits(void)\n{\n',b'static int pvr_fits(void)\n{\n    if (!jp_present_timing_ready()) return FALSE;\n')
    frame=b'#include "jp_native_present.h"\n'+frame;present=b'#include "jp_native_present.h"\n'+present
    frame_path=d/'external_frame_native.c';frame_path.write_bytes(frame);present_path=d/'external_present_native.c';present_path.write_bytes(present)
    fixture=p/'jp_port_tools/frame_wait_diagnostic_fixture.c';trace=fixture.read_bytes().replace(b'\r\n',b'\n')
    for line in [b'KMSTATUS kmRender(void){event(10);return KMSTATUS_SUCCESS;}\n',b'KMSTATUS kmFlipFrameBuffer(void){event(11);return KMSTATUS_SUCCESS;}\n',b'void kmResetPointer(void){event(8);}\n']:
        assert trace.count(line)==1;trace=trace.replace(line,b'')
    hold=function(present,'present_hold');fits=function(present,'pvr_fits')
    diagnostic=frame+b'\nstatic int holding;\n'+hold+b'\n'+trace+b'''
static volatile struct kmi_device_extension *diag_device;
static Uint32 step0_cost;
#define FIRST_ROOM (VSYNC_COUNTS / 10)
#define RENDER_ROOM (VSYNC_COUNTS / 10)
#define kmiDeviceExtension (*diag_device)
extern struct kmi_interrupt_statistics kmiIntStatisticData;
'''+fits+b'''
#undef kmiDeviceExtension
JP_PROBE void jp_present_probe_device(void *device){diag_device=device;}
JP_PROBE int jp_present_probe_fits(Uint32 cost){step0_cost=cost;return pvr_fits();}
JP_PROBE unsigned jp_present_probe_busy(unsigned index)
{switch(index){case 0:return diag_device->render_busy;case 1:return diag_device->ta_busy;case 2:return diag_device->flip_pending;case 3:return diag_device->write_slot;default:return 0xffffffff;}}
'''
    diag_path=d/'external_present_diagnostic.c';diag_path.write_bytes(diagnostic)
    common=list(wb['commands'][0]['command']);common=common[:common.index('-c')]
    for path,name in [(frame_path,'frame_pacing.obj'),(present_path,'game_present.obj'),(diag_path,'diagnostic.obj')]:commands.append(common+['-c',str(path),'-o',str(d/name)])
    clocks=[Path(n) for n in wb['existing_clock_objects_sha256']]
    for n in clocks:assert sha(n)==wb['existing_clock_objects_sha256'][str(n)]
    idle=a.wait/'windows_idle.obj';assert sha(idle)==wb['production_objects_sha256']['windows_idle.obj']
    exports=['frame_pacing_init','frame_set_interval','frame_wait','frame_wait_step','frame_vsync','present_hold','njSetVSyncFunction','syTmrGetCount','kmRender','kmFlipFrameBuffer','kmSetEORCallback','kmResetPointer']
    dll=d/'jp_native_present.dll';commands.append([str(a.compiler),'-shared','-fuse-ld=lld',str(d/'diagnostic.obj'),str(d/'wait_service.obj'),str(d/'kamui_present.obj'),str(idle),*map(str,clocks),
        str(d/'frames/jp_native_frames.lib'),str(a.gpu/'jp_native_window.lib'),*[('-Wl,/export:'+n) for n in exports],'-Wl,kernel32.lib','-Wl,user32.lib','-o',str(dll)])
    regression=d/'jp_native_wait_service.dll';regression_exports=['frame_pacing_init','frame_set_interval','frame_wait','frame_wait_step','frame_vsync','present_hold','njSetVSyncFunction','syTmrGetCount']
    commands.append([str(a.compiler),'-shared','-fuse-ld=lld',str(a.wait/'diagnostic.obj'),str(d/'wait_service.obj'),str(idle),*map(str,clocks),str(a.gpu/'jp_native_window.lib'),
        *[('-Wl,/export:'+n) for n in regression_exports],'-Wl,kernel32.lib','-Wl,user32.lib','-o',str(regression)])
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,cwd=r,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in original_frame_objects.items():assert sha(Path(n))==h,n
    sources=dict(wb['authored_source_sha256'])
    for path in authored+[p/'native/jp_host_types.h',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    result=dict(status='real_native_Kamui_present_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),authored_source_sha256=sources,
        source_adaptations=changes,generated_source_sha256={n.name:sha(n) for n in [service,frame_path,present_path,diag_path]},
        production_objects_sha256={n:sha(d/n) for n in ['wait_service.obj','kamui_present.obj','present_sdk.obj','frame_pacing.obj','game_present.obj']},
        original_frame_owner_objects_sha256=original_frame_objects,frame_owner_relink_changes_exports_only=True,frames_DLL=str((d/'frames/jp_native_frames.dll').resolve()),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),
        GPU_DLL=str(a.gpu.resolve()/'jp_native_window.dll'),GPU_DLL_sha256=sha(a.gpu/'jp_native_window.dll'),DLL=str(dll),DLL_sha256=sha(dll),
        wait_regression_DLL=str(regression),wait_regression_DLL_sha256=sha(regression),existing_clock_objects_sha256=wb['existing_clock_objects_sha256'],windows_idle_object=str(idle.resolve()),windows_idle_object_sha256=sha(idle),
        wait_baseline=str(a.wait.resolve()),wait_baseline_report_sha256=sha(a.wait/'build_verification.json'),external_source_sha256=dict(wb['external_source_sha256'],**{'sdk/include/kamui.h':sha(sdk_path/'kamui.h')}),
        diagnostic_audio_bitmap_loader_providers=True,diagnostic_render_flip_providers=False,original_game_or_SDK_bodies_public=False,full_game_loop_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
