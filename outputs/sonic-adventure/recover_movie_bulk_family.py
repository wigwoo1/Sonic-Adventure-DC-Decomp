"""Original driver bulk-transfer, control, request and callback family."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[]
excluded={}
included={0x8c65a8c8,0x8c65a27c,0x8c65a5dc,0x8c65aaf4,0x8c65a518}
for folder in ('movie-bulk-family',):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==5
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+"\\n if(n==4u && ((a&0x1fffffffu)==0x00710000u || (a&0x1fffffffu)==0x00710004u))return 1;"+"\\n if((n==1u||n==2u||n==4u) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;\\n if(n==4u && (a==0xff00001cu||a==0xffa00040u))return 1;\\n if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f6800u && (a&0x1fffffffu)<0x005f7d00u)return 1;\\n if(n==4u && !(a&31u) && a>=0xf4000000u && a<0xf4004000u)return 1;\\n if(n==2u && (a==0xffd00000u||a==0xffd00004u||a==0xffd00008u||a==0xffd0000cu||a==0xffd80010u))return 1;\\n if(n==1u && (a==0xffd80000u||a==0xffd80004u))return 1;\\n if(n==4u && (a==0xffd80008u||a==0xffd8000cu))return 1;")
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_bulk_family_').replace('source/sa_boot_file.c','source/sa_movie_bulk_family.c').replace('analysis/boot_file_translation.json','analysis/movie_bulk_family_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_bulk_family.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
hook=r""" if start==0x8c6656e2:
  function=function.replace(' int take;',' unsigned driver_stall_visits=0,driver_clear_visits=0;(void)driver_stall_visits;(void)driver_clear_visits;\n int take;')
  marker='L8c665968: ';assert function.count(marker)==1
  function=function.replace(marker,'L8c665968:\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x8c665968u;if(++driver_stall_visits>=2u)return -4;\n#endif\n ',1)
  record['bounded_research_self_stalls']=[dict(pc='0x8c665968',max_visits=2,status=-4)]
  marker='L8c665998: ';assert function.count(marker)==1
  function=function.replace(marker,'L8c665998:\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x8c665998u;if(!(*sr&1u) && ++driver_clear_visits>=2u)return -4;\n#endif\n ',1)
  record['bounded_research_waits']=[dict(pc='0x8c665998',continue_when_t=False,max_continue_tests=2,status=-4)]
"""
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n function=function.replace('(void)entry_pr;','(void)r;(void)entry_pr;')\n function=re.sub(r'status=sa_sh4_cache_purge\\(ctx,(r\\[\\d+\\])\\);',r'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,\\1,3u);',function)\n"+hook+marker)
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original disc/file-library initialization and reached ISO directory/read helpers, including separate complete-state adapters for existing thunk/record/store bodies. Missing platform services remain actual stops; no firmware or media result is fabricated.')
code=code.replace('functions=[];routines=[]',"header=header.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original driver-control word update; explicit machine/platform state.')\nfunctions=[];routines=[]")
exec(compile(code,str(parent),'exec'),globals())
