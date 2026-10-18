from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'create_sound_work_controls.py').read_text()
start=s.index('specs=[');end=s.index('\n',start)
s=s[:start]+"specs=[('sound_work_consumed_samples_set',0x8c65bc0a,0x8c65bc0e,0x34),('sound_work_completed_samples_set',0x8c65bc24,0x8c65bc2a,0x40),('sound_work_completed_bytes_set',0x8c65bc30,0x8c65bc36,0x44)]"+s[end:]
s=s.replace("else 'remaining limit3c=R5;'", "else f'accounting field{offset:x}=R5;'")
s=s.replace('register_sound_work_controls.py','register_sound_work_accounting.py')
exec(compile(s,str(__file__),'exec'))
