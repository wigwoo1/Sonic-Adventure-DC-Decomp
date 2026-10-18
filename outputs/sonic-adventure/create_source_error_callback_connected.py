from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_source_contiguous_check.py').read_text()
s=s.replace("0x8c65b760:load('sound_work_state_get')", "0x8c65e44a:load('sound_source_identity_get'),0x8c65e44e:load('sound_source_error_callback_set')")
s=s.replace('[0,0x8c65b760]','[0,0x8c65e44a]')
s=s.replace('(0x28,callback),(0x2c,0x8ca50000)','(0x28,0),(0x2c,0)')
s=s.replace('  assert a in helpers,hex(a)',"  assert a in helpers,hex(a)\n  if a==0x8c65e44a:\n   assert tuple(regs[i] for i in range(4,6))==(0x8ca20000,0xfffffffd)\n   callback_calls.append(a)")
s=s.replace(' @Call\n def call', ' callback_calls=[]\n @Call\n def call')
needle=" reference(ram,s,{},fpu=f,special={'macl':0})"
extra=''' s.r[4:7]=[0x8ca20000,callback,0x8ca20000]
 for index in range(4,7):r[index]=s.r[index]
 s.pc=0x8c65e44e
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert helpers[0x8c65e44e](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 installed=bytearray(original);struct.pack_into('<2I',installed,0x28,callback,0x8ca20000)
 assert native[0xa20000:0xa20040]==installed
 original=bytes(installed)
 s.r[4:8]=[0x8ca20000,mode,request,0x8ca30004]
 for index in range(4,8):r[index]=s.r[index]
 s.pc=0x8c65e712
'''
assert s.count(needle)==1;s=s.replace(needle,extra+needle)
s=s.replace(' cases+=1'," assert callback_calls==([0x8c65e44a] if mode not in [0,1] and callback else [])\n cases+=1")
s=s.replace('sound_source_contiguous_check_verification','source_error_callback_connected_verification')
a=s.index("scope='");z=s.index("'),indent=2)",a)
scope='2100 persistent native source callback-installation-to-contiguous-check sequences. Setter65e44e installs callback/userdata,then checker65e712 uses the same RAM/CPU/FPU state. Full retail RAM/CPU/FPU/SR/PR comparisons after setter and checker;native source locks and controlled callback identity getter65e44a, no modeled calls. Independent whole64-byte source object after installation and checking,exact output count/R0,output guards,interrupt mask and stack. Valid modes0/1 never call callback;three invalid modes call installed callback exactly once when non-null with configured source pointer R4 and error-3 R5. Seven request patterns,five position/capacity fixtures,three mirror values,both banks,callback absent/present. Callback is a controlled native test routine,not the retail default handler. Full source construction/error-handler integration and fullgame unverified.'
s=s[:a]+'scope='+repr(scope)+s[z+1:]
(p/'verify_source_error_callback_connected.py').write_text(s)
reg=(p/'register_sound_source_pool_connected.py').read_text().replace('sound_source_pool_connected','source_error_callback_connected')
reg=reg.replace("evidence['cases_passed']==36 and evidence['native_operations_compared']==9396", "evidence['cases_passed']==2100")
reg=reg.replace("names={'sa_sound_source_pool_initialize','sa_sound_source_create','sa_sound_source_reset','sa_sound_source_destroy'}", "names={'sa_sound_source_error_callback_set','sa_sound_source_identity_get','sa_sound_source_contiguous_check','sa_sound_source_lock_enter','sa_sound_source_lock_exit'}")
reg=reg.replace('Persistent native source-pool lifecycles:', 'Native source callback installation and error-path invocation:')
(p/'register_source_error_callback_connected.py').write_text(reg)
