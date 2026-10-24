"""Original CCR instruction-invalidation leaf, preserving its P1/P2 addresses."""
import copy,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[];excluded={}
folder='boot-cache';path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 assert note['completed']=='true' and not note['error']
 if int(note['address'],16)==0x8c5fc0c2:notes.append(dict(note,folder=folder))
assert len(notes)==1
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+'\\n if(n==4u && a==0xff00001cu)return 1;')
'''+code[end:]
code=code.replace('sa_boot_file_','sa_boot_cache_').replace('source/sa_boot_file.c','source/sa_boot_cache.c').replace('analysis/boot_file_translation.json','analysis/boot_cache_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_boot_cache.ps1']")
exec(compile(code,str(parent),'exec'),globals())
# This leaf contains only PC-relative literal loads, register operations and a
# delayed RTS. Literal ADDRESS operands and stop PCs move with P2; literal DATA
# is read unchanged from the same physical RAM. No alias credit duplicates it.
assert len(routines)==1 and routines[0]['instruction_rows']==6
original=functions[0];alias=original.replace('sa_boot_cache_8c5fc0c2','sa_boot_cache_ac5fc0c2').replace('L8c5fc','Lac5fc')
alias=re.sub(r'0x8c5fc([0-9a-f]{3})u',r'0xac5fc\1u',alias)
source=P/'source/sa_boot_cache.c';source.write_text(source.read_text()+'\n'+alias)
record=copy.deepcopy(routines[0]);record['address']='0xac5fc0c2';record['end_exclusive']='0xac5fc0ce';record['name']='sa_boot_cache_ac5fc0c2';record['reachable_addresses']=[hex(int(a,16)+0x20000000) for a in record['reachable_addresses']];record['canonical_physical_entry']='0x8c5fc0c2';record['alias_duplicate_credit']=0
report['routines'].append(record);report['instruction_rows']=12;report['unique_physical_instruction_rows']=6;report['canonical_entries']=1;report['scope']='Six original CCR instruction-invalidation instructions with distinct P1/P2 PCs and PC-relative literal addresses. Two native aliases share one physical body and receive no duplicate credit. Only privileged MMU-off execution with explicit software CCR policy is qualified.'
report['sha256'][str(source)]=hashlib.sha256(source.read_bytes()).hexdigest()
(P/'analysis/boot_cache_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated six original rows at both cached/uncached addresses')
