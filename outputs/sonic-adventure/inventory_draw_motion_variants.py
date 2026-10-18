"""Byte-check ten subsequent motion/draw proposals; no native recovery credit."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 binary_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes();base=0x8c010000
 directory=PROJECT/'analysis/draw-motion-variants';diagnostics=directory/'diagnostics.tsv';notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'));assert len(notes)==7 and all(r['completed']=='true' and not r['error'] for r in notes)
 proposals=[(int(r['address'],16),directory,int(r['body_bytes'])//2) for r in notes]
 proposals.extend((a,PROJECT/'analysis'/folder,count) for a,folder,count in ((0x8c0409c0,'overlay-draw-missing-helpers',23),(0x8c041c92,'overlay-draw-missing-helpers',47),(0x8c5fde20,'overlay-draw-unindexed-targets',277)))
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));registered={int(r['address'],16) for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'}
 files=[Path(__file__),registry_path,binary_path,diagnostics,PROJECT/'ghidra/export_draw_motion_variants.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java',PROJECT/'ghidra/export_overlay_draw_missing_helpers.ps1',PROJECT/'ghidra/export_overlay_draw_unindexed_targets.ps1'];records=[];all_targets=set()
 for entry,folder,count in proposals:
  listing=folder/f'{entry:08x}_instructions.tsv';draft=folder/f'{entry:08x}_draft.c';rows=list(csv.DictReader(listing.open(),delimiter='\t'));body={int(r['address'],16) for r in rows};assert len(rows)==count
  transfers=[];outside=[]
  for row in rows:
   a=int(row['address'],16);raw=bytes.fromhex(row['bytes']);assert len(raw)==int(row['length'])==2 and binary[a-base:a-base+2]==raw
   text=row['instruction'].lstrip('_');w=int.from_bytes(raw,'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f) or w>>12==0xa:
    bits=12 if w>>12==0xa else 8;offset=w&((1<<bits)-1);offset=offset-(1<<bits) if offset>>(bits-1) else offset;target=a+4+offset*2
    if target not in body:outside.append(dict(pc=hex(a),target=hex(target)))
   if text.startswith(('jsr ','bsr ','jmp ','braf ','bsrf ')):
    targets=[int(t,16) for t in row['flows'].strip().split()];all_targets.update(targets);transfers.append(dict(pc=hex(a),instruction=text,annotated_targets=[dict(address=hex(t),registered=t in registered) for t in targets],computed_target_exhaustive=False))
  records.append(dict(entry=hex(entry),instruction_rows=len(rows),byte_checked=True,listing=listing.relative_to(PROJECT).as_posix(),draft=draft.relative_to(PROJECT).as_posix(),registered=entry in registered,nonlocal_transfers=transfers,direct_branch_targets_outside_proposal=outside,native_recovery_credit=0));files.extend((listing,draft))
 entries={a for a,_,_ in proposals};report=dict(routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),unregistered_static_targets_outside_family=[hex(a) for a in sorted(all_targets-entries-registered)],newly_recovered_entries=0,reused_routine_credit=0,parent_recovery_credit=0,scope='Three preserved outer drawing boundaries plus seven direct dependency proposals. Original bytes and direct branch annotations checked; drafts only. 8c5fdbec is temporarily discovered in the read-only project; global indexes remain unchanged. Static targets are not exhaustive computed-call proof. No native behavior, complete-CFG, numerical or pixel credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_motion_variants_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Next motion/draw proposals:',len(records),'bodies /',report['instruction_rows'],'byte-checked rows; zero recovery credit.')
 print('Direct branches outside proposals:',sum(len(r['direct_branch_targets_outside_proposal']) for r in records),'further unregistered static targets:',len(report['unregistered_static_targets_outside_family']))
if __name__=='__main__':main()
