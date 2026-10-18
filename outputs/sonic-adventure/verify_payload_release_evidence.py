"""Validate payload-release dependency exports against original retail opcodes."""
import csv,hashlib,json
from verify_startup import ROOT

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();items=[]
 for entry in ['8c663950','8c66385a','8c664940','8c66497c','8c6652c0','8c665478','8c665440']:
  p=out/f'analysis/instructions/{entry}.tsv';rows=list(csv.DictReader(p.open(),delimiter='\t'))
  for row in rows:
   offset=int(row['address'],16)-0x8c010000
   assert int.from_bytes(binary[offset:offset+2],'little')==int(row['opcode'],16),row
  calls=[{'address':row['address'],'instruction':row['instruction'],'reference':row['references']} for row in rows if 'CALL:' in row['references']]
  items.append({'entry':'0x'+entry,'instructions_verified':len(rows),'export_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'calls':calls})
 report={'binary_sha256':hashlib.sha256(binary).hexdigest(),'exports':items,'status':'Verified instruction evidence; all seven bodies still require source recovery.','observations':{'8c66385a':'Lookup writes stack result at r5; absent first result returns 7. Paired allocation flag bit 2 selects address +/- global 8c67c2d4 according to signed halfword lookup result. Missing partner releases first allocation and returns 0. Partner flag bit 4 can clear first allocation bit 4; other branch releases both.','8c663950':'Metadata service fills 20 stack bytes. Traverses global linked list 8c67c308, compares masked header type and payload slot address. Clears selected slot; only when all slots are empty does it release block allocation, unlink block and mark pool record unused. No matching block returns 7.','8c665478':'Unlinks node using predecessor search and node+4 next pointer; caller is expected to supply an existing node.','8c665440':'Scans 0x13000-byte pool at 8c8c03e4 with stride 0x4c and clears header bit 0 of matching address.'}}
 (out/'analysis/payload_release_dependency_evidence.json').write_text(json.dumps(report,indent=2)+'\n')
 print(f'{sum(x["instructions_verified"] for x in items)} original instructions validated across {len(items)} payload dependency exports')
if __name__=='__main__':main()
