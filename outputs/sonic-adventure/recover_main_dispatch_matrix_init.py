"""Validate the exact instruction inventory for structured matrix initialization."""
import csv,hashlib,json
from pathlib import Path

PROJECT=Path(__file__).resolve().parent
ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
START,END=0x8C057B40,0x8C057BAC
binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
listing=PROJECT/'analysis/main-dispatch-matrix-init/8c057b40_instructions.tsv'
rows={}
for row in csv.DictReader(listing.open(),delimiter='\t'):
    a=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
    assert len(raw)==int(row['length'])==2 and START<=a<END and a not in rows
    assert binary[a-0x8C010000:a-0x8C010000+2]==raw
    rows[a]=int.from_bytes(raw,'little')
assert len(rows)==50
seen=set();todo=[START];edges=set()
while todo:
    a=todo.pop()
    if a in seen:continue
    assert a in rows,hex(a)
    seen.add(a);w=rows[a]
    if w==11:
        assert a+2 in rows;seen.add(a+2)
    elif w>>8==0x8B:
        d=w&255;d-=256 if d&128 else 0
        for target in (a+2,a+4+2*d):edges.add((a,target));todo.append(target)
    else:
        assert w>>12 not in (0xA,0xB) and w&0xF0FF!=0x400B
        todo.append(a+2)
assert seen==set(rows) and len(edges)==2
literals={hex(a):hex(int.from_bytes(binary[a-0x8C010000:a-0x8C010000+4],'little')) for a in (0x8C057B50,0x8C057B54,0x8C057BAC)}
assert literals=={'0x8c057b50':'0x8c88f698','0x8c057b54':'0x8c88f69c','0x8c057bac':'0x8c88f5f8'}
source=PROJECT/'source/sa_main_dispatch_matrix_init.c'
files=[BINARY,listing,source,Path(__file__)]
report={'name':'sa_main_dispatch_matrix_init_8c057b40','address':hex(START),'end_exclusive':hex(END),
        'instruction_rows':50,'instruction_bytes':100,'source':'source/sa_main_dispatch_matrix_init.c',
        'reachable_addresses':[hex(a) for a in sorted(seen)],'conditional_edges':[[hex(a),hex(b)] for a,b in sorted(edges)],
        'call_sites':[],'literals':literals,
        'retail_span_sha256':hashlib.sha256(binary[START-0x8C010000:END-0x8C010000]).hexdigest(),
        'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
        'scope':'Raw inventory/branch proof for 50-instruction self-contained matrix push/initialization. Structured C behavior is tested separately; no external callees.'}
(PROJECT/'analysis/main_dispatch_matrix_init_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('50 byte-checked matrix-init rows; 2 conditional edges; 0 calls; 3 literal values')
