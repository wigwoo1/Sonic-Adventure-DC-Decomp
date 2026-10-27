"""Independent drawing-state/FPU/register oracle for original24-instruction body."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_float_initialize as fast
v=fast.old;P=v.P;ROOT=v.ROOT;ENTRY=0x8c60edb0
POINTER,SIZE,ZERO,INDEX,FLAGS,FLOATS=0x8c88f64c,0x8c88f650,0x8c88f62c,0x8c88f660,0x8c88f664,0x8c88f668
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def cases():
 for mode,payload,args,sr in itertools.product((0,2,4,6),(0,0x80000000,0x7fa12345,0xffffffff),((0,0),(0xffffffff,0x80000000),(0x8c7ac8e0,4096)),(0,0x600000f3)):
  yield dict(mode=mode,payload=payload,arguments=args,sr=sr)
 for mode,payload,args in itertools.product((1,3,5,7),(0,0x7fa12345),((0,0),(0xffffffff,4096))):yield dict(mode=mode,payload=payload,arguments=args,sr=0x40000001)
def main():
 ram=v.seed_ram();records=[];seen=set();checks=0
 assert [v.load(ram,a) for a in range(0x8c60ee2c,0x8c60ee4c,4)]==[POINTER,SIZE,ZERO,INDEX,FLAGS,FLOATS,0x437f0000,0x8c88f680]
 for number,data in enumerate(cases(),1):
  c=v.Input(dict(entry=ENTRY,mode=data['mode'],float_initialize_arguments=[*data['arguments'],0x12345678,0xfedcba98]));m,s,f=v.prepare(ram,c);s.sr=data['sr']
  m[0x88f620:0x88f690]=bytes(((i*73)^0xa5)&255 for i in range(112))
  f.fpscr=0x40000|(data['mode']<<19)|(0x1f07f if data['payload']&1 else 0);bank=(f.fpscr>>21)&1
  for b in range(2):
   for i in range(16):f.banks[b][i]=(0x7fa00000^(b*0x80+i*0x12345))&0xffffffff
  f.banks[bank][5]=data['payload'];before=v.effects.State.from_buffer_copy(s);expected_fp=v.effects.Fpu.from_buffer_copy(f);expected=bytearray(m)
  pair=bool(f.fpscr&0x100000);unsupported=bool(f.fpscr&0x80000)
  # Pointer and size are raw words, not dereferenced by this initializer.
  v.store(expected,POINTER,before.r[4]);v.store(expected,SIZE,before.r[5])
  wanted=list(before.r);wanted[0]=INDEX;wanted[1]=ZERO;wanted[2]=FLAGS;wanted[3]=0xffffffff;wanted[4]=0
  if not unsupported:
   for address,value in ((ZERO,0),(INDEX,0xffffffff),(FLAGS,0),(FLOATS,0x437f0000),(FLOATS+4,0),(FLOATS+8,0),(FLOATS+12,0)):v.store(expected,address,value)
   if pair:
    # Four overlapping paired stores leave only the final FR5 word beyond the
    # ordinary four-float range. Odd FR3 load targets the opposite physical bank.
    v.store(expected,FLOATS+16,data['payload']);expected_fp.banks[bank^1][2]=0x437f0000;expected_fp.banks[bank^1][3]=0x8c88f680
   else:expected_fp.banks[bank][3]=0x437f0000
   expected_fp.banks[bank][4]=0;wanted[0]=12;wanted[4]=FLOATS
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  assert r['full_state_match'] and r['ordered_boundaries_match'];n=2
  assert r['status']==(-1 if unsupported else 0);n+=1
  assert a==expected and bytes(f)==bytes(expected_fp);n+=2
  assert list(s.r)==wanted and s.sr==before.sr and s.pr==before.pr;n+=3
  assert s.pc==(0x8c60edc2 if unsupported else before.pr) and s.r[15]==before.r[15];n+=2
  assert r['macl']==c.get('macl',0x12345678) and r['final_gbr']==c.get('gbr',0);n+=2
  assert len(r['boundaries'])==1 and r['boundaries'][0]['target']==hex(ENTRY);n+=2
  assert not any(r['sound_state']) and r['runtime_state'] is None and r['pvr_dma_state'][6]==0 and not r['aperture_events'];n+=4
  checks+=n;records.append(dict(input=data,status=r['status'],steps=len(path),independent_checks=n,paired_word=hex(v.load(a,FLOATS+16)),guarded_unsupported_PR=unsupported));print(number,data['mode'],r['status'],flush=True)
 setter=0x8c60d1f2;destination=v.load(ram,0x8c60d26c)
 for word,sr in itertools.product((0,1,0xffffffff,0x80000000,0x8c7ac8e0,0x8c7ac8e1,0x7fa12345,4096),(0,0x600000f3)):
  c=v.Input(dict(entry=setter,mode=(word&7),float_initialize_arguments=[word,0x76543210,0x89abcdef,0x13579bdf]));m,s,f=v.prepare(ram,c);s.sr=sr
  for offset in range(-16,20,4):v.store(m,destination+offset,0xa5a5f000+(offset&255))
  before=v.effects.State.from_buffer_copy(s);fp=bytes(f);expected=bytearray(m);v.store(expected,destination,word);wanted=list(before.r);wanted[3]=destination
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  assert r['full_state_match'] and r['ordered_boundaries_match'];n=2
  assert r['status']==0 and a==expected and bytes(f)==fp;n+=3
  assert list(s.r)==wanted and s.sr==before.sr and s.pr==before.pr;n+=3
  assert s.pc==before.pr and s.r[15]==before.r[15] and v.load(a,destination)==word;n+=3
  assert len(r['boundaries'])==1 and not any(r['sound_state']) and r['runtime_state'] is None;n+=3
  checks+=n;records.append(dict(input=dict(entry=hex(setter),word=word,sr=sr),status=0,steps=len(path),independent_checks=n,guarded_unsupported_PR=False));print('SETTER',hex(word),hex(sr),flush=True)
 missing=set().union(*(set(body['reachable_addresses']) for body in v.manifest['routines']))-{hex(a) for a in seen};assert not missing and sum(b['instruction_rows'] for b in v.manifest['routines'])==27 and not any(b['conditional_edges'] for b in v.manifest['routines'])
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'write_watch_buffer_initialize.py',v.MP,P/'analysis/movie_float_initialize_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),successful_cases=sum(not r['guarded_unsupported_PR'] for r in records),unsupported_PR_prefix_cases=sum(r['guarded_unsupported_PR'] for r in records),independent_checks=checks,cases=records,executed_rows=27,conditional_outcomes=0,missing_rows=[],setter_destination=hex(destination),full_ram_and_fpu_oracle=True,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original24-rowdrawingstateinitializer and3-rowglobalwordsetter;pointer/size rawwordstores,resets,float255/zeros,FRbankselection,overlappingSZpairedstores,neighborpreservation,allGPR/SR/PR/stack/FPSCR/FPUL/FPUbanks checked independently. Initializer96PR-clearcompletecases/16unsupportedPR-prefixcontrols;PR-setstops-1atFLDI0withrealpriorwrites,no guestunwind. Setter16raw-word/SRcases includingnull/unalignedwordswithoutdereference andactualRTSdelay-slotstore. Completeoriginal/nativeCPU/RAM/VRAM/Sound/platform/orderedboundaries compare. Pairedmodebyteeffects followexistinginstructionmodel;physicalalignment/illegalinstructionexceptiondelivery/reset/gameplaynotclaimed. HighlevelAPI/nameinferred.')
 (P/'analysis/movie_float_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('FLOAT INIT',len(records),checks)
if __name__=='__main__':main()
