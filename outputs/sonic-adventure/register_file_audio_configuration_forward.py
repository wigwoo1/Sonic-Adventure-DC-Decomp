import hashlib, json
from pathlib import Path

root = Path(__file__).resolve().parent
name = 'sa_file_audio_configuration_forward'
report = 'analysis/file_audio_configuration_forward_verification.json'
evidence = json.loads((root/report).read_text())
assert evidence['cases_passed'] == 128
source = root/f'source/{name}.c'
text = source.read_text()
start = text.index('/* Static C recovery')
end = text.index('SA_STAGE_EXPORT', start)
source.write_text(text[:start] + '/* Retail file audio configuration wrapper 644be6..644bfe.\n * Tail-forwards extension, rate and file-handle fields +48/+50 to65adc0.\n * Lower calculation remains an external call. */\n' + text[end:])
binary = (root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path = root/'analysis/recovered_routines.json'
registry = json.loads(path.read_text())
if not any(item['name'] == name for item in registry['routines']):
    registry['routines'].append(dict(module='1ST_READ.BIN', name=name,
        address='0x8c644be6', end_exclusive='0x8c644bfe', source=f'source/{name}.c',
        retail_span_sha256=hashlib.sha256(binary[0x634be6:0x634bfe]).hexdigest(),
        verification=report, status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry, indent=2)+'\n')
path = root/'project_status.json'; status = json.loads(path.read_text())
status['verified_progress']['file_audio_configuration_forward'] = evidence
path.write_text(json.dumps(status, indent=2)+'\n')
path = root/'SOURCE_HANDOFF.txt'; text = path.read_text()
marker = 'File audio configuration wrapper644be6:'
if marker not in text:
    path.write_text(text+'\n'+marker+' build_file_audio_configuration_forward.ps1 and verify_file_audio_configuration_forward.py reproduce128 full retail comparisons. Native12-instruction tail wrapper forwards extension/rate and handle+48/+50, restores stack and propagates return. Independent guarded writes and call trace; lower65adc0 modeled. Full initializer integration, calculation and physical audio remain unverified. Overall project10%.\n')
path = root/'build_and_verify.ps1'; text = path.read_text()
if 'verify_file_audio_configuration_forward.py' not in text:
    path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_file_audio_configuration_forward.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_file_audio_configuration_forward.py')\nif ($LASTEXITCODE -ne 0) { throw 'File audio configuration forwarding verification failed' }\n")
