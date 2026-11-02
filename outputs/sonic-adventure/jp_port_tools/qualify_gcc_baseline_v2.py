"""Validate the completed pinned GCC manifest and SH-4 outputs, not gameplay."""
import argparse,hashlib,json,os,struct,subprocess,sys
from pathlib import Path

def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(4*1024*1024),b''):h.update(b)
 return h.hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);ap.add_argument('--checkout',type=Path,required=True);a=ap.parse_args();r=a.workspace.resolve();s=a.checkout.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure'
 terminal_path=w/'jp_gcc_baseline_build_terminal.json';terminal=json.loads(terminal_path.read_text());assert terminal['exit_code']==0 and terminal['original_source_unchanged'] and terminal['original_247_matching_image_pairs_unchanged']
 assert terminal['upstream_commit']=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
 assert subprocess.check_output(['git','-C',str(s),'rev-parse','HEAD'],text=True).strip()==terminal['upstream_commit']
 assert not subprocess.check_output(['git','-C',str(s),'status','--porcelain'],text=True).strip()
 os.chdir(s);sys.path.insert(0,str(s/'tools'))
 import manifest,images
 mp=s/'build/gcc/manifest.txt';raw=mp.read_bytes();verified=manifest.recorded(raw,str(mp));assert set(verified.images)==set(images.images())
 original=json.loads((w/'japanese_v1007_full_matching_verification.json').read_text());assert set(images.binaries())=={v['image'] for v in original['images']} and len(images.binaries())==247 and len(images.motions())==7
 outputs=[]
 for name,image in verified.images.items():
  final=Path(image.files[0]);header=final.read_bytes()[:52]
  assert header[:4]==b'\x7fELF' and header[4]==1 and header[5]==1 and struct.unpack_from('<H',header,18)[0]==42,(name,'Expected little-endian 32-bit SuperH ELF')
  files=[]
  for relative in image.files:
   path=Path(relative);digest=sha(path);assert digest==verified.hashes[relative]
   files.append(dict(path=relative,bytes=path.stat().st_size,sha256=digest))
  assert image.origin<=image.end<=image.origin+image.length
  outputs.append(dict(image=name,kind='motion_data' if name in images.motions() else 'code_image',origin=hex(image.origin),length=image.length,end=hex(image.end),ELF_class=32,ELF_machine=42,ELF_endian='little',files=files))
 prepared=json.loads((w/'jp_gcc_worktree_preparation.json').read_text())
 assert all(sha(s/n)==h for n,h in prepared['source_sha256'].items())
 version=subprocess.check_output([str(s/'tools/sh-elf/bin/sh-elf-gcc'),'--version'],text=True).splitlines()[0]
 compiler_receipt=s/'tools/sh-elf/request.txt';assert compiler_receipt.exists()
 assert sha(w/'jp_gcc_baseline_build.log')==terminal['log_sha256']
 copied=p/'analysis/jp_gcc_build_manifest.txt';assert not copied.exists();copied.write_bytes(raw);assert sha(copied)==sha(mp)
 report=dict(status='verified_complete_pinned_GCC_Dreamcast_baseline_runtime_not_tested',upstream_commit=terminal['upstream_commit'],checkout=str(s),command=terminal['command'],command_exit_code=0,build_elapsed_seconds=terminal['elapsed_seconds'],built_images=len(outputs),code_images=len(images.binaries()),motion_data_images=len(images.motions()),independent_SH4_ELF_headers_checked=len(outputs),upstream_manifest_parser_validated_all_recorded_file_hashes=True,recorded_manifest_file_hashes=len(verified.hashes),manifest_sha256=sha(mp),toolchain_version=version,toolchain_receipt_sha256=sha(compiler_receipt),toolchain_files=[dict(path=n,sha256=verified.hashes[n]) for n in verified.tools],images=outputs,upstream_tracked_files_unchanged=True,original_247_matching_image_pairs_preserved=True,original_checkout_preserved=True,game_source_publicly_imported=False,complete_game_binaries_kept_private=True,retail_assets_published=False,Dreamcast_runtime_tested=False,native_PC_build=False,English_localization_implemented=False,first_runner_checksum_literal_correction_preserved=True,initial_manifest_count_expectation_corrected='The matching build has 247 code images. GCC also relinks seven motion-data images. This version checks the complete image set and both classifications against pinned upstream placement metadata.',initial_failed_qualifier_preserved=True,terminal_report_sha256=sha(terminal_path),build_log_sha256=terminal['log_sha256'],qualifier_sha256=sha(Path(__file__)),overall_completion_percent=10,goal_paused=False,playable_PC_port=False)
 target=p/'analysis/jp_gcc_baseline_v2_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
 print('Qualified',len(outputs),'GCC Dreamcast code/data images and all manifest hashes. Original matching baseline intact; runtime and native PC remain untested.',flush=True)
if __name__=='__main__':main()
