from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_backend_readback_errors.py').read_text().replace('sound_backend_readback','sound_backend_pan_get').replace('0x8c66adc4','0x8c66bb4c').replace('0x8c66b8c4','0x8c66bb4c')
s=s.replace('for (enabled,present),value,bank in itertools.product([(0,0),(0,1),(1,0)]','for (enabled,present,output_present),value,bank in itertools.product([(0,0,1),(0,1,1),(1,0,1),(1,1,0)]')
s=s.replace('s.r[6]=0x8ca20004;', 's.r[6]=0x8ca20004 if output_present else 0;')
s=s.replace('error=0x07000103 if enabled else 0x0d000001','error=(0x07000103 if not present else 0x07000203) if enabled else 0x0d000001')
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='56 complete retail RAM/CPU/FPU/SR/PR comparisons of backend pan reader66bb4c error paths. Disabled context with null/present handle,enabled context with null handle,valid handle with null output;seven selectors,both banks. Native diagnostic recorder,independent exact error/label globals,40-byte output and144-byte object preserved,restored stack. No modeled services. Successful mono/stereo path unverified;not full routine recovery.'"+s[z:]
(p/'verify_sound_backend_pan_get_errors.py').write_text(s)
