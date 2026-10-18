from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_work_destroy.py').read_text()
s=s.replace('cases=0',"helpers={0x8c10d31c:fill}\nfor address,stem in [(0x8c65ad68,'sound_critical_enter'),(0x8c65ad8e,'sound_critical_exit')]:\n helpers[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);helpers[address].argtypes=abi\ncases=0")
s=s.replace('for kind,pattern,bank,stack in itertools.product(', 'for kind,pattern,bank,stack,depth,mask in itertools.product(').replace('[0,0x8ca40000]):','[0,0x8ca40000],[0,1,2],[0,5,15]):')
s=s.replace(' ram[0xa20000:'," struct.pack_into('<2I',ram,0x67c1e0,depth,9)\n ram[0xa20000:",1)
s=s.replace('if address==0x8c10d31c:return fill(', 'if address in helpers:return helpers[address](')
s=s.replace('[0x8c667440,0x8c65ad68,0x8c65ad8e]},fpu=', '[0x8c667440]},fpu=')
s=s.replace('s.sr=0x70000063','s.sr=0x70000003|(mask<<4)')
s=s.replace(" assert actual==([] if kind==0 else ([0x8c667440] if kind==2 else [])+[0x8c65ad68,0x8c65ad8e])", " assert actual==([0x8c667440] if kind==2 else [])\n assert struct.unpack_from('<2I',native,0x67c1e0)==(depth,mask if kind and depth==0 else 9)\n assert (sr.value>>4)&15==mask")
s=s.replace('sound_work_destroy_verification','work_destroy_native_critical_verification').replace('84 full','756 full')
s=s.replace('Stream destruction and critical enter/exit65ad68/65ad8e modeled; exact ordered calls,stream argument and restored stack checked.', 'Critical enter/exit65ad68/65ad8e native across depths0/1/2 and masks0/5/15. Independent restored nesting/interrupt mask and saved mask on outer entry; stream destruction alone modeled,exact stream argument and restored stack checked.')
(p/'verify_work_destroy_native_critical.py').write_text(s)
s=(p/'register_work_native_configuration.py').read_text().replace('work_native_configuration','work_destroy_native_critical').replace('==54','==756')
start=s.index('names={');end=s.index('\n',start)
s=s[:start]+"names={'sa_sound_work_destroy','sa_sound_critical_enter','sa_sound_critical_exit'}"+s[end:]
s=s.replace('Native work constructor buffer/callback configuration:', 'Native nested critical sections in work destruction:')
(p/'register_work_destroy_native_critical.py').write_text(s)
