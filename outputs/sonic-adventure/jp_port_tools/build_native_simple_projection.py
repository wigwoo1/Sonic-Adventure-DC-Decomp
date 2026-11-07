"""Link genuine Simple loop entries to bounded, explicit native value bodies."""
import argparse,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
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
    providers={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0}
    names=['njSimpleCalcModel_Loop'+str(i) for i in range(1,5)]
    assert not set(names)&providers
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
        assert not x.returncode and not x.stderr,x.stderr.decode(errors='replace')[-5000:]
        return x.stdout
    prefix=b'#include "jp_native_simple_projection.h"\n#define calc_work jp_simple_calc_work\n'
    for i in range(1,5):
        prefix+=('static void loop'+str(i)+'_body(struct jp_simple_calc_work *w,NJS_MATRIX *m){(void)m;jp_simple_projection_loop(w,'+str(i)+');}\n').encode()
    source=d/'original_simple_projection_entries.c'
    source.write_bytes(prefix+b'\n'.join(owner_function(raw,n) for n in names))
    objects=[]
    for file,tag in ((source,'simple_entries'),(p/'native/jp_native_simple_projection.c','simple_projection')):
        obj=d/(tag+'.obj')
        flags=[x for x in sdk if x not in ('-ffreestanding','-fno-builtin')] if tag=='simple_projection' else sdk
        run(flags+['-c',str(file),'-o',str(obj)],tag);objects.append(obj)
    layout=d/'simple_layout.c';fields=['count','out','points','normals','clip_and','clip_or','scale_x','scale_y','clip_left','clip_right','clip_top','clip_bottom']
    layout.write_text('#include "jp_native_simple_projection.h"\nconst unsigned simple_layout[]={sizeof(struct jp_simple_calc_work),'+','.join('__builtin_offsetof(struct jp_simple_calc_work,'+n+')' for n in fields)+'};\n')
    layouts={}
    for tag,target in (('native','x86_64-pc-windows-msvc'),('original_LP32','i686-pc-windows-msvc')):
        flags=[('--target='+target) if x.startswith('--target=') else x for x in sdk]
        out=d/(tag+'_simple_layout.ll');run(flags+['-S','-emit-llvm',str(layout),'-o',str(out)],tag+'_layout')
        match=re.search(r'@simple_layout\s*=.*?constant \[\d+ x i32\] \[(.*?)\]',out.read_text(),re.S);assert match
        values=[int(v) for v in re.findall(r'i32 (\d+)',match[1])]
        assert len(values)==13;layouts[tag]=dict(zip(['sizeof']+fields,values))
    assert layouts['native']['sizeof']==64 and layouts['original_LP32']['sizeof']==48
    link[link.index('-o'):link.index('-o')]=list(map(str,objects))+['-Wl,/export:'+n for n in names]
    link[-1]=str(d/'jp_native_frames.dll');run(link,'link')
    assert all(sha(n)==h for n,h in kept.items())
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
            if k!='frames_DLL':assert updates[k+'_sha256']==b[k+'_sha256']
    authored=[Path(__file__).resolve(),p/'native/jp_native_simple_projection.c',p/'native/jp_native_simple_projection.h']
    updates.update(status='original_Simple_projection_entries_native_value_bodies_linked_runtime_unverified',
        simple_projection_baseline_sha256=sha(base/'build_verification.json'),
        simple_projection_recipe=dict(upstream_commit=PIN,original_source_sha256=hashlib.sha256(raw).hexdigest(),
            original_entry_sha256={n:hashlib.sha256(owner_function(raw,n)).hexdigest() for n in names},
            generated_entry_sha256=sha(source),layouts=layouts,
            objects_sha256={str(x):sha(x) for x in objects},removed_unused_speculative_reads=True,
            hardware_cache_QACR_or_FPSCR_side_effects_emulated=False,
            exact_preserved_store_queue_word_values_claim_pending_verification=True,
            full_Simple_model_or_aura_renderer_linked=False),
        simple_projection_kept_objects_sha256=kept,commands=commands+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],'four real entries; two verified ABI layouts',flush=True)
if __name__=='__main__':main()
