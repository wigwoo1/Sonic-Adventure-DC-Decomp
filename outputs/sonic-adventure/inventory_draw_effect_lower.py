"""Byte-checked next render-state/texture/visibility proposals; no source credit."""
import csv,hashlib,json,struct
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));registered={(r.get('module','1ST_READ.BIN'),int(r['address'],16)):r for r in registry['routines']}
 binary_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes();base=0x8c010000;directory=PROJECT/'analysis/draw-effect-lower';diagnostics=directory/'diagnostics.tsv';notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'))
 assert len(notes)==3 and all(r['completed']=='true' and not r['error'] for r in notes)
 files=[Path(__file__),registry_path,binary_path,diagnostics,PROJECT/'ghidra/export_draw_effect_lower.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java'];records=[]
 for note in notes:
  address=int(note['address'],16);listing=directory/f'{address:08x}_instructions.tsv';draft=directory/f'{address:08x}_draft.c';rows=list(csv.DictReader(listing.open(),delimiter='\t'));calls=[]
  assert len(rows)*2==int(note['body_bytes'])
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-base:a-base+2]==bytes.fromhex(row['bytes'])
   if not row['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp ','braf ','bsrf ')):continue
   targets=[]
   for text in row['flows'].strip().split():
    t=int(text,16);old=registered.get(('1ST_READ.BIN',t));targets.append(dict(address=hex(t),registered=bool(old),source=old['source'] if old else None))
   calls.append(dict(site=row['address'],instruction=row['instruction'],static_targets=targets,computed_target_exhaustive=False))
  old=registered.get(('1ST_READ.BIN',address));record=dict(entry=hex(address),instruction_rows=len(rows),listing=listing.relative_to(PROJECT).as_posix(),draft=draft.relative_to(PROJECT).as_posix(),currently_registered=bool(old),registered_source=old['source'] if old else None,transfer_sites=calls)
  if address==0x8c654e60:
   exported={int(r['address'],16) for r in rows};switches=[]
   for pc,table in ((0x8c654fc0,0x8c654fcc),(0x8c6550a2,0x8c6550a8)):
    assert binary[pc-base:pc-base+4]==bytes.fromhex('23000900')
    targets=[pc+4+struct.unpack_from('<h',binary,table-base+i*2)[0] for i in range(12)]
    assert all(address<=t<=max(exported) and not t&1 for t in targets)
    switches.append(dict(pc=hex(pc),table=hex(table),selector_count=12,targets=[hex(t) for t in targets],distinct_targets=len(set(targets)),missing_case_heads=[hex(t) for t in sorted(set(targets)-exported)],bound_evidence='Original CMP/HS against 12 before table access; signed word offset loaded before BRAF.'))
   record['decoded_original_switches']=switches;record['complete_cfg_verified']=False;record['required_next_step']='Expand read-only CFG from every decoded switch target, retain original snapshot, byte-check all added instructions, then translate/test all valid selectors.'
  # Preserve this historical proposal snapshot; link separate expanded proof.
  if old and old.get('source')=='source/sa_draw_render_headers.c':
   proof_path=PROJECT/'analysis/draw_render_headers_verification.json';proof=json.loads(proof_path.read_text());item=next(r for r in proof['coverage_by_routine'] if int(r['address'],16)==address)
   assert not item['missing_rows'] and not item['missing_edges']
   record['expanded_native_evidence']=dict(report=proof_path.relative_to(PROJECT).as_posix(),rows_covered=item['rows_covered'],conditional_outcomes=item['conditional_edges_covered'],unreachable_outcomes=item['unreachable_edges'],registration_credit_here=0)
   record['required_next_step']='Expanded original body is separately checked and registered; preserve this smaller historical proposal snapshot. Recover missing services identified in overlay_draw_render_headers_verification.json.'
   files.append(proof_path)
  records.append(record);files.extend((listing,draft))
 report=dict(routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),newly_recovered_entries=0,reused_routine_credit=0,parent_recovery_credit=0,scope='Three existing main-executable function proposals exported read-only and every listed instruction byte checked against 1ST_READ. Static transfer annotations and drafts only; no complete computed-target, native-source or behavior credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_effect_lower_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Next lower renderer proposals:',len(records),'bodies /',report['instruction_rows'],'byte-checked rows; zero recovery credit.')
if __name__=='__main__':main()
