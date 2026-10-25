"""Qualify full-state renderer-setter integration, preserving canonical credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_render_setup_translation','movie_render_setup_connection','movie_render_setup_verification','movie_render_setup_prefix_verification','movie_timing_constructors_qualification','movie_render_setup_frontier')]
 translation,connection,direct,prefix,prior,frontier=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for proof in (translation,connection,direct,prefix,prior,frontier):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert direct['dll_sha256']==prefix['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_render_setup.dll')
 assert direct['cases_passed']==170 and direct['status_counts']=={'0':138,'-4':24,'-3':8} and direct['independent_checks']==504
 assert len(direct['coverage_by_routine'])==6
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in direct['coverage_by_routine'])
 assert sum(r['static_rows'] for r in direct['coverage_by_routine'])==154
 assert sum(r['conditional_edges_covered'] for r in direct['coverage_by_routine'])==20
 assert prefix['cases_passed']==6 and prefix['independent_checks']==120 and prefix['status_counts']=={'-2':4,'-3':2}
 assert prefix['next_dependencies']==['0x8c10d4bc','0x8c6021d2']
 raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 def literal(a):return int.from_bytes(raw[a-0x8c010000:a-0x8c010000+4],'little')
 initialized=0
 for case in prefix['cases']:
  boundaries=case['boundaries']
  if case['input']['initialize_context']:
   assert case['stop_pc']=='0x8c6021d2';initialized+=1
   for address,pointer in ((0x8c64133e,0x8c604b88),(0x8c64162a,0x8c604b90),(0x8c64167e,0x8c604b98)):
    actual=[b for b in boundaries if b['target']==hex(address)];assert len(actual)==1 and actual[0]['arguments'][0]==literal(pointer)
   assert sum(b['target']=='0x8c6412c2' for b in boundaries)>=3
  else:assert case['stop_pc']=='0x8c10d4bc'
 assert initialized==4
 files.append(Path(__file__))
 report=dict(routines=translation['routines'],reused_connected_bodies=['0x8c6412c2','0x8c6411b0'],compiled_source_count=159,
  newly_registered_entries=0,newly_connected_existing_bodies=4,integrated_original_bodies=6,integrated_instruction_rows=154,executed_conditional_outcomes=20,
  direct_comparisons=170,independent_direct_checks=504,direct_normal_returns=138,direct_privilege_faults=24,direct_memory_faults=8,
  startup_profiles=6,startup_independent_checks=120,initialized_profiles_advanced=4,boot_rows_covered=prefix['boot_rows_covered'],boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,
  next_dependency='0x8c6021d2',next_tail_target='0x8c6021d6',dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Four existing original bodies gain complete-state native integration: decrement/store and three callback wrappers. The original installer/default reset stay connected unchanged. Actual original callback pointers are installed during four declared initialized startup profiles, advancing to the allocation thunk. No duplicate recovery credit.',
  limitations=['Direct callback/argument/driver/index/SR/stack fixtures are explicit inputs; setters store callbacks and do not establish callback execution.',
              'Status -4 in the 24 direct cases means a modeled CPU privilege fault, not input exhaustion. The eight -3 cases are memory/stack faults; no successful return is fabricated.',
              'Earlier runtime tables, driver/context staging and software disc/device policies remain declared fixtures. This is not natural reset-to-main boot or physical interrupt delivery.',
              'Original canonical source/proof/name fields and prior DLLs remain unchanged. Two next-boundary functions are exported only, without recovery credit.',
              'No pixels, playable game, actual FPS/resolutions/settings, full movie startup or completed platform port is established.'])
 (P/'analysis/movie_render_setup_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified renderer setter integration: 154 rows / 20 outcomes; four startup profiles advance to allocation')
if __name__=='__main__':main()
