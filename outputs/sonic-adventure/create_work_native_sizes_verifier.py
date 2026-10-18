from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_work_native_configuration.py').read_text()
s=s.replace("pool=0x8a3910;cases=0", "for address,stem,name in [(0x8c65e79c,'sound_source_ring_size_get','sa_sound_source_ring_size_get'),(0x8c65e7a0,'sound_source_mirror_size_get','sa_sound_source_mirror_size_get')]:\n helpers[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);helpers[address].argtypes=fn.argtypes\npool=0x8a3910;cases=0")
s=s.replace(" struct.pack_into('<I',ram,0xa3001c,0x8ca40000)"," struct.pack_into('<3I',ram,0xa3001c,0x8ca40000,1025,257)\n source_before=bytes(ram[0xa30000:0xa30030])")
s=s.replace('assert len(actual)==(3 if slot<8 else 0)','assert len(actual)==(1 if slot<8 else 0)\n assert native[0xa30000:0xa30030]==source_before')
s=s.replace('work_native_configuration_verification','work_native_sizes_verification')
s=s.replace('Size/mirror getters and stream creation modeled;', 'Ring size65e79c and mirror size65e7a0 getters native,independent full48-byte source object unchanged. Only stream creation modeled;')
(p/'verify_work_native_sizes.py').write_text(s)
s=(p/'register_work_native_configuration.py').read_text().replace('work_native_configuration','work_native_sizes')
s=s.replace("'sa_sound_stream_input_callback_set'}", "'sa_sound_stream_input_callback_set','sa_sound_source_ring_size_get','sa_sound_source_mirror_size_get'}")
s=s.replace('Native work constructor buffer/callback configuration:', 'Native work constructor size accessors and callback configuration:')
(p/'register_work_native_sizes.py').write_text(s)
