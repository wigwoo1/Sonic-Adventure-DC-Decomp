"""Supplemental motion primitive cases; preserve the first comparison report."""
import collections,itertools,json,sys
from pathlib import Path
import verify_draw_motion_variants as base
from verify_overlay_selector_initializer import load,store
PROJECT=base.PROJECT
COMPLETE=(base.SETUP,base.SELECT,base.CHANNEL,base.CHANNEL_SELECT,*base.DEFAULTS,0x8c041732)

def cases():
 for entry,alias,bank in itertools.product(COMPLETE,(0x0c000000,0x8c000000,0xac000000),range(2)):
  yield dict(entry=entry,alias=alias,mode=bank*4,channel=bank,semantic=True,model=True)
 for entry,flags,model,bank in itertools.product((0x8c041732,),(0,8,0xffffffff),(False,True),range(2)):
  yield dict(entry=entry,flags=flags,model=model,mode=bank*4,channel=bank,semantic=True)
 for entry,channels,fmt,flags,bank in itertools.product((base.SELECT,base.CHANNEL_SELECT),range(8),(0,3,8),(0,32),range(2)):
  yield dict(entry=entry,channels=channels,format=fmt,flags=flags,mode=bank*4,channel=bank,semantic=True)
 for entry,fmt,channels,bank in itertools.product((base.SETUP,base.CHANNEL),(0,0xc003,0xffff),(0,7,0x8000,0xffff),range(2)):
  yield dict(entry=entry,format=fmt,channels=channels,channel=bank,mode=bank*4,semantic=True)
 for entry,special,enable,bank in itertools.product((base.SETUP,base.CHANNEL),(0,0x80000000,1,0x7f800000,0xff800000,0x7fc12345,0x7f812345),(0,0xf80),range(2)):
  yield dict(entry=entry,special=special,enable=enable,mode=bank*4,channel=bank,probe=True)
 for entry,payload,bank in itertools.product(base.DEFAULTS,(0,0x80000000,1,0x7fc12345,0x7f800000),range(2)):
  yield dict(entry=entry,payload=payload,mode=bank*4,channel=bank,probe=True)
 for entry,bad,bank in itertools.product(COMPLETE,('stack','source'),range(2)):
  yield dict(entry=entry,bad=bad,mode=bank*4,channel=bank)

def prepare(ram,c):
 m,s,f=base.prepare(ram,c)
 entry=c['entry'];channel=c.get('channel',0)
 # Distinct output sentinels make omitted stores observable, independently of
 # register comparisons. These are pre-call output cells, never callee results.
 if entry==base.SELECT:
  for a in (0x8c88fe24,0x8c88fe2c,0x8c88fe30,0x8c88fe34):store(m,a,0xdeadbeef)
 if entry==base.CHANNEL_SELECT:
  for a in (0x8c88fe80,0x8c88fe98,0x8c88fea0,0x8c88fea8):store(m,a+channel*4,0xdeadbeef)
  store(m,0x8c19ad44,0xdeadbeef)
 if entry in (base.SETUP,base.CHANNEL):
  output=base.MOTION if entry==base.SETUP else base.DUAL_STATE+channel*28
  for off in (0,4,8,12,16,20,24):store(m,output+off,0xa5a5a5a5)
  for a in ((0x8c749100,0x8c749102) if entry==base.SETUP else (base.FORMATS+channel*2,base.TYPES+channel*2)):store(m,a,0xbeef,2)
 if 'alias' in c:s.r[4]=(s.r[4]&0xffffff)|c['alias']
 if 'payload' in c:
  for off in (8,12,16,0x14,0x18,0x1c,0x20,0x24,0x28):store(m,base.NODE+off,c['payload'])
 store(m,base.MODEL,0x12345678);store(m,base.MODEL+4,0xa5b6c7d8)
 return m,s,f

def main():
 proofs=[PROJECT/'analysis/draw_motion_variants_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert base.digest(p)==h,p
 ram=bytearray(16<<20)
 for path,offset in ((base.SEED.state.gate.support.morph.MAIN,0x10000),(base.SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[offset:offset+len(data)]=data
 ram=base.caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]
 records=[];coverage=set();edges=set();boundaries=0
 selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:12]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c)
  # The independent model-pointer copy observes actual original instructions;
  # its destination words are distinct from the deliberately nonzero inputs.
  checks=0
  if c['entry']==0x8c041732 and c.get('semantic') and not c.get('bad'):
   initial=bytearray(m);check_state=base.State.from_buffer_copy(s);check_f=base.Fpu.from_buffer_copy(f)
   ref=base.QueueReference();ref.ram=m;base.namespace['queue_reference']=ref
   rc,*_=base.reference_run(m,check_state,check_f,c,tracked=())
   if rc==0:
    channel=c.get('channel',0);a=load(initial,0x8c0418f8)+channel*4;b=load(initial,0x8c0418fc)+channel*4
    wanted=(0x12345678,0xa5b6c7d8) if c.get('model') and not c.get('flags',0)&8 else (load(initial,a),load(initial,b))
    assert (load(m,a),load(m,b))==wanted,(c,'independent model-pointer buffer record');checks=1
   m=initial
  record,path,b=base.compare(m,s,f,c);record['independent_checks']+=checks
  records.append(record);coverage.update(path);boundaries+=b
  for i,a in enumerate(path[:-1]):
   if a not in base.OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%128==0:print(n+1,'supplemental comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Supplemental smoke passed:',len(records));return
 files=[Path(__file__),PROJECT/'verify_draw_motion_variants.py']+proofs
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),instruction_coverage=[hex(a) for a in sorted(set(base.OWN)&coverage)],conditional_outcomes=[[hex(a),hex(b)] for a,b in sorted(edges)],boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),external_game_service_fixtures=0,dll_sha256=base.digest(base.DLL),sha256={str(p):base.digest(p) for p in files},scope='Supplemental original-instruction comparisons: 0c/8c/ac RAM aliases, full 16-bit type/masked-format setup, both banks/channels, special FP inputs, enabled exceptions, invalid source/stack and complete model-pointer buffer helper. Existing numerical/valid-RAM domains retained; no whole-renderer credit.',cases=records)
 assert not report['unconnected_targets']
 (PROJECT/'analysis/draw_motion_variants_extended_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','independent_cases','independent_checks','boundary_comparisons')},indent=2))
if __name__=='__main__':main()
