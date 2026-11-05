"""Link original game card lifecycle and real filesystem into the frame owner.
Original game/SDK bodies and combined diagnostic DLLs remain private.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
NAMES=['card_init','card_finish','card_notice_busy','card_complete','card_progress','card_init_done']
def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p): return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','upstream','frames','filesystem','presentation','present-source','output'):
        ap.add_argument('--'+name,dest=name.replace('-','_'),type=Path,required=True)
    a=ap.parse_args(); r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    f=read(a.frames/'build_verification.json'); fs=read(a.filesystem/'build_verification.json')
    present_file=a.presentation/'verification.json';pr=read(present_file);ps=read(a.present_source/'build_verification.json')
    assert sha(f['frames_DLL'])==f['frames_DLL_sha256']; assert sha(fs['DLL'])==fs['DLL_sha256']
    for b in (f,fs):
        for n,h in b.get('authored_source_sha256',{}).items(): assert sha(r/n)==h,n
    assert sha(a.present_source/'present_real_fog.c')==ps['generated_sources_sha256']['present_real_fog.c']
    fl=list(next(x['command'] for x in f['commands'] if '-shared' in x['command'] and x['command'][-1]==f['frames_DLL']))
    pl=list(next(x['command'] for x in pr['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_present.dll'))
    keep={n:sha(n) for cmd in (fl,pl) for n in cmd if n.endswith('.obj')}
    fs_objects=[x for x in fs['commands'][-1]['command'] if x.endswith('.obj')]
    for n in fs_objects: keep[n]=sha(n)
    definitions={}
    for n in fl:
        if n.endswith('.obj'):
            for s in coff_symbols(Path(n)):
                if s['section']>0: definitions.setdefault(s['name'],[]).append(n)
    for n in NAMES: assert n not in definitions,n
    source=u/'src/save/card.c';raw=source.read_bytes();bodies={n:function(raw,n) for n in NAMES}
    original_inputs={x.relative_to(u).as_posix():sha(x) for x in [source,u/'src/save/card.h',u/'src/system/frame.c']}
    git='C:/Program Files/Git/cmd/git.exe'
    assert subprocess.check_output([git,'-C',str(u),'rev-parse','HEAD']).decode().strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    for n,h in original_inputs.items(): assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','HEAD:'+n])).hexdigest()==h,n
    d.mkdir(); src=b'#include <sg_xpt.h>\n#include <shinobi.h>\n#include <string.h>\n#include <vmsfs.h>\n#include "save/card.h"\n#include "jp_native_card_state.h"\n#define DRIVE_BUSY ((Sint32)BUD_STAT_BUSY)\n'
    enum=re.search(rb'enum card_notice\s*\{.*?\};',raw,re.S); assert enum;src+=enum.group()+b'\n'
    for name in ('card_library_work','card_drives','card_notice','card_notice_on'):
        found=re.findall(rb'^static [^;\r\n]*\b'+name.encode()+rb'\b[^;\r\n]*;',raw,re.M);assert len(found)==1,name;src+=found[0]+b'\n'
    reused={};added=[]
    for name in ('save_slot','save_drive','save_disabled','card_saving'):
        if name in definitions:
            assert len(definitions[name])==1,(name,definitions[name]);reused[name]=definitions[name][0]
        else:
            found=re.findall(rb'^(?:int|Bool) '+name.encode()+rb' = [^;\r\n]*;',raw,re.M);assert len(found)==1,name;src+=found[0]+b'\n';added.append(name)
    src+=b'static void card_init_done(void);\n'
    for name in NAMES: src+=bodies[name]+b'\n'
    src+=b'#include "jp_native_card_observer.inc"\n'; generated=d/'external_card_lifecycle.c';generated.write_bytes(src)
    common=list(fs['commands'][0]['command']);common=common[:common.index('-c')]
    obj=d/'card_lifecycle.obj';commands=[common+['-c',str(generated),'-o',str(obj)]]
    data_exports=[arg for n in added for arg in ('-Xlinker','/export:'+n+',DATA')]
    fl[fl.index('-o'):fl.index('-o')]=[str(obj)]+fs_objects+['-Wl,/export:'+n for n in NAMES if n!='card_init_done']+['-Wl,/export:'+n for n in fs['exports']]+data_exports
    fl[-1]=str(d/'jp_native_frames.dll');commands.append(fl)
    text=(a.present_source/'present_real_fog.c').read_bytes(); diagnostic=b'void buExecServer(void){event(1);}'
    assert text.count(diagnostic)==1;text=text.replace(diagnostic,b'')
    # Keep the existing test trace, but the call itself now resolves to the
    # actual filesystem server in the shared frame/state owner.
    vsync=function(text,'frame_vsync');assert vsync.count(b'buExecServer();')==1
    text=text.replace(vsync,b'#define buExecServer() (buExecServer(),event(1))\n'+vsync+b'#undef buExecServer\n')
    assert function(text,'frame_vsync')==vsync
    present=d/'present_with_card_service.c';present.write_bytes(text)
    old_obj=next(n for n in pl if n.endswith('present_real_fog.obj'))
    pc=list(next(x['command'] for x in ps['commands'] if '-c' in x['command'] and x['command'][-1]==old_obj))
    pc[-3]=str(present);pc[-1]=str(d/'present_with_card_service.obj');commands.append(pc)
    pl[pl.index(old_obj)]=pc[-1]
    for i,n in enumerate(pl):
        if n.endswith('jp_native_frames.lib'): pl[i]=str(d/'jp_native_frames.lib')
    pl[-1]=str(d/'jp_native_present.dll');commands.append(pl)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in keep.items(): assert sha(n)==h,n
    for n,h in original_inputs.items(): assert sha(u/n)==h,n
    kept_frame={n:sha(n) for n in fl if n.endswith('.obj')}
    authored=[p/'native/jp_native_card_state.h',p/'native/jp_native_card_observer.inc',Path(__file__).resolve()]
    result=dict(status='original_card_lifecycle_and_real_frame_vsync_backup_server_linked_not_runtime_verified',commands=results,frames_DLL=str(d/'jp_native_frames.dll'),frames_DLL_sha256=sha(d/'jp_native_frames.dll'),present_DLL=str(d/'jp_native_present.dll'),present_DLL_sha256=sha(d/'jp_native_present.dll'),authored_source_sha256={str(x.relative_to(r)):sha(x) for x in authored},original_source_sha256=original_inputs,original_functions_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},generated_sources_sha256={x.name:sha(x) for x in (generated,present)},kept_input_objects_sha256=keep,kept_frame_objects_sha256=kept_frame,reused_original_global_owners=reused,new_original_global_owners=added,original_card_function_bodies_unchanged=True,original_frame_vsync_body_tokens_unchanged=True,diagnostic_backup_server_replaced_by_actual_original_service=True,filesystem_build_sha256=sha(a.filesystem/'build_verification.json'),frame_baseline_sha256=sha(a.frames/'build_verification.json'),presentation_baseline_sha256=sha(present_file),artifact_publishable=False,full_system_boot_or_Chao_game_verified=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
