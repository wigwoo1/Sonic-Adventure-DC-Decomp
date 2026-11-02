"""Exercise the two special modes selected by original literal words."""
import dataclasses,gc,json
from pathlib import Path
import verify_model_view_producer as scene
P=Path(__file__).resolve().parent;q=scene.q
def main():
 target=P/'analysis/model_view_producer_special_modes_verification.json';assert not target.exists()
 raw,_,_=scene.prefix.checkpoint();binary=(scene.W/'extracted/1ST_READ.BIN').read_bytes()
 modes=[int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little') for a in (0x8c63aba0,0x8c63aba2)]
 assert modes==[0x88,0xc8];records=[];seen=set()
 for mode in modes:
  for bank in (0,1):
   m,s,f,c=scene.prefix.fresh(raw)
   values={field.name:getattr(c['graphics_in'],field.name) for field in dataclasses.fields(c['graphics_in'])};c['graphics_in']=scene.graphics.GraphicsState(**values)
   f.fpscr=(f.fpscr&~0x200000)|(bank<<21)
   record=q.get(m,0x8c012508);s.pc=c['entry']=0x8c63aa94;s.pr=0x8cfed000;s.r[4]=record;s.r[5]=mode
   result,path,after,ss,ff=q.fast.compare(m,s,f,c)
   assert result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match']
   special=set(range(0x8c63ab86,0x8c63ab98,2));assert special<=set(path)
   assert q.get(after,0x8c8ea3e0)==record and q.get(after,0x8c8ea460)==1
   seen.update(path);records.append(dict(mode=hex(mode),bank=bank,status=0,steps=len(path),boundaries=result['boundary_comparisons'],full_state_match=True,ordered_boundaries_match=True,special_rows_covered=9))
   print('SPECIAL MODE VERIFIED',hex(mode),bank,flush=True)
   del m,s,f,c,result,path,after,ss,ff;gc.collect()
 previous=P/'analysis/model_view_producer_units_verification.json';units=json.loads(previous.read_text());coverage=[]
 for row in units['coverage']:
  missing={int(a,16) for a in row['uncovered']};uncovered=missing-seen
  coverage.append(dict(address=row['address'],instruction_rows=row['instruction_rows'],covered=row['instruction_rows']-len(uncovered),uncovered=list(map(hex,sorted(uncovered)))))
 assert sum(x['covered'] for x in coverage)==223 and not any(x['uncovered'] for x in coverage)
 files=[Path(__file__),previous,P/'analysis/model_view_producer_translation.json',q.v.effects.DLL]
 report=dict(status='special_modes_and_combined_all223row_coverage_verified',comparisons=4,prior_comparisons=units['comparisons'],combined_unit_comparisons=units['comparisons']+4,cases=records,combined_coverage=coverage,explicit_unit_inputs=True,mode_values_from_original_literals=['0x8c63aba0','0x8c63aba2'],original_native_complete_state_and_ordered_boundaries_match=True,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):scene.sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('ALL223 ROWS COVERED',flush=True)
if __name__=='__main__':main()
