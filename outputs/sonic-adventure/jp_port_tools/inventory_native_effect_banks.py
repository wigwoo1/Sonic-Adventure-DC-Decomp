"""Read-only JP MLT inventory and streaming-ring memory compatibility evidence.

Reports hashes/ranges only. No retail payloads are written or published. This
is an independent disc/schema oracle, not a production bank loader.
"""
import argparse,collections,hashlib,json,re,subprocess
from pathlib import Path
from struct import unpack_from
from adx_media_fixtures import Disc
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
RAM=0x200000
OFFSETS={'SMSB':0,'SMPB':0x80,'SOSB':0x100,'SPSR':0x180,'SFPB':0x200,'SFOB':0x280,'SFPW':0x288}
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def parse(data,check):
    check(len(data)>=32 and data[:4]==b'SMLT','MLT header and magic')
    version=int.from_bytes(data[4:5],'little',signed=True)
    count=unpack_from('<i',data,8)[0]
    start=16 if version<1 and unpack_from('<i',data,16)[0]!=-1 else 32
    check(0<=count<=(len(data)-start)//32,'complete bounded 32-byte bank table')
    rows=[];payload_ranges=[]
    for index in range(count):
        kind,number,address,allocated,offset,size,_,_=unpack_from('<4s7i',data,start+32*index)
        kind=kind.decode('ascii');active=allocated>=0 or size>0
        row=dict(index=index,type=kind,number=number,address=address,allocation_bytes=allocated,data_offset=offset,data_bytes=size)
        if active:
            check(kind in OFFSETS and 0<=number<(16 if kind in ('SMSB','SMPB','SOSB','SPSR') else 1),'valid live bank type and number')
            if allocated>=0:check(0<=address<=RAM and 0<=allocated<=RAM-address,'allocated range fits actual 2 MiB sound RAM')
            if size>0:
                check(offset>=start+32*count and size<=len(data)-offset,'payload lies after the table and inside the file')
                check(allocated>=size and 0<address<RAM and size<=RAM-address,'payload fits its declared sound allocation')
                check(not ((offset|size|address)&31),'payload source, destination and size align to 32-byte DMA units')
                row['payload_sha256']=hashlib.sha256(data[offset:offset+size]).hexdigest()
                payload_ranges.append((offset,offset+size))
        rows.append(row)
    ordered=sorted(payload_ranges)
    check(all(a[1]<=b[0] for a,b in zip(ordered,ordered[1:])),'payload file spans do not overlap')
    return dict(version=version,table_offset=start,bank_records=rows)
def ring_ranges(channels):
    return [(RAM-(index+1)*0x4040,RAM-index*0x4040) for index in range(channels)]
def overlaps(record,rings):
    if record['allocation_bytes']<=0:return False
    a=record['address'];b=a+record['allocation_bytes']
    return any(a<end and start<b for start,end in rings)
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    originals={}
    for n in ('sdk/sofdec/mw_sdcs.c','sdk/sofdec/mw_sdbuf.c','sdk/shinobi/sd_multi_unit_download.c','sdk/shinobi/sd_work.h','src/sound/sound.c'):
        originals[n]=sha(u/n)
        assert originals[n]==hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest(),n
    labels=[]
    def check(ok,label):assert ok,label;labels.append(label)
    disc=Disc(a.track.resolve());rows=[]
    try:
        for name,(fad,size) in sorted(disc.files.items()):
            if not name.endswith('.MLT'):continue
            data=disc.read(name,0,size);parsed=parse(data,lambda ok,label:check(ok,name+': '+label))
            check(32<=size<=2<<20,name+': proposed owned staging limit covers the whole real file')
            rows.append(dict(file=name,fad=fad,bytes=size,file_sha256=hashlib.sha256(data).hexdigest(),**parsed))
        track_sha=disc.sha256
    finally:disc.close()
    refs=sorted(set(n.decode().upper() for n in re.findall(rb'"([^"\n]+\.mlt)"',(u/'src/sound/sound.c').read_bytes(),re.I)))
    missing=sorted(set(refs)-{row['file'] for row in rows})
    check(not missing,'all literal MLT references in original game sound module exist on disc')
    records=[rec for row in rows for rec in row['bank_records']]
    positive=[rec for rec in records if rec['data_bytes']>0]
    check(len(rows)==112 and len(positive)==711,'complete previously qualified retail corpus: 112 files and 711 payloads')
    compatibility=[]
    for channels in range(1,9):
        conflicts=[]
        for row in rows:
            hits=[rec['index'] for rec in row['bank_records'] if overlaps(rec,ring_ranges(channels))]
            if hits:conflicts.append(dict(file=row['file'],record_indexes=hits))
        compatibility.append(dict(allocated_stream_channels=channels,ring_ranges=ring_ranges(channels),conflicting_files=len(conflicts),conflicts=conflicts))
    check(not compatibility[2]['conflicts'],'actual game stereo music plus mono voice ring reservations do not overlap any retail bank allocation')
    # Treat wider capacity as a reported frontier; do not silently shrink the
    # original middleware's eight buffer slots or claim that all can coexist.
    result=dict(status='all_JP_retail_MLT_ranges_and_streaming_ring_reservations_independently_inventoried',checks=len(labels),assertions=labels,
        bank_files=len(rows),bank_records=len(records),positive_payloads=len(positive),payload_bytes=sum(x['data_bytes'] for x in positive),
        max_staging_bytes=max(row['bytes'] for row in rows),literal_game_sound_bank_references=len(refs),missing_literal_references=missing,
        types=dict(collections.Counter(x['type'] for x in records)),stream_ring_compatibility=compatibility,files=rows,
        upstream_pin=PIN,original_source_sha256=originals,track_sha256=track_sha,inventory_tool_sha256=sha(__file__),
        actual_effects_loader_or_queue_runtime_tested=False,retail_payloads_exported=False,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','checks','bank_files','bank_records','positive_payloads','max_staging_bytes','literal_game_sound_bank_references','missing_literal_references')},indent=2))
    print('Streaming channels / overlapping bank files:',[(x['allocated_stream_channels'],x['conflicting_files']) for x in compatibility])
if __name__=='__main__':main()
