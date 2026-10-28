"""Additive normal-entry input-alias evidence; preserve the published caller proofs."""
import hashlib
import inspect
import itertools
import json
import types
from pathlib import Path

import collision_contact_fixtures as fixture

P = Path(__file__).resolve().parent
ROOT = P.parents[1]
SKIP = (0x8c02a592, 0x8c02a5f4)
ACCUMULATE = (0x8c02a592, 0x8c02a596)


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def edges(path, ram):
    result = set()
    for i, pc in enumerate(path):
        op = int.from_bytes(ram[pc & 0xffffff:(pc & 0xffffff) + 2], 'little') >> 8
        if op in (0x89, 0x8b, 0x8d, 0x8f):
            j = i + (2 if op in (0x8d, 0x8f) else 1)
            if j < len(path):
                result.add((pc, path[j]))
    return result


def compare_with_readonly_trace(m, s, f, c):
    """Compose with the CPU's existing trace hook; preserve its path observer."""
    namespace = fixture.fast.namespace
    original = namespace['reference']
    signature = inspect.signature(original)
    samples = []

    def traced(*args, **kwargs):
        bound = signature.bind(*args, **kwargs)
        prior = bound.arguments.get('trace')
        ram = bound.arguments['ram']

        def observe(pc, word, regs, sr, pr, fp):
            if prior is not None:
                prior(pc, word, regs, sr, pr, fp)
            if pc in (0x8c02a41a,0x8c02a48c,0x8c02a512,0x8c02a56c,
                      0x8c02a58c,0x8c02a592,0x8c02a5fa,0x8c02a606):
                samples.append(dict(pc=hex(pc),counter=hex(fixture.get(ram,fixture.COUNT)),
                                    r1=hex(regs[1]),r2=hex(regs[2]),r5=hex(regs[5]),
                                    r10=hex(regs[10]),sp=hex(regs[15]),sr=hex(sr)))
        bound.arguments['trace'] = observe
        return original(*bound.args, **bound.kwargs)

    namespace['reference'] = traced
    try:
        result = fixture.fast.compare(m,s,f,c)
    finally:
        namespace['reference'] = original
    return result,samples


def main():
    # This relocates the QUERY INPUT object before execution. It does not change
    # code, a callee result, the reference CPU, or a runtime stack frame.
    alias_query = fixture.COUNT - 0x54
    relocated = types.FunctionType(
        fixture.prepare.__code__, dict(fixture.prepare.__globals__, QUERY=alias_query),
        'prepare_query_counter_alias')
    ram = fixture.v.seed_ram()
    seen, observed, records, checks = set(), set(), [], 0

    def require(condition, label):
        nonlocal checks
        assert condition, label
        checks += 1

    for view, bank, overlap in itertools.product(
            (0x0c000000, 0x8c000000, 0xac000000), range(2), (False, True)):
        config = dict(point=(0, .25, .5), face_count=2, alias=view,
                      mode=bank * 4, case_id=len(records))
        query = alias_query if overlap else fixture.QUERY
        prepare = relocated if overlap else fixture.prepare
        m, s, f, c = prepare(ram, config)
        initial_s = type(s).from_buffer_copy(s)
        initial_f = type(f).from_buffer_copy(f)
        before = hashlib.sha256(m).hexdigest()
        require(s.r[4] == view | (query & 0xffffff), 'input query address')
        require(fixture.get(m, fixture.COUNT) == 0x12345678, 'counter is authored input before original reset')
        (result, path, actual, s, f), samples = compare_with_readonly_trace(m,s,f,c)
        got = edges(path, ram)
        require(result['status'] == 0, 'actual caller and game dependencies complete')
        require(result['full_state_match'] and result['ordered_boundaries_match'],
                'full original/native CPU/FPU/RAM/device/queue and ordered call boundaries')
        require(s.pc == s.pr == initial_s.pr and s.r[15] == initial_s.r[15], 'normal return/stack')
        require(list(s.r[8:15]) == list(initial_s.r[8:15]), 'callee-saved integer registers')
        require(list(f.banks[bank][12:]) == list(initial_f.banks[bank][12:]), 'callee-saved physical FP registers')
        require(s.r[0] == 1, 'original caller reports contact')
        require((SKIP if overlap else ACCUMULATE) in got, 'derived alias branch versus disjoint control')
        require((ACCUMULATE if overlap else SKIP) not in got, 'opposite accumulation branch not executed')
        require((0x8c02a48c, 0x8c02a490) in got, 'result-copy branch remains ordinary')
        require(fixture.get(actual, fixture.COUNT) == (0x7fbfffff if overlap else 2),
                'final counter aliases canonical NaN output, or remains two in disjoint control')
        sample = {int(x['pc'],16):x for x in samples}
        require(sample[0x8c02a41a]['counter']=='0x2' and sample[0x8c02a41a]['r10']=='0x1',
                'actual contacts generated before result selection')
        require(sample[0x8c02a58c]['counter']==('0x0' if overlap else '0x2'),
                'original query stores change the aliased counter before its reload')
        require(sample[0x8c02a592]['r5']==('0xffffffff' if overlap else '0x1'),
                'actual reload then decrement controls the branch')
        if overlap:
            require(sample[0x8c02a56c]['r1']==hex(view | (fixture.COUNT & 0xffffff))
                    and sample[0x8c02a56c]['r2']=='0x0', 'original first output store writes zero over COUNT')
        seen.update(path)
        observed.update(got)
        records.append(dict(configuration=config,query=hex(view | (query & 0xffffff)),
                            query_overlaps_counter=overlap,query_output_offset=0x54 if overlap else None,
                            initial_ram_sha256=before,status=result['status'],stop_pc=result['stop_pc'],
                            original_rows=len(path),result=s.r[0],final_counter=hex(fixture.get(actual, fixture.COUNT)),
                            full_state_match=True,ordered_boundaries_match=True,boundaries=result['boundaries'],
                            original_readonly_trace=samples,
                            observed_conditional_edges=[[hex(a),hex(b)] for a,b in sorted(got)]))
        print('COUNTER ALIAS',len(records),hex(view),'bank',bank,'overlap',overlap,'PASS',flush=True)

    body = fixture.v.ANALYSIS_ROUTINES[0]
    wanted = set(map(lambda x:int(x,16),body['reachable_addresses']))
    outcomes = {tuple(int(x,16) for x in edge) for edge in body['conditional_edges']}
    files = [Path(__file__), P/'collision_contact_fixtures.py',P/'collision_contact_reference.py',
             P/'write_watch_collision_contact.py',P/'verify_collision_contact_direct.py',
             fixture.v.MP,P/'analysis/collision_contact_connection.json',
             P/'analysis/collision-contact/8c029b00_instructions.tsv',
             P/'source/sa_collision_contact.c',fixture.v.effects.DLL]
    report = dict(status='verified_additive_input_alias_branch_evidence',
                  original_native_comparisons=len(records),independent_checks=checks,
                  independent_complete_physics_oracles=0,status_counts={'0':len(records)},cases=records,
                  coverage_by_routine=[dict(address=body['address'],static_rows=len(wanted),
                    executed_rows=len(wanted & seen),executed_outcomes=len(outcomes & observed),
                    missing_rows=list(map(hex,sorted(wanted-seen))),
                    missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-observed)])],
                  newly_executed_outcomes=[[hex(a),hex(b)] for a,b in (SKIP,)],
                  new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,
                  dll_sha256=digest(fixture.v.effects.DLL),sha256={str(p):digest(p) for p in files},
                  scope='Normal029b00entry with authored linked world and two real generated triangle contacts. '
                  'QUERY=COUNT-54 aliases query result output with COUNT. Original A56c writes that output '
                  'before A58c reloads COUNT; A592 skip is reachable. Disjoint controls execute accumulation. '
                  'Three mapped MMU-off memory views and both physical FP banks in ordinary PR0/SZ0 mode. '
                  'Unmodified complete-memory comparison and real callees; no provided outputs/frames. '
                  'Canonical NaN final counter is compared as raw bits, not a valid contact count. '
                  'Input alias does not prove naturally initialized gameplay uses this layout. '
                  'No complete physical-FPU, exception-delivery, GPU or gameplay claim; A48c skip remains open.')
    dest=P/'analysis/collision_contact_counter_alias_verification.json'
    dest.write_text(json.dumps(report,indent=2)+'\n')
    print('COUNTER ALIAS VERIFIED',len(records),'comparisons',checks,'checks',flush=True)


if __name__ == '__main__':
    main()
