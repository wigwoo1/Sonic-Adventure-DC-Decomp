"""Conservative byte-backed entry spans and call dependencies for the large dispatcher."""
import bisect,csv,hashlib,json,re,struct
from collections import defaultdict
from pathlib import Path
from verify_startup import ROOT
P=Path(__file__).resolve().parent
listing=P/'analysis/main-dispatch-enrichment/8c0a966c_instructions.tsv';table_path=P/'analysis/main-dispatch-enrichment/table_targets.tsv'
rows=list(csv.DictReader(listing.open(),delimiter='\t'));by_address={int(r['address'],16):r for r in rows}
image_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';image=image_path.read_bytes();base=0x8c010000
selectors=defaultdict(list)
for row in csv.DictReader(table_path.open(),delimiter='\t'):selectors[int(row['target'],16)].append(int(row['index'])+1)
entries=sorted(selectors);assert len(entries)==80
index_path=P/'analysis/ghidra-all/function_index.tsv';index={int(r['address'],16):r for r in csv.DictReader(index_path.open(),delimiter='\t')}
registry_path=P/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text())
native={int(r['address'],16):r['name'] for r in registry['routines'] if r['module']=='1ST_READ.BIN'}
leaders=set(entries)|{0x8c0a966c};direct=[]
for row in rows:
 a=int(row['address'],16);w=int.from_bytes(bytes.fromhex(row['bytes']),'little');target=None;kind=None
 assert image[a-base:a-base+int(row['length'])].hex()==row['bytes']
 if w>>12 in (10,11):
  v=w&0xfff;v=v-0x1000 if v&0x800 else v;target=(a+4+v*2)&0xffffffff;kind='call' if w>>12==11 else 'branch'
 elif w&0xff00 in (0x8900,0x8b00,0x8d00,0x8f00):
  v=w&255;v=v-256 if v&128 else v;target=(a+4+v*2)&0xffffffff;kind='conditional'
 if target is not None:
  leaders.add(target);direct.append({'site':a,'target':target,'kind':kind})
 if w>>12==11 or w&0xf0ff==0x400b:leaders.add(a+4)
 if w&0xff00 in (0x8900,0x8b00):leaders.add(a+2)
 if w&0xff00 in (0x8d00,0x8f00):leaders.add(a+4)
def owner(address):
 n=bisect.bisect_right(entries,address)-1
 return entries[n] if n>=0 else None
def written_registers(row):
 text=row['instruction'].lstrip('_');op,_,args=text.partition(' ');out=set()
 out.update(int(n) for n in re.findall(r'@-r(\d+)|@r(\d+)\+',args) for n in n if n)
 if op.startswith(('cmp','tst','fcmp')) or op in ('jsr','jmp','bra','bsr','bt','bf','bt/s','bf/s','rts'):return out
 destination=args.rsplit(',',1)[-1].strip()
 if re.fullmatch(r'r\d+',destination):out.add(int(destination[1:]))
 return out
def resolve(site,n):
 cursor=site
 for unused in range(16):
  if cursor in leaders:return None,'target register enters from another block',None,None
  previous=by_address.get(cursor-2)
  if previous is None:return None,'noncontiguous listing',None,None
  w=int.from_bytes(bytes.fromhex(previous['bytes']),'little')
  if previous['instruction'].lstrip('_').startswith(('jsr ','bsr ')):return None,'intervening call',None,None
  if n in written_registers(previous):
   if w>>12==13 and ((w>>8)&15)==n:
    literal=((cursor-2+4)&~3)+(w&255)*4
    if not base<=literal<=base+len(image)-4:return None,'literal outside main image',None,None
    return struct.unpack_from('<I',image,literal-base)[0],'same-block PC-relative pointer',cursor-2,literal
   return None,'target assigned without constant pointer proof',None,None
  cursor-=2
 return None,'backward proof limit',None,None
calls=[]
for edge in direct:
 if edge['kind']=='call':calls.append({'site':edge['site'],'kind':'bsr','target':edge['target'],'resolution':'raw signed branch offset'})
for row in rows:
 a=int(row['address'],16);w=int.from_bytes(bytes.fromhex(row['bytes']),'little')
 if w&0xf0ff not in (0x400b,0x402b):continue
 target,reason,load_site,literal_address=resolve(a,(w>>8)&15)
 calls.append({'site':a,'kind':'jsr' if w&255==11 else 'jmp','target':target,'resolution':reason,'literal_load_site':load_site,'literal_address':literal_address})
for c in calls:
 if c['kind']=='jmp' and c['target'] is not None:leaders.add(c['target'])
for c in calls:
 if c['kind'] in ('jsr','jmp'):
  word=int.from_bytes(bytes.fromhex(by_address[c['site']]['bytes']),'little')
  target,reason,load_site,literal_address=resolve(c['site'],(word>>8)&15)
  c.update({'target':target,'resolution':reason,'literal_load_site':load_site,'literal_address':literal_address})
for c in calls:
 c['entry_span']=owner(c['site']);c['indexed_function']=index.get(c['target'],{}).get('name');c['registered_native']=native.get(c['target'])
spans=[]
for i,entry in enumerate(entries):
 end=entries[i+1] if i+1<len(entries) else max(by_address)+2
 inside=[r for r in rows if entry<=int(r['address'],16)<end]
 edges=[e for e in direct if entry<=e['site']<end and not entry<=e['target']<end]
 deps=[c for c in calls if c['entry_span']==entry]
 spans.append({'entry':entry,'selectors':selectors[entry],'span_end_exclusive':end,'instruction_rows':len(inside),'instruction_bytes':sum(int(r['length']) for r in inside),'call_sites':len(deps),'unresolved_transfer_sites':sum(c['target'] is None for c in deps),'outgoing_direct_edges':edges})
out=P/'analysis/main-dispatch-enrichment'
with (out/'entry_spans.tsv').open('w',newline='') as f:
 writer=csv.writer(f,delimiter='\t');writer.writerow(['entry','selectors','span_end_exclusive','instruction_rows','call_sites','unresolved_transfer_sites'])
 for span in spans:writer.writerow([hex(span['entry']),','.join(map(str,span['selectors'])),hex(span['span_end_exclusive']),span['instruction_rows'],span['call_sites'],span['unresolved_transfer_sites']])
paths=[Path(__file__),listing,table_path,image_path,index_path,registry_path]
report={'entry_spans':spans,'transfers':calls,'direct_edges':direct,'entry_count':len(spans),'selector_count':sum(len(s['selectors']) for s in spans),'export_instruction_rows':len(rows),'prefix_instruction_rows':sum(int(r['address'],16)<entries[0] for r in rows),'transfer_sites':len(calls),'resolved_transfer_sites':sum(c['target'] is not None for c in calls),'unresolved_transfer_sites':sum(c['target'] is None for c in calls),'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},'scope':'Conservative entry-address spans, not independent semantic functions or complete case CFGs. Raw direct branches and same-basic-block PC-relative transfer literals only. Unresolved transfers retained explicitly; callback behavior is not recovered by identifying a target. Cross-span edges and shared exits retained. No routine-completion or whole-game credit.'}
assert sum(s['instruction_rows'] for s in spans)+report['prefix_instruction_rows']==len(rows)
(out/'case_dependency_map.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(spans),'entry spans;',len(calls),'transfer sites;',report['resolved_transfer_sites'],'resolved conservatively;',report['unresolved_transfer_sites'],'left explicit')
