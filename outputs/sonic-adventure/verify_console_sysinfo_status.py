"""Direct ABI/status/pass-through controls for the additive console service adapter."""
import ctypes,hashlib,json
from pathlib import Path
import console_service_reference as model
P=Path(__file__).resolve().parent;ROOT=P.parents[1];U=ctypes.c_uint32;B=ctypes.c_uint8
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 dll=ROOT/'work/sonic-adventure/build/movie_system_state.dll';lib=ctypes.CDLL(str(dll));model.declare(lib);args=lib.sa_console_service.argtypes
 lib.sa_console_status_service.argtypes=args;lib.sa_console_status_service.restype=ctypes.c_int
 info=(B*24)(*((i*19+7)&255 for i in range(24)));flash=(B*64)(*((i*11+3)&255 for i in range(64)));records=[];checks=0
 cases=[dict(name='init',channel=0,selector=0),dict(name='repeat-init',channel=0,selector=0,initialized=1),dict(name='pointer',channel=0,selector=3,initialized=1),dict(name='pointer-before-init',channel=0,selector=3),dict(name='unconfigured-info',channel=0,selector=0,info=False),dict(name='wrong-info-size',channel=0,selector=0,size=23)]
 cases += [dict(name='unsupported-selector',channel=0,selector=x) for x in (1,2,4,0xffffffff)]
 cases += [dict(name='flash-read',channel=1,selector=1,first=3,second=a,count=n) for a in (0x0c900000,0x8c900000,0xac900000) for n in (0,1,15,24)]
 cases += [dict(name='flash-range',channel=1,selector=1,first=63,second=0x8c900000,count=2),dict(name='flash-target',channel=1,selector=1,first=0,second=0,count=4),dict(name='unconfigured-flash',channel=1,selector=1,flash=False),dict(name='unsupported-channel',channel=2,selector=0)]
 cases += [dict(name='null-'+key,channel=0,selector=0,null=key) for key in ('state','ram','result','write_result')]
 for number,c in enumerate(cases,1):
  memories=[bytearray([0xa5])*0x1000000 for _ in range(2)];views=[(B*len(m)).from_buffer(m) for m in memories];states=[model.Console(),model.Console()];results=[]
  for s in states:
   assert lib.sa_console_service_init(ctypes.byref(s))==0
   if c.get('info',True):assert lib.sa_console_set_info(ctypes.byref(s),info,24)==0
   if c.get('flash',True):assert lib.sa_console_set_flash(ctypes.byref(s),flash,64)==0
   s.initialized=c.get('initialized',0)
   if 'size' in c:s.info_size=c['size']
  for i,fn in enumerate((lib.sa_console_service,lib.sa_console_status_service)):
   out=U(0x12345678);write=U(0x87654321);status=fn(None if c.get('null')=='state' else ctypes.byref(states[i]),None if c.get('null')=='ram' else views[i],c['channel'],c['selector'],c.get('first',0xffffffff),c.get('second',0x8c900000),c.get('count',0),None if c.get('null')=='result' else ctypes.byref(out),None if c.get('null')=='write_result' else ctypes.byref(write));results.append((status,out.value,write.value))
  legacy,current=results;status=legacy[0]
  assert current[0]==status and memories[0]==memories[1] and bytes(states[0])==bytes(states[1]);n=3
  expected=bytearray([0xa5])*0x1000000
  if status==0 and c['channel']==0 and c['selector']==0:
   assert legacy==(0,0x12345678,0) and current==(0,0,1);n+=2
   expected[0x68:0x80]=bytes(info);assert states[1].initialized==1 and states[1].info_calls==1;n+=2
  else:assert current==legacy;n+=1
  if c['name']=='pointer':assert current==(0,model.ADDRESS,1);n+=1
  if c['name']=='pointer-before-init':assert status==-4;n+=1
  if c['name']=='flash-read':
   first,count=c['first'],c['count'];expected[0x900000:0x900000+count]=bytes(flash)[first:first+count];assert current==(0,count,1);n+=1
  if c['name']=='flash-range':assert current==(0,0xffffffff,1);n+=1
  if c['name']=='flash-target':assert status==-3;n+=1
  if c['name'].startswith('unconfigured-') or c['name'].startswith('null-'):assert status==-2;n+=1
  if c['name']=='wrong-info-size':assert status==-3;n+=1
  if c['name'].startswith('unsupported-'):assert status==-8;n+=1
  assert memories[1]==expected and bytes(info)==bytes((i*19+7)&255 for i in range(24)) and bytes(flash)==bytes((i*11+3)&255 for i in range(64));n+=3
  if status:assert current[1:]==(0x12345678,0x87654321) and not states[1].info_calls;n+=2
  checks+=n;records.append(dict(input=c,legacy=legacy,current=current,independent_checks=n));print(number,c['name'],current,flush=True)
 files=[Path(__file__),Path(model.__file__),P/'source/console_sysinfo_status.c',P/'source/console_sysinfo_status.h',P/'source/native_console_service.c',P/'source/native_console_service.h',dll]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,original_recovery_credit=0,old_API_state_data_guards_preserved=True,corrected_guest_SYSINFO_INIT_R0=0,dll_sha256=digest(dll),sha256={str(p):digest(p) for p in files},scope='AuthoredadditivefirmwareABIadapter. SuccessfulSYSINFOselector0R0=0/writeflag1;legacyAPIstillpreservesitsoldresult. Complete16MiBRAM/state/counters/data matcholdparent;onlydocumentedstatusoutputs differ. Initrepeat,ID/beforeinit,selector/channel/size/null/unconfigured/errors,FLASHcount/range/DRAMaliases andimmutability controls. No BIOSimplementationcopied orphysical/resetbootclaim.')
 (P/'analysis/console_sysinfo_status_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CONSOLE STATUS',len(records),checks)
if __name__=='__main__':main()
