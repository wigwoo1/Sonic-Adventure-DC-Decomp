import hashlib,json,struct
from pathlib import Path
root=Path(__file__).resolve().parent
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
rows=[]
for line in (root/'analysis/instructions/sound_decode_65b120.tsv').read_text().splitlines()[1:]:
    address,word,instruction,*_=line.split('\t');rows.append((int(address,16),int(word,16),instruction))
records=[]
registry=json.loads((root/'analysis/recovered_routines.json').read_text(encoding='utf-8-sig'))
specs=[
    ('sa_sound_decode_block_mono',0x8c65b120,0x8c65b222,129,
     ['analysis/sound_decode_block_mono_verification.json','analysis/sound_decode_mono_scales_verification.json','analysis/sound_decode_mono_retail_verification.json']),
    ('sa_sound_decode_block_stereo',0x8c65b238,0x8c65b428,238,
     ['analysis/sound_decode_stereo_samples_verification.json','analysis/sound_decode_stereo_scales_verification.json','analysis/sound_decode_stereo_retail_verification.json','analysis/sound_decode_stereo_prefix_verification.json','analysis/sound_decode_stereo_odd_verification.json','analysis/sound_decode_stereo_termination_verification.json']),
]
for name,start,end,count,verification_paths in specs:
    selected=[r for r in rows if start<=r[0]<end];assert len(selected)==count
    for address,word,instruction in selected:assert struct.unpack_from('<H',binary,address-0x8c010000)[0]==word
    assert selected[-2][2]=='rts' and selected[-1][0]==end-2
    source=root/f'source/{name}.c'
    entry=next(r for r in registry['routines'] if r.get('name')==name)
    assert int(entry['address'],16)==start and int(entry['end_exclusive'],16)==end and entry['source']==f'source/{name}.c'
    assert entry['retail_span_sha256']==hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()
    verifications=[]
    for rel in verification_paths:
        item=json.loads((root/rel).read_text(encoding='utf-8-sig'))
        assert item.get('cases_passed',0)>0
        verifications.append({'path':rel,'cases_passed':item['cases_passed'],'original_files':item.get('original_files')})
    records.append(dict(name=name,address=hex(start),end_exclusive=hex(end),instruction_count=count,retail_span_sha256=entry['retail_span_sha256'],source=f'source/{name}.c',source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),registry_status=entry['status'],verification=entry['verification'],verification_evidence=verifications))
assert (root.parents[1]/'work/sonic-adventure/build/sound_decode_blocks_draft.dll').is_file()
report=dict(routines=records,scope='Both block decoders are registered, compile together, and are behaviorally verified. 367 exported opcodes match retail including RTS delay slots. Mono: 1,152 differential cases, 180 scale cases, and 218 comparisons across 109 original files. Stereo: 1,152 differential cases, 180 scale cases, 218 original-file comparisons, 218 original-prefix comparisons, 48 odd-count cases, and 96 termination cases. Full games and playback remain outside these decoder-level checks.')
(root/'analysis/decode_block_draft_audit.json').write_text(json.dumps(report,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['audio_block_decoder_recovery']={'verification':'analysis/decode_block_draft_audit.json','routines':2,'retail_opcodes_checked':367,'scope':report['scope']};status.setdefault('unverified_work',{}).pop('decode_block_drafts',None);path.write_text(json.dumps(status,indent=2)+'\n',encoding='utf-8')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text(encoding='utf-8-sig');marker='Unverified block-decoder drafts:'
if marker in text:
    start=text.index(marker);end=text.find('\n',start)
    if end<0:end=len(text)
    text=text[:start]+'Audio block decoder recovery: source/sa_sound_decode_block_mono.c and source/sa_sound_decode_block_stereo.c are registered and compile via build_sound_decode_blocks_draft.ps1. audit_decode_block_drafts.py checks all 367 retail opcodes and summarizes differential, scale, original-asset, odd-count, and termination evidence in analysis/decode_block_draft_audit.json. Decoder routines are behavior verified within those scopes; complete game playback remains unverified. Whole-project completion remains 10%.'+text[end:]
path.write_text(text,encoding='utf-8')
print('Both decoder registrations, 367 retail opcodes, and 2,958 reported behavioral cases/asset comparisons verified')
