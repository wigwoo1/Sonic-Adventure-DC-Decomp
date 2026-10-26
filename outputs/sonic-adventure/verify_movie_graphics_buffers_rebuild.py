"""Rebuild privately and compare the complete PE image except its COFF timestamp."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def normalized(data):
 data=bytearray(data);assert data[:2]==b'MZ';header=int.from_bytes(data[0x3c:0x40],'little');assert data[header:header+4]==b'PE\0\0';timestamp=header+8;data[timestamp:timestamp+4]=bytes(4);return bytes(data),timestamp
def main():
 script=P/'build_movie_graphics_init.ps1';destination=ROOT/'work/sonic-adventure/graphics-buffers-build-check';destination.mkdir(parents=True,exist_ok=True)
 original=ROOT/'work/sonic-adventure/build/movie_graphics_init.dll';before=digest(original)
 subprocess.run(['powershell.exe','-NoProfile','-File',str(script),'-Workspace',str(destination)],check=True)
 rebuilt=destination/'work/sonic-adventure/build/movie_graphics_init.dll';a,ta=normalized(original.read_bytes());b,tb=normalized(rebuilt.read_bytes());assert ta==tb and a==b,'Fresh DLL differs outside its COFF timestamp;inspect before qualifying'
 assert digest(original)==before
 files=[Path(__file__),script,original,rebuilt,P/'analysis/movie_graphics_init_connection.json']
 report=dict(compiled_source_count=201,fresh_build_and_link_passed=True,published_dll_preserved=True,published_dll_sha256=before,rebuilt_dll_sha256=digest(rebuilt),pe_bytes_compared=len(a),normalized_complete_pe_sha256=hashlib.sha256(a).hexdigest(),only_ignored_metadata='COFF TimeDateStamp four bytes',timestamp_file_offset=ta,all_other_pe_bytes_equal=True,sha256={str(p):digest(p) for p in files},scope='Original 201-source bounded research build runs in a separate workspace output directory. Complete existing/rebuilt DLL bytes compare after zeroing only the four-byte COFF timestamp;machine code,data,exports,unwind/relocation tables and all remaining headers match. The published DLL/source/proof/build bytes are untouched. This does not rerun older runtime/startup tests or establish a playable game.')
 (P/'analysis/movie_graphics_buffers_rebuild_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Fresh201-source build/link passed;allPEbytes except timestamp match published DLL',flush=True)
if __name__=='__main__':main()
