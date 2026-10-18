from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_initialize_original_headers.py').read_text()
s=s.replace("[(0x8c65b7a8,'sound_stream_work_initialize'","[(0x8c65bb88,'sound_stream_work_update','sa_sound_stream_work_update'),(0x8c65b7a8,'sound_stream_work_initialize'")
s=s.replace('s.pc=0x8c65b7a8','s.pc=0x8c65bb88').replace('assert lower[0x8c65b7a8](','assert lower[0x8c65bb88](')
s=s.replace('Native work initializer,source','Public work update65bb88 state1 tail-dispatches native work initializer65b7a8; source')
s=s.replace('initialize_original_headers_verification','public_initialize_headers_verification').replace('original header work initialization comparisons passed','public original header initialization comparisons passed')
(p/'verify_public_initialize_headers.py').write_text(s)
r=(p/'register_initialize_original_headers.py').read_text().replace('initialize_original_headers','public_initialize_headers').replace("'sa_sound_driver_split_range'}","'sa_sound_driver_split_range','sa_sound_stream_work_update'}").replace('Original headers in native work initialization:','Original headers through public work initialization:')
(p/'register_public_initialize_headers.py').write_text(r)
