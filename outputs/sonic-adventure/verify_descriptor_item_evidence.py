"""Validate exported resource-release instructions against the local retail binary."""
import csv,hashlib,json,struct
from verify_startup import ROOT

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();exports=[]
 for address in ['8c64ddc0','8c64f206','8c653d68','8c64e720']:
  p=root/f'analysis/instructions/{address}.tsv';rows=list(csv.DictReader(p.open(),delimiter='\t'))
  for row in rows:
   offset=int(row['address'],16)-0x8c010000
   assert struct.unpack_from('<H',binary,offset)[0]==int(row['opcode'],16),row
  exports.append({'entry':'0x'+address,'instruction_count':len(rows),'export_sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
 for offset,value in [(0x63dee8,0xff00),(0x63deec,0x8c64f206),(0x63def0,0x8c653d68),(0x63def4,0x8c67348c),(0x63def8,0x8c673484),(0x63defc,0x8c673488),(0x63df00,0x8c64e720),(0x63df04,0x8c88f7b0)]:assert struct.unpack_from('<I',binary,offset)[0]==value
 assert struct.unpack_from('<4H',binary,0x63f22e)==(0x500,0x600,0x700,0x800)
 report={'binary_sha256':hashlib.sha256(binary).hexdigest(),'exports':exports,'validated_literals':12,'recovery_status':'Instruction evidence only; these four functions have not yet been registered as recovered source.','observations':{'8c64ddc0':'Tests resource+0x10 bit 1; calls type classifier with (resource+0x18 >>16)&0xff00. Classifier result 1 scans 68-byte records at global 8c673484, count 8c673488, matching resource identifier. Other types decrement a nonzero halfword counter at +0x40. Clearing active bit uses flags & ~2. Final error path writes 1 to 8c88f7b0 and returns -1; other terminal paths return 1. Nested calls and branch-specific SR effects require source verification.','8c64f206':'Exact equality classifier for 0x500,0x600,0x700,0x800; returns 1 or 0.','8c64e720':'Clears descriptor fields and sets identifiers to -1, then computes its 68-byte table index using another service. Full downstream behavior still unrecovered.'}}
 (root/'analysis/descriptor_item_dependency_evidence.json').write_text(json.dumps(report,indent=2)+'\n')
 print(f'{sum(e["instruction_count"] for e in exports)} exported instructions and 12 literals matched retail bytes')
if __name__=='__main__':main()
