"""Qualify complete contexts; preserve the genuine unresolved bootstrap frontier."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__),P/'source/sh4_exception_state.h'];reports={}
 for name in ('movie_interrupt_context_translation','movie_interrupt_context_connection','movie_interrupt_context_verification','movie_interrupt_context_lifecycle_verification','movie_interrupt_context_regression_verification','movie_interrupt_context_compatibility_verification','sh4_exception_state_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 t=reports['movie_interrupt_context_translation'];direct=reports['movie_interrupt_context_verification'];life=reports['movie_interrupt_context_lifecycle_verification'];reg=reports['movie_interrupt_context_regression_verification'];compat=reports['movie_interrupt_context_compatibility_verification'];backend=reports['sh4_exception_state_verification']
 assert t['instruction_rows']==474 and len(t['routines'])==5 and reports['movie_interrupt_context_connection']['compiled_source_count']==192
 assert direct['cases_passed']==107 and direct['independent_checks']==1561
 assert all(r['static_rows']==r['covered_rows'] and r['conditional_outcomes']==r['covered_outcomes'] and not r['missing_rows'] and not r['missing_outcomes'] for r in direct['coverage_by_routine'])
 assert life['lifecycles_passed']==life['authored_host_transfer_cases']==life['authored_irq_admission_cases']==16 and life['original_native_comparisons']==64
 assert reg['previous_dispatch_comparisons']==137 and reg['previous_handler_comparisons']==110 and reg['previous_cpu_comparisons']==240 and reg['retained_wait_comparisons']==2 and reg['total_comparisons']==489
 assert compat['legacy_abi_comparisons']==6
 assert backend['total_cases']==1092 and backend['newly_registered_entries']==0
 assert digest(ROOT/'work/sonic-adventure/build/sh4_exception_state.dll')==backend['dll_sha256']
 dll=digest(ROOT/'work/sonic-adventure/build/movie_interrupt_context.dll');assert all(r['dll_sha256']==dll for r in (direct,life,reg,compat))
 routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines']]
 complete=[r for r in routines if r['address'] in ('0x8c65e80c','0x8c65e958')];partial=[r for r in routines if r['address']=='0x8c65eaec'];fragments=[r for r in routines if r['kind']=='copied_vector_fragment']
 assert sum(r['instruction_rows'] for r in complete)==345 and sum(len(r['conditional_edges']) for r in complete)==8
 limitations=[
 'Two complete original contexts qualify345own instruction rows and8conditional outcomes in initialized software CPU/bus state. Both full FPU banks, FPUL/FPSCR, all visible/hidden integer banks, SR/PR/SPC/SSR/SGR, stack and saved specials are compared. Copied52-byte vector templates contain20instruction rows each; three VBR aliases are relocated fragments with zero additional routine credit.',
 'Original65eaec executes all89own rows and4outcomes but tail-jumps to unresolved bootstrap ac000010. This partial helper is not registered as complete. No instruction, RTE, guest result or checkpoint is inserted at its frontier.',
 'Sixteen lifecycles run original645de0 vector initialization, original event registration and channel start, real authored G2 bytes, explicit untimed IRQ admission, actual copied vector, context, ASIC, generic traversal, registered handler, SDK completion and original RTE. Entire interrupted CPU/FPU/banked/special state is restored. Registers, bytes and software pending input are carried explicitly between four guest ABI calls. Independent architecture admission and byte-copy support receive zero retail-code credit.',
 'IRQ admission is a declared host action at a safe instruction boundary, not physical timing or automatic runtime delivery. No uninterrupted early startup, continuous guest scheduling, ARM execution, audio or playable port is claimed. Default REQUEST still waits at65a6ec without explicit completion. Older full-copy/video-clear/earliest-caller evidence remains historical, not rerun.',
 'Whole game, PC/Android/3DS ports, stereoscopic3D,60/120/240FPS,8K,SUPIR,HD/texture-resolution/render-resolution/aspect-ratio settings remain unfinished. TextureOff must use vertex colors. Overall10% is a weighted milestone estimate, not measured recovered-code coverage.'
 ]
 report=dict(routines=complete,partial_helpers=partial,copied_fragments=fragments,newly_registered_entries=2,newly_qualified_rows=345,qualified_conditional_outcomes=8,executed_integration_rows=474,executed_conditional_outcomes=12,fragment_additional_credit=0,partial_helper_additional_credit=0,compiled_source_count=192,direct_comparisons=107,lifecycle_comparisons=64,regression_comparisons=489,legacy_abi_comparisons=6,total_comparisons=666,authored_host_transfer_cases=16,authored_irq_admission_cases=16,host_support_recovery_credit=0,independent_direct_checks=1561,independent_lifecycle_checks=life['independent_checks'],regression_checks=reg['independent_and_record_checks'],architecture_support_verification='analysis/sh4_exception_state_verification.json',retained_default_frontier='0x8c65a6ec',partial_bootstrap_frontier='0xac000010',dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Original transfer interrupt path now traverses copied vectors and full original context through ASIC/generic/handler/SDK and RTE; complete interrupted state restored. Explicit untimed architecture input, separate ABI calls and authored real data movement remain declared.',limitations=limitations)
 (P/'analysis/movie_interrupt_context_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified345context rows,8outcomes;666original/native comparisons;partial bootstrap retained')
if __name__=='__main__':main()
