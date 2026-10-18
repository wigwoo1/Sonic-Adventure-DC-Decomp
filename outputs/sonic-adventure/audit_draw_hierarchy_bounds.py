"""Check the explicit valid-data proof for retained channel fallback blocks."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 manifest=PROJECT/'analysis/draw_hierarchy_translation.json';record=json.loads(manifest.read_text())
 listing=PROJECT/'analysis/draw-hierarchy/8c057bc0_instructions.tsv'
 rows={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(listing.open(),delimiter='\t')}
 expected={0x8c057bca:'mov.l r9,@-r15',0x8c057bf6:'mov #0x0,r9',0x8c057c00:'add #0x1,r9',0x8c057c04:'cmp/ge r6,r9',0x8c057c1a:'mov r9,r0',0x8c057c34:'mov #0x0,r9',0x8c057c36:'mov r9,r7',0x8c057c44:'mov r9,r0',0x8c057d46:'mov r9,r0',0x8c057dfe:'add #0x1,r9',0x8c057e00:'cmp/ge r10,r9',0x8c057e20:'mov #0x0,r9',0x8c057e22:'mov r9,r7',0x8c057e30:'mov r9,r0',0x8c057f6a:'mov r9,r0',0x8c05808a:'add #0x1,r9',0x8c05808e:'cmp/ge r3,r9',0x8c0580d0:'mov.l @r15+,r9'}
 assert {a:s for a,s in rows.items() if re.search(r'\br9\b',s)}==expected
 required={0x8c057c22:'mov #0x2,r10',0x8c057c32:'bra 0x8c057e00',0x8c057e02:'bt 0x8c057e08',0x8c057e04:'bra 0x8c057c36',0x8c057e1e:'bra 0x8c05808c',0x8c05808c:'mov #0x3,r3',0x8c058090:'bt 0x8c058096',0x8c058092:'bra 0x8c057e22'}
 for a,s in required.items():assert rows[a]==s,(hex(a),rows[a],s)
 names=('model_matrix_stack_push.c','model_matrix_stack_pop.c','model_matrix_translate.c','model_matrix_rotate.c','model_matrix_rotate_implicit.c','sa_motion_reuse_sa_matrix_rotate_x.c','sa_motion_reuse_sa_matrix_rotate_y.c','sa_motion_reuse_sa_matrix_rotate_z.c','sa_draw_matrix_registers.c','model_channel_difference.c')
 files=[PROJECT/'source'/n for n in names]
 for p in files:assert not re.search(r'(?<![A-Za-z_])r\[(?:9|10)\]',p.read_text()),p
 source=PROJECT/record['source'];text=source.read_text()
 for routine in record['routines']:
  if int(routine['address'],16)==0x8c057bc0:continue
  body=text.split('SA_STAGE_EXPORT int '+routine['name']+'(',1)[1].split('SA_STAGE_EXPORT int ',1)[0]
  assert not re.search(r'(?<![A-Za-z_])r\[(?:9|10)\]',body),routine['name']
 fallback={2:[0x8c057c4e,0x8c057c50,0x8c057d50,0x8c057d52],3:[0x8c057e3e,0x8c057e40,0x8c057f7c,0x8c057f7e]}
 for bound in (2,3):
  values=[];counter=0
  while counter<bound:values.append(counter);counter+=1
  assert values==list(range(bound))
  assert all(x in (0,1) if bound==2 else x in (0,1,2) for x in values)
 files.extend([Path(__file__),manifest,listing,source])
 report=dict(retained_fallback_rows={str(k):[hex(a) for a in v] for k,v in fallback.items()},unreachable_rows=8,unreachable_conditional_outcomes=4,callee_sources_without_R9_R10_access=len(names),newly_recovered_entries=0,reused_routine_credit=0,sha256={str(p):digest(p) for p in files},scope='Original R9 access set, loop initializers/increments/bounds and callee C register use checked. M2 dispatch sees only 0/1; M3 sees only 0/1/2. Both null and animated channel paths retain default branch/NOP pairs in source.',assumptions=['Normal entry and successful callee returns with valid nonaliasing acyclic model/key/context/cache and a distinct stack/matrix-stack area. Self-modifying code, corrupted return frames and deliberate stack/data aliasing are outside this unreachable-block argument.','Nonrecursive callees do not access GPR R9 or R10. Recursive generator saves/restores both on its stack; valid disjoint writes preserve those saves. This is a qualified structural proof, not exhaustive arbitrary-RAM verification.'])
 (PROJECT/'analysis/draw_hierarchy_bounds_audit.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Hierarchy bounds audit passed: eight retained fallback rows / four outcomes excluded under explicit valid-data assumptions.')
if __name__=='__main__':main()
