"""Strict full274-source compile and original instruction/return provenance audit."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    build=P/'build_player_response_clip.ps1';sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())];assert len(sources)==len(set(sources))==274
    flags=['-fsyntax-only','-Wall','-Wextra','-Werror'];subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
    original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes();assert sha(original)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    count=0;files=[Path(__file__),build,original];allcalls=set();prefixes=('player_response_clip',)
    for prefix in prefixes:
        path=P/f'analysis/{prefix}_translation.json';manifest=json.loads(path.read_text());files.append(path)
        for path,expected in manifest['sha256'].items():assert sha(path)==expected,path
        source=P/manifest['source'];assert 'SA_BOUND_RETAIL_STALLS' not in source.read_text()
        calls=set()
        for body in manifest['routines']:
            with Path(body['listing']).open() as stream:rows=list(csv.DictReader(stream,delimiter='\t'))
            for row in rows:
                a=int(row['address'],16);b=bytes.fromhex(row['bytes']);assert len(b)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==b
                if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')):calls.add(hex(a+4))
                count+=1
        returns=P/manifest['return_manifest'];ret=json.loads(returns.read_text());files.append(returns)
        assert calls==set(ret['original_call_returns'])
        allcalls.update(calls)
    assert count==296
    final=json.loads((P/'analysis/player_response_clip_returns.json').read_text());assert allcalls<=set(final['allowed_call_returns'])
    connection=P/'analysis/player_response_clip_connection.json';c=json.loads(connection.read_text());assert c['compiled_source_count']==274;files.append(connection)
    for path,expected in c['sha256'].items():assert sha(path)==expected,path
    scripts=sorted(P.glob('*player_response_clip*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    files+=scripts
    result=dict(status='strict_unbounded274_source_and_original296_row_provenance_passed',source_count=274,compiler_exit_code=0,flags=flags,original_instruction_rows_byte_checked=count,python_scripts_parsed=len(scripts),new_guest_wait_bounds=0,flat_return_membership=len(final['allowed_call_returns']),new_original_routine_credit=0,overall_completion_percent=10,
                source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_response_clip_compile_checks.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('STRICT274 PASS;296original rows/real callreturns checked;',len(scripts),'scripts parsed',flush=True)
if __name__=='__main__':main()
