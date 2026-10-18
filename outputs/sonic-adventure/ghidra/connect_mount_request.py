from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_mount_native_reserved_handle.py').read_text()
t=t.replace('from verify_tikal_controller import Call,P','from verify_tikal_controller import Call,P\nfrom verify_engine_cache_writeback import IO,Read,Write')
setup='''functions={0x8c600d82:acquire}
for address,name,dll in [(0x8c600cf0,'sa_file_release_wait','file_release_wait'),(0x8c6013a4,'sa_file_status_get','file_status_get'),(0x8c602618,'sa_file_elapsed_get','file_timer_helpers')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
clock=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll')).sa_file_clock_get_io;clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
'''
t=t.replace('cases=0',setup+'cases=0')
t=t.replace('for first,second,bank in itertools.product([0,0x8cb10000],[0,0x8cb20000],[0,0x200000]):','for first,second,bank,scenario in itertools.product([0,0x8cb10000],[0,0x8cb20000],[0,0x200000],[0,1,2,3,4]):')
t=t.replace('(0xa0101c,0x8ca02080)','(0xa0101c,0)').replace('(0xa02080,0x8ca01000)', '(0xa02080,0x8ca01000),(0xa0100c,0x8cb03000),(0xa0103c,0x8cb03000),(0xb0301c,0x8cb30000),(0xb03020,0x8cb40000),(0xb03048,0x8cb50000)')
a=t.index(' def service(');b=t.index(' hooks={}',a)
t=t[:a]+''' def put(m,o,data):
  for i,v in enumerate(data):m[o+i]=v
 def service(target,m,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8c64594c:regs[0]=0x8cb10000
  elif target==0x8cb30000:regs[0]=0 if scenario==0 else 0x1234
  elif target==0x8cb50000:
   put(m,0xa020d0,struct.pack('<H',5));put(m,0xa020d2,struct.pack('<H',0xffef if scenario==4 else 0xffe3));regs[0]=0
  elif target==0x8cb40000:
   put(m,regs[4]&0xffffff,struct.pack('<I',6 if scenario==3 else 3));regs[0]=0 if scenario==1 else 1
  else:raise AssertionError(hex(target))
''' +t[b:]
t=t.replace('for target in [0x8c64594c]:','for target in [0x8c64594c,0x8cb30000,0x8cb40000,0x8cb50000]:').replace('service(target,regs,events)','service(target,m,regs,events)')
t=t.replace(' @Call\n', ''' def clock_hook(m,regs,status):regs[2]=0xffd8000c;regs[3]=0xffffffff;regs[0]=0;return status
 hooks[0x8c60260e]=clock_hook
 @Read
 def read(ctx,a,n):assert a==0xffd8000c and n==4;return 0xffffffff
 @Write
 def write(*args):raise AssertionError('unexpected write')
 io=IO(None,read,write)
 @Call
''')
a=t.index('  if target==0x8c600d82:');b=t.index(' native=bytearray',a)
t=t[:a]+'''  args=[m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if target==0x8c60260e:return clock(*args,ctypes.byref(io),None,call)
  if target in functions:return functions[target](*args,None,call)
  service(target,m,regs,actual);return 0
''' +t[b:]
t=t.replace('assert r[0]==0xfffffffe','assert r[0]==([0xfffffffe,0xfffffffe,0xffffffe3,0xffffffe9,0xffffffef][scenario])')
t=t.replace('file_mount_native_reserved_handle_verification.json','file_mount_native_request_errors_verification.json')
a=t.index("report={'cases_passed'");b=t.index("\n(root/",a)
t=t[:a]+"report={'cases_passed':cases,'scope':'Native mount, reserved-handle acquire, release wait, status and timer helpers with full retail RAM/CPU/FPU/SR/PR equality. Modeled callback registration and device request/poll/info. Zero request, failed disc info, request error-29, disc6 mapping to-23 and other error-17. Independent error results; successful volume mounting not executed.'}"+t[b:]
(p/'verify_file_mount_native_request_errors.py').write_text(t)
