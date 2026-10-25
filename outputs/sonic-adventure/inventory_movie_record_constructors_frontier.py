"""Pin the actual diagnostic frontier and read-only lower dependencies."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def main():
 raw_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();files=[Path(__file__),raw_path];routines=[]
 groups=(('movie-record-constructors-frontier',('8c65a2fc',),'export_movie_record_constructors_frontier.ps1'),('movie-record-diagnostics-lower',('8c10d400','8c65a33e'),'export_movie_record_diagnostics_lower.ps1'),('movie-record-diagnostics-format',('8c10dbc4',),'export_movie_record_diagnostics_format.ps1'))
 for folder,addresses,script in groups:
  base=P/'analysis'/folder;files.extend((base/'diagnostics.tsv',base/'references.tsv',P/'ghidra'/script))
  for address in addresses:
   path=base/f'{address}_instructions.tsv';rows=list(csv.DictReader(path.open(),delimiter='\t'))
   for row in rows:
    pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+int(row['length'])]==bytes.fromhex(row['bytes'])
   calls=[dict(address=r['address'],instruction=r['instruction'],targets=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp '))]
   routines.append(dict(address='0x'+address,instruction_rows=len(rows),calls=calls));files.extend((path,base/f'{address}_draft.c'))
 report=dict(reached_boundary='0x8c65a2fc',partial_parent='0x8c666b34',routines=routines,new_registry_credit=0,scope='The diagnostic wrapper is the actual reached missing entry. Formatting/output helpers are static lower dependencies. All are read-only byte-checked research exports, not executed/qualified code. The unknown output callback stays unresolved.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_record_constructors_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned diagnostic wrapper and three static lower dependencies; zero recovery credit')
if __name__=='__main__':main()
