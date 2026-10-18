"""Record byte-checked default device bindings; no claim of verified startup."""
import hashlib
import json
import struct
from pathlib import Path

PROJECT=Path(__file__).resolve().parent
binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
BASE=0x8C010000
def word(address):return struct.unpack_from('<I',binary,address-BASE)[0]
assert word(0x8C645344)==0x8C66D240
assert word(0x8C66D254)==0x8C6451A4
assert word(0x8C6451F8)==0x8C10D9D8
assert word(0x8C10D9E4)==8 and word(0x8C10D9E8)==0x8C0000BC
assert binary[0x8C6451A4-BASE:0x8C6451AA-BASE]==bytes.fromhex('14 d3 2b 43 09 00')
assert binary[0x8C10D9D8-BASE:0x8C10D9E4-BASE]==bytes.fromhex('00 e6 02 d7 02 d0 02 60 2b 40 09 00')
assert word(0x8C66C4FC)==0x8C6005C2  # Backend pump.
assert word(0x8C66D280)==0x8C6452D6  # Low-level pump.
assert word(0x8C64533C)==0x8C10D960
assert word(0x8C10D96C)==2 and word(0x8C10D970)==0x8C0000BC
report={'binary_sha256':hashlib.sha256(binary).hexdigest(),
        'default_device_table':'0x8c66d240','device_table_getter':'0x8c645328',
        'cancellation':{'slot':'0x8c66d254','wrapper':'0x8c6451a4','literal':'0x8c6451f8',
                        'service_tail':'0x8c10d9d8','operation':8,'runtime_vector_word':'0x8c0000bc'},
        'pump':{'backend_slot':'0x8c66c4fc','backend_target':'0x8c6005c2',
                'device_slot':'0x8c66d280','device_target':'0x8c6452d6',
                'service_tail':'0x8c10d960','operation':2,'runtime_vector_word':'0x8c0000bc'},
        'scope':'Original binary pointer words and cancellation instruction bytes checked. Default bindings only.',
        'limitations':['Complete driver initialization, runtime selection of alternate tables and the service-vector implementation are not established by static pointer values.',
                       'The runtime vector word lies below the executable load address; its callable target is supplied explicitly by tests/platform state.']}
(PROJECT/'analysis/request_device_callback_bindings.json').write_text(json.dumps(report,indent=2)+'\n')
print('Default pump and cancellation bindings verified against retail binary words')
