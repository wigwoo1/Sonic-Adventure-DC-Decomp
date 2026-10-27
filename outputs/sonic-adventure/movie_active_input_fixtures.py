"""Declared scheduler records and independently decoded saved CPU contexts."""
import movie_active_input_reference as v
import movie_countdown_fixtures as prior
ROOT,RECORD,CONTEXT=0x8cd80000,0x8cda0000,0x8cdc0000
put,half,offset=prior.put,prior.half,prior.offset
def literal(m,a,x):put(m,v.load(m,a),x)
def fields(m):return dict(record=offset(m,0x8c668e9a),active=offset(m,0x8c668f2e),counter=offset(m,0x8c668f30),stride=offset(m,0x8c668f32),count=offset(m,0x8c668f34),live=offset(m,0x8c668d70),callback=offset(m,0x8c668d6a))
def context(m,base,regs,pr,fpscr,fp,fpul,mach,macl,gbr,wide=False):
 # Scalars are relative to the cursor after fifteen postincrements. A wide
 # load consumes eight bytes and alternates physical bank, exactly as encoded.
 delta=60 if wide else 0;scalar=base+delta
 for i in range(64):put(m,base+i*4,(0x1020304*(i+1))&0xffffffff)
 values={0:regs[15],4:regs[14],8:pr,0x30:regs[4],0x44:mach,0x48:macl,0x4c:gbr,0x90:fpul,0x94:fpscr}
 values.update({0xc+i*4:regs[13-i] for i in range(9)})
 values.update({0x34+i*4:regs[3-i] for i in range(3)})
 for off,value in values.items():put(m,scalar+off,value)
 if fp is not None:
  for reg in range(16):put(m,base+0x50+reg*4,fp[reg])
 return scalar
def prepare(ram,c):
 wanted=dict(c)
 if c['entry'] in v.NEW:m,s,f=v.prepare(ram,c)
 else:m,s,f=prior.prepare(ram,c)
 c.update(wanted);s.sr=c['sr'];entry=c['entry']
 for bank in range(2):
  for reg in range(16):f.banks[bank][reg]=(c.get('pattern',0x7fa12345)^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
 f.fpscr|=0x1f07f
 fs=fields(m)
 if entry in (v.CD_INPUT,v.CD_TICK) and c.get('input_enabled'):
  literal(m,0x8c660880,ROOT)
  for off,value in ((fs['count'],0),(fs['counter'],c.get('scheduler_counter',7)),(fs['live'],0)):put(m,ROOT+off,value)
 if entry==v.AI_SCHEDULE:
  s.r[4]=ROOT;literal(m,0x8c668ea4,0x13572468);literal(m,0x8c668eb4,0x24681357)
  put(m,ROOT+fs['count'],c.get('task_count',len(c.get('states',[]))));put(m,ROOT+fs['live'],9);put(m,ROOT+fs['counter'],c.get('counter',7));put(m,ROOT+fs['active'],0x89abcdef)
  for i,status in enumerate(c.get('states',[])):
   rec=ROOT+fs['record']+i*fs['stride'];put(m,rec+0xc,status);put(m,rec+0x14,c.get('deadline',c.get('counter',7)+1));put(m,rec+fs['callback'],c.get('callback',0))
   if status==1:
    regs=list(s.r);regs[9]=v.load(m,0x8c668ea4);regs[10]=v.load(m,0x8c668eb0);regs[11]=3;regs[12]=i;regs[13]=ROOT;regs[14]=rec;regs[15]=s.r[15]-32
    context(m,rec+0x18,regs,0x8c668ef2 if not c.get('resume') else 0x8c668e92,v.load(m,0x8c668eb8),list(f.banks[0]),getattr(f,'fpul',0),c['mach'],c['macl'],c['gbr'])
    if c.get('resume'):
     # Resume an explicitly declared completed-iteration continuation; restore
     # r12/r14 after that iteration so the real loop terminates naturally.
     put(m,rec+0x18+0x10,len(c['states']));put(m,rec+0x18+4,rec+fs['stride'])
  if c.get('unknown_context'):
   rec=ROOT+fs['record'];put(m,rec+0x18+8,0x8cfef000)
 if entry==v.AI_CLEANUP:
  s.r[4]=ROOT;s.r[5]=RECORD;put(m,ROOT+fs['live'],c.get('live',0));put(m,RECORD+0xc,3);put(m,RECORD+fs['callback'],c.get('callback',0))
 if entry==v.AI_RESTORE:
  s.r[4]=CONTEXT;s.r[5]=c.get('value',0);regs=[(c.get('pattern',1)^0x11223344*(i+1))&0xffffffff for i in range(16)];regs[15]=0x8ce31000
  fpscr=c.get('restored_fpscr',0x40000);context(m,CONTEXT,regs,s.pr if not c.get('unknown_context') else 0x8cfef000,fpscr,None,0xa5b6c7d8,0x89abcdef,0x76543210,0x8ce01234,bool(f.fpscr&0x100000))
 return m,s,f
def restore_expected(m,before,fp,c,base=CONTEXT):
 wanted=bytearray(m);put(wanted,v.load(m,0x8c110950),before.r[5]);regs=list(before.r);newfp=type(fp).from_buffer_copy(fp);width=8 if fp.fpscr&0x100000 else 4;bank=(fp.fpscr>>21)&1
 for reg in range(16):
  address=base+0x50+reg*width
  if width==4:newfp.banks[bank][reg]=v.load(m,address)
  else:
   physical=bank^(reg&1);j=reg&~1;newfp.banks[physical][j]=v.load(m,address);newfp.banks[physical][j+1]=v.load(m,address+4)
 scalar=base+(60 if width==8 else 0)
 for reg,off in [(15,0),(14,4),(4,0x30)]+[(13-i,0xc+i*4) for i in range(9)]+[(3-i,0x34+i*4) for i in range(3)]:regs[reg]=v.load(m,scalar+off)
 regs[0]=before.r[5] or 1;pr=v.load(m,scalar+8);newfp.fpscr=v.load(m,scalar+0x94);newfp.fpul=v.load(m,scalar+0x90)
 return wanted,regs,(before.sr&~1)|int(before.r[5]==0),pr,newfp,[v.load(m,scalar+off) for off in (0x44,0x48,0x4c,0x90)]
def scheduler_expected(m,before,fp,c):
 wanted=bytearray(m);sp=before.r[15];fs=fields(m);scratch=sp-32
 for j,reg in enumerate((14,13,12,11,10,9),1):put(wanted,sp-j*4,before.r[reg])
 put(wanted,sp-28,before.pr);put(wanted,scratch,fp.fpscr)
 literal(wanted,0x8c668eb4,v.load(m,v.load(m,0x8c668ea4)));literal(wanted,0x8c668ea4,ROOT)
 # Independently construct actual110954 context-save footprint at ROOT+4.
 regs=list(before.r);regs[4]=ROOT+4;regs[9]=v.load(m,0x8c668ea4);regs[12]=0;regs[13]=ROOT;regs[14]=ROOT+fs['record'];regs[15]=scratch;regs[1]=v.load(m,0x8c668eb8);regs[2]=v.load(m,0x8c668eb4);regs[3]=v.load(m,0x8c668eac)
 base=ROOT+4;values={0:regs[15],4:regs[14],8:0x8c668e92,0x44:c['mach'],0x48:c['macl'],0x4c:c['gbr'],0x90:getattr(fp,'fpul',0),0x94:v.load(m,0x8c668eb8)}
 values.update({0xc+i*4:regs[13-i] for i in range(10)});values.update({0x34+i*4:regs[3-i] for i in range(4)})
 for off,value in values.items():put(wanted,base+off,value)
 for reg in range(16):put(wanted,base+0x50+reg*4,fp.banks[0][reg])
 live=9;iterations=max(0,c.get('task_count',len(c.get('states',[]))));states=c.get('states',[])
 for i,status in enumerate(states[:iterations]):
  rec=ROOT+fs['record']+i*fs['stride'];put(wanted,ROOT+fs['active'],rec)
  if status==3:
   live=(live-1)&0xffffffff;put(wanted,ROOT+fs['live'],live);put(wanted,rec+0xc,0);put(wanted,scratch-4,rec)
  if status==2:put(wanted,rec+0xc,3);put(wanted,rec+0x20,v.load(m,0x8c668f38))
  if status==1 and c.get('deadline',c.get('counter',7)+1)<=c.get('counter',7):
   put(wanted,v.load(m,0x8c110950),1)
   if c.get('unknown_context'):return wanted
   if c.get('resume'):break
 put(wanted,ROOT+fs['counter'],(c.get('counter',7)+1)&0xffffffff);literal(wanted,0x8c668ea4,v.load(wanted,v.load(m,0x8c668f3c)))
 return wanted
