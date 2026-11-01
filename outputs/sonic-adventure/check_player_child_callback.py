"""Strict276-source compile and original callback/dependency/return provenance."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/player_child_callback_compile_checks.json';assert not target.exists()
    build=P/'build_player_child_callback_dependencies.ps1';sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())]
    assert len(sources)==len(set(sources))==276
    flags=['-fsyntax-only','-Wall','-Wextra','-Werror'];subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
    original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes();assert sha(original)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    count=0;files=[Path(__file__),build,original];allcalls=set()
    for prefix in ('player_child_callback','player_child_callback_dependencies'):
        mp=P/f'analysis/{prefix}_translation.json';t=json.loads(mp.read_text());files.append(mp)
        for path,value in t['sha256'].items():assert sha(path)==value,path
        assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
        calls=set()
        for b in t['routines']:
            for row in csv.DictReader(Path(b['listing']).open(),delimiter='\t'):
                a=int(row['address'],16);value=bytes.fromhex(row['bytes']);assert len(value)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==value;count+=1
                if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')):calls.add(hex(a+4))
        ret=P/t['return_manifest'];data=json.loads(ret.read_text());files.append(ret)
        assert calls==set(data['original_call_returns']);allcalls|=calls
    assert count==74 and allcalls=={'0x8c0b3a5a','0x8c0b3a78'}
    final=json.loads((P/'analysis/player_child_callback_dependencies_returns.json').read_text());assert allcalls<=set(final['allowed_call_returns'])
    for prefix,number in (('player_child_callback',275),('player_child_callback_dependencies',276)):
        cp=P/f'analysis/{prefix}_connection.json';c=json.loads(cp.read_text());files.append(cp);assert c['compiled_source_count']==number
        for path,value in c['sha256'].items():assert sha(path)==value,path
    scripts=sorted(P.glob('*player_child_callback*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    files+=scripts
    report=dict(status='strict276_source_and74_original_row_provenance_passed',source_count=276,compiler_exit_code=0,flags=flags,original_instruction_rows_byte_checked=74,new_unregistered_original_rows=68,canonical_reused_original_rows=6,python_scripts_parsed=len(scripts),new_guest_wait_bounds=0,flat_return_membership=len(final['allowed_call_returns']),new_original_routine_credit=0,overall_completion_percent=10,
        source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('STRICT276 PASS;74original rows/real call returns checked;',len(scripts),'Python scripts parsed',flush=True)
if __name__=='__main__':main()
