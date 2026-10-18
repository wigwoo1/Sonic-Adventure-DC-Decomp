from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_transfer_controls.py').read_text()
s=s.replace('cases=0', "class Registers(dict):\n def __init__(self,values):super().__init__(values);self.ready=[]\n def __getitem__(self,a):\n  if a==0xa05f688c:\n   value=0x80000020 if len(self.ready)%2==0 else 0x80000000;self.ready.append(value);return value\n  return super().__getitem__(a)\ncases=0")
s=s.replace(' mmio[0xa05f688c]=0', ' mmio[0xa05f688c]=0;mmio=Registers(mmio)')
s=s.replace('if a==0xa05f688c:ready_reads.append((a,n));return mmio[a]', 'if a==0xa05f688c:\n   value=0x80000020 if len(ready_reads)%2==0 else 0x80000000;ready_reads.append((a,n,value));return value')
s=s.replace('assert ready_reads==[(0xa05f688c,4)]*10', 'assert ready_reads==[(0xa05f688c,4,v) for _ in range(10) for v in [0x80000020,0x80000000]]\n assert mmio.ready==[0x80000020,0x80000000]*10')
s=s.replace('ten readiness reads','twenty readiness reads').replace('controlled device data and immediate readiness MMIO. Delayed hardware and physical hardware/full game integration unverified.', 'controlled device data and alternating busy/ready MMIO with unrelated high bit set. Each of ten transfers requires two readiness polls;independent exact20-register-read sequence verified in native and reference execution. Nonterminating devices and physical hardware/full game integration unverified.')
s=s.replace('backend_readback_native_transfer_controls_verification.json','backend_readback_native_delayed_transfer_verification.json')
(p/'verify_backend_readback_native_delayed_transfer.py').write_text(s)
reg=(p/'register_backend_readback_native_transfer_controls.py').read_text().replace('native_transfer_controls','native_delayed_transfer').replace('native transfer controls:', 'native delayed transfer:')
(p/'register_backend_readback_native_delayed_transfer.py').write_text(reg)
