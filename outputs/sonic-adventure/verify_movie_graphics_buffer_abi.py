"""Check the original pointer-builder calling layout, rather than a draft signature."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/'analysis/movie-graphics-init-dependencies/8c6574b8_instructions.tsv',P/'analysis/movie-command-handoff-next/8c656940_instructions.tsv'];retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';image=retail.read_bytes();checked=0;tables=[]
 for path in paths:
  rows=list(csv.DictReader(path.open(),delimiter='\t'));table={int(row['address'],16):row for row in rows};tables.append(table)
  for address,row in table.items():
   raw=bytes.fromhex(row['bytes']);assert image[address-0x8c010000:address-0x8c010000+len(raw)]==raw;checked+=1
 body,caller=tables
 assert [body[a]['instruction'] for a in range(0x8c6574b8,0x8c6574c6,2)]==[f'mov.l r{r},@-r15' for r in range(14,7,-1)]
 assert body[0x8c6574c6]['instruction']=='add #-0x20,r15';frame=7*4+32;assert frame==60
 assert body[0x8c6574ec]['instruction']=='mov.l @(0x3c,r15),r3' and 0x3c-frame==0
 assert body[0x8c657506]['instruction']=='mov #0x44,r0' and body[0x8c65751a]['instruction']=='mov.l @(r0,r15),r0' and 0x44-frame==8
 assert caller[0x8c656c9a]['instruction']=='mov.l r3,@-r15' and caller[0x8c656c9c]['instruction']=='mov.l r12,@-r15' and caller[0x8c656ca0]['instruction']=='mov.l r3,@-r15'
 assert caller[0x8c656ca8]['instruction']=='bsr 0x8c6574b8' and caller[0x8c656cac]['instruction']=='add #0xc,r15'
 direct_path=P/'analysis/movie_graphics_buffers_verification.json';direct=json.loads(direct_path.read_text());cases=[r['input'] for r in direct['cases'] if r['input']['entry']==0x8c6574b8]
 assert {c['gfx_arguments'][5] for c in cases if c['kind']=='pointer_unused_stack_word'}=={1,0xdeadbeef,0xffffffff}
 assert {c['gfx_arguments'][6] for c in cases}=={0,1,2}
 files=[Path(__file__),retail,direct_path]+paths
 report=dict(address='0x8c6574b8',retail_instruction_rows_checked=checked,prologue_bytes=frame,fifth_argument_caller_sp_offset=0,unused_stack_word_caller_sp_offset=4,buffer_count_caller_sp_offset=8,caller_stack_argument_words=3,caller_stack_cleanup_bytes=12,buffer_counts_tested=[0,1,2],ignored_word_variations=[1,0xdeadbeef,0xffffffff],sha256={str(p):digest(p) for p in files},scope='Actual checked SH4 prologue,stack loads and reached root call establish five size slots,one unused stack word and count at callerSP+8. Ghidra draft six-argument signature is not authoritative. Direct comparisons exercise unused-word variations with valid zero/one/two-buffer backing;larger counts and corrupt caller storage are not qualified by this ABI proof. No native source or original instruction is changed.')
 (P/'analysis/movie_graphics_buffer_abi_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Verified original pointer builder ABI:five size slots,unused SP+4,count SP+8')
if __name__=='__main__':main()
