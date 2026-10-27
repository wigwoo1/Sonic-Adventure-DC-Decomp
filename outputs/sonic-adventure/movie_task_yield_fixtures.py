"""Explicit coherent task/owner frames; no callback or saved-context results mocked."""
import movie_task_yield_reference as v
import movie_active_input_fixtures as old
ROOT,RECORD=old.ROOT,old.RECORD
put,offset,literal=old.put,old.offset,old.literal
def prepare(ram,c):
 wanted=dict(c);m,s,f=v.prepare(ram,c);c.update(wanted);s.sr=c['sr'];f.fpscr=0x40001|(c.get('mode',0)<<19)
 for bank in range(2):
  for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
 f.fpul=0x12345678;s.r[4]=c.get('delay',7);literal(m,0x8c668ea4,ROOT);put(m,ROOT+offset(m,0x8c668ea0),RECORD);put(m,ROOT+offset(m,0x8c668e9e),c.get('counter',11));put(m,RECORD+12,1)
 regs=[(0x31415926^((j+1)*0x01020304))&0xffffffff for j in range(16)];regs[15]=s.r[15]-8
 if c['entry']==v.YIELD_RESUME:
  s.r[0]=c.get('value',1);put(m,s.r[15],s.pr if not c.get('unknown') else 0x8cfef000);put(m,s.r[15]+4,s.r[14]);regs[15]=s.r[15]
  s.r[14]=v.load(m,0x8c668ea4)
 old.context(m,ROOT+4,regs,0x8c668e56 if not c.get('unknown_restore') else 0x8cfef000,f.fpscr,list(f.banks[(f.fpscr>>21)&1]),f.fpul,c['mach'],c['macl'],c['gbr'])
 return m,s,f
def save_context(m,base,regs,pr,f,params):
 values={0:regs[15],4:regs[14],8:pr,0x44:params['mach'],0x48:params['macl'],0x4c:params['gbr'],0x90:f.fpul,0x94:f.fpscr}
 values.update({0xc+j*4:regs[13-j] for j in range(10)});values.update({0x34+j*4:regs[3-j] for j in range(4)})
 for off,value in values.items():put(m,base+off,value)
 bank=(f.fpscr>>21)&1;cursor=base+0x8c
 for reg in reversed(range(16)):
  put(m,cursor,f.banks[bank][reg]);cursor-=4
def expected(m,before,fp,c):
 wanted=bytearray(m);regs=list(before.r);entry=c['entry'];sp=before.r[15]
 if entry!=v.YIELD_RESUME:
  put(wanted,sp-4,before.r[14]);put(wanted,sp-8,before.pr);delay=0 if entry==v.FINISH else 1 if entry==v.YIELD_ONE else c.get('delay',7)
  deadline=0x7fffffff if delay&0x80000000 else (c.get('counter',11)+delay)&0xffffffff
  put(wanted,RECORD+0x14,deadline)
  if entry==v.FINISH:put(wanted,RECORD+0xc,3)
  regs[0]=offset(m,0x8c668ea0);regs[1]=deadline;regs[2]=RECORD;regs[3]=v.load(m,0x8c668eac);regs[4]=RECORD+0x18;regs[14]=v.load(m,0x8c668ea4);regs[15]=sp-8
  save_context(wanted,RECORD+0x18,regs,0x8c668e48,fp,c)
 if entry!=v.YIELD_RESUME or not c.get('value',1):
  restore_before=v.effects.State.from_buffer_copy(before);restore_before.r[5]=1
  wanted,regs,sr,pr,newfp,special=old.restore_expected(wanted,restore_before,fp,c,ROOT+4)
  if c.get('unknown_restore'):return wanted,regs,sr,pr,newfp,special
 else:regs=list(before.r);sr=before.sr&~1;newfp=type(fp).from_buffer_copy(fp);special=[c['mach'],c['macl'],c['gbr'],fp.fpul]
 cursor=regs[15];pr=v.load(wanted,cursor);regs[15]+=4;regs[14]=v.load(wanted,regs[15]);regs[15]+=4
 return wanted,regs,sr,pr,newfp,special
