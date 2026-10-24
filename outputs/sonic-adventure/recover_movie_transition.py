"""Original ten-row display transition wait; no substituted completion."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[];excluded={}
folder='movie-transition';path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder=folder))
assert len(notes)==1
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+'\\n if(n==2u && a==0xff800030u)return 1;')
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_transition_').replace('source/sa_boot_file.c','source/sa_movie_transition.c').replace('analysis/boot_file_translation.json','analysis/movie_transition_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_transition.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n"+marker)
exec(compile(code,str(parent),'exec'),globals())
