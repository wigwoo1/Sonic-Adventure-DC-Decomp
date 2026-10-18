from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_registered_callback.py').read_text()
t=t.replace('cases=0',"backend=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_backend_poll.dll')).sa_file_backend_poll;backend.argtypes=abi\ncases=0")
t=t.replace('for gate,result,bank in itertools.product([0,1,2,0xffffffff],[0,0xffffffef],[0,0x200000]):','for gate,result,bank,active in itertools.product([0,1,2,0xffffffff],[0,0xffffffef],[0,0x200000],[0,1]):')
t=t.replace(' events=[];actual=[]'," for o,v in [(0x673414,0x8ca00000),(0xa0000c,0x8ca02000),(0xa02040,0x8cb10000),(0xa0001c,0x8ca03000 if active else 0)]:struct.pack_into('<I',ram,o,v)\n events=[];actual=[]")
t=t.replace('def service(regs,log):log.append(tuple(regs[i] for i in range(4,8)));regs[0]=result','def service(target,regs,log):log.append((target,tuple(regs[i] for i in range(4,8))));regs[0]=result')
t=t.replace('def hook(m,regs,status):service(regs,events);return status', 'def start_hook(m,regs,status):service(0x8cb10000,regs,events);return status\n def completion_hook(m,regs,status):service(0x8c5ffa12,regs,events);return status')
t=t.replace('def call(ctx,target,m,f,status,regs,pr):assert target==0x8c6005c2;service(regs,actual);return 0','def call(ctx,target,m,f,status,regs,pr):\n  if target==0x8c6005c2:return backend(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  assert target in [0x8cb10000,0x8c5ffa12];service(target,regs,actual);return 0')
t=t.replace('hooks={0x8c6005c2:hook}','hooks={0x8cb10000:start_hook,0x8c5ffa12:completion_hook}')
t=t.replace('([] if gate==1 else [args])','([] if gate==1 else [(0x8cb10000,args)]+([(0x8c5ffa12,args)] if active else []))')
t=t.replace('file_registered_callback_verification.json','file_registered_backend_poll_verification.json').replace('Backend polling/completion explicitly modeled.','Native backend polling connected; device-start and completion-processing bodies explicitly modeled. Active/absent handle verified with independent dispatch order.').replace('registered filesystem callback comparisons passed','registered callback/backend poll comparisons passed')
(p/'verify_file_registered_backend_poll.py').write_text(t)
