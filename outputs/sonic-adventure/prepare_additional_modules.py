"""Stage mapped, unindexed modules at their evidence-backed load addresses."""
import hashlib,json,shutil,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'outputs/sonic-adventure'
def main():
    mapping=json.loads((OUT/'analysis/overlay_load_map.json').read_text(encoding='utf-8-sig'))
    covered={item['module'] for item in json.loads((OUT/'analysis/recovery_coverage_audit.json').read_text(encoding='utf-8-sig'))['module_indexes'] if not item['index'].startswith('analysis/additional-modules/')}
    rows=[]
    for item in mapping['modules']:
        source=Path(item['decoded_path'])
        if source.name in covered:continue
        data=source.read_bytes();assert hashlib.sha256(data).hexdigest()==item['decoded_sha256'],source
        base=int(item['load_address'],16);first=struct.unpack_from('<H',data)[0]
        root=first in [0x2fe6,0x5548,0x4f22] or first>>12==0xd or first&0xff00==0x7f00
        prologues=set()
        for offset in range(0,len(data)-3,4):
            pointer=struct.unpack_from('<I',data,offset)[0]
            if pointer&1 or not base<=pointer<base+len(data)-2:continue
            opcode=struct.unpack_from('<H',data,pointer-base)[0]
            if opcode==0x4f22 or (opcode&0xff0f==0x2f06 and 8<=((opcode>>4)&15)<=14):prologues.add(pointer)
        directory=ROOT/f'work/sonic-adventure/additional-modules/{base:08x}'
        directory.mkdir(parents=True,exist_ok=True);destination=directory/source.name
        shutil.copyfile(source,destination)
        rows.append({**item,'module':source.name,'staged_path':str(destination),'bytes':len(data),'header_hex':data[:32].hex(),'plausible_entry_candidate':root,'pointed_to_prologue_candidates':len(prologues),'classification':'Mapped binary awaiting executable/data boundary validation; seeds are candidates only'})
    (OUT/'analysis/additional_mapped_modules.json').write_text(json.dumps(rows,indent=2)+'\n')
    for row in rows:print(f"{row['module']}: base {row['load_address']}, root={row['plausible_entry_candidate']}, pointed prologues={row['pointed_to_prologue_candidates']}")
if __name__=='__main__':main()
