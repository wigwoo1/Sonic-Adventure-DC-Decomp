"""Test byte-identical native helper spans without fake renderer services.
The complete renderer already compiles separately; only selected helpers link
here. This is not a whole-renderer runtime or graphics-backend qualification.
"""
import hashlib,json,re,subprocess
from pathlib import Path
import argparse
ap=argparse.ArgumentParser()
for name in ['workspace','upstream','sh4zam','compiler','renderer-build','output']:ap.add_argument('--'+name,type=Path,required=True)
a=ap.parse_args();R,U,Z,C,O,D=(getattr(a,n.replace('-','_')).resolve() for n in ['workspace','upstream','sh4zam','compiler','renderer-build','output'])
W=R/'work/sonic-adventure';P=R/'outputs/sonic-adventure'
assert D.is_relative_to(W) and not D.is_relative_to(W/'build') and not D.exists();D.mkdir(parents=True)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
build=json.loads((O/'build_verification.json').read_text());assert build['exit_code']==0
assert sha(O/'external_model_native.c')==build['generated_source_sha256']
raw=(O/'external_model_native.c').read_bytes();pieces=[];spans=[]
for kind,names in [('type',['screen','point','vertex','projection','projecting','strips','vertex_words']),
                   ('function',['next_vector','project_end','take_point','send_textured','next_index','next_uv'])]:
 for name in names:
  pattern=(rb'^struct '+name.encode()+rb'\s*\{.*?^\};\r?\n' if kind=='type' else
           rb'^(?:static inline|ALWAYS_INLINE)[^;{}]*?\b'+name.encode()+rb'\([^;{}]*?\)\r?\n\{.*?^\}\r?\n')
  matches=list(re.finditer(pattern,raw,re.M|re.S));assert len(matches)==1,name
  match=matches[0];pieces.append(match.group());spans.append(dict(kind=kind,name=name,byte_start=match.start(),byte_end=match.end(),sha256=hashlib.sha256(match.group()).hexdigest()))
constant=re.search(rb'^#define INV_255[^\r\n]*',raw,re.M);assert constant
unit=D/'external_helpers_native.c';unit.write_bytes(b'#include <machine.h>\n'+constant.group()+b'\n'+b'\n'.join(pieces))
probe=(P/'native/jp_renderer_helper_probe_v2.c').read_bytes()
source=D/'authored_probe.c';source.write_bytes(probe)
command=[str(C),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror',
 '-Wno-unused-parameter','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-Wno-unknown-attributes',
 '-include',str(P/'native/jp_host_types.h'),'-include',str(P/'native/jp_renderer_host_compat_v2.h'),
 '-DJP_NATIVE_RENDERER=1','-DJP_NATIVE_RENDERER_SOURCE="'+unit.as_posix()+'"','-I'+str(O/'include')]
command+=['-I'+str(U/f) for f in ['sdk/include','src','modules','events','gcc/src','.']]
command+=['-isystem',str(Z/'include'),'-shared','-fuse-ld=lld',str(source),'-o',str(D/'jp_renderer_helpers.dll')]
result=subprocess.run(command,capture_output=True);log=D/'build.txt';log.write_bytes(result.stdout+result.stderr)
report=dict(status='actual_adapted_renderer_helper_spans_build_only',command=command,exit_code=result.returncode,
 diagnostic_bytes=log.stat().st_size,log_sha256=sha(log),source_renderer_build_report_sha256=sha(O/'build_verification.json'),
 source_renderer_generated_sha256=sha(O/'external_model_native.c'),source_renderer_directory=str(O),exact_selected_spans=spans,
 selected_unit_sha256=sha(unit),authored_probe_sha256=sha(P/'native/jp_renderer_helper_probe_v2.c'),
 DLL_sha256=sha(D/'jp_renderer_helpers.dll') if result.returncode==0 else None,
 complete_renderer_runtime_linked=False,dummy_graphics_texture_or_presentation_functions=False,
 game_source_artifact_publishable=False,whole_renderer_runtime_verified=False,overall_completion_percent=10,goal_paused=False,playable=False)
(D/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print('Exact adapted helper spans build',result.returncode,'diagnostic bytes',log.stat().st_size)
if result.returncode:print(log.read_text()[:6000])
raise SystemExit(result.returncode)
