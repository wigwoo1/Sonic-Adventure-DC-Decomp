"""Link the bounded model dispatcher with the existing original drawers."""
import argparse
import json
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def main():
    parser=argparse.ArgumentParser()
    for name in ('workspace','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args()
    root,base,out=(p.resolve() for p in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    manifest=base/'build_verification.json'
    baseline=json.loads(manifest.read_bytes())
    assert digest(baseline['frames_DLL'])==baseline['frames_DLL_sha256']
    recipe=next(x for x in baseline['simple_smooth']['commands']
                if '-c' in x and Path(x[-1]).name=='smooth_caller.obj')
    source=root/'outputs/sonic-adventure/native/jp_native_simple_model.c'
    obj=out/'simple_model.obj'
    dll=out/'jp_native_frames.dll'
    out.mkdir()
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    link=baseline['simple_smooth_tex_strips']['link'].copy()
    inputs={x:digest(x) for x in link if x.endswith('.obj')}
    link[link.index('-o'):link.index('-o')]=[str(obj)]
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    commands=[recipe[:recipe.index('-c')]+['-c',str(source),'-o',str(obj)], [link[0],'@'+str(rsp)]]
    for tag,cmd in zip(('compile','link'),commands):
        result=subprocess.run(cmd,capture_output=True)
        (out/(tag+'.stderr')).write_bytes(result.stderr)
        if result.returncode or result.stderr:
            raise RuntimeError(result.stderr.decode(errors='replace'))
    assert all(digest(x)==sha for x,sha in inputs.items())
    baseline.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_model=dict(
        source_sha256=digest(source),object_sha256=digest(obj),parent_sha256=digest(manifest),
        link=link,commands=commands,unchanged_objects_sha256=inputs))
    (out/'build_verification.json').write_text(json.dumps(baseline,indent=2)+'\n')
    print('Bounded multi-mesh Simple dispatcher compiled and linked',flush=True)


if __name__=='__main__':
    main()
