"""Ensure the strengthened boundary test rejects the preserved incorrect DLL."""
import hashlib,json
from pathlib import Path
import verify_startup_callback_flag_reset_8c107aaa as test
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
history=PROJECT/'analysis/history/startup_callback_flag_reset_v1/PRESERVED_INPUTS.json'
preserved=json.loads(history.read_text())
for item in preserved['files']:
 assert hashlib.sha256((ROOT/item['preserved']).read_bytes()).hexdigest()==item['sha256']
old=ROOT/'work/sonic-adventure/build/history/startup_callback_flag_reset_8c107aaa_v1.dll'
loader=test.ctypes.CDLL
try:
 test.ctypes.CDLL=lambda unused:loader(str(old))
 try:test.main()
 except AssertionError as error:
  assert 'native callback entry' in str(error),str(error)
  rejected=str(error)
 else:raise AssertionError('The old DLL was accepted by the corrected boundary test')
finally:test.ctypes.CDLL=loader
test.main()
proof=PROJECT/'analysis/startup_callback_flag_reset_8c107aaa_verification.json'
report=dict(superseded_dll_rejected=True,observed_failure=rejected,original_callback_pr='0x8c107aba',superseded_callback_pr='0x8c107ab8',preservation_manifest=history.relative_to(PROJECT).as_posix(),preservation_manifest_sha256=hashlib.sha256(history.read_bytes()).hexdigest(),corrected_verification=proof.relative_to(PROJECT).as_posix(),corrected_verification_sha256=hashlib.sha256(proof.read_bytes()).hexdigest(),scope='The same corrected callback-entry test rejects the original preserved DLL and passes the corrected current build. Five original source/build/test/proof artifacts retain their recorded bytes.')
(PROJECT/'analysis/startup_callback_flag_reset_regression.json').write_text(json.dumps(report,indent=2)+'\n')
print('Original defective DLL rejected; corrected 48-case suite passed; five preserved hashes match')
