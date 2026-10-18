"""Decode the retail owner-helper switch and check its expanded listing."""
import hashlib,json,struct
from pathlib import Path
from verify_startup import ROOT

def main():
 p=ROOT/'outputs/sonic-adventure'
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 old={int(x.split('\t')[0],16) for x in (p/'analysis/instructions/8c079476.tsv').read_text().splitlines()[1:]}
 rows=[x.split('\t') for x in (p/'analysis/instructions/owner_helper_full_range.tsv').read_text().splitlines()[1:]]
 by={int(x[0],16):x for x in rows}
 for a,r in by.items():assert int(r[1],16)==struct.unpack_from('<H',b,a-0x8c010000)[0],r
 entries=[]
 for state in range(15):
  displacement=struct.unpack_from('<h',b,0x69510+state*2)[0];target=0x8c07950e+displacement
  assert target in by,hex(target)
  entries.append(dict(state=state,signed_displacement=displacement,target=hex(target),present_in_routine_export=target in old,present_in_range_export=True))
 calls=[dict(address=r[0],instruction=r[2],references=r[3]) for r in rows if r[2].split()[0] in ['jsr','bsr']]
 fp=sorted({r[2].split()[0].lstrip('_') for r in rows if r[2].lstrip('_').startswith('f')})
 report=dict(routine='0x8c079476',binary_sha256=hashlib.sha256(b).hexdigest(),jump_instruction='0x8c07950a',pc_relative_base='0x8c07950e',table_address='0x8c079510',entries=entries,range_export='analysis/instructions/owner_helper_full_range.tsv',range_instruction_count=len(rows),range_opcodes_match_retail=True,calls=calls,floating_point_mnemonics=fp,status='All 15 branch targets present in expanded instruction listing; every exported opcode matches retail. Source recovery and behavior verification remain pending.')
 (p/'analysis/owner_helper_branch_map.json').write_text(json.dumps(report,indent=2)+'\n')
 print(f'{len(rows)} opcode checks passed; all 15 switch targets present; {len(calls)} call sites')
if __name__=='__main__':main()
