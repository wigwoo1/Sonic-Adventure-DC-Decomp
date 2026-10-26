"""Translate original audio-mode and SDK command-queue bodies from checked bytes."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:begin]+'''notes=[];diagnostics=[];excluded={}
included={0x8c643c18}
excluded={}
folder='movie-post-audio-controls-frontier';path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 assert note['completed']=='true' and not note['error']
 if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==1
header=header.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original MIDI packet packing; explicit complete state.')
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+"\\n if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;")
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_midi_packet_').replace('source/sa_boot_file.c','source/sa_movie_midi_packet.c').replace('analysis/boot_file_translation.json','analysis/movie_midi_packet_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_post_audio_controls_frontier.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n function=function.replace('(void)entry_pr;','(void)r;(void)entry_pr;')\n record['kind']='original_routine'\n record['additional_recovery_credit']='requires_qualification'\n"+marker)
code=code.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original audio-control command proposals; explicit complete state.')
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original stack-argument four-byte MIDI packet construction uses actual native tracing/error helpers. Source proposal requires qualification; no packed bytes/helper results/checkpoint are supplied.')
exec(compile(code,str(parent),'exec'),globals())
