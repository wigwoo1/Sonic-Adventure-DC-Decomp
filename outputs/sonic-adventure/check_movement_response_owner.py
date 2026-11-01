"""Unchanged274-source strict compile and owner/response/constructor provenance."""
import ast,csv,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/movement_response_owner_compile_checks.json';assert not target.exists()
    previous=P/'analysis/player_response_clip_compile_checks.json';old=json.loads(previous.read_text())
    for p,value in old['source_sha256'].items():assert sha(p)==value,p
    sources=[Path(p) for p in old['source_sha256']];assert len(sources)==274
    flags=['-fsyntax-only','-Wall','-Wextra','-Werror']
    subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
    retail=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes()
    assert sha(retail)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    files=[Path(__file__),previous,retail];count=0
    for prefix,entry in (('player_movement_owner','0x8c072b44'),('player_movement_dependencies','0x8c072946'),('player_work_initialize','0x8c06c350')):
        mp=P/f'analysis/{prefix}_translation.json';t=json.loads(mp.read_text());files.append(mp)
        for p,value in t['sha256'].items():assert sha(p)==value,p
        b=next(x for x in t['routines'] if x['address']==entry);listing=Path(b['listing']);files.append(listing)
        for row in csv.DictReader(listing.open(),delimiter='\t'):
            a=int(row['address'],16);value=bytes.fromhex(row['bytes'])
            assert len(value)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==value;count+=1
    assert count==1146
    scripts=sorted(P.glob('*movement_response_owner*.py'))
    for p in scripts:ast.parse(p.read_text(encoding='utf-8-sig'),filename=str(p))
    dll=R/'work/sonic-adventure/build/player_response_clip.dll';prior=json.loads((P/'analysis/collision_response_qualification.json').read_text());assert sha(dll)==prior['dll_sha256']
    files+=[*scripts,dll]
    report=dict(status='unchanged_native_sources_strict_compile_and1146_original_rows_match',source_count=274,compiler_exit_code=0,flags=flags,original_instruction_rows_byte_checked=count,python_scripts_parsed=len(scripts),native_sources_changed=0,native_DLL_rebuilt=False,existing_verified_DLL_reused=True,dll_sha256=sha(dll),new_guest_wait_bounds=0,flat_return_membership=old['flat_return_membership'],new_original_routine_credit=0,overall_completion_percent=10,source_sha256=old['source_sha256'],sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('STRICT274 PASS;1146original rows match;verified DLL unchanged;',len(scripts),'Python scripts parsed',flush=True)
if __name__=='__main__':main()
