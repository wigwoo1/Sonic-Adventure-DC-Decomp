from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_subordinate_apply.py').read_text();i=s.index('cases=0')
s=s[:i]+"helpers={}\nfor address,name in [(0x8c66869a,'sa_sound_driver_zero_query_a'),(0x8c6686a4,'sa_sound_driver_zero_query_b')]:\n helper=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_zero_queries.dll')),name);helper.argtypes=abi;helpers[address]=helper\n"+s[i:]
s=s.replace(' vals=[(value+i)&0xffffffff for i in range(8)]',' vals=[(value+i)&0xffffffff for i in range(4)]+[0]*4')
s=s.replace('assert a in targets;actual.append((a,r[4],r[5],r[6]));service(a,m,r);return 0','assert a in targets;actual.append((a,r[4],r[5],r[6]))\n  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  service(a,m,r);return 0')
s=s.replace('for a in targets},fpu=', 'for a in targets if a not in helpers},fpu=')
s=s.replace(' and actual==expected',' and [x for x in actual if x[0] not in helpers]==expected')
s=s.replace('driver reads and seven setters modeled','two native zero-pair driver reads66869a/6686a4 connected;independent zero values forwarded to effect and auxiliary setters. Other driver reads and seven setters modeled')
s=s.replace('sound_subordinate_apply_verification.json','sound_apply_native_zero_queries_verification.json')
(p/'verify_sound_apply_native_zero_queries.py').write_text(s)
reg=(p/'register_sound_aux_connected_readback.py').read_text().replace("['sound_aux']","['sound_apply']").replace('sound_aux_connected_readback_verification.json','sound_apply_native_zero_queries_verification.json').replace('==392','==168').replace("{'sa_sound_subordinate_aux_set','sa_sound_subordinate_aux_get','sa_sound_error_report'}","{'sa_sound_subordinate_apply','sa_sound_driver_zero_query_a','sa_sound_driver_zero_query_b'}").replace('connected_readback','native_zero_queries')
(p/'register_sound_apply_native_zero_queries.py').write_text(reg)
