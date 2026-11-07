"""Recover genuine full model projection with an independent32-byte owner."""
import argparse,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def derive(raw):
    body=owner_function(raw,'_njSimpleCalcModel')
    assert body.count(b'struct calc_work work;')==body.count(b'(Uint8 *)(struct calc_vertex *)(Uint32)_nj_vertex_buf_')==1
    body=body.replace(b'struct calc_work work;',b'struct calc_work work = {0};')
    body=body.replace(b'(Uint8 *)(struct calc_vertex *)(Uint32)_nj_vertex_buf_',b'(Uint8 *)_nj_vertex_buf_')
    for i in range(1,5):
        old=('njSimpleCalcModel_Loop'+str(i)+'(&work, mp);').encode();assert body.count(old)==1
        body=body.replace(old,('jp_simple_calc_loop(&work, mp, '+str(i)+');').encode())
    prefix=b'''#include "jp_native_simple_calc.h"
#include "jp_native_sprite_numeric.h"
#define GCC 1
#define calc_work jp_simple_calc_work
#define CLIP_NEAR 1
#define CLIP_FAR 2
#define CLIP_SIDES 0x3c
extern Float port_draw_aspect;
static Uint32 model_clip_or;
NJS_MATRIX *mp;
static NJS_VECTOR base_light_direction;
NJS_VECTOR light2;
static Float base_light_scale;
static Float projection_x,projection_y;
void jp_simple_calc_context_light(NJS_VECTOR *in,Float scale)
{base_light_direction=*in;base_light_scale=scale;}
__declspec(dllexport) void jp_simple_calc_context_snapshot(uint32_t *out)
{
    union {float f;uint32_t u;} word;
    out[0]=model_clip_or;
    word.f=light2.x;out[1]=word.u;word.f=light2.y;out[2]=word.u;word.f=light2.z;out[3]=word.u;
    word.f=projection_x;out[4]=word.u;word.f=projection_y;out[5]=word.u;
}
'''
    return prefix+body
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve()
    base=a.baseline.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):assert sha(v)==b[k+'_sha256']
    raw=(u/'sdk/ninja/njModelS.c').read_bytes()
    assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':sdk/ninja/njModelS.c'])
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
    kept={n:sha(n) for n in link if n.endswith('.obj')}
    recipe=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_tree_original.obj')
    sdk=recipe[:recipe.index('-c')]
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.dll','.lib','.obj','.h','.c'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
    commands=[]
    def run(cmd,label):
        actual=cmd;extra={}
        if len(subprocess.list2cmdline(cmd))>=30000:
            text='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in cmd[1:])+'\n'
            assert shlex.split(text)==cmd[1:];rsp=d/(label+'.rsp');rsp.write_text(text)
            actual=[cmd[0],'@'+str(rsp)];extra=dict(response_file=str(rsp),response_file_sha256=sha(rsp))
        x=subprocess.run(actual,capture_output=True);(d/(label+'.stderr')).write_bytes(x.stderr)
        commands.append(dict(command=cmd,exit_code=x.returncode,diagnostic_bytes=len(x.stderr),**extra))
        assert not x.returncode and not x.stderr,x.stderr.decode(errors='replace')[-6000:];return x.stdout
    original=d/'original_simple_calc.c';original.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(original)],'calc_ast'))
    adapted,stats=rewrite(original.read_bytes(),tree,['_njSimpleCalcModel'])
    rz=d/'original_simple_calc_rz.c';rz.write_bytes(adapted)
    objects=[]
    for source,tag in ((rz,'simple_calc_original'),(p/'native/jp_native_simple_calc.c','simple_calc_native')):
        obj=d/(tag+'.obj')
        flags=[x for x in sdk if x not in ('-ffreestanding','-fno-builtin')] if tag=='simple_calc_native' else sdk
        run(flags+['-c',str(source),'-o',str(obj)],tag);objects.append(obj)
    ir=d/'simple_calc_original.ll';run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'calc_llvm')
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    link[link.index('-o'):link.index('-o')]=list(map(str,objects));link[-1]=str(d/'jp_native_frames.dll');run(link,'link')
    assert all(sha(n)==h for n,h in kept.items());updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
            if k!='frames_DLL':assert updates[k+'_sha256']==b[k+'_sha256']
    authored=[Path(__file__).resolve(),p/'native/jp_native_simple_calc.c',p/'native/jp_native_simple_calc.h']
    updates.update(status='genuine_full_Simple_model_projection_and_owned32_byte_scratch_linked_runtime_unverified',
        simple_calc_baseline_sha256=sha(base/'build_verification.json'),
        simple_calc_recipe=dict(upstream_commit=PIN,original_source_sha256=hashlib.sha256(raw).hexdigest(),
            original_function_sha256={n:hashlib.sha256(owner_function(raw,n)).hexdigest() for n in ('_njSimpleCalcModel','SimpleCalcModelMatrix','SimpleCalcModelLight')},
            scalar_arithmetic_adaptation=stats,
            generated_source_sha256={x.name:sha(x) for x in (original,rz,ir)},
            objects_sha256={str(x):sha(x) for x in objects},
            full_width_scratch_pointer_repair=True,uninitialized_work_normals_repair=True,
            bounded_loop_service_call_adaptations=4,
            separate_owned32_byte_scratch_restores_borrowed_globals=True,
            full_Simple_model_material_mesh_or_aura_rendering_verified=False),
        simple_calc_kept_objects_sha256=kept,commands=commands+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],stats['total_operators_rewritten'],'RZ operations',flush=True)
if __name__=='__main__':main()
