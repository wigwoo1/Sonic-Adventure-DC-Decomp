"""Translate original audio-mode and SDK command-queue bodies from checked bytes."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:begin]+'''notes=[];diagnostics=[];excluded={}
for folder in ('movie-audio-mode','movie-audio-copy20'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error']
  notes.append(dict(note,folder=folder))
assert len(notes)==3
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+"\\n if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;")
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_audio_mode_').replace('source/sa_boot_file.c','source/sa_movie_audio_mode.c').replace('analysis/boot_file_translation.json','analysis/movie_audio_mode_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_audio_mode.ps1',PROJECT/'ghidra/export_movie_audio_copy20.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n function=function.replace('(void)entry_pr;','(void)r;(void)entry_pr;')\n record['kind']='shared_copy_fragment' if start==0x8c10cd82 else 'original_routine'\n record['additional_recovery_credit']=0 if start==0x8c10cd82 else 'requires_qualification'\n"+marker)
code=code.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original audio-mode and SDK command-ring proposals; explicit complete state.')
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original MONO/STRO audio-mode request and SDK20-byte command-ring sender. Actual existing indexed copy and error logger remain native; no helper return or ARM acknowledgement is supplied. Proposals alone receive no recovery credit. Command enqueue does not establish ARM execution or audible output.')
exec(compile(code,str(parent),'exec'),globals())
