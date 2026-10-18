import json,struct
from pathlib import Path
p=Path(__file__).resolve().parent;root=p.parents[1];assets=[];count=0
for file in (root/'work/sonic-adventure/extracted/files').rglob('*'):
 if not file.is_file():continue
 count+=1
 with file.open('rb') as f:header=f.read(64)
 if len(header)>=16 and header[:2]==b'\x80\x00':
  offset=struct.unpack_from('>H',header,2)[0]+4
  assets.append(dict(path=str(file.relative_to(root)),bytes=file.stat().st_size,header_hex=header.hex(),retail_data_offset=offset,raw_metadata_bytes=list(header[4:8]),rate=struct.unpack_from('>I',header,8)[0],length_field=struct.unpack_from('>I',header,12)[0]))
(p/'analysis/magic_audio_header_inventory.json').write_text(json.dumps(dict(files_scanned=count,assets=assets,scope='Leading64-byte headers of extracted files with magic8000. Format identity/full validity not asserted; embedded/compressed banks not scanned.'),indent=2)+'\n')
print(count,'files scanned;',len(assets),'magic8000 candidates')
