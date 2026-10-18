"""Raw retail reference candidates, including addresses absent from function index.

Opcode-looking data is possible. This inventory is not a proven call graph.
"""
import json,struct
from pathlib import Path
p=Path(__file__).resolve().parent
b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();base=0x8c010000
target=0x8c667696
literals=[i for i in range(0,len(b)-3) if struct.unpack_from('<I',b,i)[0]==target]
loads=[]
for i in range(0,len(b)-1,2):
    word=struct.unpack_from('<H',b,i)[0]
    if word>>12!=0xd:continue
    literal=((base+i+4)&~3)+(word&255)*4-base
    if literal in literals:loads.append(dict(address=hex(base+i),opcode=hex(word),literal=hex(base+literal),following_words=[hex(struct.unpack_from('<H',b,j)[0]) for j in range(i+2,min(i+22,len(b)-1),2)]))
report=dict(target=hex(target),literal_addresses=[hex(base+i) for i in literals],candidate_loads=loads,scope='Raw main executable scan for output callback setter pointer and PC-relative loads. Includes unindexed regions; instructions versus embedded data and actual call edges require disassembly. Overlay pointers and computed registration unresolved.')
(p/'analysis/stream_output_callback_reference_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
