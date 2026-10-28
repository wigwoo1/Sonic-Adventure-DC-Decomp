"""Read-only named-export inventory; support/continuation exports get no routine credit."""
import struct
from pathlib import Path

def inventory(dll,routines):
    b=Path(dll).read_bytes();u16=lambda o:struct.unpack_from('<H',b,o)[0];u32=lambda o:struct.unpack_from('<I',b,o)[0]
    assert b[:2]==b'MZ';pe=u32(0x3c);assert b[pe:pe+4]==b'PE\0\0';opt=pe+24;magic=u16(opt);assert magic in (0x10b,0x20b)
    table=opt+u16(pe+20);sections=[]
    for i in range(u16(pe+6)):
        o=table+i*40;sections.append((u32(o+12),u32(o+8),u32(o+16),u32(o+20)))
    def offset(rva):
        for address,virtual_size,raw_size,raw in sections:
            if address<=rva<address+max(virtual_size,raw_size):
                at=raw+rva-address;assert rva-address<raw_size and at<len(b);return at
        raise ValueError('Unmapped PE RVA')
    directory=offset(u32(opt+(112 if magic==0x20b else 96)));names=offset(u32(directory+32));exports=set()
    for i in range(u32(directory+24)):
        at=offset(u32(names+i*4));exports.add(b[at:b.index(b'\0',at)].decode('ascii'))
    linked=[r for r in routines if r['name'] in exports];unique={(r.get('module','1ST_READ.BIN'),r['address']) for r in linked}
    return dict(registry_entries=len(routines),registry_unique_module_address_pairs=len({(r.get('module','1ST_READ.BIN'),r['address']) for r in routines}),confirmed_linked_registry_entries_by_name=len(linked),confirmed_linked_unique_routines_by_name=len(unique),all_named_DLL_exports_including_support_and_continuations=len(exports),method='Exact registered-name intersection with actual PE named exports;support/continuation-only exports excluded. Other internal/renamed implementations are not counted by this method. Registry recovery count is distinct from current combined-DLL integration count.')
