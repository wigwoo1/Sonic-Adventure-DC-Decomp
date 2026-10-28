"""Regression checks that derivations reject changed evidence and overstated scope."""
import copy,hashlib,json
from pathlib import Path
import prove_collision_wrapper_scalar_returns as s
import check_collision_wrapper_paired_aliases as p

def main():
 binary,rows,files=s.tables();r=s.derive(binary,rows);a=p.frame_aliases(rows);tests=[]
 def rejects(label,operation):
  try:operation()
  except (AssertionError,s.UnclosedDomainError):tests.append(label)
  else:raise AssertionError('Invalid certificate accepted:'+label)
 rejects('arbitrary paired domain cannot be claimed',lambda:s.derive(binary,rows,domain='all_FPSCR_modes'))
 changed=dict(rows);changed['8c10f04a']='fmul fr5,fr15';rejects('changed original transform rejected',lambda:s.derive(binary,changed))
 changed=dict(rows);changed['8c10e7c6']='fmov.s fr4,@r5';rejects('changed paired store base rejected',lambda:p.frame_aliases(changed))
 bad=bytearray(binary);address=int.from_bytes(binary[0x10fbb4-0x10000:0x10fbb8-0x10000],'little')-0x8c010000;bad[address:address+4]=(0x7f800000).to_bytes(4,'little');rejects('infinite polynomial coefficient rejected',lambda:s.derive(bad,rows))
 bad=bytearray(binary);address=int.from_bytes(binary[0x10f124-0x10000:0x10f128-0x10000],'little')-0x8c010000;bad[address:address+4]=(0x7f800000).to_bytes(4,'little');rejects('infinite return constant rejected',lambda:s.derive(bad,rows))
 assert not r['whole_wrappers_qualified'] and not a['whole_paired_domain_closed'];tests.append('partial results preserve qualification boundary')
 assert r['infinity_result_possible'] is False and a['scale_can_change_caller_R14'];tests.append('scalar and paired conclusions remain distinct')
 files+=[Path(__file__),Path(s.__file__),Path(p.__file__)]
 report=dict(checks_passed=len(tests),checks=tests,negative_controls_use_memory_copies_only=True,retail_or_native_sources_modified=False,new_routine_credit=0,overall_completion_percent=10,sha256={str(x):s.digest(x) for x in files})
 (s.P/'analysis/collision_wrapper_derivation_tests.json').write_text(json.dumps(report,indent=2)+'\n');print('DERIVATION TESTS PASS',len(tests))
if __name__=='__main__':main()
