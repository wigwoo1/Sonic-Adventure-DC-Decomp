"""Find retail SH4 PC-relative loads of decode lifecycle function pointers.

This is a binary reference inventory, not proof that a candidate executes.
Only loads within indexed function spans are retained. Delay-slot calls and
register forwarding require instruction export before claiming call edges.
"""
import bisect,csv,json,struct
from pathlib import Path
root=Path(__file__).resolve().parents[2]
out=root/'outputs/sonic-adventure'
binary=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
base=0x8c010000
targets={0x8c644c1a:'mono_request',0x8c644c38:'stereo_request',
         0x8c644c56:'activate',0x8c644c68:'reset',
         0x8c644c7e:'acknowledge',0x8c644cb0:'process'}
functions=[]
with (out/'analysis/ghidra-all/function_index.tsv').open(newline='') as file:
    for row in csv.DictReader(file,delimiter='\t'):
        values=list(row.values())
        functions.append((int(values[0],16),int(values[2]),values[1]))
functions.sort();starts=[f[0] for f in functions]
literal_offsets={offset:targets[struct.unpack_from('<I',binary,offset)[0]]
                 for offset in range(0,len(binary)-3,4)
                 if struct.unpack_from('<I',binary,offset)[0] in targets}
records=[]
for offset in range(0,len(binary)-1,2):
    word=struct.unpack_from('<H',binary,offset)[0]
    if word>>12!=0xd:continue
    address=base+offset;literal=((address+4)&~3)+(word&255)*4
    if literal-base not in literal_offsets:continue
    index=bisect.bisect_right(starts,address)-1
    if index<0:continue
    start,size,name=functions[index]
    if address>=start+size:continue
    records.append(dict(load_address=f'0x{address:08x}',opcode=f'{word:04x}',
                        destination_register=(word>>8)&15,literal_address=f'0x{literal:08x}',
                        target=literal_offsets[literal-base],function_address=f'0x{start:08x}',
                        function_name=name,indexed_size=size,
                        following_words=[f'{struct.unpack_from("<H",binary,p)[0]:04x}'
                                         for p in range(offset+2,min(offset+18,len(binary)-1),2)]))
report=dict(scope='Main retail executable only. Aligned pointer literals and SH4 mov.l PC-relative loads within indexed function spans; candidate references require disassembly/control-flow validation, not proven calls. Overlay callers and indirect tables unresolved.',
            candidate_loads=len(records),references=records)
(out/'analysis/decode_request_literal_references.json').write_text(json.dumps(report,indent=2)+'\n')
for record in records:print(record['function_address'],record['load_address'],record['target'],record['following_words'])
print(len(records),'candidate loads')
