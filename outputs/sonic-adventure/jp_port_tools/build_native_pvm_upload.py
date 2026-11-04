"""Extend the frozen native memory uploader; retain real original PVM helpers.
All game/SDK bodies and their compiled DLLs remain private external inputs.
Exactly one native memory, texture, queue, frame, video and device owner.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((base/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    original=p/'native/jp_native_texture_upload.c';raw=original.read_bytes();assert sha(original)==b['authored_source_sha256'][original.relative_to(r).as_posix()]
    old=b'if(source)return fail(JP_MEMORY_STATE);';new=b'if(source || jp_pvm_destination_active())return fail(JP_MEMORY_STATE);'
    shutdown=function(raw,'jp_memory_shutdown');assert shutdown.count(old)==1
    source=b'__declspec(dllexport) int jp_pvm_destination_active(void);\n'+raw.replace(shutdown,shutdown.replace(old,new))+b'\n'+(p/'native/jp_native_pvm_extension.c').read_bytes()
    external=u/'sdk/ninja/njLoadTexturePvmFile.c';assert sha(external)=='b94876a9e32513d7addb9e6867758509d22065d4ded2c47657cba44ac1a9975f'
    sdk=external.read_bytes();names=['get_w','get_l','njCalcPvmStatusSize','njSetTexListToPvmM','njSearchEntryCount','njSetPvmTextureList'];bodies={n:function(sdk,n) for n in names}
    table=bodies['njSetTexListToPvmM'];assert table.count(b'    Uint32 banked_index;')==1 and table.count(b'get_l(&banked_index, entry + offset);')==1
    table=table.replace(b'    Uint32 banked_index;\n',b'').replace(b'get_l(&banked_index, entry + offset);',b'get_l(&texname->texaddr, entry + offset);');bodies['njSetTexListToPvmM']=table
    d.mkdir();(d/'frames').mkdir();authored=d/'authored_pvm_memory.c';authored.write_bytes(source)
    helper=d/'external_pvm_helpers.c';helper.write_bytes(b'#include <string.h>\n#include "jp_native_pvm.h"\n#define pvmh_header jp_pvm_header\n#define PVMH_PVRNAME 1\n#define PVMH_CATEGORYCODE 2\n#define PVMH_ENTRYINFO 4\n#define PVMH_GLOBALINDEX 8\n#define PVMH_BANKID 0x400\n#define njMemCopy(a,b,n) memcpy((a),(b),(size_t)(n))\n'+b'\n'.join(bodies[n] for n in names))
    flags=list(b['commands'][0]['command']);flags=flags[:flags.index('-c')];assert flags[0]==str(a.compiler)
    commands=[flags+['-c',str(authored),'-o',str(d/'memory_pvm.obj')],flags+['-c',str(helper),'-o',str(d/'pvm_helpers.obj')]]
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']))
    old_objects=[v for v in link if v.endswith('.obj') and Path(v).name=='memory_upload.obj'];assert len(old_objects)==1
    replaced={old_objects[0]:sha(Path(old_objects[0]))};link[link.index(old_objects[0])]=str(d/'memory_pvm.obj')
    kept={str(Path(v).resolve()):sha(Path(v)) for v in link if v.endswith('.obj') and v!=str(d/'memory_pvm.obj')}
    exports=['njLoadTexturePvmMemory','njSetPvmTextureList','njCalcPvmStatusSize']
    at=link.index('-o');link[at:at]=[str(d/'pvm_helpers.obj'),*['-Wl,/export:'+n for n in exports],'-Xlinker','/export:_nj_current_ctx_,DATA'];link[link.index('-o')+1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,cwd=r,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(Path(n))==h,n
    sources=dict(b['authored_source_sha256'])
    for path in [p/'native/jp_native_pvm.h',p/'native/jp_native_pvm_extension.c',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    ext=dict(b['external_source_sha256']);ext['sdk/ninja/njLoadTexturePvmFile.c']=sha(external)
    result=dict(status='bounded_PVM_memory_native_upload_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),upstream_commit=b['upstream_commit'],authored_source_sha256=sources,external_source_sha256=ext,
        generated_source_sha256={n.name:sha(n) for n in [authored,helper]},production_objects_sha256={n:sha(d/n) for n in ['memory_pvm.obj','pvm_helpers.obj']},
        kept_frame_owner_objects_sha256=kept,replaced_memory_owner_objects_sha256=replaced,retained_original_PVM_helpers=names,banked_global_index_store_repaired=True,
        original_memory_source_sha256=sha(original),authored_shutdown_guard=dict(old=old.decode(),new=new.decode()),
        frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=b['DLL'],DLL_sha256=b['DLL_sha256'],GPU_DLL=b['GPU_DLL'],GPU_DLL_sha256=b['GPU_DLL_sha256'],
        baseline=str(base),baseline_report_sha256=sha(base/'build_verification.json'),single_memory_texture_frame_video_queue_device_owner=True,original_present_diagnostic_DLL_unchanged=True,
        original_source_and_proofs_modified=False,game_or_SDK_bodies_public=False,retail_assets_published=False,full_game_loop_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
