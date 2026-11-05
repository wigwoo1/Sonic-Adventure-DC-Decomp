"""Link original game PRNG/state and complete shuffle into the shared owner.
External SDK/game bodies and linked runtime DLLs remain private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']));kept={n:sha(n) for n in link if n.endswith('.obj')}
    assert not any(s['section']>0 and s['name'] in ['rand','srand','_rnext','array_shuffle'] for n in kept for s in coff_symbols(Path(n)))
    d.mkdir();(d/'frames').mkdir();external=['sdk/runtime/_rnext.c','sdk/runtime/srand.c','sdk/runtime/rand.c','sdk/include/stdlib.h','src/system/shuffle.c','src/system/shuffle.h','src/system/util.h']
    rng=d/'external_game_random.c';rng.write_bytes(b'\n'.join((u/n).read_bytes() for n in external[:3])+b'\n_Static_assert(sizeof(unsigned long)==4 && sizeof(unsigned int)==4 && sizeof(_rnext)==4 && RAND_MAX==32767,"original 32-bit PRNG ABI");\n')
    raw=(u/'src/system/shuffle.c').read_bytes();native=raw.replace(b'#include "system/util.h"',b'#include "jp_native_random_pick.h"\n#define RAND_SCALE (1.0F / (RAND_MAX + 1))')
    old=b'pick = (int)((Float)rand() * (Float)left * RAND_SCALE);';assert native.count(old)==1;native=native.replace(old,b'pick = jp_random_pick((Uint32)rand(),left);')
    body=function(native,'array_shuffle');at=body.index(b'{')+1
    guard=b'\n    if (!jp_shuffle_arguments(array,count,size)) return;\n    if (count <= 1) return;\n    if (!size) { for (int draws=1;draws<count;++draws) (void)rand(); return; }\n'
    native=native.replace(body,body[:at]+guard+body[at:]);shuffle=d/'external_game_shuffle.c';shuffle.write_bytes(native)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]+['-ffreestanding','-fno-builtin','-I'+str(u/'src')]
    authored=p/'native/jp_native_random_pick.c';commands=[]
    for source,name in [(rng,'game_random'),(shuffle,'game_shuffle'),(authored,'random_pick')]:
        commands.append(common+['-c',str(source),'-o',str(d/(name+'.obj'))])
        if name!='game_random':commands.append(common+['-S','-emit-llvm',str(source),'-o',str(d/(name+'.ll'))])
    link[link.index('-o'):link.index('-o')]=[str(d/(n+'.obj')) for n in ['game_random','game_shuffle','random_pick']]+['-Wl,/export:'+n for n in ['rand','srand','array_shuffle']]+['-Xlinker','/export:_rnext,DATA'];link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    newlibs={'frames_DLL':d/'frames/jp_native_frames.lib'}
    for label,name,imports in [('GPU_DLL','jp_native_window_input.dll',['frames_DLL']),('present_DLL','jp_native_present.dll',['frames_DLL','GPU_DLL']),('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        cmd=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b[label]))
        for n in imports:
            oldlib=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(oldlib)==1;cmd[cmd.index(oldlib)]=str(newlibs[n])
        cmd[-1]=str(d/name);newlibs[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for name in ['game_shuffle','random_pick']:assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp|sitofp|uitofp|fptosi|fptoui)\b',(d/(name+'.ll')).read_text())
    symbols=coff_symbols(d/'game_shuffle.obj');undefined=sorted(s['name'] for s in symbols if s['section']==0 and s['value']==0);assert 'rand' in undefined and '__imp_rand' not in undefined
    for n,h in kept.items():assert sha(n)==h,n
    sources=dict(b['authored_source_sha256'])
    for source in [authored,p/'native/jp_native_random_pick.h',Path(__file__).resolve()]:sources[source.relative_to(r).as_posix()]=sha(source)
    generated=dict(b['generated_sources_sha256'])
    for source in [rng,shuffle]:generated[source.name]=sha(source)
    for n,h in b['generated_sources_sha256'].items():shutil.copyfile(base/n,d/n);assert sha(d/n)==h
    shutil.copytree(base/'inputs',d/'inputs')
    shared=dict(status='original_PRNG_state_and_complete_game_shuffle_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256=kept,game_random_external_files_sha256={n:sha(u/n) for n in external},game_random_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in ['game_random','game_shuffle','random_pick']},shuffle_original_function_sha256=hashlib.sha256(function(raw,'array_shuffle')).hexdigest(),shuffle_native_source_sha256=sha(shuffle),shuffle_direct_undefined_symbols=undefined,original_unsigned_PRNG_width_bits=32,original_rand_max=32767,native_shuffle_host_FP_arithmetic_or_conversions=0,zero_size_shuffle_consumes_original_draw_count=True,previous_texture_initializer_build_sha256=sha(base/'build_verification.json'),frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),all_prior_frame_objects_preserved=True,frame_owner_bytes_unchanged=False,original_sources_modified=False,game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:shutil.copyfile(d/n,fd/n)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
