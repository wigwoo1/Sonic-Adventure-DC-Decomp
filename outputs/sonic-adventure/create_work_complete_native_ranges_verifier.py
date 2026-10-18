from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_stream_work_complete.py').read_text()
s=s.replace('services=[0x8c6676f6',"split=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_split_range.dll')).sa_sound_driver_split_range;split.argtypes=fn.argtypes\ncopylib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_pool_queue_initialize.dll'))\nhelpers={0x8c668af4:split}\nfor address,name in [(0x8c10cd38,'sa_engine_record_copy_dispatch'),(0x8c10cd8e,'sa_engine_record_copy8_tail')]:\n    helpers[address]=getattr(copylib,name);helpers[address].argtypes=fn.argtypes\nservices=[0x8c6676f6")
s=s.replace('0x8c6677d8,0x8c668af4,0x8c6676ae','0x8c6677d8,0x8c6676ae')
s=s.replace('    before=bytes(ram[0xa60000:0xa60090])',"    for offset,base,length in [(0x14,0x8ca10000,64),(0x1c,0x8ca20000,100),(0x24,0x8ca30000,100)]:struct.pack_into('<2I',ram,0xa60000+offset,base,length)\n    before=bytes(ram[0xa60000:0xa60090])")
s=s.replace("        else:assert address in [0x8c010100,0x8c010200,0x8c667796]",'''        elif address in [0x8c010100,0x8c010200]:
            encoded=r[4]==0x8ca80000
            if encoded:base=0x8ca10000;length=64;first=36;head=r[5]==0
            else:
                index=(r[4]-0x8ca80100)//256;assert index in [0,1]
                base=0x8ca20000+index*0x10000;length=100
                signed=ctypes.c_int32(chosen*2).value;first=min(length,signed)&0xffffffff;head=r[5]==1
            expected_descriptor=(base,first) if head else ((base+first)&0xffffffff,(length-first)&0xffffffff)
            offset=r[6]&0xffffff
            descriptor=tuple(sum(int(mem[offset+i*4+j])<<(j*8) for j in range(4)) for i in range(2))
            assert descriptor==expected_descriptor,(descriptor,expected_descriptor)
        else:assert address==0x8c667796''')
s=s.replace('def call(ctx,address,mem,fp,srp,r,prp):service(address,mem,r,actual);return 0',"def call(ctx,address,mem,fp,srp,r,prp):\n        if address in helpers:return helpers[address](mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        service(address,mem,r,actual);return 0")
s=s.replace('0x8c6677d8,0x8c668af4,0x8c010200','0x8c6677d8,0x8c010200').replace('0x8c6676ae,0x8c668af4,0x8c010200','0x8c6676ae,0x8c010200')
s=s.replace('stream_work_complete_verification','work_complete_native_ranges_verification').replace('Getters,range splitter,virtual commits,ack and callback modeled.','Range splitter668af4 and8-byte copy dispatcher/tail native. Independent head/remainder descriptors checked at every virtual commit: encoded bytes36 of64 and per-channel signed limited sample bytes of100; source descriptors preserved. Getters,virtual commit effects,ack and callback modeled.').replace('Native dependency integration,','Remaining native dependency integration,').replace('stream work completion comparisons','native range work completion comparisons')
(p/'verify_work_complete_native_ranges.py').write_text(s)
r=(p/'register_stream_native_allocation.py').read_text().replace('stream_native_allocation','work_complete_native_ranges').replace('== 162','== 432')
a=r.index('names = ');z=r.index('\npath = ',a)
r=r[:a]+"names = {'sa_sound_stream_work_complete','sa_sound_driver_split_range'}"+r[z:]
r=r.replace('Native stream and decode-handle allocation:','Native ranges in stream work completion:')
(p/'register_work_complete_native_ranges.py').write_text(r)
