from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_audio_filter_native.py').read_text()
text=text.replace('cases=0', "lower[0x8c65adc0]=fn\nfn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_audio_configuration_forward.dll')).sa_file_audio_configuration_forward\nfn.argtypes=wrapper.argtypes\ncases=0")
text=text.replace('ram[0xa20000:0xa20010]=bytes([0x55])*16','ram[0xa20000:0xa20040]=bytes([0x55])*64')
text=text.replace('s.r[4:8]=[extension,rate,0x8ca20004,0x8ca20008]','s.r[4:7]=[0x8ca20000,rate,extension]')
text=text.replace('s.pc=0x8c65adc0','s.pc=0x8c644be6')
text=text.replace("target=bytearray([0x55])*16;struct.pack_into('<H',target,4,a);struct.pack_into('<H',target,8,b)", "target=bytearray([0x55])*64;struct.pack_into('<H',target,0x30,a);struct.pack_into('<H',target,0x32,b)")
text=text.replace('native[0xa20000:0xa20010]==target','native[0xa20000:0xa20040]==target')
text=text.replace('audio_filter_native_verification.json','file_audio_configuration_native_verification.json')
text=text.replace('native audio coefficient routine65adc0,','native file configuration wrapper644be6,audio coefficient routine65adc0,')
text=text.replace('guarded16-bit outputs,','independently guarded64-byte file handle and16-bit outputs at+48/+50,')
text=text.replace('fully native audio filter comparisons passed','fully native file audio configuration comparisons passed')
(root/'verify_file_audio_configuration_native.py').write_text(text)
