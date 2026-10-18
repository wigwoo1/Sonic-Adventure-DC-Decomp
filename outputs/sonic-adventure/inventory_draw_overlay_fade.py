"""Byte-check four upcoming overlay fade/draw proposals; no native recovery credit."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 binary_path=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN';binary=binary_path.read_bytes();base=0xc900000
 directory=PROJECT/'analysis/draw-overlay-fade';diagnostics=directory/'diagnostics.tsv';notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'));assert len(notes)==4 and all(r['completed']=='true' and not r['error'] for r in notes)
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));registered={(r.get('module','1ST_READ.BIN'),int(r['address'],16)) for r in registry['routines']}
 files=[Path(__file__),registry_path,binary_path,diagnostics,PROJECT/'ghidra/export_draw_overlay_fade.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java'];records=[]
 for note in notes:
  entry=int(note['address'],16);listing=directory/f'{entry:08x}_instructions.tsv';draft=directory/f'{entry:08x}_draft.c';rows=list(csv.DictReader(listing.open(),delimiter='\t'));body={int(r['address'],16) for r in rows};transfers=[];outside=[]
  assert len(rows)*2==int(note['body_bytes'])
  for row in rows:
   a=int(row['address'],16);raw=bytes.fromhex(row['bytes']);assert len(raw)==int(row['length'])==2 and binary[a-base:a-base+2]==raw
   text=row['instruction'].lstrip('_');w=int.from_bytes(raw,'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f) or w>>12==0xa:
    bits=12 if w>>12==0xa else 8;offset=w&((1<<bits)-1);offset=offset-(1<<bits) if offset>>(bits-1) else offset;target=a+4+offset*2
    if target not in body:outside.append(dict(pc=hex(a),target=hex(target)))
   if text.startswith(('jsr ','bsr ','jmp ','braf ','bsrf ')):transfers.append(dict(pc=hex(a),instruction=text,annotated_flows=row['flows'].strip(),targets=[dict(address=hex(int(t,16)),registered=(('AL_MAIN.BIN' if int(t,16)<0x10000000 else '1ST_READ.BIN'),int(t,16)) in registered) for t in row['flows'].strip().split()]))
  records.append(dict(entry=hex(entry),instruction_rows=len(rows),byte_checked=True,listing=listing.relative_to(PROJECT).as_posix(),draft=draft.relative_to(PROJECT).as_posix(),registered=('AL_MAIN.BIN',entry) in registered,nonlocal_transfers=transfers,direct_branch_targets_outside_proposal=outside,native_recovery_credit=0))
  if records[-1]['registered']:
   proof_path=PROJECT/'analysis/draw_overlay_fade_verification.json';proof=json.loads(proof_path.read_text());item=next(r for r in proof['coverage_by_routine'] if int(r['address'],16)==entry)
   assert not item['missing_rows'] and not item['missing_edges']
   records[-1]['native_evidence']=dict(report=proof_path.relative_to(PROJECT).as_posix(),source=proof['source'],rows_covered=item['rows_covered'],conditional_outcomes=item['conditional_edges_covered'],retained_unreachable_rows=item['unreachable_rows'],retained_unreachable_edges=item['unreachable_edges'],registration_credit_here=0)
   files.append(proof_path)
  files.extend((listing,draft))
 report=dict(routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),newly_recovered_entries=0,reused_routine_credit=0,parent_recovery_credit=0,scope='Four original overlay fade/draw proposals rooted at the preserved 0c90c384 boundary. This inventory grants no native recovery credit. Read-only drafts and original instruction byte checks are retained. Names/roles remain inferred; no native behavior, numerical, complete CFG or pixel proof.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_overlay_fade_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Upcoming overlay fade/draw proposals:',len(records),'bodies /',report['instruction_rows'],'byte-checked rows; zero recovery credit.')
 print('Direct branches outside proposals:',sum(len(r['direct_branch_targets_outside_proposal']) for r in records),'nonlocal transfer sites:',sum(len(r['nonlocal_transfers']) for r in records))
if __name__=='__main__':main()
