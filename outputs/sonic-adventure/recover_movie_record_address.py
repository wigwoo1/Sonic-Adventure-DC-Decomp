"""Translate two previously canonical record address helpers; zero new credit."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:begin]+'''notes=[];diagnostics=[];excluded={}
included={0x8c601874,0x8c601f5a}
folder='movie-record-address';path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 assert note['completed']=='true' and not note['error'] and int(note['address'],16) in included
 notes.append(dict(note,folder=folder))
assert len(notes)==2
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_record_address_').replace('source/sa_boot_file.c','source/sa_movie_record_address.c').replace('analysis/boot_file_translation.json','analysis/movie_record_address_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,P/'ghidra/export_movie_record_address.ps1']")
marker="source=PROJECT/'source/sa_movie_record_address.c';source.write_text(header+'\\n'.join(functions))";assert code.count(marker)==1
code=code.replace(marker,"header=header.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Canonical record address thunk/arithmetic; complete state; zero extra credit.')\nfunctions=[function.replace('(void)gbr;','(void)gbr;(void)valid_access;') for function in functions]\n"+marker)
code=code.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original four-record initializer and status queries; names inferred; complete explicit state.')
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Two existing canonical pointer arithmetic/thunk entries receive zero additional credit; real original instructions execute. Reproduction and complete state qualification remain separate.')
exec(compile(code,str(parent),'exec'),globals())
