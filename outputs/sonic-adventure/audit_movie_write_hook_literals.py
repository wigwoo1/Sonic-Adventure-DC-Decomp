"""Bounded whole-disc/decoded literal search; never an alias/unreachability proof."""
import hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];CELL=0x8c67c27c
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    inventory=P/'disc_inventory.json';prs_path=P/'analysis/prs_inventory.json'
    rows=json.loads(inventory.read_text())['files'];prs=json.loads(prs_path.read_text());inputs=[];hits=[]
    aliases=(0x0c67c27c,0x8c67c27c,0xac67c27c)
    patterns=[(a,order,a.to_bytes(4,order)) for a in aliases for order in ('little','big')]
    def scan(path,size,sha,kind,resource):
        data=path.read_bytes();assert len(data)==size and hashlib.sha256(data).hexdigest()==sha,resource
        found=[]
        for address,order,pattern in patterns:
            offset=data.find(pattern)
            while offset>=0:
                found.append(dict(offset=hex(offset),pointer_alias=hex(address),byte_order=order,aligned_word=offset%4==0));offset=data.find(pattern,offset+1)
        inputs.append(dict(path=path.relative_to(ROOT).as_posix(),resource=resource,kind=kind,bytes=size,sha256=sha,literal_hits=len(found)))
        if found:hits.append(dict(resource=resource,kind=kind,hits=found))
    for row in rows:
        if row['directory']:continue
        scan(ROOT/'work/sonic-adventure/extracted/files'/row['path'],row['size'],row['sha256'],'extracted',row['path'])
    for row in prs:
        assert row['status']=='decoded'
        scan(ROOT/'work/sonic-adventure/decoded'/Path(row['source']).with_suffix('.BIN'),row['decoded_bytes'],row['decoded_sha256'],'decoded_prs',row['source'])
    mainpath=ROOT/'work/sonic-adventure/extracted/files/1ST_READ.BIN';data=mainpath.read_bytes();nearby=[]
    # Conservative main-image pool candidates also include nearby bases from
    # which a small offset could construct the cell address. They are leads,
    # not established pointers, accesses, writers or feasible execution paths.
    for offset,(value,) in enumerate(struct.iter_unpack('<I',data[:len(data)//4*4])):
        physical=value&0x1fffffff
        if 0x0c67c000<=physical<0x0c67c400:nearby.append(dict(address=hex(0x8c010000+offset*4),value=hex(value),offset_to_cell=(CELL&0x1fffffff)-physical))
    report=dict(callback_cell=hex(CELL),extracted_files_checked=sum(r['kind']=='extracted' for r in inputs),decoded_prs_files_checked=len(prs),bytes_scanned=sum(r['bytes'] for r in inputs),aliases=list(map(hex,aliases)),byte_orders=['little','big'],matches=hits,main_image_nearby_aligned_word_candidates=nearby,inputs=inputs,sha256={str(p):digest(p) for p in (Path(__file__),inventory,prs_path)},scope='All inventory-extracted files and all verified decoded PRS resources searched bytewise for exact32bit physical/P1/P2 cellaliases inbothbyteorders,withsize/hash checks. Aligned nearby main-image words listedaspossiblebaseleads only. Data/color/compressed matchescanbefalsepositives. Doesnotfind computed/partial/relocated aliases orprove hookunreachable/nonnullinstallationabsent. No gameassets copied into report.')
    out=P/'analysis/movie_write_hook_literal_audit.json';out.write_text(json.dumps(report,indent=2)+'\n');print('HOOK LITERAL AUDIT',report['extracted_files_checked'],len(prs),report['bytes_scanned'],'bytes;',len(hits),'matching resources;',len(nearby),'nearby leads',flush=True)
    for match in hits:print(match['kind'],match['resource'],len(match['hits']),flush=True)
if __name__=='__main__':main()
