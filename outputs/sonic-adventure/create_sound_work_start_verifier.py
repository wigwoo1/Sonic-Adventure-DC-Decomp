from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_work_reset.py').read_text().replace('sound_work_reset','sound_work_start').replace('0x8c65b606','0x8c65b778')
s=s.replace('cases=0', "reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_reset.dll')).sa_sound_work_reset\nreset.argtypes=fn.argtypes\ncases=0")
s=s.replace("    def call(*args):raise AssertionError('Unexpected service')", "    def call(ctx,address,m,f,srp,r,prp):\n        assert address==0x8c65b606\n        return reset(m,f,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)")
s=s.replace('    wanted[3]=0','    wanted[1]=1;wanted[3]=0')
s=s.replace('work reset65b606..65b624', 'work start65b778..65b78e with native reset65b606')
s=s.replace('all other bytes preserved,no services.', 'state byte1 set to1; all other bytes preserved. Reset callback uses recovered native source; no modeled services.')
(p/'verify_sound_work_start.py').write_text(s)
build=(p/'build_sound_work_reset.ps1').read_text().replace('sound_work_reset','sound_work_start').replace('Sound work reset','Sound work start')
(p/'build_sound_work_start.ps1').write_text(build)
