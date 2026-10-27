"""Translate24 checked original drawing-state initialization instructions."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:begin]+'''notes=[];diagnostics=[];excluded={}
for folder in ('movie-buffer-initialize-next','movie-float-initialize-next'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error'] and int(note['address'],16) in (0x8c60edb0,0x8c60d1f2)
  notes.append(dict(note,folder=folder))
assert len(notes)==2
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_float_initialize_').replace('source/sa_boot_file.c','source/sa_movie_float_initialize.c').replace('analysis/boot_file_translation.json','analysis/movie_float_initialize_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_buffer_initialize_next.ps1',PROJECT/'ghidra/export_movie_float_initialize_next.ps1']")
code=code.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original drawing-state initialization; inferred name, explicit complete state.')
code=code.replace('Original file lookup/open/read/close proposals; platform progress stays explicit.','Original drawing-state initializer from checked instructions.')
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original24-instruction branchless initializer. ExplicitFPSCR/FR/SZ handling andoriginalunsupportedPR guard; proposed source receives no credit until separatequalification. No inferred highlevel SDKname orphysicalexception claim.')
exec(compile(code,str(parent),'exec'),globals())
