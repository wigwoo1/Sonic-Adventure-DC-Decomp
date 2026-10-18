import subprocess
from pathlib import Path
p=Path(__file__).resolve().parent
for stem in ['sound_critical_enter','sound_critical_exit']:
 text=(p/'build_sound_work_reset.ps1').read_text().replace('sound_work_reset',stem)
 (p/f'build_{stem}.ps1').write_text(text)
 subprocess.run(['powershell','-NoProfile','-File',str(p/f'build_{stem}.ps1')],check=True)
