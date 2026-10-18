"""Inspect header-declared encoded end boundaries in original ADX assets.

This reads actual file tails; it does not prove decoding or loop behavior.
"""
import collections,hashlib,json,struct
from pathlib import Path
p=Path(__file__).resolve().parent;root=p.parents[1]
assets=json.loads((p/'analysis/magic_audio_header_inventory.json').read_text())['assets'];records=[]
for asset in assets:
    path=root/asset['path'];size=path.stat().st_size
    with path.open('rb') as f:
        header=f.read(64);assert header.hex()==asset['header_hex']
        offset=struct.unpack_from('>H',header,2)[0]+4
        block_bytes,bit_depth,channels=header[5],header[6],header[7]
        samples=struct.unpack_from('>I',header,12)[0]
        assert block_bytes>2 and bit_depth and ((block_bytes-2)*8)%bit_depth==0
        samples_per_block=(block_bytes-2)*8//bit_depth
        pairs=(samples+samples_per_block-1)//samples_per_block
        ending=offset+pairs*channels*block_bytes
        inside_file=ending<=size
        start=max(offset,ending-channels*block_bytes)
        if inside_file:
            f.seek(start);window=f.read(min(size-start,channels*block_bytes+64))
        else:window=b''
        f.seek(max(0,size-64));tail=f.read(64)
        f.seek(max(offset,size-4096));physical_tail_start=f.tell();physical_tail=f.read()
    last_pair=window[:ending-start];after=window[ending-start:]
    records.append(dict(path=asset['path'],file_bytes=size,data_offset=offset,block_bytes=block_bytes,bit_depth=bit_depth,channels=channels,total_samples=samples,samples_per_block=samples_per_block,declared_channel_block_pairs=pairs,final_pair_valid_samples=(samples%samples_per_block or samples_per_block),declared_encoded_end=ending,declared_end_inside_file=inside_file,bytes_after_declared_end=size-ending,final_pair_hex=last_pair.hex(),final_pair_sha256=hashlib.sha256(last_pair).hexdigest() if inside_file else None,after_declared_end_hex=after.hex(),first_word_after_end=hex(int.from_bytes(after[:2],'big')) if len(after)>=2 else 'outside_file',last64_hex=tail.hex()))
    records[-1]['aligned_terminal_word_candidates_in_last4096']=[physical_tail_start+i for i in range(len(physical_tail)-1) if (physical_tail_start+i-offset)%block_bytes==0 and physical_tail[i:i+2]==b'\x80\x01']
summary=collections.Counter(r['first_word_after_end'] for r in records)
report=dict(original_files=len(records),end_word_counts=dict(summary),assets=records,scope='Original header64 and header-declared encoded end calculations for all indexed magic8000 assets. Actual final channel pair and up to64 following bytes read when declared end is inside file; actual file tail always read. Header sample/block/bit-depth/channel arithmetic inspected; declared ends outside file explicitly recorded without trusting the header. Terminal marker semantics,final sample trimming,loop metadata,fullpayload decoding and physical playback remain unverified.')
(p/'analysis/magic_audio_endings_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(records),'original endings inspected; words after declared end:',dict(summary))
print('partial final pairs:',sum(r['final_pair_valid_samples']!=r['samples_per_block'] for r in records))
for r in records:
    if not r['declared_end_inside_file']:print('declared end outside file:',r['path'],r['declared_encoded_end'],r['file_bytes'],'aligned terminal candidates:',r['aligned_terminal_word_candidates_in_last4096'])
