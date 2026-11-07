"""Recover real Ninja motion helpers; leave Simple drawing unresolved explicitly."""
import argparse,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
from rewrite_sprite_float_operations import rewrite,span
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def convert_casts(raw,tree,names):
    candidates={}
    def visit(node):
        if node.get('kind')=='CStyleCastExpr' and node.get('castKind') in ('IntegralToFloating','FloatingToIntegral'):
            assert len(node['inner'])==1
            child=node['inner'][0];kind=node['castKind']
            integer=child if kind=='IntegralToFloating' else node
            t=integer['type'];t=t.get('desugaredQualType',t['qualType'])
            assert t in ('int','long'),t
            outer,inner=span(node),span(child);assert outer and inner and outer[0]<=inner[0]<inner[1]<=outer[1]
            candidates[outer]=(kind,inner)
        for child in node.get('inner',[]):visit(child)
    for node in tree['inner']:
        if node.get('kind')=='FunctionDecl' and node.get('name') in names:
            for body in node.get('inner',[]):
                if body.get('kind')=='CompoundStmt':visit(body)
    def render(lo,hi,exclude=None):
        choices=sorted((s for s in candidates if s!=exclude and lo<=s[0] and s[1]<=hi),key=lambda s:(s[0],-s[1]))
        out=[];at=lo
        for s in choices:
            if s[0]<at:continue
            kind,inner=candidates[s];fn=b'jp_sdk_motion_from_int' if kind=='IntegralToFloating' else b'jp_sdk_motion_to_int'
            out.extend([raw[at:s[0]],fn+b'('+render(*inner,exclude=s)+b')']);at=s[1]
        out.append(raw[at:hi]);return b''.join(out)
    return render(0,len(raw)),{kind:sum(v[0]==kind for v in candidates.values()) for kind in ('IntegralToFloating','FloatingToIntegral')}
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure'
    u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):assert sha(v)==b[k+'_sha256']
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
    kept={n:sha(n) for n in link if n.endswith('.obj')}
    providers={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0}
    originals={}
    for n in ('sdk/ninja/njMotSrc.c','sdk/ninja/njMotionS.c','sdk/ninja/njGlobal.c','sdk/ninja/njControl.c','sdk/ninja/njmotion_zam.h'):
        raw=(u/n).read_bytes()
        assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])
        originals[n]=raw
    recipe=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_tree_original.obj')
    sdk=recipe[:recipe.index('-c')]
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.dll','.lib','.c','.h','.obj'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
    commands=[]
    def run(cmd,label):
        actual=cmd;extra={}
        if len(subprocess.list2cmdline(cmd))>=30000:
            content='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in cmd[1:])+'\n'
            assert shlex.split(content)==cmd[1:]
            rsp=d/(label+'.rsp');rsp.write_text(content);actual=[cmd[0],'@'+str(rsp)]
            extra=dict(response_file=str(rsp),response_file_sha256=sha(rsp))
        x=subprocess.run(actual,capture_output=True)
        (d/(label+'.stderr')).write_bytes(x.stderr)
        commands.append(dict(command=cmd,exit_code=x.returncode,diagnostic_bytes=len(x.stderr),**extra))
        assert not x.returncode and not x.stderr,(label,x.stderr.decode(errors='replace')[-6000:])
        return x.stdout
    units={};objects=[]
    for tag in ('njMotSrc','njMotionS'):
        raw=originals['sdk/ninja/'+tag+'.c'];anchor=b'#include <sg_xpt.h>\n';assert raw.count(anchor)==1
        if tag=='njMotSrc':
            include=b'#ifdef GCC\n#include "ninja/njmotion_zam.h"\n#endif\n'
            assert raw.count(include)==1;raw=raw.replace(include,b'')
            # Original njInitShape was recovered with the boot-buffer owner.
            # Reuse that exact shared state/function rather than duplicate it.
            assert 'njInitShape' in providers
            raw=raw.replace(owner_function(raw,'njInitShape'),b'')
        raw=raw.replace(anchor,anchor+b'#define GCC 1\n#include "jp_native_sprite_numeric.h"\n'+
                        (b'#include "jp_native_sdk_motion_numeric.h"\n' if tag=='njMotSrc' else b''))
        source=d/(tag+'.c');source.write_bytes(raw)
        tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],tag+'_ast'))
        names=[]
        for node in tree['inner']:
            if node.get('kind')!='FunctionDecl' or node.get('loc',{}).get('includedFrom'):continue
            loc=node.get('loc',{})
            if any(c.get('kind')=='CompoundStmt' for c in node.get('inner',[])) and 'offset' in loc and raw[loc['offset']:loc['offset']+loc['tokLen']].decode()==node['name']:names.append(node['name'])
        adapted,stats=rewrite(raw,tree,names);rz=d/(tag+'_rz.c');rz.write_bytes(adapted)
        casts={}
        if tag=='njMotSrc':
            cast_tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(rz)],tag+'_cast_ast'))
            adapted,casts=convert_casts(adapted,cast_tree,names)
            rz=d/(tag+'_rz_casts.c');rz.write_bytes(adapted)
        obj=d/(tag+'.obj');run(sdk+['-c',str(rz),'-o',str(obj)],tag+'_compile')
        ir=d/(tag+'.ll');run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],tag+'_llvm')
        assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
        syms=coff_symbols(obj);defined={s['name'] for s in syms if s['section']>0};undefined={s['name'] for s in syms if s['section']==0}
        units[tag]=dict(functions=names,scalar_arithmetic_adaptation=stats,object_sha256=sha(obj),
                        integer_float_cast_adaptation=casts,
                        generated_source_sha256={x.name:sha(x) for x in (source,rz,ir)},
                        defined=sorted(defined),undefined=sorted(undefined))
        if tag=='njMotSrc':objects.append(obj)
    needed=set(units['njMotSrc']['undefined'])-providers-{'_fltused','jp_sdk_motion_sum','jp_sdk_motion_sum_angles','jp_sdk_motion_from_int','jp_sdk_motion_to_int'}
    needed|=set(units['njMotionS']['undefined'])-providers-set(units['njMotSrc']['defined'])-{'_fltused','njSimpleDrawModel'}
    declarations=[]
    for name in sorted(needed):
        assert name.startswith('_nj_'),name
        found=re.findall(rb'^[^\r\n]*\b'+name.encode()+rb'\b[^;]*;\r?\n',originals['sdk/ninja/njGlobal.c'],re.M)
        assert len(found)==1,(name,found);declarations.append(found[0])
    assert '_nj_constant_material_' in providers
    state=d/'sdk_motion_state.c';state.write_bytes(b'#include <ninja.h>\n'+b''.join(declarations)+
            (owner_function(originals['sdk/ninja/njControl.c'],'njSetConstantMaterial')
             if 'njSetConstantMaterial' not in providers else b''))
    sources=[(state,'sdk_motion_state'),(p/'native/jp_native_sdk_motion_numeric.c','sdk_motion_numeric'),
             (p/'source/sh4_float_i32.c','sdk_motion_float'),(p/'source/sh4_truncate_i32.c','sdk_motion_truncate')]
    for source,tag in sources:
        flags=[x for x in sdk if x not in ('-ffreestanding','-fno-builtin')] if tag=='sdk_motion_numeric' else sdk
        obj=d/(tag+'.obj');run(flags+['-I'+str(p/'source'),'-c',str(source),'-o',str(obj)],tag+'_compile');objects.append(obj)
    defined=set(providers);undefined=set()
    for obj in objects:
        syms=coff_symbols(obj)
        fresh={s['name'] for s in syms if s['section']>0}
        assert all(re.fullmatch('__real@[0-9a-f]+',n) for n in fresh&defined),fresh&defined
        defined|=fresh;undefined|={s['name'] for s in syms if s['section']==0}
    assert not undefined-defined-{'_fltused'},sorted(undefined-defined-{'_fltused'})
    existing={m.group(1) for arg in link if (m:=re.search(r'/export:([^,]+)',arg))}
    exports=sorted(({n for n in units['njMotSrc']['defined'] if not n.startswith('__real@')}|needed|
                    {'njSetConstantMaterial','_nj_constant_material_','_nj_motion_data_info_',
                     '_nj_shape_data_info_','_nj_shape_buf_','_nj_motion_callback_'})-existing)
    link[link.index('-o'):link.index('-o')]=list(map(str,objects))+['-Wl,/export:'+n for n in exports]
    link[-1]=str(d/'jp_native_frames.dll');run(link,'link')
    assert all(sha(n)==h for n,h in kept.items())
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
            if k!='frames_DLL':assert updates[k+'_sha256']==b[k+'_sha256']
    authored=[Path(__file__).resolve(),p/'native/jp_native_sdk_motion_numeric.c',
              p/'native/jp_native_sdk_motion_numeric.h',p/'source/sh4_float_i32.c',p/'source/sh4_float_i32.h',
              p/'source/sh4_truncate_i32.c']
    updates.update(status='original_Ninja_motion_helpers_and_material_setter_linked_Simple_renderer_unresolved',
        sdk_motion_baseline_sha256=sha(base/'build_verification.json'),
        sdk_motion_recipe=dict(upstream_commit=PIN,original_source_sha256={n:hashlib.sha256(raw).hexdigest() for n,raw in originals.items()},
            units=units,new_globals=sorted(needed),new_exports=exports,
            new_objects_sha256={str(x):sha(x) for x in objects},
            generated_state_sha256=sha(state),
            njMotionS_compiled_but_not_linked=True,
            remaining_Simple_missing_provider='njSimpleDrawModel',
            upstream_GCC_SKIP_angles_repair_enabled=True,
            all_previous_compiled_objects_preserved=True,
            existing_material_setter_owner_reused='njSetConstantMaterial' in providers,
            motion_helper_runtime_or_aura_rendering_verified=False),
        sdk_motion_kept_objects_sha256=kept,
        commands=commands+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],len(units['njMotSrc']['functions']),'source functions;147 RZ expressions',flush=True)
if __name__=='__main__':main()
