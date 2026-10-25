"""Connect original early platform services, aliases and heap caller."""
import copy,csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
# Ghidra indexes the shared five-row return tail separately. Preserve both
# original exports; create a reproducible combined listing for entry 5fc06c.
folder=P/'analysis/movie-startup-cache-combined';folder.mkdir(exist_ok=True)
parts=[P/f'analysis/boot-cache/{a}_instructions.tsv' for a in ('8c5fc06c','8c5fc0b8')]
combined=[r for path in parts for r in csv.DictReader(path.open(),delimiter='\t')]
with (folder/'8c5fc06c_instructions.tsv').open('w',newline='') as f:
 writer=csv.DictWriter(f,fieldnames=('address','length','bytes','instruction','flows'),delimiter='\t');writer.writeheader();writer.writerows(combined)
with (folder/'diagnostics.tsv').open('w',newline='') as f:
 writer=csv.DictWriter(f,fieldnames=('address','function','body_bytes','minimum','maximum','completed','error'),delimiter='\t');writer.writeheader()
 writer.writerow(dict(address='8c5fc06c',function='combined_original_cache_setup',body_bytes=len(combined)*2,minimum='8c5fc06c',maximum='8c5fc0c1',completed='true',error=''))
parent=P/'recover_boot_file.py';code=parent.read_text();marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[];excluded={}
for folder in ('movie-heap-caller','movie-startup-platform','movie-startup-cache-tail','movie-startup-cache-combined'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder=folder))
assert len(notes)==10
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+"\\n if(n==4u && a==0xff00001cu)return 1;\\n if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f6800u && (a&0x1fffffffu)<0x005f7d00u)return 1;\\n if(n==4u && !(a&31u) && a>=0xf4000000u && a<0xf4004000u)return 1;\\n if(n==2u && (a==0xffd00000u||a==0xffd00004u||a==0xffd00008u||a==0xffd0000cu||a==0xffd80010u))return 1;\\n if(n==1u && (a==0xffd80000u||a==0xffd80004u))return 1;\\n if(n==4u && (a==0xffd80008u||a==0xffd8000cu))return 1;")
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_startup_').replace('source/sa_boot_file.c','source/sa_movie_startup.c').replace('analysis/boot_file_translation.json','analysis/movie_startup_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_heap_caller.ps1',PROJECT/'ghidra/export_movie_startup_platform.ps1',PROJECT/'ghidra/export_movie_startup_cache.ps1']")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n function=function.replace('(void)entry_pr;','(void)r;(void)entry_pr;')\n function=re.sub(r'status=sa_sh4_cache_purge\\(ctx,(r\\[\\d+\\])\\);',r'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,\\1,3u);',function)\n"+marker)
exec(compile(code,str(parent),'exec'),globals())
aliases=[]
for entry in (0x8c5fc06c,0x8c5fc848):
 original=next(r for r in routines if int(r['address'],16)==entry);function=functions[routines.index(original)]
 # Only code PCs/labels and PC-relative ADDRESS operands move to P2. Loaded
 # literal DATA (including the cached helper pointer 8c5fc81c) stays unchanged.
 body={int(row['address'],16):row['instruction'].lstrip('_') for row in csv.DictReader(Path(original['listing']).open(),delimiter='\t')}
 addresses=set(body)
 for text in body.values():
  if text.startswith(('mov.w 0x','mov.l 0x','mova 0x')):addresses.add(int(text.split(' ')[1].split(',')[0],16))
 function=function.replace(f'sa_movie_startup_{entry:08x}',f'sa_movie_startup_{entry+0x20000000:08x}')
 for address in sorted(addresses,reverse=True):
  function=function.replace(f'L{address:x}',f'L{address+0x20000000:x}').replace(f'0x{address:x}u',f'0x{address+0x20000000:x}u')
 record=copy.deepcopy(original);record.update(address=hex(entry+0x20000000),end_exclusive=hex(int(original['end_exclusive'],16)+0x20000000),name=f'sa_movie_startup_{entry+0x20000000:08x}',canonical_physical_entry=hex(entry),alias_duplicate_credit=0)
 record['reachable_addresses']=[hex(int(a,16)+0x20000000) for a in record['reachable_addresses']]
 record['conditional_edges']=[[hex(int(a,16)+0x20000000),hex(int(b,16)+0x20000000)] for a,b in record['conditional_edges']]
 aliases.append(record);source.write_text(source.read_text()+'\n'+function)
canonical_rows=sum(r['instruction_rows'] for r in routines)
report['routines'].extend(aliases);report['instruction_rows']=sum(r['instruction_rows'] for r in report['routines']);report['unique_physical_instruction_rows']=canonical_rows
report['scope']='Original early initialization caller, platform/vector setup, byte/word register stores and cached/uncached cache setup. Shared return-tail bytes are retained; P2 aliases move code/literal addresses only. Missing services remain stops; no heap or device result is fabricated.'
for path in [Path(__file__),*parts,P/'analysis/boot-cache/8c5fc06c_draft.c']:
 report['sha256'][str(path)]=hashlib.sha256(path.read_bytes()).hexdigest()
report['sha256'][str(source)]=hashlib.sha256(source.read_bytes()).hexdigest()
(P/'analysis/movie_startup_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated cached and uncached startup bodies; physical rows',report['unique_physical_instruction_rows'])
