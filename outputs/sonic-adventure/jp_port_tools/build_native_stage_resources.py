"""Link the native stage resource owner into the qualified Windows runtime."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'baseline', 'queue-import', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    base, out, queue_import = (path.resolve() for path in
                               (args.baseline, args.output, args.queue_import))
    root = args.workspace.resolve()
    assert out.is_relative_to(root/'work/sonic-adventure')
    assert queue_import.name == 'jp_native_read_queue.lib'
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    assert digest(base/'jp_native_read_queue.dll') == manifest['read_queue_DLL_sha256']
    assert digest(queue_import.with_suffix('.dll')) == manifest['read_queue_DLL_sha256']
    recipe, driver = manifest['land_task_owner']['commands']
    source = root/'outputs/sonic-adventure/native/jp_native_stage_resources.c'
    out.mkdir()
    dll, obj = out/'jp_native_frames.dll', out/'stage_resources.obj'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, out/dep.name)
    link = shlex.split((base/'link.rsp').read_text())
    at = link.index('-o')
    link[at:at] = [str(obj), str(queue_import)]
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    stage_resources=dict(source_sha256=digest(source), object_sha256=digest(obj),
                                         parent_sha256=digest(parent), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native stage resource owner compiled and linked')


if __name__ == '__main__':
    main()
