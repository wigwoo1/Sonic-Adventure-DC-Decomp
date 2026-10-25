"""Original transfer start/status/callback and cache paths, including real P2 PCs."""
import ast,copy,csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
retail=P.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes()
inputs=[P/'analysis/movie-transfer-start/8c5fc58a_instructions.tsv',P/'analysis/movie-transfer-cache/8c5fc614_instructions.tsv']
all_rows={}
for path in inputs:
 for row in csv.DictReader(path.open(),delimiter='\t'):
  a=int(row['address'],16)
  if a in all_rows:assert all_rows[a]==row
  all_rows[a]=dict(row)
for row in list(csv.DictReader(inputs[1].open(),delimiter='\t')):
 a=int(row['address'],16);alias=copy.deepcopy(row);alias['address']=f'{a+0x20000000:08x}'
 alias['instruction']=re.sub(r'0x8c5fc[0-9a-f]+',lambda m:hex(int(m[0],16)+0x20000000),alias['instruction'])
 word=int.from_bytes(bytes.fromhex(row['bytes']),'little')
 if word>>12 in (0xa,0xb) or word>>8 in (0x89,0x8b,0x8d,0x8f):alias['flows']=' '.join(f'{int(x,16)+0x20000000:x}' for x in row['flows'].split())
 all_rows[a+0x20000000]=alias
seen=set();todo=[0x8c5fc58a]
def signed(x,n):return x-(1<<n) if x&(1<<(n-1)) else x
while todo:
 a=todo.pop()
 if a in seen:continue
 assert a in all_rows,hex(a);seen.add(a);row=all_rows[a];w=int.from_bytes(bytes.fromhex(row['bytes']),'little')
 physical=(a&0x1fffffff)-0x0c010000;assert raw[physical:physical+2]==bytes.fromhex(row['bytes'])
 if w==0xb or w>>12 in (0xa,0xb) or w&0xf0ff in (0x400b,0x402b):
  assert a+2 in all_rows;seen.add(a+2)
  if w==0xb:continue
  if w>>12==0xa:todo.append(a+4+2*signed(w&4095,12))
  elif w&0xf0ff==0x402b:
   targets=[int(x,16) for x in row['flows'].split()];assert targets,(hex(a),row);todo.extend(targets)
  else:todo.append(a+4)
 elif w>>8 in (0x89,0x8b,0x8d,0x8f):
  delayed=w>>8 in (0x8d,0x8f)
  if delayed:assert a+2 in all_rows;seen.add(a+2)
  todo.extend((a+4+2*signed(w&255,8),a+(4 if delayed else 2)))
 else:todo.append(a+2)
folder=P/'analysis/movie-transfer-cache-combined';folder.mkdir(exist_ok=True)
with (folder/'8c5fc58a_instructions.tsv').open('w',newline='') as f:
 writer=csv.DictWriter(f,fieldnames=('address','length','bytes','instruction','flows'),delimiter='\t');writer.writeheader();writer.writerows(all_rows[a] for a in sorted(seen))
with (folder/'diagnostics.tsv').open('w',newline='') as f:
 writer=csv.DictWriter(f,fieldnames=('address','function','body_bytes','minimum','maximum','completed','error'),delimiter='\t');writer.writeheader();writer.writerow(dict(address='8c5fc58a',function='byte_checked_cache_p1_p2_reachable_paths',body_bytes=2*len(seen),minimum='8c5fc58a',maximum=f'{max(seen)+1:08x}',completed='true',error=''))

def cache_generator(chosen,listing):
 marker="        assert not text.startswith('f'),text";assert chosen.count(marker)==1
 chosen=chosen.replace(marker,"        if text.startswith('ocbwb '):return f'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[{n}],2u);if(status)return status;'\n"+marker)
 marker='    seen=set();todo=[start];edges=set();calls=set()';assert chosen.count(marker)==1
 chosen=chosen.replace(marker,"    declared_flows={int(row['address'],16):[int(x,16) for x in row['flows'].split()] for row in csv.DictReader(listing.open(),delimiter='\\t')}\n"+marker)
 marker='            if w&0xF0FF==0x402B:calls.add(a);continue';assert chosen.count(marker)==1
 chosen=chosen.replace(marker,"            if w&0xF0FF==0x402B:\n                calls.add(a);todo.extend(target for target in declared_flows[a] if target in rows);continue")
 marker="            code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'";assert chosen.count(marker)==1
 return chosen.replace(marker,"            code=f'target=r[{w>>8&15}];'+simple(a+2)+''.join(f'if(target==0x{target:x}u)goto L{target:x};' for target in declared_flows[a] if target in rows)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'")

parent=P/'recover_movie_queue_family.py';code=parent.read_text()
code=code.replace('included={0x8c667080,0x8c666f98,0x8c666d10,0x8c666e02,0x8c666c2c,0x8c10f9f4}','included={0x8c10faa4,0x8c66abfc,0x8c668f40}')
code=code.replace("for folder in ('movie-queue-family','movie-queue-hal'):","for folder in ('movie-transfer-start',):")
code=code.replace('assert len(notes)==6',"path=PROJECT/'analysis/movie-transfer-cache-combined/diagnostics.tsv';diagnostics.append(path)\nfor note in csv.DictReader(path.open(),delimiter='\\t'):notes.append(dict(note,folder='movie-transfer-cache-combined'))\nassert len(notes)==4")
code=code.replace('sa_movie_queue_family_','sa_movie_transfer_start_').replace('source/sa_movie_queue_family.c','source/sa_movie_transfer_start.c').replace('analysis/movie_queue_family_translation.json','analysis/movie_transfer_start_translation.json')
code=code.replace("PROJECT/'ghidra/export_movie_queue_family.ps1',PROJECT/'ghidra/export_movie_queue_hal.ps1'","PROJECT/'recover_movie_queue_family.py',PROJECT/'ghidra/export_movie_transfer_start.ps1',PROJECT/'ghidra/export_movie_transfer_cache.ps1'")
marker='exec(compile(code,str(parent),\'exec\'),globals())';assert code.count(marker)==1
injection=r'''code=code.replace("exec(compile(chosen,str(TEMPLATE),'exec'),globals())","chosen=cache_generator(chosen,listing);chosen=chosen.replace('binary[a-0x8C010000:a-0x8C010000+2]','binary[(a&0x1fffffff)-0x0c010000:(a&0x1fffffff)-0x0c010000+2]');exec(compile(chosen,str(TEMPLATE),'exec'),globals())")
'''
code=code.replace(marker,injection+marker)
exec(compile(code,str(parent),'exec'),globals())
cache=next(r for r in routines if r['address']=='0x8c5fc58a');cache['end_exclusive']='0x8c5fc7de';cache['retail_span_sha256']=hashlib.sha256(raw[0x8c5fc58a-0x8c010000:0x8c5fc7de-0x8c010000]).hexdigest()
cache['physical_instruction_rows']=len({a&0x1fffffff for a in seen});cache['uncached_pc_rows']=sum(a>=0xa0000000 for a in seen);cache['alias_duplicate_credit']=0
for item in cache['retail_instruction_ranges']:
 a=int(item['start'],16);b=int(item['end_exclusive'],16);physical=(a&0x1fffffff)-0x0c010000;item['sha256']=hashlib.sha256(raw[physical:physical+b-a]).hexdigest();item['physical_start']=hex((a&0x1fffffff)|0x80000000)
report['scope']='Static byte-checked original transfer/status/queue callback bodies and complete reachable cache-writeback path with actual cached/uncached PCs. Coherent RAM/cache-tag input remains explicit software policy. Qualification and registration are separate; no asynchronous completion or helper return is supplied.'
report['sha256'].update({str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in [P/'recover_movie_transfer_start.py',*inputs,P/'analysis/movie-transfer-cache/diagnostics.tsv',P/'source/sa_engine_cache_writeback_io.c',P/'analysis/engine_cache_writeback_large_verification.json']})
(P/'analysis/movie_transfer_start_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Cache logical/physical/P2rows',cache['instruction_rows'],cache['physical_instruction_rows'],cache['uncached_pc_rows'])
