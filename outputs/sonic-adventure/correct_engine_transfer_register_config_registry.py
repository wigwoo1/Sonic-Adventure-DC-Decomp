import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent;binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=p/'analysis/recovered_routines.json';data=json.loads(path.read_text())
start,end=0x8c10ee64,0x8c10ee8c;name='sa_engine_transfer_register_config'
matches=[r for r in data['routines'] if r['name']==name]
assert len(matches)==1 and matches[0]['address']=='0x8c10fb48'
data['routines'].remove(matches[0])
data['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification='analysis/engine_transfer_register_config_verification.json',status='Compiled and matched within report scope'))
path.write_text(json.dumps(data,indent=2)+'\n')
reg=p/'register_engine_transfer_register_config.py';s=reg.read_text().replace("2349923144, 2349923158", "2349923044, 2349923084")
reg.write_text(s)
