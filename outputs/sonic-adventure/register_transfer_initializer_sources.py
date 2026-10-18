import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent;binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
registry_path=p/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text())
report='analysis/engine_transfer_initialize_host_verification.json'
evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==12 and evidence['recovered_instructions']==312
entries=[
 ('sa_engine_transfer_initialize',0x8c10d5a8,0x8c10d72a,'source/sa_engine_transfer_initialize.c'),
 ('sa_engine_transfer_descriptor_reset_io',0x8c10ed54,0x8c10ee18,'source/sa_engine_transfer_descriptor_reset_io.c'),
 ('sa_engine_transfer_address_window_set_io',0x8c10ee64,0x8c10ee8c,'source/sa_engine_transfer_address_window_set_io.c'),
 ('sa_memory_fill',0x8c10d31c,0x8c10d334,'source/sa_memory_fill.c'),
]
for name,start,end,source in entries:
 old=[r for r in registry['routines'] if r['name']==name]
 if old:
  assert len(old)==1 and old[0]['address']==hex(start) and old[0]['end_exclusive']==hex(end), (name,old)
  continue
 registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=source,retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within aggregate initializer report scope'))
status_path=p/'project_status.json';status=json.loads(status_path.read_text());status['verified_progress']['transfer_initializer_registry_correction']=dict(report=report,entries=[dict(name=n,address=hex(a),end_exclusive=hex(b),source=s) for n,a,b,s in entries],scope='Registry correction for four existing native sources already compiled and compared by the 12-case,312-instruction initializer verification. This adds no new disassembly or coverage claim.')
registry_path.write_text(json.dumps(registry,indent=2)+'\n');status_path.write_text(json.dumps(status,indent=2)+'\n')
handoff=p/'SOURCE_HANDOFF.txt';text=handoff.read_text();marker='transfer initializer registry correction:'
if marker not in text:
 handoff.write_text(text+'\n'+marker+' The native initializer, descriptor reset, address-window configuration, and memory fill sources are registered against '+report+'; see analysis/recovered_routines.json and project_status.json. Overall project10%.\n')
