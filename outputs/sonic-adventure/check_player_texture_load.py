"""Strict279-source and complete original texture-loader provenance audit."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_load_compile_checks.json';assert not target.exists()
 build=P/'build_player_texture_load.ps1';sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())];assert len(sources)==len(set(sources))==279
 flags=['-fsyntax-only','-Wall','-Wextra','-Werror'];subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
 mp=P/'analysis/player_texture_load_translation.json';cp=P/'analysis/player_texture_load_connection.json';t=json.loads(mp.read_text());c=json.loads(cp.read_text());assert len(t['routines'])==39 and c['compiled_source_count']==279
 for report in (t,c):
  for path,value in report['sha256'].items():assert sha(path)==value,path
 original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes();assert sha(original)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 count=canonical=0;calls=set()
 for b in t['routines']:
  canonical+=b['instruction_rows'] if b['canonical_registered_reuse'] else 0
  for row in csv.DictReader(Path(b['listing']).open(),delimiter='\t'):
   a=int(row['address'],16);value=bytes.fromhex(row['bytes']);assert len(value)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==value;count+=1
   if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')):calls.add(hex(a+4))
 assert count==2715 and canonical==127 and len(t['canonical_reused_entries'])==7
 assert raw[0x6044e6-0x10000:0x6044ea-0x10000]==bytes.fromhex('fb8f0474')
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 ret=P/t['return_manifest'];returns=json.loads(ret.read_text());assert calls==set(returns['original_call_returns']) and calls<=set(returns['allowed_call_returns'])
 scripts=sorted(P.glob('*player_texture_load*.py'))
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 exports=[P/'ghidra'/n for n in ('export_player_child_particle_loader.ps1','export_player_texture_load_dependencies.ps1','export_player_texture_decode_family.ps1','export_player_texture_load_closure.ps1','export_player_texture_transfer_closure.ps1')]
 dll=R/'work/sonic-adventure/build/player_texture_load.dll';lib=dll.with_suffix('.lib');assert dll.read_bytes()[:2]==b'MZ' and lib.read_bytes()[:8]==b'!<arch>\n'
 files=[Path(__file__),mp,cp,build,original,ret,dll,lib,*scripts,*exports]
 report=dict(status='strict279_source_and2715_original_loader_rows_verified',source_count=279,compiler_exit_code=0,flags=flags,original_instruction_rows_byte_checked=count,new_unregistered_original_rows=count-canonical,canonical_reused_original_rows=canonical,canonical_reused_entries=t['canonical_reused_entries'],canonical_duplicate_credit=0,python_scripts_parsed=len(scripts),new_guest_wait_bounds=0,flat_return_membership=len(returns['allowed_call_returns']),new_original_routine_credit=0,overall_completion_percent=10,source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('STRICT279 PASS;2715original rows/127canonical rows;zero new qualification credit',flush=True)
if __name__=='__main__':main()
