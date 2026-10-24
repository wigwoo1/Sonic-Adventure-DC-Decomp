"""Original main-binary dependencies reached by MOVIE startup; no return fixtures."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[];excluded={}
for folder in ('movie-main-prefix','movie-main-setup','movie-main-services','movie-main-allocation'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder=folder))
assert len(notes)==16
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_main_').replace('source/sa_boot_file.c','source/sa_movie_main.c').replace('analysis/boot_file_translation.json','analysis/movie_main_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent]+[PROJECT/'ghidra'/name for name in ('export_movie_main_prefix.ps1','export_movie_main_setup.ps1','export_movie_main_services.ps1','export_movie_main_allocation.ps1')]")
exec(compile(code,str(parent),'exec'),globals())
