"""Guarded original status/cache adapters; unknown uncached blocks stay open."""
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def cache_generator(chosen,listing):
 marker="        assert not text.startswith('f'),text";assert chosen.count(marker)==1
 chosen=chosen.replace(marker,"        if text.startswith('ocbp '):return f'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[{n}],3u);if(status)return status;'\n"+marker)
 marker='    seen=set();todo=[start];edges=set();calls=set()';assert chosen.count(marker)==1
 chosen=chosen.replace(marker,"    declared_flows={int(row['address'],16):[int(x,16) for x in row['flows'].split()] for row in csv.DictReader(listing.open(),delimiter='\\t')}\n"+marker)
 marker='            if w&0xF0FF==0x402B:calls.add(a);continue';assert chosen.count(marker)==1
 chosen=chosen.replace(marker,"            if w&0xF0FF==0x402B:\n                calls.add(a);todo.extend(target for target in declared_flows[a] if target in rows);continue")
 marker="            code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'";assert chosen.count(marker)==1
 chosen=chosen.replace(marker,"            code=f'target=r[{w>>8&15}];'+simple(a+2)+''.join(f'if(target==0x{target:x}u)goto L{target:x};' for target in declared_flows[a] if target in rows)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'")
 return chosen
parent=PROJECT/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[];excluded={}
selected={0x8c6450ea,0x8c10d94c,0x8c5fc0d8,0x8c5fc336}
for folder in ('boot-disc','boot-file-support'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in selected:notes.append(dict(note,folder=folder))
assert len(notes)==4
'''+code[end:]
code=code.replace('sa_boot_file_','sa_boot_disc_').replace('source/sa_boot_file.c','source/sa_boot_disc.c').replace('analysis/boot_file_translation.json','analysis/boot_disc_translation.json')
code=code.replace("exec(compile(chosen,str(TEMPLATE),'exec'),globals())","chosen=cache_generator(chosen,listing);exec(compile(chosen,str(TEMPLATE),'exec'),globals())")
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_boot_disc.ps1']")
exec(compile(code,str(parent),'exec'),globals())
