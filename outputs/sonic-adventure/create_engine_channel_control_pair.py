from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('engine_channel_control_restore',0x8c10f9bc,0x8c10f9ec),('engine_channel_control_install',0x8c10fa60,0x8c10fa96)]
for stem,start,end in specs:
 (p/f'build_{stem}.ps1').write_text((p/'build_engine_transfer_wait.ps1').read_text().replace('engine_transfer_wait',stem))
reg=(p/'register_engine_transfer_control_pair.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:]
reg=reg.replace('engine_transfer_control_pair','engine_channel_control_pair')
(p/'register_engine_channel_control_pair.py').write_text(reg)
