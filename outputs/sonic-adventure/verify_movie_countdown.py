"""Independent signed-load, decrement, call, channel and full-RAM expectations."""
import hashlib,itertools,json,sys
from pathlib import Path
import write_watch_countdown as fast
import movie_countdown_fixtures as fixtures
v=fast.old;P=v.P;CONFIGS=((0,0),(1,0x400000f3),(4,0x600003a1),(7,0x40008000))
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def case(entry,mode=0,sr=0x400000a1,**extra):return dict(entry=entry,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234)|extra
def cases():
 values=(0,1,127,128,255,256,0x7fff,0x8000,0xffff)
 for a,b in itertools.product(values,values):yield case(v.CD_VARIANT,variant_a=a,variant_b=b)
 for (mode,sr),(a,b) in itertools.product(CONFIGS[1:],((0xffff,0),(0,0x8000),(0x8000,0xffff),(0x7fff,0x7fff))):yield case(v.CD_VARIANT,mode,sr,variant_a=a,variant_b=b)
 for mode,sr in CONFIGS:
  yield case(v.CD_NOP,mode,sr)
  for ticks in (0,2,0xffffffff,0x80000000):yield case(v.CD_TICK,mode,sr,countdown_value=ticks,game_mode=0)
  for game_mode,(a,b) in itertools.product((0,4,5,9,10),((0,0),(0,4),(0,8),(1,0),(1,4))):yield case(v.CD_TICK,mode,sr,countdown_value=1,game_mode=game_mode,variant_a=a,variant_b=b)
  yield case(v.CD_TICK,mode,sr,countdown_value=1,game_mode=4,variant_a=0,variant_b=5)
  for enabled in (0,1):yield case(v.CD_INPUT,mode,sr,input_enabled=enabled)
  for ticks in (1,2):yield case(v.CD_TICK,mode,sr,input_enabled=1,countdown_value=ticks,game_mode=0)
  yield case(v.CD_AUDIO,mode,sr,root=False)
  for audio_mode in (0,2):yield case(v.CD_AUDIO,mode,sr,audio_mode=audio_mode)
 for (mode,sr),pending,enabled,(first,second) in itertools.product((CONFIGS[0],CONFIGS[2]),(0,1,0xffffffff),(0,1),((0,1),(1,1),(1,0))):yield case(v.CD_AUDIO,mode,sr,pending=pending,word_enabled=enabled,word1=first,word2=second)
 for entry,(mode,sr),index,status in itertools.product((v.CD_ON,v.CD_OFF),CONFIGS,(0,1,7),(0,1,0xffffffff)):yield case(entry,mode,sr,index=index,record_status=status)
 for mode,payload in itertools.product(range(8),(0,0x80000000,0xffffffff,0x7fa12345)):yield case(v.CD_FPU_WRAPPER,mode,0x400000a1,payload=payload)
 for mode in (0,7):yield case(v.CD_FPU_WRAPPER,mode,0x400080a1,payload=0x12345678)
 for mode in (0,4,7):yield case(v.CD_FPU_WRAPPER,mode,0x400000a1,payload=0x12345678,callback=0x8cfef000)
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 selected=list(cases());selected=selected[:1]+[next(c for c in selected if c['entry']==entry) for entry in (v.CD_NOP,v.CD_TICK,v.CD_AUDIO,v.CD_ON,v.CD_OFF)]+[next(c for c in selected if c['entry']==v.CD_AUDIO and c.get('pending')==1)] if '--smoke' in sys.argv else selected
 for i,data in enumerate(selected,1):
  c=v.Input(data);m,s,f=fixtures.prepare(ram,c);assert all(c[k]==value for k,value in data.items());before=v.effects.State.from_buffer_copy(s);fp=bytes(f);fp_state=type(f).from_buffer_copy(f);entry=c['entry'];expected_status=0;wanted=None
  if entry==v.CD_TICK:wanted,expected_status,expected_pc=fixtures.tick_expected(m,before,c)
  elif entry==v.CD_AUDIO:wanted,expected_calls=fixtures.audio_expected(m,before,c)
  elif entry in (v.CD_ON,v.CD_OFF):wanted,regs,sr=fixtures.channel_expected(m,before,c)
  elif entry==v.CD_INPUT and c.get('input_enabled'):expected_status=-2
  elif entry==v.CD_FPU_WRAPPER:expected_status=-4 if c['sr']&0x8000 else (-2 if c.get('callback')==0x8cfef000 else 0)
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');delta=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+delta<len(path):edges.add((pc,path[j+delta]))
  assert r['status']==expected_status and r['full_state_match'] and r['ordered_boundaries_match'],(i,c,r['status'],r['stop_pc']);n=3
  expected_f=type(f).from_buffer_copy(fp)
  if entry==v.CD_FPU_WRAPPER and expected_status==-2:expected_f.fpscr&=~0x100000
  assert bytes(f)==bytes(expected_f) and r['final_macl']==c['macl'] and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=4
  if entry==v.CD_VARIANT:
   regs=list(before.r);regs[2]=v.load(m,0x8c04f81c);regs[1]=v.load(m,0x8c04f820);regs[3]=fixtures.sx16(c['variant_b'])&0xffffffff;regs[0]=fixtures.packed(c['variant_a'],c['variant_b'])
   assert a==m and list(s.r)==regs and s.sr==before.sr and s.pc==s.pr==before.pr;n+=4
  elif entry==v.CD_NOP:assert a==m and list(s.r)==list(before.r) and s.sr==before.sr and s.pc==s.pr==before.pr;n+=4
  elif entry==v.CD_TICK:
   assert a==wanted and s.pc==expected_pc;n+=2
   assert v.load(a,v.load(m,0x8c051884))==(c['countdown_value']-1)&0xffffffff;n+=1
   if not r['status']:assert s.r[15]==before.r[15] and s.pr==before.pr and list(s.r[8:])==list(before.r[8:]);n+=3
  elif entry==v.CD_AUDIO:
   assert a==wanted and s.r[0]==0 and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]);n+=4
   assert [int(b['target'],16) for b in r['boundaries'][1:]]==expected_calls;n+=1
  elif entry in (v.CD_ON,v.CD_OFF):assert a==wanted and list(s.r)==regs and s.sr==sr and s.pc==s.pr==before.pr;n+=4
  elif entry==v.CD_INPUT:
   assert v.load(a,v.load(m,0x8c602f00))==c.get('input_enabled',0);n+=1
   if r['status']:assert r['stop_pc']=='0x8c668e60';n+=1
   else:assert list(s.r[8:])==list(before.r[8:]) and s.pc==s.pr==before.pr;n+=2
  elif entry==v.CD_FPU_WRAPPER:
   wanted=bytearray(m);regs=list(before.r);pr=before.pr;pc=before.pr
   if expected_status==-4:pc=v.CD_FPU_WRAPPER
   else:
    fixtures.put(wanted,before.r[15]-4,fp_state.fpscr);bank=(fp_state.fpscr>>21)&1
    for j in range(16):fixtures.put(wanted,before.r[15]-8-j*4,fp_state.banks[bank][j])
    fixtures.put(wanted,before.r[15]-72,before.pr);regs[1]=0x100000;regs[3]=before.r[4];regs[4]=before.r[5]
    if expected_status==-2:regs[0]=fp_state.fpscr&~0x100000;regs[15]-=72;pr=0x8c65258c;pc=before.r[4]
    else:regs[0]=fp_state.fpscr
   assert a==wanted and list(s.r)==regs and s.sr==before.sr and s.pr==pr and s.pc==pc;n+=5
  checks+=n;records.append(dict(input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
  if i%50==0 or '--smoke' in sys.argv:print('COUNTDOWN',i,hex(entry),r['status'],flush=True)
 if '--smoke' in sys.argv:print('Countdown smoke PASS',len(records));return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-{(hex(a),hex(b)) for a,b in edges}
  if int(body['address'],16) in (v.CD_TICK,v.CD_VARIANT,v.CD_AUDIO,v.CD_ON,v.CD_OFF,v.CD_NOP,v.CD_FPU_WRAPPER):assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),missing_rows=sorted(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_outcomes=sorted(missing_edges)))
 assert 0x8c051836 in seen,'SHAD row flagged by draft decompiler must actually execute'
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_countdown_connection.json',v.effects.DLL,P/'analysis/countdown_fpu_primary_evidence.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,qualified_new_entries=['0x8c643f98','0x8c602df8','0x8c602e34','0x8c05180c','0x8c652554'],canonical_tick_all55rows_and8outcomes_executed=True,canonical_variant_signed_halfword_cases=93,shad051836_executed=True,fpu_wrapper_vectors=32,fpu_disabled_entry_fault_cases=2,missing_wrapper_callback_cases=3,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual countdown/input/audio callees; independent entireRAM/stack, signed packing, wrap/decrement, table overlap/realNOP/null-target, error records, channel writes/allGPR and preserved-register/call expectations per case. FPUwrapper all49rows,32raw bank/width vectors,2reachedFDentry prefixes,3actual missing-callee prefixes;fullRAM/CPU/FPU expectations. Host-4fault policy,not hardware trap delivery. Input inactive completes;active reaches real668e60. No fake completion/callbacks/IRQ progress. Static table unchanged. Finite standalone coherent RAM fixtures,immutable code/literals/private frames. Canonical tick/variant zero extra credit;IRQlifecycle separate.')
 (P/'analysis/movie_countdown_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COUNTDOWN DIRECT PASS',len(records),checks)
if __name__=='__main__':main()
