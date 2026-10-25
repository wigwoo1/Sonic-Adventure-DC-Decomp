"""Byte-check the next original transfer/control/callback family; no credit."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def main():
 folder=P/'analysis/movie-bulk-family';raw_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes()
 files=[Path(__file__),raw_path,P/'ghidra/export_movie_bulk_family.ps1',folder/'diagnostics.tsv',folder/'references.tsv'];routines=[]
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
 for address in ('8c65a8c8','8c65a27c','8c65a5dc','8c65aaf4','8c65a518'):
  path=folder/f'{address}_instructions.tsv';rows=list(csv.DictReader(path.open(),delimiter='\t'))
  for row in rows:
   pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+int(row['length'])]==bytes.fromhex(row['bytes'])
  canonical=[{k:r[k] for k in ('name','source','verification')} for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x'+address]
  calls=[dict(address=r['address'],instruction=r['instruction'],targets=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp '))]
  routines.append(dict(address='0x'+address,instruction_rows=len(rows),calls=calls,already_registered_canonical=canonical));files.extend((path,folder/f'{address}_draft.c'))
 report=dict(routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),new_registry_credit=0,scope='Read-only byte-checked next original bulk-transfer/control/callback/information family. Drafts and static calls are not compiled behavior or completed decompilation. Existing canonical bodies retain their names/proof/status; no duplicate credit. The 2-MiB transfer and missing memory domain still require actual original/native execution.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_bulk_family_inventory.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned five next-family bodies:',report['instruction_rows'],'rows; zero recovery credit')
if __name__=='__main__':main()
