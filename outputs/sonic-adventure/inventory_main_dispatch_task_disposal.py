"""Revalidate existing register-preserving cleanup sources against retail exports."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
SPECS=((0x8C09865C,0x8C098786,'sa_task_delete.c','sa_task_delete',119),
       (0x8C098998,0x8C0989C8,'sa_task_data_release.c','sa_task_data_release',24),
       (0x8C098E60,0x8C098E98,'sa_task_children_delete.c','sa_task_children_delete',28),
       (0x8C098E98,0x8C098EBA,'sa_task_children_update.c','sa_task_children_update',17),
       (0x8C098B42,0x8C098C2C,'sa_heap_release.c','sa_heap_release',100))
def signed(v,b):return v-(1<<b) if v&(1<<(b-1)) else v
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    records=[];files=[Path(__file__),BINARY]
    for start,end,source,name,count in SPECS:
        listing=PROJECT/f'analysis/main-dispatch-task-disposal/{start:08x}_instructions.tsv';rows={}
        for r in csv.DictReader(listing.open(),delimiter='\t'):
            a=int(r['address'],16);raw=bytes.fromhex(r['bytes'])
            assert start<=a<end and len(raw)==int(r['length'])==2 and a not in rows
            assert binary[a-0x8C010000:a-0x8C010000+2]==raw
            rows[a]=int.from_bytes(raw,'little')
        assert len(rows)==count
        seen=set();todo=[start];edges=set();calls=set()
        while todo:
            a=todo.pop()
            if a in seen:continue
            assert a in rows,hex(a)
            seen.add(a);w=rows[a]
            if w==11 or w>>12 in (10,11) or w&0xF0FF in (0x400B,0x402B):
                assert a+2 in rows;seen.add(a+2)
                if w==11:continue
                if w>>12==10:todo.append(a+4+2*signed(w&4095,12))
                else:
                    calls.add(a)
                    if w&0xF0FF!=0x402B:todo.append(a+4)
            elif w>>8 in (0x89,0x8B,0x8D,0x8F):
                fall=a+4 if w>>8 in (0x8D,0x8F) else a+2
                if fall==a+4:seen.add(a+2)
                for target in (fall,a+4+2*signed(w&255,8)):todo.append(target);edges.add((a,target))
            else:todo.append(a+2)
        assert seen==set(rows)
        path=PROJECT/'source'/source;assert name in path.read_text()
        records.append({'address':hex(start),'end_exclusive':hex(end),'name':name,'source':'source/'+source,
            'instruction_rows':count,'reachable_addresses':[hex(a) for a in sorted(seen)],
            'conditional_edges':[[hex(a),hex(b)] for a,b in sorted(edges)],'call_sites':[hex(a) for a in sorted(calls)],
            'source_sha256':digest(path),'retail_span_sha256':hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()})
        files.extend((listing,path))
    report={'routines':records,'instruction_rows':288,'existing_sources_reused':True,
        'sha256':{str(p):digest(p) for p in files},'scope':'Retail-byte and raw reachability revalidation of five existing native sources. This is not instruction-by-instruction C proof; behavioral evidence is separate. Heap corruption fail-stop paths are retained.'}
    (PROJECT/'analysis/main_dispatch_task_disposal_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    print('288 retail rows revalidated across five existing cleanup sources; no new original routines counted.')


if __name__=='__main__':main()
