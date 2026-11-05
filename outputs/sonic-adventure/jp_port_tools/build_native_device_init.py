"""Adapt full Ninja init to actual host GPU/window/video/wait ownership.

Generate original code privately; use real original frame calculations and
typed state, preserving every existing source/object/proof. No hardware stubs.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','preserved','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(a.baseline/'build_verification.json');history=read(a.preserved/'preserved_source.json')
    for n,h in b['authored_source_sha256'].items():
        if n in history:assert history[n]['sha256']==h and sha(history[n]['preserved_path'])==h,n
        else:assert sha(r/n)==h,n
    for kind in ('frames','present','GPU'):assert sha(b[kind+'_DLL'])==b[kind+'_DLL_sha256']
    fl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    pl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['present_DLL']))
    oldobjects={n:sha(n) for cmd in (fl,pl) for n in cmd if n.endswith('.obj')}
    names=['_nj_display_mode_','_nj_frame_mode_','_nj_waitvsync_count','_nj_frame_size_w','_nj_frame_size_h','_nj_framebuffer_surface0_','_nj_framebuffer_surface1_','_nj_ppSurfaceArray']
    vertex=[]
    for path in oldobjects:
        definitions={s['name'] for s in coff_symbols(Path(path)) if s['section']>0}
        assert not definitions.intersection(names+['kmiFBControl']),path
        if '_nj_vertex_buffer_' in definitions:vertex.append(path)
    assert len(vertex)==1,vertex
    source_names=['sdk/ninja/njSystem.c','sdk/ninja/njGlobal.c','sdk/ninja/video_kamui_lib.c','sdk/include/kamui.h','sdk/include/ninja.h']
    originals={n:sha(u/n) for n in source_names};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+n])).hexdigest()==h,n
    d.mkdir();generated=[];commands=[]
    raw=(u/'sdk/ninja/njGlobal.c').read_bytes();state=b'#include <kamui.h>\n#include <ninja.h>\n'
    for n in names:
        found=re.findall(rb'^(?:Int|NJS_TEXSURFACE|PKMSURFACEDESC) '+n.encode()+rb'(?:\[2\])?;',raw,re.M);assert len(found)==1,(n,found);state+=found[0]+b'\n'
    path=d/'external_ninja_device_state.c';path.write_bytes(state);generated.append(path)
    raw=(u/'sdk/ninja/video_kamui_lib.c').read_bytes();prefix=raw[:raw.index(b'void kmiDACControl(KMINT32 level, KMINT32 green_sync, KMINT32 power_down);')]
    path=d/'external_frame_control.c';path.write_bytes(prefix+b'\n#define GCC\n'+function(raw,'store_frame')+function(raw,'kmiFBControl'));generated.append(path)
    frame_common=list(b['commands'][0]['command']);frame_common=frame_common[:frame_common.index('-c')]
    additions=[]
    for path in generated:
        obj=path.with_suffix('.obj');additions.append(str(obj));commands.append(frame_common+['-c',str(path),'-o',str(obj)])
    exports=['-Wl,/export:kmiFBControl','-Wl,/export:jp_frame_queue_empty']+[arg for n in names+['_nj_vertex_buffer_'] for arg in ('-Xlinker','/export:'+n+',DATA')]
    fl[fl.index('-o'):fl.index('-o')]=additions+exports;fl[-1]=str(d/'jp_native_frames.dll');commands.append(fl)
    oldpresent=next(n for n in pl if n.endswith('kamui_present.obj'))
    pc=list(next(x['command'] for x in b['commands'] if '-c' in x['command'] and x['command'][-1]==oldpresent))
    pc[pc.index('-c')+1]=str(p/'native/jp_native_present.c');pc[-1]=str(d/'kamui_present.obj');commands.append(pc)
    pl[pl.index(oldpresent)]=pc[-1]
    for i,n in enumerate(pl):
        if n.endswith('jp_native_frames.lib'):pl[i]=str(d/'jp_native_frames.lib')
    pl[pl.index('-o'):pl.index('-o')]=['-Wl,/export:kmSetWaitVsyncCount'];pl[-1]=str(d/'jp_native_present.dll');commands.append(pl)
    raw=(u/'sdk/ninja/njSystem.c').read_bytes();original=function(raw,'njInitSystem');body=original.replace(b'void njInitSystem(',b'void jp_original_ninja_system(',1);changes=[]
    def patch(old,new,count=1):
        nonlocal body
        assert body.count(old)==count,(old,body.count(old));body=body.replace(old,new);changes.append(dict(old=old.decode(),new=new.decode(),count=count))
    patch(b'waitVsync();',b'waitVsync();\n        if (jp_device_status()) return;',2)
    before=re.findall(rb'    VIDEO_CONTROL = VIDEO_CONTROL \| VIDEO_BLANK;\s+INT_STATUS = INT_VBLANK_IN;\s+while \(!\(INT_STATUS & INT_VBLANK_IN\)\) \{\s+\}',body);assert len(before)==1
    patch(before[0],b'    if (jp_device_wait_tick()) return;')
    for old,code in [(b'kmInitDevice(video);','GPU'),(b'kmSetDisplayMode(display, bpp, TRUE, anti_alias);','VIDEO'),
        (b'kmSetEORCallback(njSetEORflag, NULL);','CALLBACK'),(b'kmSetEndOfVertexCallback(njSetEOVflag, NULL);','CALLBACK'),
        (b'kmfSetFatalErrorCallback(njSetERRflag);','CALLBACK'),(b'kmSetWaitVsyncCount(_nj_waitvsync_count);','WAIT')]:
        patch(old,b'if ('+old[:-1]+b') { jp_device_fail(JP_DEVICE_'+code.encode()+b'); return; }')
    patch(b'njInitGlobal();',b'njInitGlobal();\n    if (jp_ninja_global_status()) jp_device_fail(JP_DEVICE_GLOBAL);')
    prefix=b'''#include <kamui.h>
#include <ninja.h>
#include <sg_xpt.h>
#include <shinobi.h>
#include "ninja/njkamui.h"
#include "jp_native_device_internal.h"
void waitVsync(void);
void njSetVideoMode(Uint32 cable);
void njSetEOVflag(void *arg);
void njSetERRflag(void *arg);
void njInitGlobal(void);
int jp_ninja_global_status(void);
#define DSP_VIDEO 3
#define CABLE_SHIFT 8
#define GCC
'''
    path=d/'external_ninja_system_native.c';path.write_bytes(prefix+body);generated.append(path)
    # Import actual external data through the platform's native DLL ABI. This
    # is a linkage transform only, leaving original types/field accesses intact.
    sdk_sources=[path,p/'native/jp_native_device_sdk.c'];ir_records=[]
    for source in sdk_sources:
        ir=d/(source.stem+'.raw.ll');cmd=frame_common+['-S','-emit-llvm',str(source),'-o',str(ir)];commands.append(cmd)
    inc=d/'include';inc.mkdir()
    imported=[]
    for name in ('jp_native_clock.h','jp_native_packets.h'):
        source=p/'native'/name;target=inc/name;target.write_bytes(source.read_bytes().replace(b'__declspec(dllexport)',b'__declspec(dllimport)'));imported.append(target)
    force=inc/'device_imports.h';force.write_text('#include "jp_native_clock.h"\n#undef JP_CLOCK_API\n#define JP_CLOCK_API __declspec(dllimport)\n#include "jp_native_packets.h"\n#undef JP_PACKET_API\n#define JP_PACKET_API __declspec(dllimport)\n')
    compiler=frame_common[0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-include',str(force),'-I'+str(inc),'-I'+str(p/'native')]
    commands.append(host+['-c',str(p/'native/jp_native_device.c'),'-o',str(d/'device.obj')])
    results=[]
    def run(cmd):
        i=len(results);v=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(v.stdout+v.stderr)
        results.append(dict(command=cmd,exit_code=v.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not v.returncode and not log.stat().st_size,log.read_text()
    for cmd in commands:run(cmd)
    for source in sdk_sources:
        ir=d/(source.stem+'.raw.ll');text=ir.read_text();data=[]
        def imported_global(m):
            data.append(m.group(1));attributes=m.group(2).replace('dso_local ','')
            return '@'+m.group(1)+' = external dllimport '+attributes+'global'
        changed=re.sub(r'^@([A-Za-z_]\w*) = external ((?:(?:dso_local|local_unnamed_addr|unnamed_addr|thread_local) )*)global',imported_global,text,flags=re.M)
        assert len(data)==len(re.findall(r'^@\w+ = external .*?global',text,re.M))
        fixed=d/(source.stem+'.imports.ll');fixed.write_text(changed);obj=d/(source.stem+'.obj')
        run([compiler,'--target=x86_64-pc-windows-msvc','-O2','-c',str(fixed),'-o',str(obj)]);ir_records.append(dict(source=str(source),imported_globals=data,raw_sha256=sha(ir),import_ir_sha256=sha(fixed)))
    dll=d/'jp_native_device.dll';run([compiler,'-shared','-fuse-ld=lld',str(d/'device.obj'),*[str(d/(s.stem+'.obj')) for s in sdk_sources],str(d/'jp_native_frames.lib'),str(d/'jp_native_present.lib'),str(Path(b['GPU_DLL']).with_suffix('.lib')),str(Path(b['vertex_lifecycle_DLL']).with_suffix('.lib')),
        *['-Wl,/export:'+n for n in ('njInitSystem','kmInitDevice','kmSetDisplayMode','waitVsync','njSetVideoMode')],'-Wl,kernel32.lib','-Wl,user32.lib','-o',str(dll)])
    for n,h in originals.items():assert sha(u/n)==h,n
    for n,h in oldobjects.items():assert sha(n)==h,n
    authored=dict(b['authored_source_sha256'])
    own=[p/'native'/n for n in ('jp_native_device.c','jp_native_device_sdk.c','jp_native_device.h','jp_native_device_internal.h')]+[Path(__file__).resolve()]
    for n in history:authored[n]=sha(r/n)
    for n in own:authored[n.relative_to(r).as_posix()]=sha(n)
    result=dict(status='full_original_Ninja_init_native_GPU_window_video_wait_resources_linked_unverified',commands=results,
        **{kind+'_DLL':str(d/('jp_native_'+('frames' if kind=='frames' else 'present')+'.dll')) for kind in ('frames','present')},
        frames_DLL_sha256=sha(d/'jp_native_frames.dll'),present_DLL_sha256=sha(d/'jp_native_present.dll'),
        GPU_DLL=b['GPU_DLL'],GPU_DLL_sha256=b['GPU_DLL_sha256'],device_DLL=str(dll),device_DLL_sha256=sha(dll),
        vertex_lifecycle_DLL=b['vertex_lifecycle_DLL'],vertex_lifecycle_DLL_sha256=sha(b['vertex_lifecycle_DLL']),
        authored_source_sha256=authored,superseded_authored_source=history,generated_sources_sha256={s.name:sha(s) for s in generated},
        original_source_sha256=originals,original_njInitSystem_function_sha256=hashlib.sha256(original).hexdigest(),source_adaptations=changes,
        original_kmiFBControl_body_unchanged=True,original_store_frame_body_unchanged=True,LLVM_data_imports=ir_records,
        kept_frame_objects_sha256={n:sha(n) for n in fl if n.endswith('.obj')},kept_present_objects_sha256={n:sha(n) for n in pl if n.endswith('.obj')},
        original_objects_preserved_sha256=oldobjects,baseline_build_sha256=sha(a.baseline/'build_verification.json'),
        existing_vertex_descriptor_owner_reused=vertex[0],real_native_resource_ownership=True,unadapted_video_MMIO_executed=False,
        console_antialias_supported=False,whole_system_boot_or_audio_implemented=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
