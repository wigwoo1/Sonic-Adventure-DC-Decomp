"""Inventory the reached next control body without awarding recovery credit."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 folder=P/'analysis/movie-hardware-control';listing=folder/'8c6046c0_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 for row in rows:
  a=int(row['address'],16);n=int(row['length']);assert raw[a-0x8c010000:a-0x8c010000+n].hex()==row['bytes']
 diagnostic=list(csv.DictReader((folder/'diagnostics.tsv').open(),delimiter='\t'));assert len(diagnostic)==1 and diagnostic[0]['completed']=='true' and not diagnostic[0]['error']
 literals={hex(a):hex(int.from_bytes(raw[a-0x8c010000:a-0x8c010000+4],'little')) for a in (0x8c604778,0x8c60477c,0x8c604780)}
 assert literals=={'0x8c604778':'0xa0702c00','0x8c60477c':'0x8c10d460','0x8c604780':'0x8c10d770'}
 files=[Path(__file__),binary,listing,folder/'diagnostics.tsv',folder/'8c6046c0_draft.c',P/'ghidra/export_movie_hardware_control.ps1'];services=[]
 service_folder=P/'analysis/movie-hardware-services';service_diagnostics=service_folder/'diagnostics.tsv';notes=list(csv.DictReader(service_diagnostics.open(),delimiter='\t'));assert len(notes)==2 and all(n['completed']=='true' and not n['error'] for n in notes)
 files.extend((service_diagnostics,P/'ghidra/export_movie_hardware_services.ps1'))
 for address,count in (('8c10d460',148),('8c10d770',194)):
  path=service_folder/f'{address}_instructions.tsv';service_rows=list(csv.DictReader(path.open(),delimiter='\t'));assert len(service_rows)==count
  for row in service_rows:
   a=int(row['address'],16);n=int(row['length']);assert raw[a-0x8c010000:a-0x8c010000+n].hex()==row['bytes']
  targets=sorted({target for row in service_rows if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')) for target in row['flows'].split()})
  services.append(dict(entry='0x'+address,instruction_rows=count,listed_call_targets=['0x'+target for target in targets],recovery_credit=0));files.extend((path,service_folder/f'{address}_draft.c'))
 report=dict(entry='0x8c6046c0',instruction_rows=len(rows),newly_registered_entries=0,retail_span_sha256=hashlib.sha256(raw[0x6046c0-0x10000:0x604702-0x10000]).hexdigest(),literal_inputs=literals,call_targets=['0x8c10d460','0x8c10d770'],scope='Read-only Ghidra export, byte-checked against the original executable. The reached body passes a0702c00 and a stack-local word through two read/write services. This is an unrecovered control dependency, not a proven video routine or a substituted service result.',limitations=['The Ghidra draft is research text, not verified native C. Both services and the actual hardware behavior need recovery and comparison before connecting this body.'],sha256={str(p):digest(p) for p in files})
 report['service_research_exports']=services;report['lower_service_frontier']=sorted({target for service in services for target in service['listed_call_targets']})
 (P/'analysis/movie_hardware_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Byte-checked next hardware control frontier:',len(rows),'rows; service drafts',sum(s['instruction_rows'] for s in services),'rows; zero credit')
if __name__=='__main__':main()
