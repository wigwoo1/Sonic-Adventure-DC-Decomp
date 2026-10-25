"""Translate original allocation and its entry thunk with complete state."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[];excluded={}
for folder in ('movie-render-setup-frontier','movie-heap-initialization'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder=folder))
assert len(notes)==4
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_heap_').replace('source/sa_boot_file.c','source/sa_movie_heap.c').replace('analysis/boot_file_translation.json','analysis/movie_heap_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_render_setup_frontier.ps1',PROJECT/'ghidra/export_movie_heap_initialization.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n"+marker)
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original circular free-list allocation and entry thunk. Source proposals are not recovery credit. Heap inputs remain explicit; no root or allocation result is substituted into startup.')
exec(compile(code,str(parent),'exec'),globals())
