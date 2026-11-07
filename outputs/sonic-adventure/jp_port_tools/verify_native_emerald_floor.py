"""Build the static native floor query and compare against local stage geometry."""
import argparse
import ctypes as C
import hashlib
import json
import subprocess
from pathlib import Path

from extract_native_emerald_collision import TRIANGLE, surfaces


class Floor(C.Structure):
    _fields_ = [('height', C.c_float), ('entry', C.c_uint32), ('flags', C.c_uint32)]


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace', 'baseline', 'geometry', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    workspace, out = args.workspace.resolve(), args.output.resolve()
    assert out.is_relative_to(workspace/'work/sonic-adventure')
    baseline = json.loads((args.baseline/'build_verification.json').read_bytes())
    compiler = baseline['sonic_movement_task']['commands'][0][0]
    source = workspace/'outputs/sonic-adventure/native/jp_native_stage_floor.c'
    out.mkdir()
    dll = out/'jp_native_stage_floor.dll'
    command = [compiler,'--target=x86_64-pc-windows-msvc','-fuse-ld=lld','-shared',
               '-std=c17','-O2','-Wall','-Wextra','-Werror',str(source),'-o',str(dll)]
    build = subprocess.run(command,capture_output=True)
    (out/'build.stderr').write_bytes(build.stderr)
    assert not build.returncode and not build.stderr,build.stderr.decode(errors='replace')
    query = C.CDLL(str(dll)).jp_stage_floor_below
    query.argtypes = [C.c_void_p,C.c_size_t,C.c_float,C.c_float,C.c_float,C.POINTER(Floor)]
    query.restype = C.c_int
    geometry = json.loads((args.geometry/'verification.json').read_bytes())
    checks = 0
    for act,(x,y,z) in enumerate(((-9,16,4),(-986.5,1145,-2856),(5815,5,1136))):
        raw = (args.geometry/f'act{act}_triangles.bin').read_bytes()
        assert hashlib.sha256(raw).hexdigest()==geometry['acts'][act]['triangle_sha256']
        buffer = C.create_string_buffer(raw)
        count = len(raw)//TRIANGLE.size
        for dx in (-16,-8,0,8,16):
            for dz in (-16,-8,0,8,16):
                px,pz = x+dx,z+dz
                hits = [hit for hit in surfaces(raw,px,pz) if hit[0]<=y and hit[-1]]
                actual = Floor(-123,123,456)
                assert query(buffer,count,px,y,pz,C.byref(actual))==bool(hits),(act,px,pz)
                if hits:
                    height,entry,flags,_ = hits[-1]
                    assert actual.height==C.c_float(height).value and (actual.entry,actual.flags)==(entry,flags)
                else:
                    assert (actual.height,actual.entry,actual.flags)==(-123,123,456)
                checks += 1
        actual = Floor(-123,123,456)
        assert not query(buffer,count,1e6,y,1e6,C.byref(actual))
        assert (actual.height,actual.entry,actual.flags)==(-123,123,456)
        checks += 1
    report = dict(status='verified_native_static_floor_query_on_original_Emerald_Coast_geometry',
                  checks=checks,stage_acts=3,build_command=command,playable=False,
                  overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f'{checks} native Emerald Coast floor cases passed')


if __name__ == '__main__':
    main()
