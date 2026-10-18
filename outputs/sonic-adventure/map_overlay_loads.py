"""Resolve constant module-loader call arguments against the retail binary."""
import hashlib, json, pathlib, re, struct
ROOT=pathlib.Path(__file__).resolve().parents[2]
BASE=0x8c010000
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    text=(ROOT/'outputs/sonic-adventure/analysis/ghidra-all/initial_pseudocode.c').read_text()
    def word(address): return struct.unpack_from('<I',binary,address-BASE)[0]
    def name(address):
        offset=address-BASE;end=binary.index(0,offset)
        return binary[offset:end].decode('ascii')
    records=[]; unresolved=[]
    for match in re.finditer(r'FUN_8c09da44\(([^,\n]+),([^;\n]+)\);',text):
        first,second=(v.strip() for v in match.groups())
        pointer=re.search(r'_(8c[0-9a-f]{6})$',first)
        destination=re.fullmatch(r'DAT_(8c[0-9a-f]{6})',second)
        if not pointer or not destination:
            unresolved.append(match.group());continue
        pointer_address=int(pointer[1],16);literal=int(destination[1],16)
        filename=name(word(pointer_address));base=word(literal)
        decoded=next((p for p in (ROOT/'work/sonic-adventure/decoded/SONICADV').glob('*.BIN') if p.stem.lower()==pathlib.Path(filename).stem.lower()),None)
        records.append({'resource':filename,'load_address':hex(base),'filename_pointer_literal':hex(pointer_address),
                        'load_address_literal':hex(literal),'decoded_path':str(decoded) if decoded else None,
                        'decoded_sha256':hashlib.sha256(decoded.read_bytes()).hexdigest() if decoded else None,
                        'evidence':'Constant arguments in draft decompilation resolved from retail literal bytes; no runtime trace.'})
    unique={(r['resource'].lower(),r['load_address']):r for r in records}
    report={'loader_address':'0x8c09da44','decompressor_address':'0x8c04c5e0','compressed_scratch_address':'0x0cd00000',
            'cache_sync_call_alias':'0xac5fc0c2','modules':list(unique.values()),'unresolved_calls':unresolved,
            'limitations':['Includes data modules loaded through the same loader; code/data boundaries remain to be established.',
                           'Call arguments are based on drafts and literal pools, pending instruction-level and runtime validation.']}
    (ROOT/'outputs/sonic-adventure/analysis/overlay_load_map.json').write_text(json.dumps(report,indent=2))
    print('Resolved module/destination pairs:',len(unique),'Unresolved calls:',len(unresolved))
    print([(r['resource'],r['load_address']) for r in unique.values()])
if __name__=='__main__':main()
