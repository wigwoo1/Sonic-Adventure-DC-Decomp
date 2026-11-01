"""Strict source consistency and syntax checks for particle qualification."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_child_particles_compile_checks.json';assert not target.exists()
 prior=P/'analysis/player_collision_attach_compile_checks.json';proof=json.loads(prior.read_text())
 for path,value in proof['source_sha256'].items():assert sha(path)==value,path
 build=P/'build_player_collision_attach.ps1';sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())];assert len(sources)==len(set(sources))==278
 flags=['-fsyntax-only','-Wall','-Wextra','-Werror'];subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
 scripts=sorted(P.glob('*player_child_particle*.py'))
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 mp=P/'analysis/player_child_update_translation.json';t=json.loads(mp.read_text());raw=(R/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();count=0;selected=[]
 assert hashlib.sha256(raw).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 for b in t['routines']:
  if b['address'] not in ('0x8c0b37f2','0x8c0b3f2c'):continue
  selected.append(b)
  for row in csv.DictReader(Path(b['listing']).open(),delimiter='\t'):
   a=int(row['address'],16);value=bytes.fromhex(row['bytes']);assert len(value)==2 and raw[a-0x8c010000:a-0x8c010000+2]==value;count+=1
 assert len(selected)==2 and count==347
 dll=R/'work/sonic-adventure/build/player_collision_attach.dll';assert sha(dll)==proof['sha256'][str(dll)]
 files=[Path(__file__),prior,build,mp,dll,*scripts]
 report=dict(status='strict278_source_consistency_and347_original_particle_row_checks_passed',source_count=278,compiler_exit_code=0,flags=flags,original_instruction_rows_byte_checked=count,python_scripts_parsed=len(scripts),fresh_native_link_performed=False,existing_278_source_build_preserved=True,new_guest_wait_bounds=0,new_original_routine_credit=0,overall_completion_percent=10,source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('STRICT278 PASS;347particle rows checked;existingbuild/source preserved',flush=True)
if __name__=='__main__':main()
