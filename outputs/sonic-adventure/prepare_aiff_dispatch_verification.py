from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_aiff_transfer8_step.ps1').read_text()
b=b.replace("(Join-Path $PSScriptRoot 'source/sa_file_pending_state_get.c')", "(Join-Path $PSScriptRoot 'source/sa_file_pending_state_get.c') (Join-Path $PSScriptRoot 'source/sa_sound_aiff_transfer_step.c') (Join-Path $PSScriptRoot 'source/sa_sound_aiff_transfer_dispatch.c')")
b=b.replace('build/sound_aiff_transfer8_step.dll','build/sound_aiff_transfer_dispatch.dll')
(p/'build_sound_aiff_transfer_dispatch.ps1').write_text(b)
s=(p/'verify_sound_aiff_transfer_step_native_pending.py').read_text()
s=s.replace('sound_aiff_transfer_step.dll','sound_aiff_transfer_dispatch.dll').replace(')).sa_sound_aiff_transfer_step;',')).sa_sound_aiff_transfer_dispatch;')
s=s.replace('cases=0', "lower={}\nfor address,name in [(0x8c669af2,'sa_sound_aiff_transfer8_step'),(0x8c6699d4,'sa_sound_aiff_transfer_step')]:\n lower[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_aiff_transfer_dispatch.dll')),name);lower[address].argtypes=abi\ncases=0")
s=s.replace('for phase,channels,frames,ready,bank in itertools.product(', 'for format_flag,phase,channels,frames,ready,bank in itertools.product([0,1,2,0xffff,0x8001],')
s=s.replace('ram[state+14]=channels','ram[state+14]=channels;struct.pack_into(\'<H\',ram,state+0xaa,format_flag)')
s=s.replace("data=struct.pack('>6H',*samples);", "if format_flag==1:samples=[0,127,128,255,1,254]\n data=bytes(samples) if format_flag==1 else struct.pack('>6H',*samples);")
s=s.replace('  if a==0x8c644c16:', '  if a in lower:return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  if a==0x8c644c16:')
s=s.replace('s.pc=0x8c6699d4','s.pc=0x8c669bfc')
s=s.replace('samples[channels*i])','(samples[channels*i]<<8) if format_flag==1 else samples[channels*i])').replace('samples[2*i+1])','(samples[2*i+1]<<8) if format_flag==1 else samples[2*i+1])')
s=s[:s.index("(out/'analysis/")]+"(out/'analysis/sound_aiff_transfer_dispatch_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native eight-instruction dispatcher669bfc selects native8-bit transfer669af2 only for signed short state+aa equal1; all other tested flags select native16-bit transfer6699d4.480 full retail RAM/CPU/FPU/SR/PR/MACL and callback trace comparisons: flags0/1/2/ffff/8001,phases0/1/2/3,mono/stereo,frames0/1/3,pending0/1,FPU banks. Native pending getter; independent exact sample conversion,buffer guards,source preservation,phase/submission count and lengths. Query/submission callbacks modeled; physical audio/fullgame unverified.'),indent=2)+'\\n')\nprint(cases,'AIFF dispatch comparisons passed')\n"
(p/'verify_sound_aiff_transfer_dispatch.py').write_text(s)
