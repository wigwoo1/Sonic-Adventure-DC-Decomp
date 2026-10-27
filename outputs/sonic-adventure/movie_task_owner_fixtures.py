"""Explicit bounded owner records and independent teardown footprints."""
import movie_task_owner_reference as v
import movie_active_input_fixtures as helpers
put,offset=helpers.put,helpers.offset
OWNER=0x8cd80000
def layout(m):return dict(pool=v.load(m,0x8c668d84),size=offset(m,0x8c668d72),record=offset(m,0x8c668d74),stride=offset(m,0x8c668d66),count=offset(m,0x8c668d76),counter=offset(m,0x8c668d70),callback=offset(m,0x8c668d6a))
def prepare(ram,c):
 wanted=dict(c);m,s,f=v.prepare(ram,c);c.update(wanted);s.sr=c['sr'];d=layout(m)
 owners=[OWNER] if c['entry']==v.OWNER_ONE else [d['pool']+j*d['size'] for j in range(8)]
 s.r[4]=OWNER if c['entry']==v.OWNER_ONE else 0x12345678
 for j,owner in enumerate(owners):
  at=owner&0xffffff;m[at:at+d['size']]=bytes([0xa5^j])*d['size']
  active=1 if c['entry']==v.OWNER_ONE else int(bool(c.get('mask',0)&(1<<j)));put(m,owner,active)
  put(m,owner+d['count'],c.get('task_count',len(c.get('states',[]))));put(m,owner+d['counter'],c.get('counter',0))
  for i,state in enumerate(c.get('states',[])):
   rec=owner+d['record']+i*d['stride'];put(m,rec+12,state);put(m,rec+d['callback'],c.get('callback',0))
 put(m,v.load(m,0x8c668c54),0x12345678)
 return m,s,f
def expected(m,before,c):
 wanted=bytearray(m);d=layout(m);sp=before.r[15];regs=list(before.r);active=[];calls=[];status=0
 for j,reg in enumerate((14,13,12),1):put(wanted,sp-j*4,before.r[reg])
 put(wanted,sp-16,before.pr)
 owner_list=[(0,OWNER)] if c['entry']==v.OWNER_ONE else [(j,d['pool']+j*d['size']) for j in range(8) if c.get('mask',0)&(1<<j)]
 for j,owner in owner_list:
  active.append(owner);inner_sp=sp if c['entry']==v.OWNER_ONE else sp-16
  if c['entry']==v.OWNER_ALL:
   for off,value in ((4,owner),(8,j),(12,8),(16,0x8c668d04)):put(wanted,sp-16-off,value)
  for i in range(max(0,c.get('task_count',len(c.get('states',[]))))):
   rec=owner+d['record']+i*d['stride']
   if not v.load(wanted,rec+12):continue
   calls.append(rec);put(wanted,inner_sp-20,rec);put(wanted,owner+d['counter'],(v.load(wanted,owner+d['counter'])-1)&0xffffffff);put(wanted,rec+12,0)
   if c.get('callback')==0x8cfef000:status=-2;return wanted,regs,None,status,calls,active
  at=owner&0xffffff;wanted[at:at+d['size']]=bytes(d['size'])
 if c['entry']==v.OWNER_ONE:
  regs[:8]=[OWNER,before.r[1],d['stride'] if c.get('task_count',len(c.get('states',[])))>0 else before.r[2],0,OWNER,0,d['size'],d['size']]
 else:
  regs[0]=active[-1] if active else before.r[0];regs[2]=d['size'];regs[3]=0
  if active:regs[4:8]=[active[-1],0,d['size'],d['size']]
 return wanted,regs,before.sr|1,status,calls,active
