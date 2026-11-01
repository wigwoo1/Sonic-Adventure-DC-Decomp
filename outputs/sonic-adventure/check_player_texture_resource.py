"""Strict final-stage compilation and complete four-stage provenance audit."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
PREFIXES=('player_texture_load','player_texture_table_initialize','player_texture_heap_initialize','player_texture_driver_defaults')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_resource_compile_checks.json';assert not target.exists()
 files=[Path(__file__)];stages=[];rows=canonical=0;original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
 assert sha(original)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a';files.append(original)
 for prefix,count in zip(PREFIXES,range(279,283)):
  build=P/f'build_{prefix}.ps1';sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())];assert len(sources)==len(set(sources))==count
  mp=P/f'analysis/{prefix}_translation.json';cp=P/f'analysis/{prefix}_connection.json';t=json.loads(mp.read_text());c=json.loads(cp.read_text());assert c['compiled_source_count']==count
  for report in (t,c):
   for path,value in report['sha256'].items():assert sha(path)==value,path;files.append(Path(path))
  calls=set();stage_rows=0
  for b in t['routines']:
   canonical+=b['instruction_rows'] if b['canonical_registered_reuse'] else 0
   for row in csv.DictReader(Path(b['listing']).open(),delimiter='\t'):
    a=int(row['address'],16);v=bytes.fromhex(row['bytes']);assert len(v)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==v;stage_rows+=1
    if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')):calls.add(hex(a+4))
  ret=P/t['return_manifest'];returns=json.loads(ret.read_text());assert calls==set(returns['original_call_returns']) and calls<=set(returns['allowed_call_returns'])
  assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
  dll=R/f'work/sonic-adventure/build/{prefix}.dll';lib=dll.with_suffix('.lib');assert dll.read_bytes()[:2]==b'MZ' and lib.read_bytes()[:8]==b'!<arch>\n'
  files += [build,mp,cp,ret,dll,lib,*sources];rows+=stage_rows;stages.append(dict(prefix=prefix,source_count=count,routines=len(t['routines']),original_rows=stage_rows,dll_sha256=sha(dll),lib_sha256=sha(lib)))
 assert rows==2813 and canonical==127 and sum(x['routines'] for x in stages)==50
 flags=['-fsyntax-only','-Wall','-Wextra','-Werror'];subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
 scripts=sorted(p for p in P.glob('*.py') if 'player_texture_' in p.name)
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 files+=scripts
 report=dict(status='strict282_sources_and_all_four_stage_original_rows_verified',source_count=282,compiler_exit_code=0,flags=flags,stages=stages,original_bodies_byte_checked=50,original_instruction_rows_byte_checked=rows,canonical_reused_original_rows=canonical,canonical_duplicate_credit=0,unregistered_proposal_rows=rows-canonical,new_native_builds=4,added_build_files=8,new_guest_wait_bounds=0,python_scripts_parsed=len(scripts),new_original_routine_credit=0,overall_completion_percent=10,source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('STRICT282 PASS;50bodies/2813original rows;127canonical rows;four preserved native stages',flush=True)
if __name__=='__main__':main()
