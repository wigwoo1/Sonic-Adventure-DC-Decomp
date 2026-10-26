"""Bus-aware static adapters for existing byte/word getters; zero duplicate credit."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[]
excluded={};included={0x8c667150,0x8c667188,0x8c66910c}
for folder in ('movie-sound-read',):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==3
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+"\\n if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;")
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_sound_read_').replace('source/sa_boot_file.c','source/sa_movie_sound_read.c').replace('analysis/boot_file_translation.json','analysis/movie_sound_read_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_sound_read.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n function=function.replace('(void)entry_pr;','(void)r;(void)entry_pr;')\n"+marker)
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Existing original sound byte/word getters with actual already connected10d460andSoundRAM bus; no modeled lower helper result. Canonical sources/proofs remain intact; zero duplicate credit.')
code=code.replace('functions=[];routines=[]',"header=header.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original sound byte/word read adapters; explicit machine/platform state.')\nfunctions=[];routines=[]")
exec(compile(code,str(parent),'exec'),globals())
