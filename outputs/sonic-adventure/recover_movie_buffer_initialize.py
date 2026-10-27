"""Translate original graphics memory accounting bodies from checked instruction bytes."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:begin]+'''notes=[];diagnostics=[];excluded={}
included={0x8c65393a,0x8c654520,0x8c6544a0,0x8c6534e0,0x8c65344e,0x8c651000,0x8c65253e,0x8c65254e,0x8c650e40,0x8c652210}
excluded={}
for folder in ('movie-render-buffer-next','movie-buffer-initialize-support','movie-buffer-initialize-transfer','boot-device','movie-buffer-initialize-cache','movie-buffer-initialize-queue','movie-buffer-initialize-interrupt'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==10
header=header.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original buffer initialization, upload and wait queries; explicit complete state.')
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+"\\n if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6900u || (a&0x1fffffffu)==0x005f6908u))return 1;")
header=header.replace(marker,marker+"\\n if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;")
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_buffer_initialize_').replace('source/sa_boot_file.c','source/sa_movie_buffer_initialize.c').replace('analysis/boot_file_translation.json','analysis/movie_buffer_initialize_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_render_buffer_next.ps1',PROJECT/'ghidra/export_movie_buffer_initialize_support.ps1',PROJECT/'ghidra/export_movie_buffer_initialize_transfer.ps1',PROJECT/'ghidra/export_movie_buffer_initialize_cache.ps1',PROJECT/'ghidra/export_movie_buffer_initialize_queue.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n function=function.replace('(void)entry_pr;','(void)r;(void)entry_pr;')\n function=function.replace('status=sa_sh4_cache_writeback(ctx,r[4]);','if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);')\n if start==0x8c651000:\n  function=function.replace(' int status;uint32_t target;',' unsigned buffer_wait_visits=0;(void)buffer_wait_visits;\\n int status;uint32_t target;',1)\n  label='L8c65100c: ';assert function.count(label)==1\n  function=function.replace(label,label+'\\n#ifdef SA_BOUND_RETAIL_STALLS\\n *stop=0x8c65100cu;if(!(*sr&1u)&&++buffer_wait_visits>=2u)return -4;\\n#endif\\n ',1)\n  record['bounded_research_waits']=[dict(pc='0x8c65100c',continue_when_t=False,max_continue_tests=2,status=-4)]\n record['kind']='original_routine_or_existing_canonical_adapter'\n record['additional_recovery_credit']='requires_qualification'\n"+marker)
code=code.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original buffer initialization, upload and wait; explicit complete state.')
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original buffer initialization, upload and wait queries use actual native SDK/copy/error providers. Draft adapters alone receive zero recovery credit; no accounting totals/helper returns/checkpoint are injected.')
marker="source=PROJECT/'source/sa_movie_buffer_initialize.c';source.write_text(header+'\\n'.join(functions))";assert code.count(marker)==1
code=code.replace(marker,'''header=header.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original buffer initialization, upload and wait; explicit complete state.')
access_marker=' if(valid_ram(a,n))return 1;';assert header.count(access_marker)==1
header=header.replace(access_marker,access_marker+"\\n if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x05000000u && (a&0x1fffffffu)<0x05800000u)return 1;\\n if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x04000000u && (a&0x1fffffffu)<0x04800000u)return 1;")
'''+marker)
exec(compile(code,str(parent),'exec'),globals())
