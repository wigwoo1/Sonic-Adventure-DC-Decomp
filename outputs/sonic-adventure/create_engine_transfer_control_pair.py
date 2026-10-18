from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('engine_transfer_control_restore',0x8c10ee20,0x8c10ee54),('engine_transfer_control_install',0x8c10ee9c,0x8c10eed0)]
for stem,start,end in specs:
 (p/f'build_{stem}.ps1').write_text((p/'build_engine_transfer_wait.ps1').read_text().replace('engine_transfer_wait',stem))
reg=(p/'register_engine_transfer_wait.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:]
reg=reg.replace("report=f'analysis/{stem}_verification.json'", "report='analysis/engine_transfer_control_pair_verification.json'").replace('==32','==128')
reg=reg.replace('build_{stem}.ps1/verify_{stem}.py.', 'build_{stem}.ps1/verify_engine_transfer_control_pair.py.')
a=reg.index(" if f'verify_{stem}.py'");z=reg.index('\npath.write_text',a)
reg=reg[:a]+''' if f'build_{stem}.ps1' not in text:build.write_text(text+f"\\n& (Join-Path $PSScriptRoot 'build_{stem}.ps1')\\n")
text=build.read_text()
if 'verify_engine_transfer_control_pair.py' not in text:build.write_text(text+"\\n& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_control_pair.py')\\nif ($LASTEXITCODE -ne 0) { throw 'Transfer control verification failed' }\\n")
'''+reg[z:]
(p/'register_engine_transfer_control_pair.py').write_text(reg)
