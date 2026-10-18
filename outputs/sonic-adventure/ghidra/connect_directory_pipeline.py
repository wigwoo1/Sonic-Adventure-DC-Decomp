from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_directory_load_multisector.py').read_text().replace('from verify_tikal_controller import Call,P','from verify_tikal_controller import Call,P\nfrom verify_engine_cache_writeback import IO,Read,Write')
pipeline=(p/'verify_file_mount_sector_read_connected.py').read_text()
a=pipeline.index('for address,name,dll');b=pipeline.index('cases=0',a)
setup=pipeline[a:b].replace('functions[address]=helper','helpers[address]=helper')
setup+="bridge=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_mount_sector_read.dll')).sa_file_mount_sector_read;bridge.argtypes=abi;helpers[0x8c5ffc98]=bridge\n"
t=t.replace('sectors=[',setup+'sectors=[').replace('[0,1,2],[0,0x200000]','[0],[0,0x200000]')
t=t.replace(' events=[];actual=[]'," for o,v in [(0x673414,0x8ca10000),(0xa10014,0x8ca11000),(0xa11000,0x8ca10000),(0xa1000c,0x8ca12000),(0xa1003c,0x8ca12000),(0xa12004,0x8cb10000),(0xa12040,0x8cb20000),(0xa12048,0x8cb30000)]:struct.pack_into('<I',ram,o,v)\n events=[];actual=[]")
a=t.index(' def service(');b=t.index(' native=bytearray',a)
t=t[:a]+''' def put(m,o,data):
  for i,v in enumerate(data):m[o+i]=v
 def device(target,m,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8cb10000:
   index=regs[4]-123;assert index in [0,1] and regs[5]==1 and regs[6]==0x8ca00000
   put(m,0xa00000,sectors[index]);regs[0]=0x1234
  elif target==0x8cb20000:
   put(m,0xa11014,struct.pack('<I',1));put(m,0xa11050,struct.pack('<H',1));put(m,0xa1001c,bytes(4));regs[0]=0
  else:assert target==0x8cb30000;regs[0]=0
 hooks={}
 for target in [0x8cb10000,0x8cb20000,0x8cb30000]:
  def hook(m,regs,status,target=target):device(target,m,regs,events);return status
  hooks[target]=hook
 @Read
 def read(ctx,a,n):assert a==0xffd8000c and n==4;return 0xffffffff
 @Write
 def write(*args):raise AssertionError('unexpected write')
 io=IO(None,read,write)
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target in [0x8cb10000,0x8cb20000,0x8cb30000]:device(target,m,regs,actual);return 0
  args=[m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if target==0x8c60260e:return clock(*args,ctypes.byref(io),None,call)
  return helpers[target](*args,None,call)
''' +t[b:]
t=t.replace('hooks={0x8c5ffc98:hook}','hooks=hooks,mmio={0xffd8000c:0xffffffff}')
t=t.replace('len(actual)==read_count','len(actual)==read_count*3').replace('[e[0] for e in actual]==list(range(123,123+read_count))','[e[1][0] for e in actual if e[0]==0x8cb10000]==list(range(123,123+read_count))')
t=t.replace('file_directory_load_multisector_verification.json','file_directory_load_native_pipeline_verification.json').replace('two modeled sector reads','native mount-sector bridge, handle acquisition, read submission/wait/status/clock with modeled device read/start/poll callbacks').replace('successful load, first/second read failures and banks','successful load and banks').replace('multi-sector directory loader comparisons passed','native pipeline directory comparisons passed')
(p/'verify_file_directory_load_native_pipeline.py').write_text(t)
