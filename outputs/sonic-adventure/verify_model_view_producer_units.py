"""Explicit unit inputs for the new original source-record dependency family."""
import dataclasses,gc,hashlib,json
from pathlib import Path
import verify_model_view_producer as scene
P=Path(__file__).resolve().parent;W=scene.W;q=scene.q
def main():
 target=P/'analysis/model_view_producer_units_verification.json';assert not target.exists()
 raw,_,_=scene.prefix.checkpoint();cases=[];records=[];seen=set()
 for bank in (0,1):
  cases += [dict(kind='power',entry=0x8c63aa04,selector=n,bank=bank) for n in (*range(21),0xffffffff)]
 for bits in (0,0x100000,0x200000,0x300000):cases.append(dict(kind='vector',entry=0x8c63aa74,bits=bits))
 for mode in (0,17,19,20):cases.append(dict(kind='initialize',entry=0x8c63aa94,mode=mode))
 cases += [dict(kind='full_table',entry=0x8c63aa94),dict(kind='invalid_record',entry=0x8c63aa94),dict(kind='invalid_vector',entry=0x8c63aa74)]
 for case in cases:
  m,s,f,c=scene.prefix.fresh(raw)
  values={field.name:getattr(c['graphics_in'],field.name) for field in dataclasses.fields(c['graphics_in'])};c['graphics_in']=scene.graphics.GraphicsState(**values)
  expected=0
  if case['kind']=='power':
   f.fpscr=(f.fpscr&~0x200000)|(case['bank']<<21);f.banks[case['bank']][4]=0x3f900000;args=[case['selector']]
  elif case['kind'] in ('vector','invalid_vector'):
   f.fpscr=(f.fpscr&~0x300000)|case.get('bits',0);f.banks[(f.fpscr>>21)&1][4]=0x3f400000
   source,dest=0x8cf30000,0x8cf30200
   for i,value in enumerate((0x3fc00000,0xbfc00000,0x3eaaaaab,0x3f800001,0x3dcccccd,0x40000000,0x3f000000,0xbf000000)):q.put(m,source+4*i,value)
   args=[dest,source if case['kind']=='vector' else 0]
   if case['kind']=='invalid_vector':expected=-3
  else:
   record=q.get(m,0x8c012508);args=[record,case.get('mode',19)]
   if case['kind']=='full_table':q.put(m,0x8c8ea460,32)
   if case['kind']=='invalid_record':args[0]=0;expected=-3
  s.pc=c['entry']=case['entry'];s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value
  result,path,after,ss,ff=q.fast.compare(m,s,f,c)
  assert result['status']==expected and result['full_state_match'] and result['ordered_boundaries_match']
  independent=[]
  if case['kind']=='full_table':
   assert q.get(after,0x8c8ea3e0)==0 and q.get(after,0x8c8ea460)==32
   assert result['boundary_comparisons']==1;independent.append('full32entrytable rejects registration without nested calls')
  if case['kind']=='initialize':
   assert q.get(after,0x8c8ea3e0)==args[0] and q.get(after,0x8c8ea460)==1
   independent.append('original stores register the supplied record and increment count once')
  if case['kind']=='invalid_record':
   assert q.get(after,0x8c8ea460)==1;independent.append('registration commits before the reached invalid record write; failed state discarded')
  seen.update(path);records.append(dict(case,status=result['status'],stop_pc=result['stop_pc'],steps=len(path),boundaries=result['boundary_comparisons'],full_state_match=True,ordered_boundaries_match=True,independent_checks=independent))
  print('PRODUCER UNIT',len(records),case['kind'],expected,flush=True)
  del m,s,f,c,result,path,after,ss,ff;gc.collect()
 translation=P/'analysis/model_view_producer_translation.json';t=json.loads(translation.read_text())
 coverage=[]
 for routine in t['routines']:
  body={int(a,16) for a in routine['reachable_addresses']};coverage.append(dict(address=routine['address'],instruction_rows=routine['instruction_rows'],covered=len(body&seen),uncovered=list(map(hex,sorted(body-seen)))))
 files=[Path(__file__),P/'verify_model_view_producer.py',translation,q.v.effects.DLL,P/'analysis/model_view_producer_verification.json']
 report=dict(status='original_source_record_family_units_verified_with_explicit_inputs',comparisons=len(records),cases=records,coverage=coverage,faulted_state_resumed=False,original_guards_weakened=False,unit_records_claimed_as_retail_assets=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):scene.sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('PRODUCER UNITS VERIFIED',len(records),coverage,flush=True)
if __name__=='__main__':main()
