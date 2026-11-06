"""Recover original sound placement callbacks with real placement/tree providers.
Generated game source, matching marker data and combined binaries stay private.
"""
import argparse,hashlib,json,re,shutil,subprocess,struct
from fractions import Fraction
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def marker_literals(raw):
    # SHC's decimal constants use binary32 round toward zero. Emit exact hex
    # literals so Clang preserves those values. Original includes stay intact.
    def convert(match):
        token=match.group();exact=Fraction(token[:-1].decode())
        value=struct.unpack('<f',struct.pack('<f',float(token[:-1])))[0]
        bits=struct.unpack('<I',struct.pack('<f',value))[0]
        if abs(Fraction(value))>abs(exact):bits-=1
        value=struct.unpack('<f',struct.pack('<I',bits))[0]
        return (value.hex()+'F').encode()
    return re.sub(rb'[-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?F',convert,raw)
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','matching_includes','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=json.loads((base/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(base/'external_game_sound.c')==b['effects_generated_source_sha256'];d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(src,d/src.name)
    for n in ('include','inputs','sound_imports','rna_inputs','adxt_inputs'):shutil.copytree(base/n,d/n)
    units={'src/sound/sound.c':['sound_fx_zone_step','sound_orbit_step'],'src/object/placement.c':['in_range','placement_try_leave_by','placement_try_leave'],'src/debug/editor.c':['placement_editing'],'src/draw/tree.c':['draw_nodes','draw_tree']}
    originals={};bodies=[];hashes={}
    for unit,names in units.items():
        raw=(u/unit).read_bytes();originals[unit]=sha(u/unit)
        assert originals[unit]==hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+unit])).hexdigest()
        for n in names:
            body=function(raw,n);hashes[n]=hashlib.sha256(body).hexdigest()
            if n=='sound_orbit_step':assert body.count(b'(int)task')==1;body=body.replace(b'(int)task',b'(intptr_t)task')
            if n=='draw_tree':body=body.replace(b'{\n',b'{\n    if(object==&marker_object&&jp_sound_marker_reserve())return;\n',1)
            bodies.append(body)
    collision=(u/'src/collision/collision.c').read_bytes();start=collision.index(b'static NJS_MATERIAL marker_materials[1]');end=collision.index(b'/* a cylinder',start)
    marker=marker_literals(collision[start:end]);originals['src/collision/collision.c']=sha(u/'src/collision/collision.c')
    includes={};inc=d/'placement_inputs';target=inc/'collision/collision';target.mkdir(parents=True)
    for name in re.findall(rb'#include "collision/collision/([^\"]+)"',marker):
        name=name.decode();src=a.matching_includes/name;shutil.copyfile(src,target/name);includes[name]=sha(src)
        saved=d/'matching_marker';saved.mkdir(exist_ok=True);shutil.copyfile(src,saved/name)
        (target/name).write_bytes(marker_literals((target/name).read_bytes()))
        if name=='marker_materials.inc':
            raw=(target/name).read_bytes();adapted,count=re.subn(rb'\{ (0x[0-9a-fA-F]+), (0x[0-9a-fA-F]+),',rb'{ { \1 }, { \2 },',raw)
            assert count==1;(target/name).write_bytes(adapted)
    # These are the original editor definitions, owned once alongside its
    # recovered membership function. The complete editor remains future work.
    editor=(u/'src/debug/editor.c').read_bytes();state=[]
    for pattern in (rb'^Sint16 edit_mode = 0;\n',rb'^static Sint16 slot_count = 1;\n',rb'^static struct task \*slot_tasks\[OBJECT_SLOTS\];\n'):
        matches=re.findall(pattern,editor,re.M);assert len(matches)==1;state.append(matches[0])
    prefix=b'''
#include "draw/model.h"
#define OBJECT_SLOTS 16
#define LEAVE_RANGE (410.0F * 410.0F)
#define NEAR_RANGE (40.0F * 40.0F)
#define UNIT_ALL (NJD_EVAL_UNIT_POS | NJD_EVAL_UNIT_ANG | NJD_EVAL_UNIT_SCL)
static int jp_sound_marker_reserve(void);
/* Native compositions of the documented original axis order, using the
 * existing qualified axis operations and actual shared matrix owner. */
void njRotateXYZ(NJS_MATRIX *m,Angle x,Angle y,Angle z){if(z)njRotateZ(m,z);if(y)njRotateY(m,y);if(x)njRotateX(m,x);}
void njRotateZXY(NJS_MATRIX *m,Angle x,Angle y,Angle z){if(y)njRotateY(m,y);if(x)njRotateX(m,x);if(z)njRotateZ(m,z);}
'''
    reservation=b'''
struct jp_marker_span {size_t indices,uvs;};
struct jp_marker_bound {uint32_t opaque,translucent,vertices,meshsets;};
extern __declspec(dllimport) int jp_model_preflight(const NJS_MODEL *,size_t,size_t,size_t,size_t,const struct jp_marker_span *,struct jp_marker_bound *);
extern __declspec(dllimport) int jp_model_reserve(const struct jp_marker_bound *);
static int marker_status;
static int jp_sound_marker_reserve(void){
    const struct jp_marker_span spans[2]={{60,0},{320,0}};struct jp_marker_bound bound;
    marker_status=jp_model_preflight(&marker_model,92,92,2,1,spans,&bound);
    if(!marker_status)marker_status=jp_model_reserve(&bound);return marker_status;
}
__declspec(dllexport) int peer_sound_marker_status(void){return marker_status;}
'''
    source=d/'external_game_sound.c';source.write_bytes(source.read_bytes()+prefix+b''.join(state)+marker+reservation+b'\n'.join(bodies)+b'''
__declspec(dllexport) void peer_sound_editor_slot(void *task){slot_count=1;slot_tasks[0]=task;}
__declspec(dllexport) void *peer_sound_marker(void){return &marker_object;}
''')
    def relocate(cmd):return [v.replace(str(base),str(d)) for v in cmd]
    sdk=relocate(next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name=='external_game_sound.c'));sdk.insert(1,'-I'+str(inc))
    sdk[sdk.index('-c')+1]=str(source);sdk[sdk.index('-o')+1]=str(d/'game_sdk.obj')
    link=relocate(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['game_sound_DLL']))
    link[4:4]=['-Wl,/export:'+n for n in ('sound_fx_zone_step','sound_orbit_step','placement_try_leave','placement_try_leave_by','placement_editing','draw_tree','njRotateXYZ','njRotateZXY','edit_mode')]
    results=[]
    for i,cmd in enumerate((sdk,link)):
        assert Path(cmd[cmd.index('-o')+1]).is_relative_to(d)
        run=subprocess.run(cmd,capture_output=True);log=d/f'placement_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'placement_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    for k in ('effects_generated_source_sha256','game_sound_generated_source_sha256','qsound_generated_source_sha256','spatial_generated_source_sha256','sound_math_generated_source_sha256','pstm_generated_source_sha256'):updates[k]=sha(source)
    updates.update(status='original_sound_placement_callbacks_and_dependencies_linked_runtime_unverified',commands=results+b['commands'],placement_original_source_sha256=originals,placement_original_function_sha256=hashes,placement_matching_marker_includes_sha256=includes,placement_source_sha256=sha(source),placement_baseline_sha256=sha(base/'build_verification.json'),placement_runtime_verified=False,placement_composite_rotation_bitwise_SH4_verified=False,authored_source_sha256=dict(b['authored_source_sha256'],**{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}),playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
