"""Actual input/tick callers with enabled input and declared empty task pool."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_active_input as fast
import movie_active_input_fixtures as fixtures
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 records=[];checks=0;ram=v.seed_ram()
 for entry,mode,counter in itertools.product((v.CD_INPUT,v.CD_TICK),range(8),(0,7,0xffffffff)):
  c=v.Input(dict(entry=entry,mode=mode,sr=0x400000a1,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,input_enabled=1,countdown_value=2,game_mode=0,scheduler_counter=counter));m,s,f=fixtures.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n=3
  assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=3
  targets=[b['target'] for b in r['boundaries']];assert targets.count('0x8c668e60')==targets.count('0x8c110954')==1 and '0x8c647908' in targets and '0x8c66085a' in targets;n+=4
  assert v.load(a,fixtures.ROOT+fixtures.fields(m)['counter'])==(counter+1)&0xffffffff;n+=1
  assert v.load(a,v.load(m,0x8c602f00))==1 and targets.count('0x8c6033e4')==targets.count('0x8c647ca4')==8;n+=3
  if entry==v.CD_TICK:assert v.load(a,v.load(m,0x8c051884))==1;n+=1
  checks+=n;records.append(dict(entry=hex(entry),mode=mode,counter_before=counter,counter_after=(counter+1)&0xffffffff,steps=len(path),boundaries=len(targets),independent_checks=n))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_active_input_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='48 fresh actual602cb8input/051810tick caller comparisons across8FPUmodes and3scheduler counters. Input enabled;actual647908->66085a->668e60->110954 completes for declared empty task pool,then real channel/status readers execute8times. Counterwrap,callee-preservation,call order and unchanged input flag checked independently. No acquired controller events,new pressed/released callbacks,natural task initialization,automatic frame timing or full caller recovery credit.')
 (P/'analysis/movie_active_input_callers_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ACTIVE INPUT CALLERS PASS',len(records),checks)
if __name__=='__main__':main()
