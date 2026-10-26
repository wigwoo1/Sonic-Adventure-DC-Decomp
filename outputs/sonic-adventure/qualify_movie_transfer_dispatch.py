"""Qualify original dispatch bodies and real software G2 completion integration."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_transfer_dispatch_translation','movie_transfer_dispatch_connection','movie_transfer_dispatch_verification','movie_transfer_dispatch_lifecycle_verification','movie_transfer_dispatch_regression_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 t=reports['movie_transfer_dispatch_translation'];direct=reports['movie_transfer_dispatch_verification'];life=reports['movie_transfer_dispatch_lifecycle_verification'];reg=reports['movie_transfer_dispatch_regression_verification']
 assert t['instruction_rows']==141 and len(t['routines'])==4 and reports['movie_transfer_dispatch_connection']['compiled_source_count']==190
 assert direct['cases_passed']==137 and direct['independent_checks']==1079
 assert all(r['static_rows']==r['covered_rows'] and r['conditional_outcomes']==r['covered_outcomes'] and not r['missing_rows'] and not r['missing_outcomes'] for r in direct['coverage_by_routine'])
 assert sum(r['conditional_outcomes'] for r in direct['coverage_by_routine'])==22
 assert life['lifecycles_passed']==life['authored_host_transfer_cases']==24 and life['original_native_comparisons']==72 and life['independent_checks']==1008
 assert reg['previous_handler_comparisons']==110 and reg['previous_cpu_comparisons']==240 and reg['retained_wait_comparisons']==2 and reg['total_comparisons']==352
 dll=digest(ROOT/'work/sonic-adventure/build/movie_transfer_dispatch.dll');assert all(r['dll_sha256']==dll for r in (direct,life,reg))
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in ('0x8c645afa','0x8c645b1e','0x8c10ebe8','0x8c668f40','0x8c666e06'):
  row=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);canonical.append({k:row[k] for k in ('address','name','source','verification')});files.extend((P/row['source'],P/row['verification']))
 routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines']]
 new=[r for r in routines if r['address'] in ('0x8c645b68','0x8c645b8e')];reused=[r for r in routines if r not in new];assert sum(r['instruction_rows'] for r in new)==104
 limitations=['Own-body qualification uses immutable code/literals and initialized acyclic callback lists with the existing software CPU/bus domain. All141instructions and22outcomes execute. Generic and primary adapters reuse earlier canonical bodies with zero duplicate credit. Unsupported callbacks, privilege faults and invalid pointers retain reached effects and stops.',
 'Original ASIC code snapshots masked normal status, shifts it by12, and prioritizes it over external/error status. It scans seven normal bits or four error bits, retains ignored-high-bit and signed-event behavior, sets IMASK4, and invokes actual generic traversal. Primary/secondary wrappers set IMASK6. Physical exception entry/return and interrupt timing are outside these ABI bodies.',
 'Existing authored g2_transfer_host.c is native support, not recovered game code. Twenty-four cases move actual64-byte varying data or33376-byte original driver payload before supplying an explicit software pending bit. Full source and2MiB destination bytes, every32-byte bus transaction, event code and original register/start/ASIC machine states are checked independently. MMIO results and data are carried explicitly between separate calls, not an uninterrupted early caller or resumed guest checkpoint.',
 'The original registered handler and SDK callback set completion, with no flag/helper result supplied. No physical IRQ acceptance, automatic runtime pumping, exception frame, ARM execution or audio is claimed. Without explicit delivery default REQUEST still waits at65a6ec. Older full-copy/video-clear/earliest-caller proofs are historical, not rerun.',
 'Complete startup/gameplay, every port/demake,60/120/240FPS,8K,SUPIR/display/texture settings remain unfinished. TextureOff uses vertex colors. Whole-project10% is a weighted milestone estimate, not recovered-code coverage.']
 report=dict(routines=new,integration_adapters=reused,newly_registered_entries=2,newly_qualified_rows=104,reused_adapter_rows=37,qualified_integration_rows=141,executed_rows=141,qualified_conditional_outcomes=22,executed_conditional_outcomes=22,compiled_source_count=190,direct_comparisons=137,independent_direct_checks=1079,lifecycle_comparisons=72,independent_lifecycle_checks=1008,authored_host_transfer_cases=24,host_support_recovery_credit=0,regression_comparisons=352,regression_checks=reg['independent_and_record_checks'],total_comparisons=561,canonical_reuse=canonical,reused_additional_credit=0,retained_default_frontier='0x8c65a6ec',dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Original event/ASIC dispatch connected to original registration/start/handler/SDK completion with real existing authored G2 byte movement and explicit software completion input. Two new complete bodies plus two reused adapters; physical delivery remains separate.',limitations=limitations)
 (P/'analysis/movie_transfer_dispatch_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified dispatch:104new+37reused rows,22outcomes,561comparisons,24host copies')
if __name__=='__main__':main()
