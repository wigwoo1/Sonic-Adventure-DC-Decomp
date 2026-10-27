"""Original multi-channel admission and interleaved coroutine lifecycles."""
import hashlib,itertools,json
from pathlib import Path
import types
import verify_movie_channel_worker as template
import write_watch_channel_decoder as fast
bindings=dict(template.__dict__,v=fast.old,fast=fast)
parent=types.SimpleNamespace(__file__=template.__file__,v=fast.old,fast=fast,admit=types.FunctionType(template.admit.__code__,bindings),run=types.FunctionType(template.run.__code__,bindings))
v=parent.v;P=v.P;fast=parent.fast
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=v.seed_ram();records=[];comparisons=0;checks=0
 for mask,mode in itertools.product((0,0x55,0xaa,0xff),(0,4)):
  c,m,s,f,r,path=parent.admit(ram,mask,0x400000a1,mode);comparisons+=1;n=3
  owner=v.load(m,v.load(m,0x8c660880));channels=[j for j in range(8) if mask&(1<<j)];steps=[]
  for cycle in range(4):
   r,path,m,s,f=parent.run(m,s,f,c,v.AI_SCHEDULE);comparisons+=1
   assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n+=3
   assert sum(b['target']=='0x8c66086a' for b in r['boundaries'])==len(channels);n+=1
   assert sum(pc==0x8c647982 for pc in path)==(len(channels) if cycle==0 else 0) and sum(pc==0x8c647b74 for pc in path)==(0 if cycle==0 else len(channels));n+=2
   for j,channel in enumerate(channels):
    rec=owner+0x210+j*0x124;sp=v.load(m,rec+0x18)
    assert v.load(m,rec+0x20)==0x8c668e48 and v.load(m,sp)==0x8c647b74;n+=2
    assert v.load(m,sp+4)==v.load(m,0x8c6479bc)+0x80*channel and v.load(m,rec+0x18+0x14)==channel;n+=2
    assert v.load(m,rec+0xc)==1 and v.load(m,rec+0x14)==cycle+1;n+=2
   assert v.load(m,owner+0x204)==cycle+1 and v.load(m,owner+0x20c)==0;n+=2
   steps.append(dict(cycle=cycle,yields=len(channels),steps=len(path)))
  r,path,m,s,f=parent.run(m,s,f,c,v.OWNER_ALL);comparisons+=1
  assert r['status']==0 and v.sample(m,owner,0x2694)==bytes(0x2694) and sum(b['target']=='0x8c668cca' for b in r['boundaries'])==len(channels);n+=3
  checks+=n;records.append(dict(mask=hex(mask),mode=mode,channels=channels,cycles=steps,original_native_comparisons=6,independent_checks=n));print('WORKER POOL',hex(mask),mode,len(channels),'tasks /4cycles',flush=True)
 files=[Path(__file__),Path(parent.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_decoder_connection.json',v.effects.DLL]
 report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Eight fresh actualoriginal admission/interleaved scheduler/teardown sequences:zero,four,eight tasks,alternating/fullmasks andFR0/FR1. Four realyield/resume cycles percreatedtask,independentdeadline/context-SP/workerPR/channelidentity/taskcounts andwholeownerzeroing. No slots/contexts/counters manufactured or repaired;originalowner+20c remainszero. Idleworker only;nonidledecoder/callbackdomains andphysicalIRQtiming remainpartial. Sixoriginal/native comparisons per lifecycle.')
 (P/'analysis/movie_channel_decoder_worker_regression.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL WORKER POOL PASS',comparisons,checks)
if __name__=='__main__':main()
