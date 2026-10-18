from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_initialized_two_chunks.py').read_text()
at=s.index('assets=json.loads(')
load="for address,stem in [(0x8c65bc12,'sound_work_completion_callback_set'),(0x8c65bc1c,'sound_work_remaining_limit_set'),(0x8c65b760,'sound_work_state_get')]:\n lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);lower[address].argtypes=abi\n"
s=s[:at]+load+s[at:]
s=s.replace(' def call_decode(', ' notifications=[]\n def call_decode(',1)
s=s.replace(' def call(ctx,a,m,f,sr,r,pr):', " def call(ctx,a,m,f,sr,r,pr):\n  if a==0x8c65b760:\n   assert r[4]==0x8ca60000\n   notifications.append((r[4],sum(int(m[0xa60040+i])<<(i*8) for i in range(4))))")
at=s.index('  for entry,target_fn,arg1,arg2 in')
addition='''  binding_before=bytearray(native[0xa60000:0xa60100])
  for address,arg1,arg2 in [(0x8c65bc1c,512,0),(0x8c65bc12,0x8c65b760,0x8ca60000)]:
   s.r[4:7]=[0x8ca60000,arg1,arg2]
   for index in range(4,7):r[index]=s.r[index]
   s.pc=address;reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
   assert lower[address](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
  struct.pack_into('<I',binding_before,0x3c,512);struct.pack_into('<2I',binding_before,0x48,0x8c65b760,0x8ca60000)
  assert native[0xa60000:0xa60100]==binding_before
'''
s=s[:at]+addition+s[at:]
s=s.replace("  assert struct.unpack_from('<I',native,0xa60034)[0]==256", "  assert struct.unpack_from('<I',native,0xa60034)[0]==256\n  assert notifications==[]",1)
s=s.replace('  feed_updates+=1',"  assert notifications==[(0x8ca60000,512)]\n  feed_updates+=1")
s=s.replace('initialized_two_chunks_verification','initialized_notification_verification')
s=s.replace('work limit512 and ring2048 controlled.', 'work limit512,configured output capacity512 and ring2048 controlled.')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'Native remaining-limit setter65bc1c and completion-callback setter65bc12 configure512 samples and a controlled native state-getter65b760 callback with work pointer userdata. Independent whole work binding,no notification after first256 samples,exactly one callback at counter512 after second chunk;callback executes native source and full retail state comparisons remain. Callback choice is a test fixture,original gameplay-installed callbacks unverified. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
(p/'verify_initialized_notification.py').write_text(s)
s=(p/'register_initialized_two_chunks.py').read_text()
s=s.replace("exec(compile(base, str(__file__), 'exec'))", "base=base.replace('initialized_two_chunks','initialized_notification').replace('Persistent original initialization and two decoded chunks:', 'Native limit/callback configuration through initialized decoding:')\nbase=base.replace(\"path = root / 'analysis/recovered_routines.json'\",\"names.update({'sa_sound_work_completion_callback_set','sa_sound_work_remaining_limit_set','sa_sound_work_state_get'})\\npath = root / 'analysis/recovered_routines.json'\")\nexec(compile(base, str(__file__), 'exec'))")
(p/'register_initialized_notification.py').write_text(s)
