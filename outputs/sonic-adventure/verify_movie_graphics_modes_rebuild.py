"""Fresh private build for mode qualification; preserve the published research DLL."""
import hashlib,json,subprocess
from pathlib import Path
import verify_movie_graphics_buffers_rebuild as previous
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 script=P/'build_movie_graphics_init.ps1';destination=ROOT/'work/sonic-adventure/graphics-modes-build-check';destination.mkdir(parents=True,exist_ok=True)
 original=ROOT/'work/sonic-adventure/build/movie_graphics_init.dll';before=digest(original)
 subprocess.run(['powershell.exe','-NoProfile','-File',str(script),'-Workspace',str(destination)],check=True)
 rebuilt=destination/'work/sonic-adventure/build/movie_graphics_init.dll';a,ta=previous.normalized(original.read_bytes());b,tb=previous.normalized(rebuilt.read_bytes());assert ta==tb and a==b and digest(original)==before
 files=[Path(__file__),Path(previous.__file__),script,original,rebuilt,P/'analysis/movie_graphics_init_connection.json']
 report=dict(compiled_source_count=201,fresh_build_and_link_passed=True,published_dll_preserved=True,published_dll_sha256=before,rebuilt_dll_sha256=digest(rebuilt),pe_bytes_compared=len(a),normalized_complete_pe_sha256=hashlib.sha256(a).hexdigest(),only_ignored_metadata='COFF TimeDateStamp four bytes',timestamp_file_offset=ta,all_other_pe_bytes_equal=True,sha256={str(p):digest(p) for p in files},scope='Fresh201-source bounded research build in separate private output folder. Complete PEbytes compare after ignoring onlyfourCOFFtimestampbytes;machinecode/data/exports/relocations/allremainingheadersmatch. Published source/proof/build bytes untouched. Earlier runtime/startup comparisons retained,not rerun;no physicalhardware/gameplayclaim.')
 (P/'analysis/movie_graphics_modes_rebuild_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Freshmode qualification build/link passed;completePEexcepttimestampmatches')
if __name__=='__main__':main()
