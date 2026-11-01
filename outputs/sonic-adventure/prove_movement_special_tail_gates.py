"""Source-bound reachability arguments for two unexecuted ordinary-entry edges.

All instructions remain in the C translation. These arguments are limited to
normal synchronous execution with unchanged code/return control, the supported
software FPU domain, and no externally injected register changes between rows.
They do not qualify nonlocal entries, asynchronous hardware or the whole game.
"""
import csv
import hashlib
import json
from pathlib import Path

P = Path(__file__).resolve().parent
ROOT = P.parents[1]


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def listing(path, raw):
    rows = {int(r['address'], 16): r for r in csv.DictReader(path.open(), delimiter='\t')}
    for pc, row in rows.items():
        assert raw[pc - 0x8c010000:pc - 0x8c010000 + 2] == bytes.fromhex(row['bytes']), hex(pc)
    return rows


def main():
    original = ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN'
    assert digest(original) == '45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    raw = original.read_bytes()
    parent_path = P / 'analysis/movement-special-tail/8c0777e0_instructions.tsv'
    child_path = P / 'analysis/movement-contact-candidates/8c075f8c_instructions.tsv'
    parent, child = listing(parent_path, raw), listing(child_path, raw)

    def exact(rows, pc, instruction):
        assert rows[pc]['instruction'].lstrip('_') == instruction, (hex(pc), rows[pc])

    for pc, instruction in (
            (0x8c077d24, 'fcmp/gt fr3,fr4'), (0x8c077d26, 'bf 0x8c077d3a'),
            (0x8c077d28, 'fcmp/gt fr3,fr4'), (0x8c077d2a, 'bt 0x8c077d30'),
            (0x8c077d2c, 'bra 0x8c078150'), (0x8c077d2e, 'nop')):
        exact(parent, pc, instruction)
    # No direct branch/call can enter the second compare/branch or dead block.
    assert not [(pc, r['flows']) for pc, r in parent.items()
                if any(int(t, 16) in range(0x8c077d28, 0x8c077d30, 2)
                       for t in r['flows'].split())]
    # For either possible comparison predicate, the first BF either leaves
    # this block, or the identical comparison makes the following BT taken.
    for predicate in (False, True):
        reaches_second = predicate
        reaches_dead = reaches_second and not predicate
        assert not reaches_dead

    exact(child, 0x8c0761d0, 'bra 0x8c076382')
    exact(child, 0x8c0761d2, 'mov #0x0,r0')
    exact(child, 0x8c076370, 'mov #0xc,r0')
    exact(child, 0x8c07639c, 'rts')
    exact(child, 0x8c07639e, 'mov.l @r15+,r14')
    assert [pc for pc, r in child.items() if r['instruction'] == 'rts'] == [0x8c07639c]
    assert not [r for r in child.values() if r['instruction'].lstrip('_').startswith(('jmp ', 'braf '))]
    incoming = [(pc, int(t, 16)) for pc, r in child.items()
                for t in r['flows'].split() if 0x8c076370 <= int(t, 16) <= 0x8c07639e]
    assert incoming == [(0x8c0761d0, 0x8c076382)], incoming
    # From the final immediate R0 assignment through RTS there are no calls,
    # branches or further R0 writes. The early epilogue entry sets R0 to zero
    # in its branch's mandatory delay slot. Both paths share this suffix.
    for pc in range(0x8c076372, 0x8c0763a0, 2):
        text = child[pc]['instruction'].lstrip('_')
        assert not text.endswith(',r0') and not text.startswith(('jsr ', 'bsr ', 'jmp ', 'bra ', 'bt ', 'bf ')), (hex(pc), text)
    for pc, instruction in (
            (0x8c077d52, 'mov r0,r4'), (0x8c077d54, 'tst r4,r4'),
            (0x8c077d56, 'bt/s 0x8c077dc8'), (0x8c077d58, 'add #0x4,r15'),
            (0x8c077d5a, 'tst r8,r8'), (0x8c077d5c, 'bf 0x8c077dc0'),
            (0x8c077dc0, 'mov r4,r0'), (0x8c077dc2, 'cmp/eq #0x2,r0'),
            (0x8c077dc4, 'bf 0x8c077dc8'), (0x8c077dc6, 'mov r4,r8')):
        exact(parent, pc, instruction)
    assert all(result != 2 for result in (0, 12))

    files = [Path(__file__), original, parent_path, child_path,
             P / 'source/sa_movement_special_tail.c',
             P / 'source/sa_movement_contact_candidates.c',
             P / 'analysis/movement_special_tail_translation.json']
    report = dict(
        status='two_source_bound_infeasible_ordinary_entry_edges',
        proven_infeasible_edges=[['0x8c077d2a', '0x8c077d2c'],
                                 ['0x8c077dc4', '0x8c077dc6']],
        corresponding_unreachable_rows=['0x8c077d2c', '0x8c077d2e', '0x8c077dc6'],
        original_instruction_rows_byte_checked=len(parent) + len(child),
        duplicate_comparison_argument='Identical FCMP operands and mode; no intervening operand mutation/call; first BF fallthrough requires the same predicate that makes the second BT taken.',
        child_normal_return_values=[0, 12],
        child_return_argument='The sole RTS epilogue is reached through the early BRA delay-slot R0=0 or the straight suffix following R0=12. No other direct control-flow entry or later R0 assignment/call exists in that suffix.',
        parent_result_argument='The parent preserves the normal child result in R4 until comparison with2, so that comparison cannot succeed.',
        runtime_code_changed=False, original_rows_removed=0, new_original_routine_credit=0,
        scope='Normal original-entry synchronous software execution in the supported numeric domain with unchanged instructions/return control. No asynchronous register mutation between rows. Fault/stops are not normal returns. Does not qualify nonlocal resumes, arbitrary return-stack corruption, physical hardware or full gameplay. Other gaps remain open.',
        sha256={str(f): digest(f) for f in files}, overall_completion_percent=10)
    target = P / 'analysis/movement_special_tail_gate_reachability.json'
    assert not target.exists(), 'Preserve an existing proof; author a new revision instead.'
    target.write_text(json.dumps(report, indent=2) + '\n')
    print('SPECIAL TAIL REACHABILITY PASS2edges/3rows;all original code retained', flush=True)


if __name__ == '__main__':
    main()
