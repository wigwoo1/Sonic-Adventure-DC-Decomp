"""Qualify four complete original release bodies; no renderer-creation claim."""
import ast, hashlib, json
from pathlib import Path
from audit_movie_linked_routines import inventory

P = Path(__file__).resolve().parent
R = P.parents[1]


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    target = P / 'analysis/renderer_context_owner_qualification.json'
    assert not target.exists()
    paths = [P / 'analysis' / (name + '.json') for name in (
        'renderer_context_owner_translation', 'renderer_context_owner_connection',
        'renderer_context_owner_compile_checks', 'renderer_context_owner_verification',
        'renderer_context_owner_texture_regression_verification')]
    translation, connection, compiler, behavior, regression = [json.loads(p.read_text()) for p in paths]
    files = [Path(__file__), P / 'audit_movie_linked_routines.py', *paths]
    for report in (translation, connection, compiler, behavior, regression):
        for path, value in report['sha256'].items():
            assert sha(path) == value, path
            files.append(Path(path))
    for path, value in compiler['source_sha256'].items():
        assert sha(path) == value, path
    assert compiler['source_count'] == connection['compiled_source_count'] == 284
    assert compiler['compiler_exit_code'] == compiler['new_guest_wait_bounds'] == 0
    assert compiler['original_bodies_byte_checked'] == 4
    assert compiler['original_instruction_rows_byte_checked'] == translation['instruction_rows'] == 102
    assert translation['classification_registry_baseline_entries'] == 1648
    assert not translation['canonical_reused_entries']
    assert behavior['original_native_comparisons'] == 124
    assert behavior['checks'] == 760 and behavior['independent_exact_oracles'] == 324
    coverage = behavior['coverage_by_routine']
    assert len(coverage) == 4 and sum(x['executed_rows'] for x in coverage) == 102
    assert sum(x['executed_outcomes'] for x in coverage) == 14
    assert all(not x['missing_rows'] and not x['unresolved_outcomes'] for x in coverage)
    assert behavior['incoming_root_argument_is_ignored'] and behavior['root88fcd4_is_not_written_by_this_family']
    assert regression['native_DLL_is_additive_extension'] and regression['CPU_executor_unchanged']
    assert regression['original_native_comparisons'] == 20
    assert regression['checks'] == 472 and regression['independent_exact_oracles'] == 429
    assert regression['valid_texture_descriptors'] == 138
    assert len({x['pointer'] for x in regression['descriptors']}) == 136
    assert all(x['error'] == 0 and x['references'] > 0 for x in regression['descriptors'])
    assert not regression['retail_pixel_trace_values_published']
    for phase in regression['phases']:
        if phase['label'] == 'actual stored visible particle draw with loaded textures':
            assert len(phase['vertex_events']) == 50
            assert {x[0] for x in phase['vertex_events']} <= {1, 2, 3, 5}
            assert len(phase['sprite_command_buffers']) == 1
            assert phase['sprite_command_buffers'][0]['bytes'] == 160
        else:
            assert not phase['vertex_events']
    for report in (behavior, regression):
        assert not report['private_execution_frames_supplied'] and not report['native_wait_guards_changed']
    assert not regression['physical_GPU_pixels_qualified'] and not regression['full_game_qualified']
    limits = [
        'Four complete original bodies are software-qualified in declared domains:6052e0 ownership wrapper,6567f2 stored-pair release,663920 fallback pair release,and665660 VRAM address inverse map. All102 original rows and14 conditional outcomes execute across both FPU banks and nearest/zero rounding.',
        '124 fresh original/native comparisons and760 checks include324 independent exact oracles. Actual constructors663140/602128 and allocators6634a0/6021d6 produce the handles,metadata lists and owned heap blocks. No allocator/callee result or private execution frame is supplied. Complete RAM,CPU,FPU,hidden graphics state and ordered boundaries match.',
        'Independent oracles cover24-bit address-bit mapping at14 boundaries,primary/fallback release,stale-handle error7,successful20-byte bookkeeping reset,error preservation,4MiB bank coalescing,and caller-owned versus externally retained system-heap blocks with the original alias mask. Caller ownership flags/bookkeeping and system-heap arena remain declared component inputs;this is not all allocation sizes or malformed metadata.',
        'Correction to prior renderer-context research:6052e0 calls cleanup. Its incoming88fcd4 argument is ignored by6567f2,which overwrites argument registers from8a33ec/8a33f0 and invokes release663920.602288 frees an owned system-heap allocation. None of these bodies creates the polygon context. Do not compose this family before texture loading as a startup producer.',
        'An additive284-source research DLL/import library compile. All284 C sources pass Wall/Wextra/Werror syntax checks. The full original138-entry texture/player/particle regression on this DLL passes20 comparisons,472 checks and429 independent exact oracles;all8MiB VRAM persist and an actual160-byte RAM quad command buffer is retained. Raw retail pixel traces and game assets remain excluded.',
        'This is software component qualification and RAM command staging,not a displayed frame or playable port. Full renderer context creation,frame submission/consumption,scene/scheduler coverage and physical GPU behavior remain unfinished. Original source/proof/build history is preserved.',
        'Whole-project estimate remains approximately10% by weighted milestones,not measured recovered-code coverage. Goal active;continue Sol High for original604702 startup/callback tracing. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR/settings remain planned. Texture Off disables sampling and uses vertex/material colours.'
    ]
    proposed = [dict(module='1ST_READ.BIN', name=b['name'], address=b['address'],
        end_exclusive=b['end_exclusive'], source=translation['source'],
        retail_span_sha256=b['retail_span_sha256'],
        verification='analysis/renderer_context_owner_qualification.json',
        status='Complete original renderer-release helper software-qualified in declared domains;all102 group rows/14 outcomes execute. Cleanup only;renderer context creation and displayed frames remain unqualified.',
        limitations=limits) for b in translation['routines']]
    registry = json.loads((P / 'analysis/recovered_routines.json').read_text())['routines']
    names = {x['name'] for x in proposed}
    prior = [x for x in registry if x['name'] not in names]
    assert len(prior) == 1648
    old_pairs = {(x.get('module', '1ST_READ.BIN'), x['address']) for x in prior}
    assert len(old_pairs) == 1645
    assert all((x['module'], x['address']) not in old_pairs for x in proposed)
    linked = inventory(R / 'work/sonic-adventure/build/renderer_context_owner.dll', prior + proposed)
    assert linked['registry_entries'] == 1652
    assert linked['registry_unique_module_address_pairs'] == 1649
    assert linked['confirmed_linked_unique_routines_by_name'] == 696
    scripts = sorted(P.glob('*renderer_context_owner*.py'))
    for path in scripts:
        ast.parse(path.read_text(encoding='utf-8-sig'), filename=str(path))
    files += scripts
    result = dict(status='four_complete_original_renderer_release_helpers_software_qualified',
        new_original_routine_credit=4, result_registry_entries=1652,
        result_unique_module_address_pairs=1649, linked_routine_inventory=linked,
        original_native_comparisons=124, checks=760, independent_exact_oracles=324,
        complete_original_instruction_rows=102, complete_conditional_outcomes=14,
        coverage_by_routine=coverage, compiled_source_count=284, fresh_native_compile_performed=True,
        added_build_files=2, regression_comparisons=20, regression_checks=472,
        regression_independent_exact_oracles=429, valid_texture_entries=138,
        unique_texture_descriptors=136, ram_command_bytes=160,
        uploaded_texture_vram_sha256=regression['uploaded_texture_vram_sha256'],
        root88fcd4_producer_qualified=False, physical_GPU_pixels_qualified=False,
        full_game_qualified=False, retail_pixel_trace_values_published=False,
        new_routine_records=proposed, limitations=limits,
        next_task='Trace original604702 startup record/callbacks and the genuine indirect producer of88fcd4;6052e0/6567f2 are cleanup. Preserve real texture/player command pipeline and do not invent a buffer pointer or private frame.',
        overall_completion_percent=10, estimate_method='weighted whole-project milestones;not measured recovered-code coverage',
        goal_paused=False, recommended_model_next_task='Sol High',
        sha256={str(p): sha(p) for p in files})
    target.write_text(json.dumps(result, indent=2) + '\n')
    print('QUALIFIED4 renderer release helpers;1652 entries/1649 unique pairs/696 exact-name links', flush=True)


if __name__ == '__main__':
    main()
