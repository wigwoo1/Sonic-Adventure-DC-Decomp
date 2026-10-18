"""Reproduce the driver upload spans from original disc sectors, not filenames alone."""
import hashlib,json,shlex,struct
from pathlib import Path
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
manifest=json.loads((project/'disc_inventory.json').read_text())
tracks=[shlex.split(line) for line in manifest['gdi'].splitlines()[1:] if line.strip()]
track=next(t for t in reversed(tracks) if t[2]=='4')
base,stride,offset=int(track[1]),int(track[3]),int(track[5])
source=Path(manifest['source'])/track[4]
out=ROOT/'work/sonic-adventure/arm-sound-drivers';out.mkdir(parents=True,exist_ok=True)
def sha(data):return hashlib.sha256(data).hexdigest()
drivers=[]
with source.open('rb') as stream:
    for path in ('MANATEE.DRV','SONICADV/MASARU.DRV'):
        file=next(f for f in manifest['files'] if f['path']==path)
        data=bytearray()
        for index in range((file['size']+2047)//2048):
            stream.seek(offset+(file['lba']+index-base)*stride+(16 if stride==2352 else 0))
            block=stream.read(2048);assert len(block)==2048;data.extend(block)
        assert sha(data[:file['size']])==file['sha256']
        assert data[:4]==b'SDRV'
        length=struct.unpack_from('<I',data,8)[0]
        # Recovered 8c643ec6/8c643ec8 supplies header word +8 and source +32.
        assert 32+length<=len(data)
        payload=bytes(data[32:32+length]);vectors=[]
        for at in range(0,32,4):
            word=struct.unpack_from('<I',payload,at)[0]
            assert word>>24==0xea
            immediate=word&0xffffff
            if immediate&0x800000:immediate-=1<<24
            target=at+8+immediate*4
            assert 0<=target<len(payload) and target%4==0
            vectors.append({'address':hex(at),'word':hex(word),'branch_target':hex(target)})
        name=Path(path).name+'.arm.bin';target=out/name;target.write_bytes(payload)
        padding=max(0,32+length-file['size'])
        drivers.append({'path':path,'file_size':file['size'],'file_sha256':file['sha256'],
          'header_hex':bytes(data[:32]).hex(),'header_word_4':hex(struct.unpack_from('<I',data,4)[0]),
          'source_offset':32,'upload_size':length,'upload_sha256':sha(payload),
          'disc_padding_bytes_in_upload':padding,'disc_padding_hex':bytes(data[file['size']:32+length]).hex(),
          'payload_path':str(target.relative_to(ROOT)).replace('\\','/'),'load_address':'0x0',
          'language':'ARM:LE:32:v4','vectors':vectors})
report={'drivers':drivers,'inventory_sha256':sha((project/'disc_inventory.json').read_bytes()),
 'preparer_sha256':sha(Path(__file__).read_bytes()),
 'upload_source_sha256':sha((project/'source/sa_sound_driver_transfer.c').read_bytes()),
 'scope':'Original file hashes, actual rounded disc reads, SDRV +8 upload length/+32 source and eight ARM branch vectors. Payloads include any real sector padding copied by the recovered uploader.',
 'limitations':'Architecture/load-address analysis inputs for draft recovery; no ARM execution, complete function-boundary proof or playback claim.'}
(project/'analysis/arm_sound_driver_payloads.json').write_text(json.dumps(report,indent=2)+'\n')
print('Prepared',len(drivers),'ARM driver upload spans;',[(d['path'],d['upload_size'],d['disc_padding_bytes_in_upload']) for d in drivers])
