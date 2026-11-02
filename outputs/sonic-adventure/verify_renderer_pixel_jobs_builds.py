"""Verify isolated clean native rebuilds preserve every tested PE section payload."""
import hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(raw):return hashlib.sha256(raw).hexdigest()
def sections(path):
 raw=path.read_bytes();assert raw[:2]==b'MZ'
 pe=struct.unpack_from('<I',raw,60)[0];assert raw[pe:pe+4]==b'PE\0\0'
 count=struct.unpack_from('<H',raw,pe+6)[0];optional=struct.unpack_from('<H',raw,pe+20)[0];table=pe+24+optional;out={}
 for i in range(count):
  at=table+i*40;name=raw[at:at+8].rstrip(b'\0').decode();size,start=struct.unpack_from('<II',raw,at+16)
  assert name not in out and start+size<=len(raw)
  out[name]=dict(bytes=size,sha256=sha(raw[start:start+size]))
 return out
def main():
 target=P/'analysis/renderer_pixel_jobs_build_verification.json';assert not target.exists();rows=[];files=[Path(__file__)]
 for name,subdir,recipe in (('renderer_render_completion.dll','pixel-scene-native-rebuild','build_renderer_render_completion.ps1'),('pvr_pixels.dll','pixel-scene-pixels-rebuild','build_pvr_pixels.ps1'),('pixel_jobs.dll','pixel-scene-jobs-rebuild','build_pixel_jobs.ps1')):
  canonical=W/'build'/name;fresh=W/subdir/'work/sonic-adventure/build'/name;a=sections(canonical);b=sections(fresh);assert a==b,name
  rows.append(dict(name=name,canonical_sha256=sha(canonical.read_bytes()),rebuilt_sha256=sha(fresh.read_bytes()),all_section_payloads_equal=True,sections=a))
  files.extend((P/recipe,canonical,fresh));print(name,'all clean-rebuild section payloads match',flush=True)
 result=dict(status='three_clean_native_rebuilds_match_tested_PE_section_payloads',compiled_sources_in_game_library=291,components=3,PE_metadata_byte_identity_claimed=False,rows=rows,sha256={str(f):sha(f.read_bytes()) for f in files},overall_completion_percent=10)
 target.write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
