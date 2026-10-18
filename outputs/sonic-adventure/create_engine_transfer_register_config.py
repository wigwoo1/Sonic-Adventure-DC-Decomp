from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'create_engine_transfer_ready_get.py').read_text().replace('engine_transfer_ready_get','engine_transfer_register_config').replace(str(0x8c10fb48),str(0x8c10ee64)).replace(str(0x8c10fb56),str(0x8c10ee8c)).replace('==48','==72').replace('transfer readiness getter','transfer hardware register configuration')
exec(compile(s,__file__,'exec'))
