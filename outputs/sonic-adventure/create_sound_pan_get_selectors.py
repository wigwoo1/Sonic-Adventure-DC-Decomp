from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_driver_pan_get.py').read_text().replace('for value,bank,answer in itertools.product(range(256),[0,1],[0,0xffffffff]):','for value,bank,answer,selector in itertools.product([0,1,8,9,126,127,128,247,255],[0,1],[0,0xffffffff],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff]):')
s=s.replace('s.r[5]=0;','s.r[5]=selector;').replace('[(0x8ca09000,0,0x8cefffe0)]','[(0x8ca09000,selector,0x8cefffe0)]')
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='252 full retail RAM/CPU/FPU/SR/PR comparisons of pan getter668112 with nine signed-byte rounding/boundary fixtures,seven full-word selectors,both banks,two backend statuses. Native signed division;independent scaling/output guard and full-word selector forwarding without upper validation. Backend669278 modeled;native backend/hardware/game integration unverified. Supplements exhaustive256-byte selector0 report.'"+s[z:]
s=s.replace('sound_driver_pan_get_verification.json','sound_pan_get_selectors_verification.json')
(p/'verify_sound_pan_get_selectors.py').write_text(s)
reg=(p/'register_backend_readback_native_descriptor.py').read_text().replace("['backend_readback']","['sound_pan_get']").replace('backend_readback_native_descriptor_verification.json','sound_pan_get_selectors_verification.json').replace('==28','==252').replace("{'sa_sound_backend_readback','sa_sound_descriptor_byte_query'}","{'sa_sound_driver_pan_get','sa_signed_division'}").replace('native_descriptor','selectors')
(p/'register_sound_pan_get_selectors.py').write_text(reg)
