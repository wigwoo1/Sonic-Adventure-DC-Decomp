"""Add verified Sonic actions to the existing canonical runtime motion owner."""
import argparse
import ctypes as C
import inspect
import json
import re
from pathlib import Path

import build_native_toe_models as builder
from build_native_stage_start import pinned
from native_packet_test_support import bind
from verify_native_motion_data import Entry
from verify_native_motion_store import View


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','upstream','inputs','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    baseline = builder.read(args.baseline/'build_verification.json')
    old_dll = Path(baseline['motion_DLL'])
    assert builder.sha(old_dll) == baseline['motion_DLL_sha256']
    link = next(row['command'] for row in baseline['commands']
                if '-shared' in row['command'] and Path(row['command'][-1]).name == old_dll.name)
    assert builder.sha(link[-1]) == baseline['motion_DLL_sha256']
    old_models = next(Path(arg) for arg in link if Path(arg).name == 'models_with_toes.obj')
    old_names = re.findall(r'^\{"([^"\n]+)",',old_models.with_suffix('.c').read_text(),re.M)
    source = pinned(args.upstream,'src/player/sonic.c')
    table = re.search(rb'^static struct player_action sonic_actions\[\] = \{.*?^\};',source,re.M|re.S).group()
    actions = [n.decode() for n in re.findall(rb'\{&(\w+),',table)]
    main_data = builder.definitions((args.inputs/'1st_read/content.c').read_text())
    roots = sorted(set(actions)&main_data.keys())
    assert len(actions) == 142 and len(roots) == 29
    recipe = inspect.getsource(builder.main)

    def replace(old,new):
        nonlocal recipe
        assert recipe.count(old) == 1,old
        recipe = recipe.replace(old,new)

    replace("Path(n).name=='models_native_data.obj'","Path(n).name=='models_with_toes.obj'")
    replace('len(old_names)==2793 and len(set(old_names))==2793',
            'old_names==prior_registry_names and len(set(old_names))==len(old_names)')
    replace("pending=['sonic_toe_'+side+'_z_'+shape for side in ('plus','minus') for shape in ('blend','shape_a','shape_b')]",
            'pending=list(action_roots)')
    replace("assert all('sonic_toe_'+side+'_z_'+shape in added for side in ('plus','minus') for shape in ('blend','shape_a','shape_b'))",
            "assert {'sonic_walk_action','sonic_land_action'}<=set(added)")
    replace(r"forward=''.join('extern '+defs[n][0]+';\n' for n in added)",
            r"forward=''.join((defs[n][0] if defs[n][0].startswith('static ') else 'extern '+defs[n][0])+';\n' for n in added)")
    recipe = recipe.replace('toe_models_','sonic_action_owner_')
    recipe = recipe.replace('canonical_model_registry_extended_with_genuine_toe_closure_unverified',
                            'canonical_model_registry_extended_with_original_Sonic_actions')
    namespace = dict(vars(builder),__file__=__file__,action_roots=roots,prior_registry_names=old_names)
    exec(compile(recipe,builder.__file__,'exec'),namespace)
    namespace['main']()

    report = args.output/'build_verification.json'
    result = builder.read(report)
    owner = C.CDLL(result['motion_DLL'])
    get = bind(owner,'jp_motion_models_entry',C.c_int,C.c_uint32,C.c_void_p)
    count = bind(owner,'jp_motion_models_count',C.c_uint32)()
    entries = []
    for index in range(count):
        item = Entry()
        assert get(index,C.byref(item)) == 0
        entries.append(item)
    assert [e.name.decode() for e in entries[:len(old_names)]] == old_names
    by_name = {e.name.decode():e for e in entries}
    init = bind(owner,'jp_motions_initialize',C.c_int,C.c_size_t,C.c_uint32)
    acquire = bind(owner,'jp_motions_acquire',C.c_int,C.c_uint32,C.c_char_p,C.c_void_p)
    release = bind(owner,'jp_motions_release',C.c_int,C.c_uint64)
    shutdown = bind(owner,'jp_motions_shutdown',C.c_int)
    assert init(16<<20,len(roots)) == 0
    views = []
    for name in roots:
        view = View()
        assert acquire(7,name.encode(),C.byref(view)) == 0,name
        assert view.kind == 5 and view.data == by_name[name].data and view.bytes == 16,name
        assert all((C.c_void_p*2).from_address(view.data)),name
        views.append(view)
    assert shutdown() == 7
    for view in views:
        assert release(view.token) == 0
    assert shutdown() == 0
    result['sonic_action_owner_verification'] = dict(leased_main_actions=len(views),
        preserved_registry_indices=len(old_names), final_registry_count=count,
        live_action_leases_block_shutdown=True)
    report.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(f'{len(views)} original main-executable actions leased through the runtime owner')


if __name__ == '__main__':
    main()
