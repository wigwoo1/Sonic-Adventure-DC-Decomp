"""Strict284-source build/provenance check for four original release bodies."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/renderer_context_owner_compile_checks.json';assert not target.exists()
 build=P/'build_renderer_context_owner.ps1';sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())];assert len(sources)==len(set(sources))==284
 flags=['-fsyntax-only','-Wall','-Wextra','-Werror'];subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
 mp=P/'analysis/renderer_context_owner_translation.json';cp=P/'analysis/renderer_context_owner_connection.json';t=json.loads(mp.read_text());c=json.loads(cp.read_text());assert len(t['routines'])==4 and c['compiled_source_count']==284 and t['classification_registry_baseline_entries']==1648
 files=[Path(__file__),build,mp,cp,*sources]
 for report in (t,c):
  for path,value in report['sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 raw_path=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();assert sha(raw_path)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 count=0;calls=set()
 for b in t['routines']:
  assert not b['canonical_registered_reuse']
  for row in csv.DictReader(Path(b['listing']).open(),delimiter='\t'):
   a=int(row['address'],16);value=bytes.fromhex(row['bytes']);assert len(value)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==value;count+=1
   if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')):calls.add(hex(a+4))
 assert count==102 and not t['canonical_reused_entries'] and 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 ret=P/t['return_manifest'];returns=json.loads(ret.read_text());assert calls==set(returns['original_call_returns']) and calls<=set(returns['allowed_call_returns'])
 scripts=sorted(P.glob('*renderer_context_owner*.py'))
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 dll=R/'work/sonic-adventure/build/renderer_context_owner.dll';lib=dll.with_suffix('.lib');assert dll.read_bytes()[:2]==b'MZ' and lib.read_bytes()[:8]==b'!<arch>\n';files += [raw_path,ret,dll,lib,*scripts]
 result=dict(status='strict284_sources_and102_original_renderer_release_rows_verified',source_count=284,compiler_exit_code=0,flags=flags,original_bodies_byte_checked=4,original_instruction_rows_byte_checked=102,canonical_duplicate_credit=0,classification_registry_baseline_entries=1648,new_guest_wait_bounds=0,added_build_files=2,new_original_routine_credit=0,python_scripts_parsed=len(scripts),overall_completion_percent=10,source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('STRICT284 PASS;4bodies/102original rows;no newwaitbounds',flush=True)
if __name__=='__main__':main()
