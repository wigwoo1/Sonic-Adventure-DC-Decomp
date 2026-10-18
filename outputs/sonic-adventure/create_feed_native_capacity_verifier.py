from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_stream_work_feed.py').read_text()
s=s.replace('cases=0',"capacity_fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_capacity.dll')).sa_sound_source_capacity;capacity_fn.argtypes=fn.argtypes\ncases=0")
s=s.replace('(0x24,0x8c010200)','(0x24,0x8c65e462)')
s=s.replace("        ram[0xa10000:0xa10002]", "        struct.pack_into('<I',ram,0xa8000c,source_capacity)\n        struct.pack_into('<I',ram,0xa80110,capacity)\n        object_before=[bytes(ram[a:a+48]) for a in [0xa80000,0xa80100]]\n        ram[0xa10000:0xa10002]")
s=s.replace('        def call(ctx,address,mem,f,sr,r,pr):service(address,mem,r,actual);return 0',"        def call(ctx,address,mem,f,sr,r,pr):\n            if address==0x8c65e462:return capacity_fn(mem,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n            service(address,mem,r,actual);return 0")
s=s.replace('[0x8c010100,0x8c010200,0x8c010300','[0x8c010100,0x8c010300')
s=s.replace('addresses=([0x8c010200] if stop else [])','addresses=[]').replace('[0x8c65bc6c,0x8c010200]','[0x8c65bc6c]')
s=s.replace('        cases+=1',"        for index,a in enumerate([0xa80000,0xa80100]):assert native[a:a+48]==object_before[index]\n        cases+=1")
s=s.replace('All subordinate methods modeled; native feeding integration/full playback/fullgame unverified.','Source capacity method executes natively in both source and sample modes; entire source objects independently unchanged. Other subordinate methods modeled; full native feeding/full playback/fullgame unverified.')
s=s.replace('sound_stream_work_feed_verification','feed_native_capacity_verification').replace('stream work tick comparisons passed','native capacity feeder comparisons passed')
(p/'verify_feed_native_capacity.py').write_text(s)
r=(p/'register_work_complete_native_getters.py').read_text().replace('work_complete_native_getters','feed_native_capacity').replace('== 216','== 256')
a=r.index('names = ');z=r.index('\npath = ',a)
r=r[:a]+"names = {'sa_sound_stream_work_feed','sa_sound_source_capacity'}"+r[z:]
r=r.replace('Native completion getters in stream work completion:','Native capacity in source feeding:')
(p/'register_feed_native_capacity.py').write_text(r)
