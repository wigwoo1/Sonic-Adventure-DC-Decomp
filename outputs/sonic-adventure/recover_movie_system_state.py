"""Translate original settings/info queries and existing canonical adapters."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:begin]+'''notes=[];diagnostics=[];excluded={}
included={0x8c5ff39c,0x8c5ff3fa,0x8c5ff494,0x8c5ff396,0x8c04fe40}
for folder in ('movie-system-state-next','movie-system-state-support','movie-system-state-info','movie-system-state-callback'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error'] and int(note['address'],16) in included
  notes.append(dict(note,folder=folder))
assert len(notes)==5
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_system_state_').replace('source/sa_boot_file.c','source/sa_movie_system_state.c').replace('analysis/boot_file_translation.json','analysis/movie_system_state_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent]+list((PROJECT/'ghidra').glob('export_movie_system_state_*.ps1'))")
code=code.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original settings/info queries and canonical adapters; inferred names, explicit complete state.')
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original47-rowsettingsquery/21-rowbytequery/13-rowinfolookup plus twoexistingcanonicaladapters. Actualnativeflash/console providers used; no gamehelperresult substitution. Proposals receive no credit before separatequalification.')
exec(compile(code,str(parent),'exec'),globals())
