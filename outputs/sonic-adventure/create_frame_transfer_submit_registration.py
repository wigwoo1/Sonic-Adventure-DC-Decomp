from pathlib import Path
p=Path(__file__).resolve().parent
line=next(x for x in (p/'build_and_verify.ps1').read_text().splitlines() if 'engine_frame_transfer_submit.dll' in x)
header=(p/'build_engine_transfer_wait.ps1').read_text().split('& $clang')[0]
(p/'build_engine_frame_transfer_submit.ps1').write_text(header+line+"\nif ($LASTEXITCODE -ne 0) { throw 'Frame transfer submission build failed' }\n")
reg=(p/'register_engine_transfer_wait.py').read_text().replace('engine_transfer_wait','engine_frame_transfer_submit').replace(str(0x8c10eee0),str(0x8c669088)).replace(str(0x8c10ef12),str(0x8c6690f4)).replace('==32','==72')
(p/'register_engine_frame_transfer_submit.py').write_text(reg)
