"""Recover original four-record initialization and reached status dependencies."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:begin]+'''notes=[];diagnostics=[];excluded={}
included={0x8c109cfe,0x8c107ec0,0x8c108808,0x8c1087d2,0x8c602ab4,0x8c602998,0x8c646c8a}
folder='movie-slot-initialize';path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 assert note['completed']=='true' and not note['error'] and int(note['address'],16) in included
 notes.append(dict(note,folder=folder))
assert len(notes)==7
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_slot_initialize_').replace('source/sa_boot_file.c','source/sa_movie_slot_initialize.c').replace('analysis/boot_file_translation.json','analysis/movie_slot_initialize_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,P/'ghidra/export_movie_slot_initialize.ps1']")
code=code.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original four-record initializer and status queries; names inferred; complete explicit state.')
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original fixed-record initialization and reached status query proposals. Existing canonical clear helper receives zero duplicate credit. Real original helper/callback code must execute; separate full behavior qualification is required before registration.')
exec(compile(code,str(parent),'exec'),globals())
