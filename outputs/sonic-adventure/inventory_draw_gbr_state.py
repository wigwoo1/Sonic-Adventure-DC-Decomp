"""Byte-check static GBR initialization and reference provenance; no recovery credit."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 binary_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes()
 word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
 long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
 loadpc=0x8c036f62;w=word(loadpc);pool=((loadpc+4)&~3)+(w&255)*4
 assert w>>12==13 and w>>8&15==1 and word(loadpc+2)==0x411e and long(pool)==0x8c8ffe00
 directory=PROJECT/'analysis/draw-gbr-state'
 transfers=list(csv.DictReader((directory/'transfers.tsv').open(),delimiter='\t'))
 refs=list(csv.DictReader((directory/'references.tsv').open(),delimiter='\t'));calls=[]
 for r in refs:
  if r['resource_address']!='8c036f60' or r['depth']!='1' or r['reference_type']!='COMPUTED_CALL':continue
  a=int(r['reference_from'],16);loadpc2=a-2;w=word(loadpc2)
  assert w>>12==13 and word(a)&0xf0ff==0x400b and w>>8&15==word(a)>>8&15
  literal=((loadpc2+4)&~3)+(w&255)*4;assert long(literal)==0x8c036f60
  calls.append(dict(call_pc=hex(a),load_pc=hex(loadpc2),literal_pc=hex(literal),indexed_owner=r['function'] or None,byte_checked=True,reachability_proved=False))
 files=[Path(__file__),binary_path,PROJECT/'ghidra/ExportGbrStateWriters.java',PROJECT/'ghidra/ExportResourceReferences.java',PROJECT/'ghidra/export_draw_gbr_state.ps1',PROJECT/'ghidra/export_draw_gbr_references.ps1',directory/'transfers.tsv',directory/'reference_targets.tsv',directory/'references.tsv']
 report=dict(analyzed_gbr_transfer_records=len(transfers),direct_register_gbr_loads=[r['address'] for r in transfers if r['instruction'].startswith('ldc ')],renderer_initializer='0x8c036f60',gbr_load_pc=hex(loadpc+2),gbr_pointer_literal_pc=hex(pool),original_renderer_record=hex(long(pool)),byte_checked_initializer_call_sites=calls,clip_rectangle_writer_candidates=[r for r in refs if r['resource_address']=='8c88f600' and r['reference_type']=='WRITE'],new_recovery_credit=0,scope='Static analyzed-main-image GBR transfers and initializer/clip-global references; load/literal/direct-call bytes checked. Does not establish exhaustive code boundaries, scheduler reachability or natural startup completion.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_gbr_state_provenance.json').write_text(json.dumps(report,indent=2)+'\n')
 print('GBR provenance:',len(transfers),'analyzed transfers;',len(calls),'byte-checked initializer call sites; zero recovery credit.')
if __name__=='__main__':main()
