"""Actual vector/display producers on raw loaded RAM, without handler fixtures."""
import hashlib,json
from pathlib import Path
import renderer_frame_ta_fixtures as q
P=q.P;R=P.parents[1]
VBR=0x8ce50000


def main():
 target=P/'analysis/renderer_frame_registration_verification.json'
 assert not target.exists()
 cases=[]
 for mask in (0,10):
  m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0))
  s.r[15]=0x8cfff000;s.sr=(s.sr&~0xf0)|(mask<<4);c['vbr']=VBR
  q.v.bind_vectors(VBR)
  assert q.get(m,VBR+0x224)==0
  stages=[]
  for entry,args,label in [(0x8c645de0,[0,0x8ce40000],'actual vector initialization'),(0x8c602128,[0x8cb00000,0x100000],'actual system heap initialization'),(0x8c604702,[1,0,1,15],'actual display and graphics registration')]:
   s.pc=c['entry']=entry;s.pr=0x8cfed000;sp=s.r[15]
   for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
   result,path,m,s,f=q.fast.compare(m,s,f,c)
   assert result['status']==0 and s.pc==s.pr==0x8cfed000 and s.r[15]==sp
   assert result['full_state_match'] and result['ordered_boundaries_match']
   stages.append(dict(label=label,entry=hex(entry),steps=len(path),boundaries=result['boundaries'],vector_slot=hex(q.get(m,VBR+0x224)),sr=hex(s.sr),graphics_output=result['graphics_output_summary']))
   c.update(graphics_in=result['graphics_out'],other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr'])
  assert q.get(m,VBR+0x224)==0x8c652210
  assert [b for b in stages[-1]['boundaries'] if b['target']=='0x8c653640'][0]['arguments'][0]==0x8c652210
  assert ((s.sr>>4)&15)==mask
  # 6930 uses the startup-register bank; the old two-word mask bank covers
  # only 6920/6924. Preserve both existing register routes.
  assert c['graphics_in'].arrays[4][(0x6930-0x6800)//4]==0x2807ec
  cases.append(dict(initial_IMASK=mask,final_IMASK=(s.sr>>4)&15,actual_registered_handler='0x8c652210',actual_vector_slot=hex(VBR+0x224),actual_graphics_mask='0x2807ec',stages=stages))
  print('Original vector/display registration matched; IMASK',mask,flush=True)
 files=[Path(__file__),P/'renderer_frame_ta_fixtures.py',P/'renderer_context_producer_fixtures.py',P/'write_watch_renderer_frame_ta.py',R/'work/sonic-adventure/build/renderer_frame_ta.dll']
 report=dict(status='actual_vector_and_display_registration_original_native_verified',original_native_comparisons=6,cases=cases,new_original_routine_credit=0,native_code_changed=False,automatic_frame_event_delivery=False,overall_completion_percent=10,scope='Raw loaded RAM plus declared CPU SR/VBR/stack and existing external display profile. Original645de0 copies vectors; original604702 calls651520/653640 and installs652210 atVBR+224. No supplied vector,handler table,game flag orcallee result. Mask0 andmask10 remain distinct;this isregistration qualification,not frame completion orreset boot.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n')


if __name__=='__main__':main()
