import bisect,csv,json,struct
from pathlib import Path
p=Path(__file__).resolve().parent;data=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
rows=list(csv.DictReader((p/'analysis/ghidra-all/function_index.tsv').open(),delimiter='\t'));starts=[int(row['address'],16) for row in rows]
targets=[0x8c66980a,0x8c669e38,0x8c66a61a,0x8c66ab2a,0x8c6698d6,0x8c669f2e,0x8c66a440,0x8c66a714]
records=[]
for target in targets:
 needle=struct.pack('<I',target);position=0
 while (position:=data.find(needle,position))>=0:
  address=position+0x8c010000;index=bisect.bisect_right(starts,address)-1
  records.append(dict(target=hex(target),literal_address=hex(address),preceding_indexed_function=rows[index]['address'],indexed_body_bytes=rows[index]['body_bytes'],scope='Raw literal occurrence; preceding indexed function is navigation hint, not proven owner/caller.'))
  position+=1
(p/'analysis/audio_format_literal_references.json').write_text(json.dumps(records,indent=2)+'\n')
for record in records:print(hex(int(record['target'],16)),record['literal_address'],record['preceding_indexed_function'])
