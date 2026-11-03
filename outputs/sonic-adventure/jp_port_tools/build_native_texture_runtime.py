"""Adapt pinned SDK record creation/release to real native handles and storage.
Generated game C, objects and resulting DLL stay private. No service stubs.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def function(raw,name):
    pattern=rb'^[A-Za-z_][^;{}\n]*\b'+name.encode()+rb'\([^;{}]*\)\r?\n\{.*?^\}\r?\n'
    found=list(re.finditer(pattern,raw,re.M|re.S));assert len(found)==1,name
    return found[0].group()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','compiler','output','queue']:
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,c,d=(getattr(a,n).resolve() for n in ['workspace','upstream','compiler','output'])
    p=r/'outputs/sonic-adventure';w=r/'work/sonic-adventure'
    assert d.is_relative_to(w) and not d.is_relative_to(w/'build') and not d.exists()
    manifest=p/'jp_port_tools/jp_texture_external_inputs.json';pin=json.loads(manifest.read_text())
    base=json.loads((p/'jp_port_tools/jp_task_external_inputs.json').read_text())
    renderer=json.loads((p/'jp_port_tools/jp_renderer_external_inputs.json').read_text())
    queue=a.queue.resolve();queue_build=json.loads((queue/'build_verification.json').read_text())
    assert sha(queue/'queue.obj')==queue_build['artifacts_sha256']['queue.obj']
    for name,h in queue_build['authored_source_sha256'].items():assert sha(r/name)==h,name
    def verify():
        for name,h in dict(base['files_sha256'],**pin['files_sha256']).items():assert sha(u/name)==h,name
        for name,h in renderer['additional_game_inputs_sha256'].items():assert sha(u/name)==h,name
    verify();d.mkdir(parents=True)
    changes=[]
    def replacement(raw,old,new,why):
        old,new=old.encode(),new.encode();assert raw.count(old)==1,(why,raw.count(old))
        changes.append(dict(reason=why,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()))
        return raw.replace(old,new)
    def guard(raw,name,statement):
        original=function(raw,name);at=original.index(b'{')+1
        new=original[:at]+b'\n    '+statement.encode()+original[at:]
        return replacement(raw,original.decode(),new.decode(),'native guard '+name)
    overlay=(u/'sdk/ninja/njtexmem.h').read_bytes()
    overlay=replacement(overlay,'#define RECORD(handle) ((NJS_TEXMEMLIST *)*(handle))',
        '#define RECORD(handle) jp_texture_resolve_field(handle)','field-aware loader handle macro')
    overlay_path=d/'include/ninja/njtexmem.h';overlay_path.parent.mkdir(parents=True)
    overlay_path.write_bytes(b'#include "jp_native_textures.h"\n'+overlay)
    sources=[]
    def save(name,raw):
        target=d/name;target.write_bytes(b'#include "jp_native_textures.h"\n#include <kamui.h>\n'+raw)
        sources.append(target)
    raw=(u/'sdk/ninja/njTexList.c').read_bytes()
    raw=guard(raw,'njInitTexMemList','if (!jp_textures_accept_sdk_init(addr,n)) return;')
    raw=guard(raw,'njEntryTexMemList','if (!jp_texture_can_enter(handle,info)) return;')
    raw=replacement(raw,'*handle = (Uint32)&_nj_texmemlist[i];',
                    'if (jp_texture_store_handle(handle,&_nj_texmemlist[i])) return;', 'store opaque native handle')
    raw=guard(raw,'njFreeTexMemList','if (!jp_texture_retire_record(tex)) return;')
    raw=replacement(raw,'i = ((Uint32)tex - (Uint32)_nj_texmemlist) / sizeof(NJS_TEXMEMLIST);',
                    'i = (Uint32)jp_texture_record_index(tex);','native-width free index')
    old=function(raw,'njSearchTexList2')
    new=b'''Sint32 njSearchTexList2(NJS_TEXMEMLIST *list, Uint32 n, NJS_TEXMEMLIST *tex)
{
    uintptr_t base=(uintptr_t)list, value=(uintptr_t)tex;
    if (!list || !tex || value<base || (value-base)%sizeof(*tex) ||
        (value-base)/sizeof(*tex)>=n) return -1;
    return (Sint32)((value-base)/sizeof(*tex));
}
'''
    raw=replacement(raw,old.decode(),new.decode(),'native-width checked search index')
    raw=guard(raw,'njTexMemList','if (!jp_texture_free_slot_available()) return NULL;')
    save('external_texlist.c',raw)
    raw=(u/'sdk/ninja/njSetTextureInfo.c').read_bytes()
    raw=guard(raw,'njSetTextureName','jp_texture_forget_field(&texname->texaddr);')
    save('external_texinfo.c',raw)
    raw=(u/'sdk/ninja/njReleaseTextureNum.c').read_bytes()
    raw=replacement(raw,'NJS_TEXMEMLIST *tex = (NJS_TEXMEMLIST *)texaddr;',
        'NJS_TEXMEMLIST *tex = jp_texture_resolve(texaddr);\n'
        '    if (!tex) { _nj_texerr_.texerr=NJD_TEXERR_OTHER; return -1; }\n'
        '    if ((tex->texaddr & TEXMEM_LOADED) && !jp_texture_surface_owned(&tex->texinfo.texsurface)) return -1;',
        'release resolves native handle and verifies actual surface ownership')
    raw=guard(raw,'njReleaseTextureNum',
        'if (!jp_renderer_texture_lookup(_nj_current_texlist,n)) { _nj_texerr_.n=n; return -1; }')
    save('external_release_num.c',raw)
    raw=(u/'sdk/ninja/njReleaseTexture.c').read_bytes()
    raw=guard(raw,'njReleaseTexture','if (!jp_textures_validate_list(texlist)) return -1;')
    save('external_release.c',raw)
    raw=(u/'sdk/ninja/njSetTextureParam.c').read_bytes()
    raw=replacement(raw,'((Uint32)surface->pSurface >> 3)','jp_texture_surface_word(surface)',
                    'native resource ID replaces hardware address field')
    raw=guard(raw,'njSetTextureParam','if (!jp_texture_surface_owned(&tex->texinfo.texsurface)) return;')
    save('external_texparam.c',raw)
    raw=function((u/'sdk/ninja/njPalette.c').read_bytes(),'njSetPaletteBankLN')
    save('external_palette_bank.c',b'#define BANK_MASK_4BPP 0x3f\n#define BANK_MASK_8BPP 0x30\n'+raw)
    save('external_palette_type.c',function((u/'sdk/ninja/njTexinner.c').read_bytes(),'njCheckPaletteType'))
    save('external_select_list.c',function((u/'sdk/ninja/njTexture.c').read_bytes(),'njSetTexture'))
    probe=function((u/'gcc/src/draw/model.c').read_bytes(),'set_texture')
    probe=replacement(probe,'(const NJS_TEXMEMLIST *)_nj_current_texlist->textures[n].texaddr',
                      'jp_renderer_texture_lookup(_nj_current_texlist, n)','same production renderer handle adaptation')
    probe=probe.replace(b'set_texture(',b'jp_texture_renderer_select(',1)
    save('external_renderer_select.c',b'#include "ninja/njkamui.h"\n#define OUT_OF_LINE\n#define TSP_UV_SIZE 0x3f\n'+probe)
    exports=['njInitTexMemList','njEntryTexMemList','njFreeTexMemList','njSearchTexList',
        'njSearchTexList2','njSearchTexList3','njSetFlagAddrAllTexture','njTexMemList',
        'njSetTextureInfo','njSetTextureName','njReleaseTextureLN','njReleaseTextureNum',
        'njReleaseTexture','njCheckPaletteType','ConvertUVnum','njSetTextureParam',
        'njSetPaletteBankLN','njSetTexture','kmFreeTexture','jp_texture_renderer_select','njGetkmPointer']
    globals=['_nj_texmemlist','_nj_texmemsize','_nj_tex_count','_nj_current_texlist',
             '_nj_current_texture','_nj_current_texture_bank','_nj_texerr_']
    common=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2',
        '-ffp-contract=off','-fno-strict-aliasing','-ffreestanding','-fno-builtin',
        '-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-unused-parameter',
        '-include',str(p/'native/jp_host_types.h'),'-I',str(p/'native'),'-I',str(d/'include'),
        '-I',str(u/'sdk/include'),'-I',str(u/'sdk')]
    commands=[];objects=[]
    for source in [p/'native/jp_native_textures.c',*sources]:
        obj=d/(source.stem+'.obj');objects.append(obj)
        commands.append(common+['-c',str(source),'-o',str(obj)])
    dll=d/'jp_native_textures.dll'
    commands.append([str(c),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',
        *['-Wl,/export:'+n for n in exports],
        *[arg for n in globals for arg in ['-Xlinker','/export:'+n+',DATA']],
        *map(str,objects),str(queue/'queue.obj'),'-o',str(dll)])
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:7000]);break
    verify();passed=len(results)==len(commands) and all(x['exit_code']==0 for x in results)
    authored=[p/'native/jp_native_textures.c',p/'native/jp_native_textures.h',p/'native/jp_host_types.h',Path(__file__)]
    report=dict(status='native_texture_runtime_linked_behavior_unverified' if passed else 'build_failed',
        commands=results,compiler_sha256=sha(c),upstream_commit=pin['upstream_commit'],
        external_input_manifest_sha256=sha(manifest),external_files_verified=len(set(base['files_sha256'])|set(pin['files_sha256'])),
        authored_source_sha256={s.relative_to(r).as_posix():sha(s) for s in authored},
        generated_private_source_sha256={s.name:sha(s) for s in sources},adaptations=changes,
        object_sha256={o.name:sha(o) for o in objects if o.exists()},
        linked_queue_object=str(queue/'queue.obj'),linked_queue_object_sha256=sha(queue/'queue.obj'),
        private_loader_macro_overlay_sha256=sha(overlay_path),
        original_loader_RECORD_consumers=136,complete_loader_compiled_or_executed=False,
        dll_sha256=sha(dll) if passed else None,game_source_linked=True,
        game_containing_artifact_publishable=False,retail_assets_embedded=False,
        complete_texture_loaders_or_renderer_qualified=False,original_sources_modified=False,
        overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',sum(x['diagnostic_bytes'] for x in results))
    raise SystemExit(0 if passed else 1)

if __name__=='__main__':main()
