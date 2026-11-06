"""Add genuine character lists and original reference loops to shared Frames."""
import argparse,hashlib,json,re,shlex,shutil,struct,subprocess
from pathlib import Path
from build_native_motion_data import records
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
ROOTS={'sonic_textures':('sonic_texlist',),'tails_textures':('tails_texlist',),'knuckles_textures':('knuckles_texlist','knu_eff_texlist'),
    'amy_textures':('amy_texlist','amy_eff_texlist'),'big_textures':('big_texlist','fishing_texlist'),'e102_textures':('e102_texlist','e102beam_texlist','e102effect_texlist'),
    'big_kaeru_textures':('big_kaeru_texlist',),'e102_bonus':('e102time_texlist',),'e102_hit':('e102hit_texlist',),'effects':('son_eff_texlist',),'eggman_textures':('eggman_texlist',),'tikal_textures':('tikal_texlist',)}
CHARACTERS=(('sonic_texlist','son_eff_texlist'),('eggman_texlist',),('tails_texlist',),('knuckles_texlist','knu_eff_texlist'),('tikal_texlist',),('amy_texlist','amy_eff_texlist'),('e102_texlist','e102beam_texlist','e102effect_texlist','e102time_texlist','e102hit_texlist'),('big_texlist','fishing_texlist','big_kaeru_texlist'))
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def pe_code(path):
    raw=Path(path).read_bytes();at=struct.unpack_from('<I',raw,60)[0];count=struct.unpack_from('<H',raw,at+6)[0];start=at+24+struct.unpack_from('<H',raw,at+20)[0]
    for i in range(count):
        row=start+40*i
        if raw[row:row+8].rstrip(b'\0')==b'.text':
            size,offset=struct.unpack_from('<II',raw,row+16);return hashlib.sha256(raw[offset:offset+size]).hexdigest()
    raise AssertionError('missing PE .text')
def definitions(text):
    # Preprocessed SDK headers contain inline functions and assignments. Match
    # only the actual file-scope texture data types so none can swallow an array.
    pattern=r'^((?:static\s+)?(?:const\s+)?NJS_TEX(?:NAME|LIST)\s+([A-Za-z_]\w*)(?:\[[^\n]*\])?) = (?:[^\n]+;\n|\{.*?^\};\n)'
    return {m[2]:(m[1],m[0]) for m in re.finditer(pattern,text,re.M|re.S)}
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','inputs','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,inputs,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','inputs','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');iv=read(inputs/'inputs.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'));kept={x:sha(x) for x in link if x.endswith('.obj')};symbols={}
    for obj in kept:
        for s in coff_symbols(Path(obj)):
            if s['section']>0:symbols.setdefault(s['name'],[]).append(obj)
    roots=[n for names in ROOTS.values() for n in names];assert len(roots)==17 and not set(roots+['character_users'])&set(symbols)
    assert not set(roots)&{x['name'] for x in read(base/'motion_view/build_verification.json')['registry']}
    original=(inputs/'original_texture.c').read_bytes();assert sha(inputs/'original_texture.c')==iv['original_texture_source_sha256']==sha(u/'src/system/texture.c')
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
    units=[];registry=[];selected_sources={};root_meta={}
    for source,selected_roots in ROOTS.items():
        meta=iv['files'][source];assert sha(inputs/(source+'.i'))==meta['preprocessed_sha256'] and sha(inputs/(source+'.txt'))==meta['table_sha256'] and sha(u/meta['source'])==meta['source_sha256']
        defs=definitions((inputs/(source+'.i')).read_text());table=records(inputs/(source+'.txt'));pending=list(selected_roots);selected=set()
        while pending:
            name=pending.pop()
            if name in selected:continue
            assert name in defs,(source,name);selected.add(name)
            head,body=defs[name];pending.extend(set(re.findall(r'\b[A-Za-z_]\w*\b',body))&set(defs)-selected)
        data={n:v for n,v in defs.items() if n in selected};assert all(re.match(r'(?:static )?(?:const )?NJS_TEX(?:NAME|LIST)\b',head) for head,body in data.values()),(source,list(data))
        # The original E102 hit list casts away const then the real loader
        # writes handles into it. Console flat memory allowed those writes;
        # Windows .rdata does not. Preserve defaults in writable native storage.
        original_hashes={n:hashlib.sha256(body.encode()).hexdigest() for n,(head,body) in data.items()};writable=[]
        for name,(head,body) in list(data.items()):
            if head.startswith('static const NJS_TEXNAME '):
                data[name]=(head.replace('static const ','static ',1),body.replace('static const ','static ',1));writable.append(name)
        prefix='#include <ninja.h>\n';forward=''.join((head if head.startswith('static ') else 'extern '+head)+';\n' for head,body in data.values())
        body='\n'.join(body for head,body in data.values());unit=d/('external_character_'+source+'.c');unit.write_text(prefix+forward+body);units.append(unit);selected_sources[source]=dict(names=list(data),original_definition_sha256=original_hashes,writable_native_texture_names=writable)
        for name in selected_roots:root_meta[name]=dict(source=source,**table[name])
    family=original[original.index(b'Sint8 character_users[CHARACTER_COUNT] = {0};'):original.index(b'/* the acts\' pvms.')]
    prefix=b'''#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#undef TRUE
#undef FALSE
#include "jp_native_textures.h"
#include "jp_native_character_textures.h"
enum {CHARACTER_COUNT=8};
struct texture_pvm {char *name;NJS_TEXLIST *texlist;};
'''+''.join('extern NJS_TEXLIST '+n+';\n' for n in roots).encode()
    descriptors='\nstruct character_texture_definition {const char *name;NJS_TEXLIST *list;uint32_t address,character,slot,bank;};\nstatic const struct character_texture_definition character_list_definitions[]={\n'
    for character,names in enumerate(CHARACTERS):
        for slot,name in enumerate(names):
            row=root_meta[name];bank=1 if character==4 else 0
            descriptors+=f'{{"{name}",&{name},0x{row["address"]:08x}u,{character},{slot},{bank}}},\n';registry.append(dict(row,character=character,slot=slot,bank=bank))
    descriptors+='};\n'
    guards=b'''static int character_dependency_failed(NJS_TEXLIST *,int);
void texture_load_pvm(char *,NJS_TEXLIST *);
#define texture_load_character jp_original_texture_load_character
#define texture_release_character jp_original_texture_release_character
#define texture_release_characters jp_original_texture_release_characters
#define texture_load_pvm(name,list) do {texture_load_pvm(name,list);if(character_dependency_failed(list,0))return;} while(0)
#define njLoadTexture(list) do {(void)njLoadTexture(list);if(character_dependency_failed(list,0))return;} while(0)
#define njReleaseTexture(list) do {(void)njReleaseTexture(list);if(character_dependency_failed(list,1))return;} while(0)
'''
    bodies={n:function(original,n) for n in ('texture_load_character','texture_release_character','texture_release_characters')}
    suffix=b'''#undef texture_load_character
#undef texture_release_character
#undef texture_release_characters
#undef texture_load_pvm
#undef njLoadTexture
#undef njReleaseTexture
__declspec(dllexport) unsigned jp_character_texture_definition_count(void){return 17;}
__declspec(dllexport) int jp_character_texture_definition(unsigned i,struct character_texture_definition *out)
{if(i>=17||!out)return 1;*out=character_list_definitions[i];return 0;}
#include "jp_native_character_textures.inc"
'''
    unit=d/'external_character_texture_family.c';unit.write_bytes(prefix+family+descriptors.encode()+guards+b'\n'.join(bodies.values())+suffix);units.append(unit)
    compiler=b['commands'][0]['command'][0];common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-fno-strict-aliasing','-fwrapv','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-Wno-missing-braces','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include')]
    commands=[common+['-c',str(unit),'-o',str(unit.with_suffix('.obj'))] for unit in units]
    link[link.index('-o'):link.index('-o')]=[str(unit.with_suffix('.obj')) for unit in units]+[value for n in roots+['character_users','e102beam_textures'] for value in ('-Xlinker','/export:'+n+',DATA')]
    link[-1]=str(d/'jp_native_frames.dll');commands.append(link)
    # Real retail VQ mipmapped textures reach the previously rejected1x1 mip.
    # Replace only that authored decoder object in the existing GPU module.
    gpu_recipe=r/'work/sonic-adventure/jp-native-boot-buffers-v3';gb=read(gpu_recipe/'build_verification.json')
    assert pe_code(gb['GPU_DLL'])==pe_code(b['GPU_DLL'])
    gpu_link=list(next(x['command'] for x in gb['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name==Path(b['GPU_DLL']).name))
    gpu_old=next(x for x in gpu_link if x.endswith('.obj') and Path(x).name=='jp_native_texels.obj')
    gpu_kept={x:sha(x) for x in gpu_link if x.endswith('.obj') and x!=gpu_old}
    commands.append(common+['-c',str(p/'native/jp_native_texels_vq1.c'),'-o',str(d/'texels_vq1.obj')])
    gpu_link[gpu_link.index(gpu_old)]=str(d/'texels_vq1.obj')
    imports=[x for x in gpu_link if Path(x).name=='jp_native_frames.lib'];assert len(imports)==1;gpu_link[gpu_link.index(imports[0])]=str(d/'jp_native_frames.lib')
    gpu_link[-1]=str(d/Path(b['GPU_DLL']).name);commands.append(gpu_link);results=[]
    for i,cmd in enumerate(commands):
        executed=cmd;response={}
        if len(subprocess.list2cmdline(cmd))>=30000:
            assert all('\n' not in arg and '\r' not in arg for arg in cmd)
            content='\n'.join('"'+arg.replace('\\','\\\\').replace('"','\\"')+'"' for arg in cmd[1:])+'\n'
            assert shlex.split(content)==cmd[1:]
            path=d/f'character_texture_build_{i}.rsp';path.write_text(content);executed=[cmd[0],'@'+str(path)];response=dict(response_file=str(path),response_file_sha256=sha(path))
        run=subprocess.run(executed,capture_output=True);log=d/f'character_texture_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,executed_command=executed,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,**response));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()[:6500]
    for name,h in kept.items():assert sha(name)==h,name
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native/jp_native_character_textures.h',p/'native/jp_native_character_textures.inc',p/'native/jp_native_texels_vq1.c',Path(__file__).resolve(),p/'jp_port_tools/prepare_native_character_textures.py']
    updates.update(status='genuine_all_eight_character_texture_definitions_original_reference_loops_and_checked_recovery_linked_unverified',
        character_texture_baseline_sha256=sha(base/'build_verification.json'),character_texture_input_manifest_sha256=sha(inputs/'inputs.json'),character_texture_original_body_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},
        character_texture_source_sha256=iv['original_texture_source_sha256'],character_texture_registry=registry,character_texture_selected_definitions=selected_sources,
        character_texture_generated_source_sha256={unit.name:sha(unit) for unit in units},character_texture_kept_objects_sha256=kept,
        character_texture_new_build_commands=len(results),character_texture_kept_GPU_objects_sha256=gpu_kept,character_texture_replaced_texel_object_sha256=sha(gpu_old),
        character_texture_GPU_recipe_sha256=sha(gpu_recipe/'build_verification.json'),character_texture_GPU_prior_text_sha256=pe_code(b['GPU_DLL']),
        commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{path.relative_to(r).as_posix():sha(path) for path in authored}),
        full_set_character_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
