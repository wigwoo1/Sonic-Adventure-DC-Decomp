"""Check the original constant-register proof for the unreachable display edge."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 mp=P/'analysis/movie_graphics_init_translation.json';translation=json.loads(mp.read_text());body=next(r for r in translation['routines'] if r['address']=='0x8c665cc0');listing=Path(body['listing']);rows=list(csv.DictReader(listing.open(),delimiter='\t'));by_address={int(r['address'],16):r for r in rows}
 image_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';image=image_path.read_bytes()
 for row in rows:
  address=int(row['address'],16);raw=bytes.fromhex(row['bytes']);assert image[address-0x8c010000:address-0x8c010000+len(raw)]==raw
 expected=['mov #0x1,r5','mov.l @r3,r0','and r5,r0','cmp/eq #0x1,r0','bt 0x8c665d80','mov.l 0x8c665d70,r1','mov.l @r1,r0','and #0x2c,r0','cmp/eq #0x20,r0','bf 0x8c665d80','tst r5,r5','bt 0x8c665d80']
 for address,text in zip(range(0x8c665d32,0x8c665d4a,2),expected):assert by_address[address]['instruction']==text
 # No branch in the complete function enters after the constant assignment.
 # Conditional exits inside this interval either leave it or fall through.
 for row in rows:
  for target in row['flows'].split():assert not 0x8c665d32<int(target,16)<=0x8c665d48
 assert int.from_bytes(bytes.fromhex(by_address[0x8c665d32]['bytes']),'little')==0xe501
 assert int.from_bytes(bytes.fromhex(by_address[0x8c665d46]['bytes']),'little')==0x2558
 assigned_value=1;t_after_tst=int((assigned_value&assigned_value)==0);assert t_after_tst==0
 branch_word=int.from_bytes(bytes.fromhex(by_address[0x8c665d48]['bytes']),'little');assert branch_word>>8==0x89
 displacement=branch_word&255;displacement=displacement-256 if displacement&128 else displacement;assert 0x8c665d48+4+2*displacement==0x8c665d80
 direct_path=P/'analysis/movie_graphics_buffers_verification.json';direct=json.loads(direct_path.read_text());coverage=next(r for r in direct['coverage_by_routine'] if r['address']=='0x8c665cc0')
 assert coverage['static_rows']==coverage['covered_rows']==113 and coverage['conditional_outcomes']==18 and coverage['covered_outcomes']==17 and coverage['missing_outcomes']==[['0x8c665d48','0x8c665d80']] and not coverage['missing_rows']
 files=[Path(__file__),mp,listing,image_path,direct_path]
 result=dict(address='0x8c665cc0',executed_rows=113,executed_conditional_outcomes=17,proven_infeasible_conditional_outcomes=[['0x8c665d48','0x8c665d80']],qualified_conditional_outcomes=18,constant_register='r5',constant_value=1,tst_result_t=0,instructions_checked_against_retail=len(rows),interior_branch_targets_absent=True,sha256={str(p):digest(p) for p in files},scope='Complete original function entry,immutable checked instruction bytes and no asynchronous register modification/interior entry. Every static control-flow entry to the checked interval passes mov1,r5; subsequent instructions do not write r5. Tst r5,r5 therefore clears T,so bt at665d48 can only fall through. No source branch/helper result is modified to force the infeasible edge. This proof supplements113dynamically executed rows/17outcomes and does not prove physical IRQ/concurrency or broader graphics behavior.')
 (P/'analysis/movie_graphics_display_infeasible_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('Verified display edge infeasible:original R5=1 forces T=0')
if __name__=='__main__':main()
