"""Translate the reached original renderer-setup helper without new credit."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[];excluded={}
for folder in ('movie-render-config-frontier','movie-render-setup-callbacks'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder=folder))
assert len(notes)==6
excluded={0x8c6412c2:'Existing original full-state callback installer remains connected unchanged.',0x8c6411b0:'Existing original full-state default callback reset remains connected unchanged.'}
notes=[note for note in notes if int(note['address'],16) not in excluded];assert len(notes)==4
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_render_setup_').replace('source/sa_boot_file.c','source/sa_movie_render_setup.c').replace('analysis/boot_file_translation.json','analysis/movie_render_setup_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_render_frontier.ps1',PROJECT/'ghidra/export_movie_render_setup.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n"+marker)
exec(compile(code,str(parent),'exec'),globals())
