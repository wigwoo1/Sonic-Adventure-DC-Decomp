"""Extend actual Frames card/font providers with claim authority in place.
Qualified sources and previous objects remain byte-identical. No new producer.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,base,d=(getattr(a,n).resolve() for n in ('workspace','baseline','output'));p=r/'outputs/sonic-adventure';w=r/'work/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(w)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','inputs','sound_imports','rna_inputs','adxt_inputs','placement_inputs','matching_marker','main_boot_imports'):shutil.copytree(path,d/path.name)
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll'))
    assert sha(link[-1])==b['frames_DLL_sha256']
    previous_objects={Path(x).name:dict(path=x,sha256=sha(x)) for x in link if x.endswith('.obj')};adaptations=[]
    def rep(raw,old,new,label):
        assert raw.count(old)==1,(label,raw.count(old));adaptations.append(dict(reason=label,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    def change_body(raw,name,line):
        matches=list(re.finditer(rb'(?m)^[^;\n{}]*\b'+name.encode()+rb'\([^;{}]*\)\s*\{',raw));assert len(matches)==1,name
        at=matches[0].end();return raw[:at]+b'\n'+line+raw[at:]
    raw=(p/'native/jp_native_vmu_fs.c').read_bytes();raw=b'#include "jp_native_boot_lifetimes.h"\n'+raw
    raw=change_body(raw,'buInit',b'    if(jp_boot_lifetime_guard(JP_BOOT_CARD,2))return BUD_ERR_BUSY;\n')
    raw=change_body(raw,'buExit',b'    if(jp_boot_lifetime_guard(JP_BOOT_CARD,0))return BUD_ERR_BUSY;\n')
    raw=rep(raw,b'    rfInitSystem(0);',b'    jp_boot_lifetime_created(JP_BOOT_CARD);\n    rfInitSystem(0);','real card initialization generation')
    raw=rep(raw,b'    if (callback) callback();',b'    if (callback){serving=1;callback();serving=0;}','card init callback is actual provider activity')
    raw+=b'\nint jp_boot_card_active(void){return serving||jp_boot_rf_active();}\n';(d/'card_with_lifetime.c').write_bytes(raw)
    raw=(p/'native/jp_native_fibers.c').read_bytes();raw=b'#include "jp_native_boot_lifetimes.h"\n'+raw
    for name,op,ret in [('rfInitSystem',2,b'return;'),('rfExitSystem',0,b'return;'),('rfCreateServer',2,b'return NULL;'),('rfCreateFiber',2,b'return -1;'),('rfDestroyServer',0,b'return;')]:
        raw=change_body(raw,name,b'    if(jp_boot_lifetime_guard(JP_BOOT_CARD,'+str(op).encode()+b'))'+ret+b'\n')
    raw+=b'\nint jp_boot_rf_active(void){return current!=NULL||(owner&&GetCurrentFiber()!=root_fiber);}\nint jp_boot_rf_retired(void){return !owner&&!root_fiber&&!current;}\n';(d/'fibers_with_lifetime.c').write_bytes(raw)
    assert sha(base/'external_native_print_format.c')==b['generated_sources_sha256']['external_native_print_format.c']
    raw=(base/'external_native_print_format.c').read_bytes();raw=b'#include "jp_native_boot_lifetimes.h"\n'+raw
    for name,new in [('njInitPrint','jp_boot_original_print_init'),('njExitPrint','jp_boot_original_print_exit')]:
        body=function(raw,name);raw=raw.replace(body,body.replace(('void '+name+'(').encode(),('static void '+new+'(').encode(),1))
    raw+=b'''
extern int jp_texture_live_surface_view(uint32_t,const void **,size_t *);
int jp_boot_font_identity(uint64_t *identity){
    if(!font.pSurface)return 0;
    /* Read the actual original print parameter, even while a sticky texture
     * error prevents mutating/query APIs that require ready(). No error clear. */
    const void *pixels;size_t bytes;uint32_t word=tex_param&0x001fffffu;
    if(!font.pSurface||!word||!jp_texture_live_surface_view(word,&pixels,&bytes)||pixels!=font.pSurface||bytes!=font.TextureSize)return 0;
    *identity=word;return 1;
}
void njInitPrint(Uint32 *texture,Uint32 size,Uint32 attr){
    if(jp_boot_lifetime_guard(JP_BOOT_PRINT,2))return;
    jp_boot_original_print_init(texture,size,attr);
    uint64_t identity;if(jp_boot_font_identity(&identity))jp_boot_lifetime_created(JP_BOOT_PRINT);
}
void njExitPrint(void){if(!jp_boot_lifetime_guard(JP_BOOT_PRINT,0))jp_boot_original_print_exit();}
''';(d/'print_with_lifetime.c').write_bytes(raw)
    raw=(p/'native/jp_native_print_transport.c').read_bytes();raw=b'#include "jp_native_boot_lifetimes.h"\n'+raw
    raw=change_body(raw,'jp_print_open',b'    if(jp_boot_lifetime_guard(JP_BOOT_PRINT,3))return NULL;\n')
    raw+=b'\nint jp_boot_print_active(void){return packet!=NULL;}\n';(d/'print_transport_with_lifetime.c').write_bytes(raw)
    texture_names=['jp_native_texture_cache.c','jp_native_texture_space.c','jp_native_texture_budget.c','jp_native_standalone_texture.c']
    for i,name in enumerate(texture_names):
        raw=(p/'native'/name).read_bytes()
        if i<3:raw=rep(raw,('"'+texture_names[i+1]+'"').encode(),('"'+(d/texture_names[i+1]).as_posix()+'"').encode(),'private texture include '+name)
        else:
            raw=b'#include "jp_native_boot_lifetimes.h"\n'+raw
            raw=change_body(raw,'jp_textures_initialize',b'    if(jp_boot_lifetime_guard(JP_BOOT_PRINT,2))return JP_TEXTURE_STATE;\n')
            raw=change_body(raw,'jp_textures_shutdown',b'    if(jp_boot_lifetime_guard(JP_BOOT_PRINT,1))return;\n')
            raw=change_body(raw,'jp_texture_replace_standalone_surface',b'    if(jp_boot_lifetime_guard(JP_BOOT_PRINT,0))return JP_TEXTURE_STATE;\n')
            raw=change_body(raw,'kmFreeTexture',b'    if(standalone_at((const NJS_TEXSURFACE *)descriptor)&&jp_boot_lifetime_guard(JP_BOOT_PRINT,0))return KMSTATUS_INVALID_SURFACE;\n')
        (d/name).write_bytes(raw)
    def recipe(folder,obj):
        for name in ('build_verification.json','verification.json'):
            path=w/folder/name
            if not path.exists():continue
            for row in read(path)['commands']:
                cmd=row['command']
                if '-c' in cmd and Path(cmd[-1]).name==obj:return cmd[:cmd.index('-c')]
        raise AssertionError((folder,obj))
    specs=[('jp_native_vmu_fs.obj','card_with_lifetime.c','jp-native-vmu-fs-v7'),('jp_native_fibers.obj','fibers_with_lifetime.c','jp-native-vmu-fs-v7'),('print_sdk.obj','print_with_lifetime.c','jp-native-print-format-v3'),('print_transport.obj','print_transport_with_lifetime.c','jp-native-print-v7'),('texture_cache.obj','jp_native_texture_cache.c','jp-native-vertex-lifecycle-v6')]
    commands=[];replaced={}
    for old,src,folder in specs:
        obj=d/('life_'+old);cmd=recipe(folder,old);cmd+=['-I'+str(p/'native'),'-c',str(d/src),'-o',str(obj)];commands.append(cmd)
        prior=previous_objects[old]['path'];assert link.count(prior)==1;link[link.index(prior)]=str(obj);replaced[old]=sha(prior)
    compiler=commands[0][0];owner=p/'native/jp_native_boot_lifetimes.c';obj=d/'boot_lifetimes.obj'
    commands.append([compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-c',str(owner),'-o',str(obj)])
    # Export the existing real transport operations for an active-packet
    # qualification fixture; no alternate producer or callback is introduced.
    link[link.index('-o'):link.index('-o')]=[str(obj),'-Wl,/export:jp_print_open','-Wl,/export:jp_print_close'];link[-1]=str(d/'jp_native_frames.dll');commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);(d/f'lifetime_build_{i}.txt').write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=len(run.stdout+run.stderr)));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode()
    updates={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):updates[key]=str(d/Path(value).name)
    updates['frames_DLL_sha256']=sha(d/'jp_native_frames.dll')
    authored=[p/'native/jp_native_boot_lifetimes.h',owner,Path(__file__).resolve()]
    updates.update(status='actual_card_font_claim_providers_linked_runtime_unverified',boot_lifetimes_baseline_sha256=sha(base/'build_verification.json'),boot_lifetimes_original_objects=previous_objects,boot_lifetimes_replaced_objects=replaced,boot_lifetimes_generated_source_sha256={x.name:sha(x) for x in d.glob('*.c') if x.name in [s[1] for s in specs]+texture_names+['fibers_with_lifetime.c']},boot_lifetimes_adaptations=adaptations,commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{path.relative_to(r).as_posix():sha(path) for path in authored}),terminal_global_exit_integrated=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
