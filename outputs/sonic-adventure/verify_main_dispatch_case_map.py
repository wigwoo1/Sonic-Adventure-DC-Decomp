"""Check map coverage and constant-transfer proofs using original SH4 loads."""
import hashlib,json,struct
from pathlib import Path
from verify_startup import ROOT,State,reference
P=Path(__file__).resolve().parent
path=P/'analysis/main-dispatch-enrichment/case_dependency_map.json';report=json.loads(path.read_text())
for file,value in report['sha256'].items():assert hashlib.sha256(Path(file).read_bytes()).hexdigest()==value,file
assert report['entry_count']==80 and report['selector_count']==83 and report['export_instruction_rows']==8122
assert sum(s['instruction_rows'] for s in report['entry_spans'])+report['prefix_instruction_rows']==8122
assert len({s['entry'] for s in report['entry_spans']})==80
assert sorted(i for s in report['entry_spans'] for i in s['selectors'])==list(range(1,84))
image=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(0x1000000);ram[0x10000:0x10000+len(image)]=image
checked=[]
for transfer in report['transfers']:
 target=transfer['target'];assert target is not None
 if transfer['kind']=='bsr':continue
 site=transfer['literal_load_site'];word=struct.unpack_from('<H',ram,site&0xffffff)[0]
 assert word>>12==13
 state=State();state.pc=site;state.pr=0x8cf00000;state.r[:]=[0x12340000+i for i in range(16)]
 steps=reference(ram,state,stop_pc=site+2,instruction_budget=1)
 assert steps==1 and state.r[(word>>8)&15]==target
 call=struct.unpack_from('<H',ram,transfer['site']&0xffffff)[0]
 assert ((word>>8)&15)==((call>>8)&15)
 checked.append({'transfer_site':transfer['site'],'load_site':site,'target':target})
result={'entry_spans_verified':80,'selector_entries_verified':83,'transfer_sites':len(report['transfers']),'literal_load_checks_passed':len(checked),'checks':checked,'sha256':{str(path):hashlib.sha256(path.read_bytes()).hexdigest(),str(Path(__file__)):hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),str(P/'verify_startup.py'):hashlib.sha256((P/'verify_startup.py').read_bytes()).hexdigest()},'scope':'Exact map input hashes, complete entry-span accounting and independent execution of original PC-relative pointer-load instructions. Does not execute callees, prove callback behavior, replace shared-CFG analysis or establish native case recovery.'}
(P/'analysis/main_dispatch_case_map_verification.json').write_text(json.dumps(result,indent=2)+'\n')
print(len(checked),'retail pointer-load checks passed; 80 spans and 83 selectors accounted for')
