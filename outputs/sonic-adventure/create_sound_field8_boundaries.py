from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_subordinate_field8_set_native_error.py').read_text()
s=s.replace('present,value,count,bank in itertools.product','present,value,count,bank,old in itertools.product').replace('[1,2,4,8,16,24,32,0xffffffff]','[0,1,2,4,8,16,24,32,0xffffffff]').replace('[0,1]):','[0,1],[0,1,7,8,16,32,0x80000000,0xffffffff]):')
s=s.replace("ram,0xa08008,16)","ram,0xa08008,old)")
s=s.replace(' before=bytes('," struct.pack_into('<I',ram,0x7ac108,0x12345678)\n before=bytes(",1)
s=s.replace('(count*16)','(count*old)')
s=s.replace("result=(abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1)","result=(abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1) if d else 0")
s=s.replace(' cases+=1'," assert struct.unpack_from('<I',native,0x7ac108)[0]==(0x44e if present and value==0 else 0x12345678)\n assert struct.unpack_from('<2I',native,0x67c37c)==(0x8c65e16e,0x8ca08000)\n cases+=1")
a=s.index("scope='");z=s.index('\n(p/',a)
scope='2016 full retail RAM/CPU/FPU/SR/PR/MACL comparisons of sound-object field8 rescaling setter65dc1c..65dc54. Null/present objects,eight old field8 patterns including0/negative/extreme,nine new field8 patterns including zero,seven count patterns,both banks. Independent complete128-byte object window: low32-bit product=count*oldfield8,signed truncation /8,native signed division by newfield8,low32-bit shift3 storedfield30,newfield8 stored. Zero divisor yields storedcount0 and errorglobal7ac108=44e;other paths preserve initialized errorglobal12345678. Native division,error dispatcher and controlled native identity callback;exact null-path message/userdata and preserved callback globals. No modeled calls. Application callback semantics,caller field meanings,audio hardware/fullgame unverified.'
s=s[:a]+'scope='+repr(scope)+s[z:];s=s.replace("f'analysis/{stem}_native_error_verification.json'","f'analysis/{stem}_boundaries_verification.json'")
(p/'verify_sound_subordinate_field8_boundaries.py').write_text(s)
reg=(p/'register_sound_setters_native_error.py').read_text().replace("['sound_subordinate_field8_set','sound_subordinate_fieldc_set']","['sound_subordinate_field8_set']").replace('_native_error','_boundaries').replace('==224','==2016').replace(' native error:',' expanded boundaries:').replace("'sa_sound_error_report'","'sa_signed_division','sa_sound_error_report'")
reg=reg.replace("'sa_signed_division',",'')
reg=reg.replace("f'verify_{stem}_boundaries.py'","'verify_sound_subordinate_field8_boundaries.py'").replace("verify_'+stem+'_boundaries.py","verify_sound_subordinate_field8_boundaries.py")
reg=reg.replace("'verify_{stem}_boundaries.py'","'verify_sound_subordinate_field8_boundaries.py'")
(p/'register_sound_subordinate_field8_boundaries.py').write_text(reg)
