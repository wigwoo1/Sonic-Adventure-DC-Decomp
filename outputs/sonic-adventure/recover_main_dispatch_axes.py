"""Inventory existing axis C against fresh raw listings without rewriting it."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
old_listing=PROJECT/'analysis/instructions/axis_rotation_helpers.tsv'
old={int(r['address'],16):int(r['opcode'],16) for r in csv.DictReader(old_listing.open(),delimiter='\t')}
legacy=json.loads((PROJECT/'analysis/axis_rotation_native_verification.json').read_text())
records=[];files=[BINARY,Path(__file__),old_listing,PROJECT/'ghidra/translate_axis_rotation.py',PROJECT/'analysis/axis_rotation_native_verification.json']
for axis,start,end,count in (('x',0x8C639EC8,0x8C639F58,72),('y',0x8C639F5C,0x8C639FF2,75),('z',0x8C63A1CC,0x8C63A262,75)):
    listing=PROJECT/f'analysis/main-dispatch-axes/{start:08x}_instructions.tsv';files.append(listing)
    rows={}
    for row in csv.DictReader(listing.open(),delimiter='\t'):
        a=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
        assert len(raw)==int(row['length'])==2 and start<=a<end and a not in rows
        assert binary[a-0x8C010000:a-0x8C010000+2]==raw
        rows[a]=int.from_bytes(raw,'little');assert rows[a]==old[a]
    assert len(rows)==count and set(rows)==set(range(start,end,2))
    seen=set();slots=set();edges=set();todo=[start];vectors={}
    while todo:
        a=todo.pop()
        if a in seen:continue
        assert a in rows;seen.add(a);w=rows[a]
        if w==11:seen.add(a+2);slots.add(a+2);assert a+2 in rows
        elif w>>8==0x8F:
            d=w&255;d-=256 if d&128 else 0
            seen.add(a+2);slots.add(a+2);assert a+2 in rows
            for target in (a+4,a+4+2*d):edges.add((a,target));todo.append(target)
        else:
            assert w>>12 not in (0xA,0xB) and w&0xF0FF!=0x400B
            if w in (0xF0FD,0xF1FD,0xF5FD):vectors[a]=w
            todo.append(a+2)
    assert seen==set(rows) and len(edges)==2 and len(vectors)==6
    source=PROJECT/f'source/sa_matrix_rotate_{axis}.c';files.append(source)
    sha=hashlib.sha256(source.read_bytes()).hexdigest()
    assert sha==legacy['dependency_source_sha256'][source.name]
    labels={int(v,16) for v in re.findall(r'^L([0-9a-f]+):',source.read_text(),re.M)}
    assert labels==seen-slots
    split=max(b for a,b in edges)
    records.append({'name':f'sa_matrix_rotate_{axis}','axis':axis,'address':hex(start),'end_exclusive':hex(end),
        'instruction_rows':count,'instruction_bytes':count*2,'source':f'source/{source.name}','source_sha256':sha,
        'retail_span_sha256':hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest(),
        'reachable_addresses':[hex(a) for a in sorted(seen)],'conditional_edges':[[hex(a),hex(b)] for a,b in sorted(edges)],
        'vector_instruction_sites':{hex(a):hex(w) for a,w in sorted(vectors.items())},
        'implicit_vector_sites':[hex(a) for a in sorted(vectors) if a<split],
        'explicit_vector_sites':[hex(a) for a in sorted(vectors) if a>=split],
        'reused_existing_source':True})
report={'routines':records,'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
        'scope':'Fresh raw-byte/branch/source-label inventory for three existing axis C bodies. No source regeneration or numerical FSCA/FTRV fidelity claim; connected behavior is verified separately.'}
(PROJECT/'analysis/main_dispatch_axes_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
print('222 rotation rows; 6 conditional edges; 18 vector instruction sites; 3 existing C sources reused')
