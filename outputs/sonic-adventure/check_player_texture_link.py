"""Strict283-source compilation and original texture-linker family provenance."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_link_compile_checks.json';assert not target.exists()
 build=P/'build_player_texture_link.ps1';sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())];assert len(sources)==len(set(sources))==283
 flags=['-fsyntax-only','-Wall','-Wextra','-Werror'];subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
 mp=P/'analysis/player_texture_link_translation.json';cp=P/'analysis/player_texture_link_connection.json';t=json.loads(mp.read_text());c=json.loads(cp.read_text());assert len(t['routines'])==22 and c['compiled_source_count']==283
 # Preserve the already-pinned generation report. Its inherited scalar label
 # says1642,but the generator actually selects/asserts1645 registry rows.
 # Independently recompute every classification from the actual baseline.
 assert t['classification_registry_baseline_entries']==1642
 generator=(P/'recover_player_texture_link.py').read_text();assert "['routines'][:1645];assert len(registry)==1645" in generator
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1645];assert len(registry)==1645
 registered={x['address'] for x in registry if x.get('module','1ST_READ.BIN')=='1ST_READ.BIN'}
 assert all(b['canonical_registered_reuse']==(b['address'] in registered) for b in t['routines'])
 assert set(t['canonical_reused_entries'])=={b['address'] for b in t['routines'] if b['address'] in registered}
 files=[Path(__file__),build,mp,cp,*sources]
 for report in (t,c):
  for path,value in report['sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes();assert sha(original)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 count=canonical=0;calls=set()
 for b in t['routines']:
  canonical+=b['instruction_rows'] if b['canonical_registered_reuse'] else 0
  for row in csv.DictReader(Path(b['listing']).open(),delimiter='\t'):
   a=int(row['address'],16);value=bytes.fromhex(row['bytes']);assert len(value)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==value;count+=1
   if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')):calls.add(hex(a+4))
 assert count==2771 and canonical==176 and len(t['canonical_reused_entries'])==7
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 ret=P/t['return_manifest'];returns=json.loads(ret.read_text());assert calls==set(returns['original_call_returns']) and calls<=set(returns['allowed_call_returns'])
 scripts=sorted(P.glob('*player_texture_link*.py'))
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 dll=R/'work/sonic-adventure/build/player_texture_link.dll';lib=dll.with_suffix('.lib');assert dll.read_bytes()[:2]==b'MZ' and lib.read_bytes()[:8]==b'!<arch>\n';files += [original,ret,dll,lib,*scripts]
 report=dict(status='strict283_source_and2771_original_texture_linker_rows_verified',source_count=283,compiler_exit_code=0,flags=flags,actual_classification_baseline_entries=1645,preserved_generation_report_stale_scalar_label=1642,classification_erratum='Inherited report label1642 is stale;generator selects/asserts1645. Every body classification independently recomputed against actual1645-row baseline and matches. Source and pinned prior report bytes retained.',original_bodies_byte_checked=22,original_instruction_rows_byte_checked=count,new_unregistered_original_rows=count-canonical,canonical_reused_original_rows=canonical,canonical_reused_entries=t['canonical_reused_entries'],canonical_duplicate_credit=0,python_scripts_parsed=len(scripts),new_guest_wait_bounds=0,flat_return_membership=len(returns['allowed_call_returns']),new_original_routine_credit=0,overall_completion_percent=10,source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('STRICT283PASS;22bodies/2771rows/176canonical rows;zero qualification credit',flush=True)
if __name__=='__main__':main()
