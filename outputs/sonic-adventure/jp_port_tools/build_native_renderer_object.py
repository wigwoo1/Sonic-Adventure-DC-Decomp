"""Adapt and compile the complete external GCC renderer as a native object.
No game source bodies or game-containing compiled artifacts are published.
This does not link graphics, texture, presentation or complete-game services.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path

# Authored host replacements for documented data-access contracts. The external
# function signatures and all other renderer C stay byte-for-byte unchanged.
BODIES = {
 'next_vector': '''
    const NJS_VECTOR *v = *from;
    shz_vec4_t result = shz_vec4_init(v->x, v->y, v->z, w);
    *from = v + 1;
    return result;
''',
 'prefetch_ahead': '''
    __builtin_prefetch((const void *)((uintptr_t)p + PREFETCH_AHEAD));
''',
 'project_end': '''
    point->at.x = p.x * p.rhw + by->cx;
    point->at.y = p.y * p.rhw + by->cy;
    point->at.rhw = p.rhw;
    return point;
''',
 'take_point': '''
    const struct point *point = &strips->points[index];
    struct vertex_words p;
    p.x = point->at.x; p.y = point->at.y; p.rhw = point->at.rhw;
    __builtin_memcpy(&p.base, &point->color.color, sizeof p.base);
    __builtin_memcpy(&p.offset, &strips->offsets[index], sizeof p.offset);
    return p;
''',
 'send_textured': '''
    sq->control = control;
    sq->at.x = p.x; sq->at.y = p.y; sq->at.rhw = p.rhw;
    sq->u = u; sq->v = v;
    __builtin_memcpy(&sq->base, &p.base, sizeof sq->base);
    __builtin_memcpy(&sq->offset, &p.offset, sizeof sq->offset);
    prefetch(sq);
    return sq + 1;
''',
 'next_index': '''
    int index = **mesh;
    ++*mesh;
    return index;
''',
 'next_uv': '''
    NJS_TEX value = **vertuv;
    ++*vertuv;
    *u = (Float)value.u * INV_255;
    *v = (Float)value.v * INV_255;
''',
}

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def coff_symbols(path):
    import struct
    raw=path.read_bytes();machine,sections,time,start,count,opt,flags=struct.unpack_from('<HHIIIHH',raw)
    assert machine==0x8664 and opt==0
    strings=start+18*count;result=[];i=0
    while i<count:
        offset=start+18*i;name,value,section,kind,storage,aux=struct.unpack_from('<8sIhHBB',raw,offset)
        if storage==2:
            if name[:4]==bytes(4):
                text=strings+struct.unpack_from('<I',name,4)[0];name=raw[text:raw.index(0,text)]
            else:name=name.rstrip(b'\0')
            result.append(dict(name=name.decode(),section=section,value=value,function=bool(kind&0x20)))
        i+=1+aux
    return result

def adapt(raw):
    changes=[];result=raw
    for name,body in BODIES.items():
        # Pinned top-level functions have their closing brace in column zero.
        pattern=rb'^((?:static inline|ALWAYS_INLINE)[^;{}]*?\b'+name.encode()+rb'\([^;{}]*?\)\r?\n)\{.*?^\}\r?\n'
        matches=list(re.finditer(pattern,result,re.M|re.S));assert len(matches)==1,name
        match=matches[0];replacement=match.group(1)+b'{'+body.encode()+b'}\r\n'
        changes.append(dict(function=name,original_function_sha256=hashlib.sha256(match.group()).hexdigest(),
                            replacement_function_sha256=hashlib.sha256(replacement).hexdigest()))
        result=result[:match.start()]+replacement+result[match.end():]
    assert b'__asm__' not in result
    return result,changes

def main():
    ap=argparse.ArgumentParser()
    for name in ['workspace','upstream','sh4zam','compiler','output']:ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r,u,z,c,d=(getattr(a,n).resolve() for n in ['workspace','upstream','sh4zam','compiler','output'])
    p=r/'outputs/sonic-adventure';w=r/'work/sonic-adventure'
    assert d.is_relative_to(w) and not d.is_relative_to(w/'build') and not d.exists()
    base=json.loads((p/'jp_port_tools/jp_task_external_inputs.json').read_text())
    assert base['upstream_commit']=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    for name,h in base['files_sha256'].items():assert sha(u/name)==h
    # The primary checkout's established pinned source inventory verifies these
    # additional renderer inputs; this first workflow records that dependency.
    pin=json.loads((w/'jp_gcc_worktree_preparation.json').read_text())
    extra=['gcc/src/draw/model.c','gcc/src/system/port.h','sdk/ninja/njkamui.h']
    for name in extra:assert sha(u/name)==pin['source_sha256'][name]
    d.mkdir(parents=True);original=u/'gcc/src/draw/model.c';adapted,changes=adapt(original.read_bytes())
    source=d/'external_model_native.c';source.write_bytes(adapted)
    obj=d/'model.obj'
    command=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin',
             '-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-Wno-unknown-attributes',
             '-include',str(p/'native/jp_host_types.h'),'-include',str(p/'native/jp_renderer_host_compat.h')]
    command+=['-I'+str(u/f) for f in ['sdk/include','src','modules','events','gcc/src','.']]
    command+=['-isystem',str(z/'include'),'-c',str(source),'-o',str(obj)]
    result=subprocess.run(command,capture_output=True);log=d/'compile.txt';log.write_bytes(result.stdout+result.stderr)
    symbols=coff_symbols(obj) if result.returncode==0 else []
    report=dict(status='complete_external_renderer_native_object_compiled' if result.returncode==0 else 'native_renderer_object_build_failed',
        command=command,exit_code=result.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log),
        upstream_commit=base['upstream_commit'],extra_external_input_sha256={n:sha(u/n) for n in extra},
        original_source_sha256=sha(original),generated_source_sha256=sha(source),adapted_functions=changes,
        external_dependency_headers_sha256={path.relative_to(z).as_posix():sha(path) for path in (z/'include').rglob('*.h')},
        authored_source_sha256={s.relative_to(r).as_posix():sha(s) for s in [Path(__file__),p/'native/jp_renderer_host_compat.h',p/'native/jp_sh4zam_host_config.h']},
        object_sha256=sha(obj) if result.returncode==0 else None,
        undefined_symbols=sorted(s['name'] for s in symbols if s['section']==0 and s['value']==0),
        defined_function_symbols=sorted(s['name'] for s in symbols if s['section']>0 and s['function']),
        original_source_modified=False,game_source_or_object_publishable=False,linked_or_runtime_qualified=False,
        overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',log.stat().st_size,'unresolved imports',len(report['undefined_symbols']))
    if result.returncode:print(log.read_text()[:7500])
    raise SystemExit(result.returncode)

if __name__=='__main__':main()
