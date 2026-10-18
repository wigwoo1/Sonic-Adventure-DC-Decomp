"""Byte-check next dependencies; exports alone are not recovered source."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 binary_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes()
 assert digest(binary_path)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 records=[];files=[Path(__file__),binary_path,PROJECT/'ghidra/export_main_dispatch_uv_constant_lower.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']
 for entry,count,end in ((2355176640, 553, 2355177818), (2355184398, 295, 2355185024), (2355191912, 504, 2355192948)):
  listing=PROJECT/f'analysis/main-dispatch-uv-constant-lower/{entry:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
  assert len(rows)==count and int(rows[0]['address'],16)==entry and int(rows[-1]['address'],16)+2==end
  addresses=set()
  for row in rows:
   address=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
   assert len(raw)==int(row['length'])==2 and entry<=address<end and address not in addresses
   assert binary[address-0x8C010000:address-0x8C010000+2]==raw
   addresses.add(address)
  records.append(dict(address=hex(entry),end_exclusive=hex(end),instruction_rows=count,listing=listing.relative_to(PROJECT).as_posix(),retail_span_sha256=hashlib.sha256(binary[entry-0x8C010000:end-0x8C010000]).hexdigest(),calls=[dict(pc='0x'+r['address'],instruction=r['instruction'],ghidra_flow_hint=r['flows']) for r in rows if r['instruction'].lstrip('_').startswith(('jsr','jmp'))]))
  files.extend((listing,PROJECT/f'analysis/main-dispatch-uv-constant-lower/{entry:08x}_draft.c'))
 files.append(PROJECT/'analysis/main-dispatch-uv-constant-lower/diagnostics.tsv')
 report=dict(routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),sha256={str(p):digest(p) for p in files},new_behavior_verified_entries=0,scope='Read-only next-dependency instruction export, bounded spans and exact original bytes only. Draft pseudocode, flow hints and function names do not establish recovered source or runtime behavior.')
 (PROJECT/'analysis/main_dispatch_uv_constant_lower_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Next primitive family: 1352 original rows byte-checked; zero recovery credit.')
if __name__=='__main__':main()
