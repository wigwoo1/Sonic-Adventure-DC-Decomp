"""Triage original Ghidra draft warnings without treating drafts as recovered code."""
import csv,hashlib,json,re,struct
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
paths=[Path(__file__)];drivers=[]
for name in ('MANATEE.DRV','MASARU.DRV'):
 draft=P/'analysis/arm-sound-drivers'/f'{name}.arm.bin/initial_pseudocode.c'
 listing=P/'analysis/arm-sequencer-enrichment'/f'{name}.arm.bin/instructions.tsv'
 paths += [draft,listing];rows=list(csv.DictReader(listing.open(),delimiter='\t'));known={int(r['address'],16):r for r in rows}
 address=None;warnings=[]
 for line_no,line in enumerate(draft.read_text().splitlines(),1):
  marker=re.search(r'/\* Address: ([0-9a-f]+) \*/',line)
  if marker:address=int(marker[1],16)
  if 'WARNING:' not in line:continue
  instruction=re.search(r'(?:at |ram,)(0x[0-9a-f]+)',line);site=int(instruction[1],16) if instruction else None
  text=line.strip();classification='requires semantic review'
  if 'stack pointer' in text:classification='stack/context warning; exception and reset proofs are separate evidence'
  elif 'overlap smaller symbols' in text:classification='draft symbol aliasing; native source uses explicit byte/word memory accesses'
  elif 'Removing unreachable' in text:classification='warning site is exported; decompiler removal is not instruction absence'
  elif 'infinite loop' in text:classification='intentional wait candidate; bounded reset-wait evidence exists separately'
  elif 'jumptable' in text and site in known and known[site]['instruction']=='mov pc,lr':classification='return instruction misclassified as jump table; caller/stack proof required separately'
  warnings.append({'function_entry':hex(address) if address is not None else None,'draft_line':line_no,'warning':text,'site':hex(site) if site is not None else None,'site_instruction':known[site]['instruction'] if site in known else None,'site_exported':site in known if site is not None else None,'classification':classification})
 missing=[];branch_count=0
 for row in rows:
  a=int(row['address'],16);w=int.from_bytes(bytes.fromhex(row['bytes']),'little')
  if w&0x0e000000!=0x0a000000:continue
  branch_count+=1;delta=w&0xffffff
  if delta&0x800000:delta-=1<<24
  target=(a+8+4*delta)&0xffffffff
  if target not in known:missing.append({'address':hex(a),'instruction':row['instruction'],'target':hex(target)})
 drivers.append({'driver':name,'warning_count':len(warnings),'warnings':warnings,'direct_branch_instructions':branch_count,'direct_targets_without_exported_instruction':missing})
related={}
for relative in ('analysis/arm_sound_native_site_audit.json','analysis/arm_sound_release_dispatch_verification.json'):
 path=P/relative;related[relative]=sha(path)
report={'drivers':drivers,'related_evidence_sha256':related,'sha256':{str(p):sha(p) for p in paths},'scope':'Triage of original draft warnings and independent raw ARM branch-target decoding against enriched export snapshots. Classification is explanatory, not a claim that warnings, dynamic targets, all paths or original code discovery are resolved. Symbol/stack/indirect-return warnings must not be counted as missing routines or used as a whole-game denominator. No milestone credit.'}
(P/'analysis/arm_sound_warning_audit.json').write_text(json.dumps(report,indent=2)+'\n')
for d in drivers:print(d['driver'],d['warning_count'],'draft warnings;',len(d['direct_targets_without_exported_instruction']),'direct branch targets absent from exports')
