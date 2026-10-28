"""Strict parse/semantic compiler check of all233 sources without wait bounds."""
import hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",(P/'build_movie_filesystem_persist.ps1').read_text())];assert len(sources)==len(set(sources))==233
flags=['-fsyntax-only','-Wall','-Wextra','-Werror']
r=subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
report=dict(source_sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},exit_code=r.returncode,flags=flags,source_count=len(sources))
(P/'analysis/movie_filesystem_persist_unbounded.json').write_text(json.dumps(report,indent=2)+'\n');print('STRICT UNBOUNDED233 PASS')
