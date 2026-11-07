"""Extend the verified Simple scratch owner without changing its frozen source."""
import argparse
import json
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def derive(raw):
    raw = raw.replace(
        b'int jp_simple_calc_model(NJS_MODEL *model,size_t points,size_t normals,',
        b'typedef int (*jp_simple_consumer)(NJS_MODEL *, const void *, size_t, void *);\n'
        b'__declspec(dllexport) int jp_simple_calc_with_records(NJS_MODEL *model,size_t points,size_t normals,')
    old = b'NJS_VECTOR *light,Float scale,int32_t *result)'
    assert raw.count(old) == 1
    raw = raw.replace(old, b'NJS_VECTOR *light,Float scale,int32_t *result,\n'
                          b'                         jp_simple_consumer consume,void *context)')
    old = b'    _nj_vertex_buf_=saved;_nj_vertex_num_=saved_count;active=0;'
    assert raw.count(old) == 1
    raw = raw.replace(old, b'    int consumed=0;\n'
                           b'    if(!value && consume && !loop_error && !jp_simple_calc_guards())\n'
                           b'        consumed=consume(model,records,(size_t)model->nbPoint,context);\n' + old)
    old = b'    *result=value;return 0;'
    assert raw.count(old) == 1
    return raw.replace(old, b'    if(consumed)return consumed;\n' + old) + b'''
int jp_simple_calc_model(NJS_MODEL *model,size_t points,size_t normals,
                         NJS_VECTOR *light,Float scale,int32_t *result)
{
    return jp_simple_calc_with_records(model,points,normals,light,scale,result,NULL,NULL);
}
'''


def main():
    ap = argparse.ArgumentParser()
    for n in ('workspace', 'baseline', 'material', 'output'):
        ap.add_argument('--' + n, type=Path, required=True)
    a = ap.parse_args()
    root, base, material, d = (p.resolve() for p in (a.workspace, a.baseline, a.material, a.output))
    assert d.is_relative_to(root / 'work/sonic-adventure')
    b = json.loads((base / 'build_verification.json').read_bytes())
    m = json.loads((material / 'verification.json').read_bytes())
    assert digest(material / 'jp_native_frames.dll') == m['dll_sha256']
    for path, sha in b['authored_source_sha256'].items():
        assert digest(root / path) == sha, path
    source = root / 'outputs/sonic-adventure/native/jp_native_simple_calc.c'
    old = next(c['command'] for c in b['commands'] if '-c' in c['command']
               and Path(c['command'][-1]).name == 'simple_calc_native.obj')
    d.mkdir()
    generated = d / 'simple_calc_with_records.c'
    generated.write_bytes(derive(source.read_bytes()))
    obj = d / 'simple_calc_with_records.obj'
    compile_cmd = [str(generated) if Path(x).name == source.name else
                   str(obj) if i == len(old) - 1 else x for i, x in enumerate(old)]
    dll = d / 'jp_native_frames.dll'
    link = [str(obj) if Path(x).name == 'simple_calc_native.obj' else
            str(dll) if i == len(m['link']) - 1 else x for i, x in enumerate(m['link'])]
    assert link.count(str(obj)) == 1 and compile_cmd.count(str(generated)) == 1
    inputs = {x: digest(x) for x in link if x.endswith('.obj') and x != str(obj)}
    import shutil
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, d / dep.name)
    rsp = d / 'link.rsp'
    rsp.write_text('\n'.join('"' + x.replace('\\', '/') + '"' for x in link[1:]) + '\n')
    for tag, command in [('compile', compile_cmd), ('link', [link[0], '@' + str(rsp)])]:
        result = subprocess.run(command, capture_output=True)
        (d / (tag + '.stderr')).write_bytes(result.stderr)
        if result.returncode or result.stderr:
            raise RuntimeError(result.stderr.decode(errors='replace'))
    assert all(digest(x) == h for x, h in inputs.items())
    b.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll), simple_lease=dict(
        source_sha256=digest(source), generated_sha256=digest(generated), object_sha256=digest(obj),
        parent_material_report_sha256=digest(material / 'verification.json'), compile=compile_cmd,
        link=link, unchanged_link_inputs_sha256=inputs))
    (d / 'build_verification.json').write_text(json.dumps(b, indent=2) + '\n')
    print('Simple scratch consumer compiled and linked', flush=True)


if __name__ == '__main__':
    main()
