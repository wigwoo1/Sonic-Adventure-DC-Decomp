import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/magic_retail_native_initializer_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==218 and evidence['original_files']==109
names={'sa_sound_stream_format_initialize','sa_sound_magic_state_initialize','sa_sound_magic_header_metadata','sa_sound_magic_extension_word','sa_sound_magic_stream_parameters','sa_file_audio_configuration_forward','sa_audio_filter_coefficients','sa_math_result_error_wrapper','sa_math_range_reduce','sa_math_trig_octant_dispatch','sa_math_cosine_polynomial','sa_math_sine_polynomial'};found=set()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['magic_retail_native_initializer']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Original-header fully native initializer:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_magic_retail_native_initializer.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_magic_retail_native_initializer.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_magic_retail_native_initializer.py')\nif ($LASTEXITCODE -ne 0) { throw 'Original-header native initializer verification failed' }\n")
