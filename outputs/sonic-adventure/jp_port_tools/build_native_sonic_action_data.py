"""Extend the verified motion-data recipe with original Sonic action roots."""
import argparse
import inspect
import json
import re
from pathlib import Path

import build_native_motion_data as motion_data
from build_native_stage_start import pinned


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace', 'upstream', 'inputs', 'baseline', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    source = pinned(args.upstream, 'src/player/sonic.c')
    table = re.search(rb'^static struct player_action sonic_actions\[\] = \{.*?^\};',
                      source, re.M|re.S).group()
    actions = [name.decode() for name in re.findall(rb'\{&(\w+),', table)]
    assert len(actions) == 142 and actions[0] == 'sonic_stand_action'
    main_data = motion_data.definitions((args.inputs/'1st_read/content.c').read_text())
    roots = set(actions) & main_data.keys()
    assert {'sonic_stand_action','sonic_walk_action','sonic_land_action'} <= roots
    assert all(main_data[name][0].startswith('NJS_ACTION ') for name in roots)
    recipe = inspect.getsource(motion_data.main)
    anchor = '    selected=set();pending=list(roots);unresolved=set()'
    assert recipe.count(anchor) == 1
    recipe = recipe.replace(anchor, '    roots.update(sonic_action_roots)\n'+anchor)
    namespace = dict(vars(motion_data), sonic_action_roots=roots)
    exec(compile(recipe, motion_data.__file__, 'exec'), namespace)
    namespace['main']()
    report = args.output/'build_verification.json'
    result = json.loads(report.read_bytes())
    by_name = {}
    for row in result['registry']:
        if row['kind'] == 5:
            by_name.setdefault(row['name'], []).append(row['bank'])
    imports = pinned(args.upstream, 'config/1st_read/imports.txt')
    owners = dict(re.findall(rb'^(\w+)\s+(\w+)\s*$', imports, re.M))
    missing = sorted(set(actions)-by_name.keys())
    standalone = pinned(args.upstream, 'src/player/sonic_effect_models.c')
    missing_main = [name for name in missing if name.encode() not in owners]
    assert all(b'NJS_ACTION '+name.encode()+b' =' in standalone for name in missing_main)
    coverage = dict(
        table_rows=len(actions), distinct_actions=len(set(actions)),
        main_executable_roots=len(roots),
        rows_with_unique_owner=sum(len(by_name.get(name, ())) == 1 for name in actions),
        missing_overlay_actions={name:owners[name.encode()].decode() for name in missing if name.encode() in owners},
        missing_main_source_actions=missing_main,
        rows_with_multiple_owners={name:by_name[name] for name in sorted(set(actions)) if len(by_name.get(name, ())) > 1})
    result['sonic_action_data_coverage'] = coverage
    result['authored_source_sha256'][Path(__file__).resolve().relative_to(args.workspace.resolve()).as_posix()] = motion_data.sha(__file__)
    report.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(f'{coverage["rows_with_unique_owner"]}/{len(actions)} Sonic action rows have one native data owner')


if __name__ == '__main__':
    main()
