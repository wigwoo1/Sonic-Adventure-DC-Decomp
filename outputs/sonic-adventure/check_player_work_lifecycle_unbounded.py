"""Strict compiler/provenance checks for the complete 268-source integration."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
build=P/'build_player_work_allocation.ps1'
sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())]
assert len(sources)==len(set(sources))==268
flags=['-fsyntax-only','-Wall','-Wextra','-Werror']
subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
assert sha(original)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
rows=0;files=[Path(__file__),build,original]
for prefix in ('player_work_initialize','player_work_dependencies','player_work_allocation'):
    file=P/f'analysis/{prefix}_translation.json';m=json.loads(file.read_text());files.append(file)
    for path,expected in m['sha256'].items():assert sha(path)==expected,path
    source=P/m['source'];assert 'SA_BOUND_RETAIL_STALLS' not in source.read_text()
    for body in m['routines']:
        listing=Path(body['listing']);files.append(listing)
        for row in csv.DictReader(listing.open(),delimiter='\t'):
            a=int(row['address'],16);b=bytes.fromhex(row['bytes'])
            assert len(b)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==b
            rows+=1
scripts=sorted({*P.glob('*player_work_*.py')})
for script in scripts:ast.parse(script.read_text(encoding='utf-8-sig'),filename=str(script))
files+=scripts
result=dict(status='strict_unbounded_268_source_and_original_byte_checks_passed',source_count=268,exit_code=0,flags=flags,
            original_instruction_rows=rows,python_scripts_parsed=len(scripts),new_guest_wait_bounds=0,
            overall_completion_percent=10,source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
target=P/'analysis/player_work_lifecycle_unbounded.json';assert not target.exists()
target.write_text(json.dumps(result,indent=2)+'\n')
print('STRICT UNBOUNDED268 PASS',rows,'original rows;',len(scripts),'Python scripts parsed',flush=True)
