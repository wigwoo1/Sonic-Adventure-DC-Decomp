"""Translate original flash-packet bodies; proposals alone receive no credit."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')"
assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:begin]+'''notes=[];diagnostics=[];excluded={}
for folder in ('movie-packet-startup','movie-packet-dependencies','movie-packet-lower','movie-packet-validation'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error']
  notes.append(dict(note,folder=folder))
assert len(notes)==24 and len({n['address'] for n in notes})==24
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_packet_startup_').replace('source/sa_boot_file.c','source/sa_movie_packet_startup.c').replace('analysis/boot_file_translation.json','analysis/movie_packet_startup_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent]+list((PROJECT/'ghidra').glob('export_movie_packet*.ps1'))")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
extra=r'''
 function=re.sub(r'(r\[\d+\])=sx\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=sx(\2);if(io_status)return io_status;\1=loaded;}',function)
 function=function.replace('(void)entry_pr;','(void)r;(void)entry_pr;')
 if start==0x8c64bddc:
  function=function.replace(' int status;uint32_t target;',' unsigned read_polls=0,write_polls=0;(void)read_polls;(void)write_polls;\n int status;uint32_t target;',1)
  for pc,counter,limit in ((0x8c64be22,'read_polls',2),(0x8c64bea0,'write_polls',3)):
   marker=f'L{pc:x}: ';assert function.count(marker)==1
   function=function.replace(marker,f'L{pc:x}:\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x{pc:x}u;if((*sr&1u) && ++{counter}>={limit}u)return -4;\n#endif\n ',1)
  record['bounded_research_waits']=[dict(pc=hex(pc),continue_when_t=True,max_continue_tests=limit,status=-4) for pc,limit in ((0x8c64be22,2),(0x8c64bea0,3))]
'''
code=code.replace(marker,extra+marker)
code=code.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original flash-packet storage proposals; explicit complete machine state.')
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original startup64bddc and flash-packet dependencies, including reused canonical bodies. Source proposals receive no recovery credit. All reached services execute native original code or stop at their actual missing platform boundary; no helper result is supplied. Production polls remain unbounded. Research read/write limits are2/3continuing tests; the write limit admits both genuine pending stages before compaction returns.')
exec(compile(code,str(parent),'exec'),globals())
