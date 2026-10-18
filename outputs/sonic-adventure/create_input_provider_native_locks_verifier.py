from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_input_provider_native_remainder.py').read_text()
s=s.replace('services=[0x8c65e368,0x8c65e380];cases=0',"lock_enter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_lock_enter.dll')).sa_sound_source_lock_enter;lock_enter.argtypes=fn.argtypes\nlock_exit=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_lock_exit.dll')).sa_sound_source_lock_exit;lock_exit.argtypes=fn.argtypes\nservices=[];cases=0")
s=s.replace('0x8c10cbb8:remainder}', '0x8c10cbb8:remainder,0x8c65e368:lock_enter,0x8c65e380:lock_exit}')
s=s.replace('assert [a for a,args in actual]==[a for index in range(channels) for a in [0x8c65e368,0x8c65e380]]','assert actual==[]')
s=s.replace('input_provider_native_remainder_verification','input_provider_native_locks_verification').replace('only lock enter/exit modeled.','native interrupt mask save/restore helpers65e368/65e380; no modeled dependencies.').replace('Native lock integration,','Host concurrency/interrupt mapping,').replace('native-remainder comparisons','native-lock comparisons')
(p/'verify_input_provider_native_locks.py').write_text(s)
r=(p/'register_input_provider_native_remainder.py').read_text().replace('input_provider_native_remainder','input_provider_native_locks').replace("'sa_sound_source_query'}","'sa_sound_source_query','sa_sound_source_lock_enter','sa_sound_source_lock_exit'}").replace('native remainder integration:','native interrupt mask integration:')
(p/'register_input_provider_native_locks.py').write_text(r)
