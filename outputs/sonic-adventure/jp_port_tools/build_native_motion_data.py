"""Compile complete generated JP motion banks and their genuine model closure.
All generated game data, native data DLLs and local retail files stay private.
"""
import argparse,hashlib,json,re,subprocess
from decimal import Decimal
from pathlib import Path
import struct

BANKS=('s_mot','a_mot','b_mot','e_mot','k_mot','s_sbmot','m_sbmot')
DEFINITION=re.compile(r'^([^\n;]*?\b([A-Za-z_]\w*)(?:\[[^\n]*\])?) = (?:[^\n]+;\n|\{.*?^\};\n)',re.M|re.S)
TOKEN=re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|(?<![\w.])(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?[fF]',re.S)
LAYOUTS={
    'NJS_OBJECT':(52,[(0,'evalflags',4,0),(4,'model',4,1),(8,'pos',36,0),(44,'child',4,1),(48,'sibling',4,1)]),
    'NJS_MODEL':(40,[(0,'points',4,1),(4,'normals',4,1),(8,'nbPoint',4,0),(12,'meshsets',4,1),(16,'mats',4,1),(20,'nbMeshset',4,0),(24,'center',16,0)]),
    'NJS_MESHSET':(24,[(0,'type_matId',4,0),(4,'meshes',4,1),(8,'attrs',4,1),(12,'normals',4,1),(16,'vertcolor',4,1),(20,'vertuv',4,1)]),
    'NJS_MOTION':(12,[(0,'mdata',4,1),(4,'nbFrame',8,0)]),
    'NJS_ACTION':(8,[(0,'object',4,1),(4,'motion',4,1)]),
    'NJS_MDATA2':(16,[(0,'p[0]',4,1),(4,'p[1]',4,1),(8,'nb[0]',8,0)]),
    'NJS_MDATA3':(24,[(0,'p[0]',4,1),(4,'p[1]',4,1),(8,'p[2]',4,1),(12,'nb[0]',12,0)]),
    'NJS_TEXNAME':(12,[(0,'filename',4,1),(4,'attr',8,0)]),
    'NJS_TEXLIST':(8,[(0,'textures',4,1),(4,'nbTexture',4,0)]),
}
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def records(path):
    result={}
    for line in Path(path).read_text().splitlines():
        f=line.split()
        if len(f)>=4 and f[2]!='-':result[f[0]]=dict(name=f[0],address=int(f[1],16),bytes=int(f[2]),original_type=' '.join(f[3:]))
    return result
def definitions(text):return {m[2]:(m[1],m[0]) for m in DEFINITION.finditer(text)}
def exact_floats(text):
    count=changed=0
    def replace(m):
        nonlocal count,changed
        s=m[0]
        if s[0] in '/"\'':return s
        count+=1;value=Decimal(s[:-1]);nearest=struct.unpack('<I',struct.pack('<f',float(value)))[0]
        actual=struct.unpack('<f',struct.pack('<I',nearest))[0]
        if Decimal.from_float(actual)>value:nearest-=1;changed+=1
        f=struct.unpack('<f',struct.pack('<I',nearest))[0]
        return f.hex()+'F'
    return TOKEN.sub(replace,text),dict(literals=count,nearest_rounding_would_change=changed)

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','inputs','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,inputs,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','inputs','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    pinned=read(inputs/'inputs.json')
    for key,v in pinned['files'].items():bank,ext=key.split('/');assert sha(inputs/bank/('content.'+ext))==v['sha256']
    defs={};meta={};roots=set()
    for bank in BANKS+('1st_read',):
        text=(inputs/bank/'content.c').read_text();defs[bank]=definitions(text);meta[bank]=records(inputs/bank/'content.txt')
        assert set(defs[bank])==set(meta[bank])
        if bank!='1st_read':roots.update(re.findall(r'^extern NJS_OBJECT (\w+);',text,re.M))
    extra=inputs/'model_sources-v3';cpp_inputs=read(extra/'inputs.json');model_defs=dict(defs['1st_read']);model_meta=dict(meta['1st_read'])
    for source,v in cpp_inputs.items():
        assert sha(extra/(source+'.i'))==v['preprocessed_sha256'] and sha(extra/(source+'.txt'))==v['body_table_sha256']
        candidate=definitions((extra/(source+'.i')).read_text());table=records(extra/(source+'.txt'))
        for name,entry in table.items():
            if name not in candidate:continue
            assert name not in model_defs,name;model_defs[name]=candidate[name];model_meta[name]=entry
    selected=set();pending=list(roots);unresolved=set()
    while pending:
        name=pending.pop()
        if name in selected:continue
        if name not in model_defs:unresolved.add(name);continue
        selected.add(name);refs=set(re.findall(r'\b[A-Za-z_]\w*\b',model_defs[name][1].split(' = ',1)[1]));pending.extend((refs&model_defs.keys())-selected)
        # Named external data must be resolved by an actual selected source.
        unresolved.update(n for n in refs if (n.startswith('dat_') or n in roots or n.startswith('amy_skirt_')) and n not in model_defs)
    assert not unresolved,sorted(unresolved)
    defs['models']={n:model_defs[n] for n in model_defs if n in selected};meta['models']={n:model_meta[n] for n in defs['models']}
    d.mkdir();units=[];registry=[];float_counts={};layouts=list(LAYOUTS);input_hashes={}
    for bank_id,bank in enumerate(BANKS+('models',)):
        data=defs[bank];rows=meta[bank];prefix='#include <ninja.h>\n#include <stddef.h>\n#include <stdint.h>\n'
        # All banks coexist natively, whereas the DC overlays share addresses.
        # Namespace local bank records, retaining canonical shared model names.
        aliases=''.join(f'#define {name} jp_{bank}_{name}\n' for name in data)
        if bank!='models':
            externs=re.findall(r'^extern NJS_OBJECT (\w+);',(inputs/bank/'content.c').read_text(),re.M)
            prefix+=''.join(f'extern NJS_OBJECT jp_models_{name};\n#define {name} jp_models_{name}\n' for name in externs)
        forward=''.join((head if head.startswith('static ') else 'extern '+head)+';\n' for head,body in data.values())
        body='\n'.join(body for head,body in data.values());body,stats=exact_floats(body);float_counts[bank]=stats
        footer='\nstruct motion_item {const char *name;void *data;size_t host_bytes;uint32_t address,original_bytes,kind,original_element,host_element;};\n'
        footer+='static const struct motion_item entries[]={\n'
        for index,(name,(head,original_body)) in enumerate(data.items()):
            row=rows[name];ctype=re.sub(r'\s+'+re.escape(name)+r'(?:\[.*\])?$','',head).strip();ctype=re.sub(r'^(?:static|const)\s+','',ctype)
            kind=layouts.index(ctype)+1 if ctype in LAYOUTS else 0;element=LAYOUTS[ctype][0] if kind else 1
            assert row['bytes']%element==0
            footer+=f'{{"{name}",&{name},sizeof({name}),0x{row["address"]:08x}u,{row["bytes"]},{kind},{element},'+(f'sizeof({ctype})' if kind else '1')+'},\n'
            registry.append(dict(row,bank=bank,bank_id=bank_id,index=index,kind=kind,ctype=ctype))
        footer+='};\n__declspec(dllexport) unsigned jp_motion_'+bank+'_count(void){return sizeof(entries)/sizeof(entries[0]);}\n'
        footer+='__declspec(dllexport) int jp_motion_'+bank+'_entry(unsigned i,struct motion_item *out){if(!out||i>=sizeof(entries)/sizeof(entries[0]))return 1;*out=entries[i];return 0;}\n'
        unit=d/(bank+'_native_data.c');unit.write_text(prefix+aliases+forward+body+footer);units.append(unit)
    layout_source='#include <ninja.h>\n#include <stdint.h>\n#include <stddef.h>\nstruct field {uint32_t original,host,bytes,pointer;};\n'
    for kind,(ctype,(size,fields)) in enumerate(LAYOUTS.items(),1):
        layout_source+=f'static const struct field type_{kind}[]={{'+','.join('{'+f'{off},offsetof({ctype},{name}),{count},{pointer}'+'}' for off,name,count,pointer in fields)+'};\n'
    layout_source+='__declspec(dllexport) unsigned jp_motion_layout(unsigned kind,unsigned index,struct field *out){switch(kind){\n'
    for kind,(ctype,(size,fields)) in enumerate(LAYOUTS.items(),1):layout_source+=f'case {kind}:if(index<{len(fields)}&&out)*out=type_{kind}[index];return {len(fields)};\n'
    layout_source+='default:return 0;}}\n';unit=d/'native_layouts.c';unit.write_text(layout_source);units.append(unit)
    compiler=b['commands'][0]['command'][0];common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-Wno-missing-braces','-include',str(p/'native/jp_host_types.h'),'-I'+str(u/'sdk/include')]
    commands=[common+['-c',str(unit),'-o',str(unit.with_suffix('.obj'))] for unit in units]
    dll=d/'jp_native_motion_data.dll';commands.append([compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*[str(x.with_suffix('.obj')) for x in units],'-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()[:6500]
    out=dict(status='all_seven_complete_native_motion_banks_and_genuine_shared_character_model_closure_linked_unverified',
        baseline_verification_sha256=sha(base/'build_verification.json'),private_inputs_manifest_sha256=sha(inputs/'inputs.json'),private_cpp_manifest_sha256=sha(extra/'inputs.json'),
        registry=registry,bank_definition_counts={n:len(defs[n]) for n in BANKS+('models',)},root_models=sorted(roots),float_literal_adaptation=float_counts,
        generated_source_sha256={x.name:sha(x) for x in units},DLL=str(dll),DLL_sha256=sha(dll),commands=results,
        authored_source_sha256={Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)},
        original_model_declarations_or_motion_bodies_replaced=False,native_motion_evaluation_or_character_loading_verified=False,
        combined_game_data_DLL_publishable=False,retail_assets_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['bank_definition_counts'],flush=True)

if __name__=='__main__':main()
