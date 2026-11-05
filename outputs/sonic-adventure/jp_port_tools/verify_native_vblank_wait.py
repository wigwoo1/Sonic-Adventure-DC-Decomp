"""Reuse the frozen abstract wait/rational-clock groups on the actual DLL.
The later legacy frame-diagnostic fixture groups are not selected or claimed.
"""
import hashlib,inspect,json,sys
from pathlib import Path
import verify_native_frame_wait as frozen
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    baseline=sha(frozen.__file__);source=inspect.getsource(frozen.main);marker='    def trace():return [event(i) for i in range(events())]';assert source.count(marker)==1
    source=source[:source.index(marker)]+"""    gpu.close(gpu.handle);check(shutdown()==0,'wait shutdown');clock.shutdown()
    report=dict(status='frozen_rational_wait_predicates_rebase_and_fault_groups_verified',checks=checks,groups=groups,verifier_sha256=wrapper_sha,baseline_verifier_sha256=baseline_sha,derived_Python_harness_sha256=derived_sha,build_report_sha256=sha(a.build/'build_verification.json'),actual_wait_DLL_sha256=sha(Path(b['DLL'])),only_abstract_wait_groups_selected=True,later_legacy_frame_fixture_groups_executed=False,native_functions_replaced=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(report,indent=2)+'\\n');print(report['status'],checks,'checks',flush=True)
"""
    frozen.wrapper_sha=sha(__file__);frozen.baseline_sha=baseline;frozen.derived_sha=hashlib.sha256(source.encode()).hexdigest();exec(compile(source,frozen.__file__,'exec'),frozen.__dict__);frozen.main()
if __name__=='__main__':main()
