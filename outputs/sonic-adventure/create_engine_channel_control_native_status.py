from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_engine_channel_control_pair.py').read_text()
s=s.replace('cases=0', "query=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_channel_status_get.dll')).sa_engine_channel_status_get;query.argtypes=abi\ncases=0")
s=s.replace('  before=bytes(ram)', "  struct.pack_into('<I',ram,((0x8ca10000+index*32+24)&0xffffff),status)\n  struct.pack_into('<I',ram,((0x8ca10000+index*32+28)&0xffffff),answer)\n  before=bytes(ram)")
s=s.replace("assert a==0x8c1101f0;actual.append((r[4],r[5]));struct.pack_into('<I',native,r[5]&0xffffff,status);r[0]=answer;return 0", "assert a==0x8c1101f0;actual.append((r[4],r[5]));return query(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)")
s=s.replace('reference(ram,s,{0x8c1101f0:hook},fpu=f)','reference(ram,s,{},fpu=f)')
s=s.replace('actual==expected==', 'actual==').replace('if status==1:', 'if status&1:')
s=s.replace('four status values','four hardware-table status words').replace('Only status1','Only bit0 set').replace('only status1','only bit0 set').replace('Status query1101f0 modeled;', 'Native status query1101f0 connected;independent table address32*index+18 and bit0 output. No modeled calls;')
s=s.replace('engine_channel_control_pair_verification.json','engine_channel_control_native_status_verification.json')
s=s.replace('two status service returns','two initial control-word values')
(p/'verify_engine_channel_control_native_status.py').write_text(s)
stem='engine_channel_status_get'
(p/f'build_{stem}.ps1').write_text((p/'build_engine_transfer_wait.ps1').read_text().replace('engine_transfer_wait',stem))
reg=(p/'register_engine_transfer_wait.py').read_text().replace('engine_transfer_wait',stem).replace(str(0x8c10eee0),str(0x8c1101f0)).replace(str(0x8c10ef12),str(0x8c110214)).replace('==32','==128').replace("report=f'analysis/{stem}_verification.json'", "report='analysis/engine_channel_control_native_status_verification.json'").replace('verify_{stem}.py','verify_engine_channel_control_native_status.py')
(p/'register_engine_channel_status_get.py').write_text(reg)
reg=(p/'register_engine_transfer_wait_native_ready.py').read_text().replace('engine_transfer_wait','engine_channel_control').replace('native_ready','native_status').replace("{'sa_engine_channel_control','sa_engine_transfer_ready_get'}", "{'sa_engine_channel_control_restore','sa_engine_channel_control_install','sa_engine_channel_status_get'}").replace('==32','==128').replace('native readiness:', 'native channel status:')
(p/'register_engine_channel_control_native_status.py').write_text(reg)
