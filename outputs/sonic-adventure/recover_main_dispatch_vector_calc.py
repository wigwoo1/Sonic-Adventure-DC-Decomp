"""Fresh byte/control inventory for existing vector-calculation C."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
START,END=0x8C638F28,0x8C638F8C
binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
listing=PROJECT/'analysis/main-dispatch-vector-calc/8c638f28_instructions.tsv'
rows={}
for row in csv.DictReader(listing.open(),delimiter='\t'):
    a=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
    assert len(raw)==int(row['length'])==2 and START<=a<END and a not in rows
    assert binary[a-0x8C010000:a-0x8C010000+2]==raw
    rows[a]=int.from_bytes(raw,'little')
assert len(rows)==50 and set(rows)==set(range(START,END,2))
seen=set();todo=[START];edges=set();vectors={};multiply=[]
while todo:
    a=todo.pop()
    if a in seen:continue
    assert a in rows;seen.add(a);w=rows[a]
    if w==11 or w>>12==0xA:
        seen.add(a+2);assert a+2 in rows
        if rows[a+2]==0xF1FD:vectors[a+2]=rows[a+2]
        if w>>12==0xA:
            d=w&4095;d-=4096 if d&2048 else 0;todo.append(a+4+2*d)
    elif w>>8 in (0x8D,0x8F):
        seen.add(a+2);assert a+2 in rows
        d=w&255;d-=256 if d&128 else 0
        for target in (a+4,a+4+2*d):edges.add((a,target));todo.append(target)
    else:
        assert w>>12!=0xB and w&0xF0FF!=0x400B
        if w in (0xF4ED,0xF8ED,0xF0ED,0xF37D):vectors[a]=w
        if w&0xF00F==0xF002:multiply.append(a)
        todo.append(a+2)
assert seen==set(rows) and len(edges)==4 and len(vectors)==6 and len(multiply)==3
assert int.from_bytes(binary[END-0x8C010000:END-0x8C010000+4],'little')==0x8C890070
source=PROJECT/'source/model_direction_transform.c'
legacy=PROJECT/'analysis/model_direction_transform_verification.json'
source_hash=hashlib.sha256(source.read_bytes()).hexdigest()
assert source_hash==json.loads(legacy.read_text())['source_sha256']
files=[BINARY,Path(__file__),listing,source,legacy]
report={'name':'sa_model_direction_transform','address':hex(START),'end_exclusive':hex(END),
        'instruction_rows':50,'instruction_bytes':100,'source':'source/model_direction_transform.c','source_sha256':source_hash,
        'reachable_addresses':[hex(a) for a in sorted(seen)],'conditional_edges':[[hex(a),hex(b)] for a,b in sorted(edges)],
        'vector_instruction_sites':{hex(a):hex(w) for a,w in sorted(vectors.items())},
        'native_multiply_sites':[hex(a) for a in sorted(multiply)],'normalization_literal':{'address':hex(END),'value':'0x8c890070'},
        'retail_span_sha256':hashlib.sha256(binary[START-0x8C010000:END-0x8C010000]).hexdigest(),
        'reused_existing_source':True,'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
        'scope':'Raw-byte/control inventory of existing vector-calculation source; four edges, six instruction-service sites and three native scalar FMUL sites. Numerical instruction services are not recovered by this inventory.'}
(PROJECT/'analysis/main_dispatch_vector_calc_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
print('50 vector-calculation rows; 4 edges; 6 vector instruction sites; 3 native FMUL sites; existing C reused')
