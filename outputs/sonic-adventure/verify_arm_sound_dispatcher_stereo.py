"""Stereo buffer selection, format/length limits and occupied-pool allocation."""
from pathlib import Path
P=Path(__file__).resolve().parent
streams_verifier=P/'verify_arm_sound_dispatcher_streams.py'
exec(compile(streams_verifier.read_text().split('for params in ')[0],str(streams_verifier),'exec'))
stereo_cases=[];pool_cases=[];fault_cases=[];budget_cases=[]
def stereo(driver,flags,slot,fmt,length,paired,volume,left_buffer,kind='fresh',budget=100000,fault=None):
    prepared,hw=bootstrap(driver);memory=bytearray(prepared);hardware=bytearray(hw)
    def literal(a,late=False):return struct.unpack_from('<I',memory,a+driver*(4 if late else 8))[0]
    def put(a,v,n=4):memory[a:a+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little')
    source=literal(0x4b2c);memory[source:source+512]=bytes(512)
    base=literal(0x77f8)+slot*literal(0x77fc);table=literal(0x77e4);buffers=literal(0x7800)
    right_buffer={0:15,7:0,15:7}[left_buffer]
    for i in range(16):put(buffers+i*8,0x1c000+i*0x1000);put(buffers+i*8+4,length)
    selector=(fmt+length+slot)%3;envelope=(0,128,255)[selector]
    for s in (base,base+0x58):
        memory[s:s+0x58]=bytes(0x58);put(s+0x18,0x80808080)
        for offset in (0x1a,0x1b,0x1e,0x1f):put(s+offset,envelope,1)
    if kind=='missing_left':put(buffers+left_buffer*8,0)
    if kind=='missing_right':put(buffers+right_buffer*8,0)
    memory[source]=0x21;memory[source+2]=slot|(fmt<<4)|(128 if paired else 0)
    memory[source+3]=volume;put(source+4,0);memory[source+6]=64;memory[source+7]=left_buffer;memory[source+8]=right_buffer
    native,nhardware,completed,result=execute(driver,flags,memory,hardware,('stereo',driver,slot,fmt,length,paired,volume,left_buffer,kind),budget,fault)
    if completed and not fault:
        def out(a):return struct.unpack_from('<I',native,a)[0]
        display=literal(0x5854)+slot*64
        if kind=='missing_left':
            assert native[base]&128 and not native[base]&1 and not native[base+0x58]&1
            assert out(table+4)==0 and native[display+0x1c]&1,'left buffer error and no ownership'
        elif kind=='missing_right':
            assert native[base]&1 and native[base+0x58]&128 and not native[base+0x58]&1
            assert out(table+4)==base and out(table+12+4)==0 and native[display+0x3c]&1,'right failure retains original partial left initialization'
        else:
            count=2 if paired else 1
            assert native[base+0x58]&1==(1 if paired else 0),'paired channel activation'
            assert out(base+0x38)==left_buffer|((right_buffer<<8)|0x10000 if paired else 0),'packed buffer selectors retained'
            for side in range(count):
                s=base+side*0x58;entry=table+side*12;index=left_buffer if side==0 else right_buffer
                address=0x1c000+index*0x1000;hw_address=struct.unpack_from('<I',memory,entry+8)[0];h=hw_address-MMIO
                limit=0x1f000 if fmt==0 else 0xf000 if fmt==1 else 0x7000
                capped=min(length,limit);loop=capped>>1 if fmt==0 else capped if fmt==1 else capped*2
                normalized=fmt if fmt<2 else 3;stored_fmt=fmt if side==0 else normalized
                expected_control=0x4200|(address>>16)|(normalized<<7)
                if not paired or side==1:expected_control|=0xc000
                assert out(entry+4)==s and native[entry]==8 and native[entry+1]==2+side,'independent ownership/class'
                assert native[s]&1 and native[s+2]==8 and native[s+3]==side and native[s+0x27]==stored_fmt
                assert out(s+0x30)==out(s+8)==address and out(s+0x34)==(length if driver else capped),(result,side,hex(out(s+0x30)),hex(out(s+8)),hex(out(s+0x34)),hex(address),hex(capped))
                assert struct.unpack_from('<I',nhardware,h+0xc)[0]==loop,'format-dependent loop count'
                assert struct.unpack_from('<I',nhardware,h)[0]==expected_control and struct.unpack_from('<I',nhardware,h+4)[0]==address&0xffff,'format/start/key-on fields'
                assert all(struct.unpack_from('<I',nhardware,h+at)[0]==0x1ff8 for at in (0x2c,0x30,0x34,0x38,0x3c)),'stream envelope constants'
                wanted_volume=max(0,min(255,256-envelope*2+volume))
                assert native[s+0x20]==nhardware[h+0x29]==wanted_volume,'independent envelope volume'
                raw_pan=31 if paired and side==0 else 15 if side else 0
                linear=memory[literal(0x824c,True)+raw_pan]+(envelope>>2)-32
                encoded=memory[literal(0x8250,True)+max(0,min(31,linear))]
                assert native[s+0x28]==raw_pan and native[s+0x21]==nhardware[h+0x24]==encoded,'independent pan envelope'
                level=max(0,min(15,(envelope-128)//8))
                assert native[s+0x2b]==nhardware[h+0x25]==level and nhardware[h+0x20]==level*16,'independent arithmetic-shift effect envelope'
    return result
def pool(driver,flags,slot,scenario,budget=100000,fault=None):
    prepared,hw=bootstrap(driver);memory=bytearray(prepared);hardware=bytearray(hw)
    def literal(a):return struct.unpack_from('<I',memory,a+driver*8)[0]
    def put(a,v,n=4):memory[a:a+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little')
    table=literal(0x77e4);target=0x11000+slot*0x58;source=literal(0x4b2c)
    memory[source:source+512]=bytes(512);memory[target:target+0x58]=bytes(0x58);put(target+0x18,0x80808080);put(target+0x1e,128,1);put(target+0x1f,128,1)
    owners=[];classes=[];words=[]
    for i in range(16):
        owner=0x11000+i*0x58 if i<6 else 0x112c0+(i-6)*0x58
        cls=1 if i<6 else 2+(i-6)%2;priority=8
        if scenario=='steal_mono' and i==1:priority=4
        if scenario=='steal_pair' and i in (6,7):priority=4
        if scenario=='zero_priority' and i==4:priority=0
        age=0
        if scenario=='word_tie' and i in (1,3):priority=4;age=9 if i==1 else 1
        packed=priority|(cls<<8)|(age<<16)
        owners.append(owner);classes.append(cls);words.append(packed)
        memory[owner:owner+0x58]=bytes(0x58);put(owner,1,1);put(owner+2,priority,1);put(owner+3,i,1)
        put(owner+0x18,0x80808080);put(owner+0x1e,128,1);put(owner+0x1f,128,1)
        put(table+i*12,packed);put(table+i*12+4,owner)
        struct.pack_into('<I',hardware,struct.unpack_from('<I',memory,table+i*12+8)[0]-MMIO,0xc200)
    holes={'free_first':(0,),'free_last':(15,),'free_two':(3,12)}.get(scenario,())
    for i in holes:put(table+i*12,0);put(table+i*12+4,0);put(owners[i],0,1)
    raw_priority={'lower':8,'equal':64,'force':0,'zero_priority':8}.get(scenario,128);priority=raw_priority>>3
    if holes:chosen=min(holes);accepted=True;removed=set()
    else:
        zero=[i for i,w in enumerate(words) if w&255==0]
        chosen=max(zero) if zero else min(range(16),key=lambda i:(words[i]&255,words[i],i))
        minimum=words[chosen]&255
        accepted=bool(zero) or priority==0 or priority>minimum or priority==minimum and priority+1>=words[chosen]
        removed={chosen,chosen+1} if classes[chosen]==2 else {chosen}
    bank=0x1c000;memory[bank:bank+0x800]=bytes(0x800);memory[bank:bank+4]=b'SOSB';put(bank+4,1);put(bank+8,0x800);put(bank+12,0);put(bank+16,0x40)
    memory[bank+0x7fc:bank+0x800]=b'ENDB';memory[bank+0x40:bank+0x44]=b'SOSD';put(bank+0x44,0x04000000)
    put(bank+0x44+0x12,0,1);put(bank+0x44+0x15,96,1);put(bank+0x44+0x24,9,1)
    put(literal(0x54e4),bank);put(literal(0x54e4)+4,0x800)
    memory[source:source+6]=bytes((0x11,0,slot,0,0,raw_priority))
    native,nhardware,completed,result=execute(driver,flags,memory,hardware,('pool',driver,slot,scenario,chosen,accepted),budget,fault)
    if completed and not fault:
        out=lambda a:struct.unpack_from('<I',native,a)[0]
        if not accepted:
            assert not native[target]&1 and native[target]&128 and native[0x13800+slot*32+0x1c]&128,'capacity/priority rejection'
            assert native[table:table+16*12]==memory[table:table+16*12] and nhardware==hardware,'rejection preserved pool and hardware'
            for owner in owners:assert native[owner:owner+0x58]==memory[owner:owner+0x58]
        else:
            assert native[target]&1 and native[target+3]==(chosen if holes or zero else 255) and out(table+chosen*12+4)==target,(result,native[target],native[target+3],hex(out(table+chosen*12+4)))
            assert native[table+chosen*12]==priority and native[table+chosen*12+1]==1
            for i,owner in enumerate(owners):
                if i in removed:
                    assert not native[owner]&1,'preempted owner deactivated'
                    if i!=chosen:assert out(table+i*12+4)==0,'paired owner released'
                else:assert native[owner:owner+0x58]==memory[owner:owner+0x58],('unselected owner changed',scenario,i)
                if i!=chosen and i not in removed:assert native[table+i*12:table+(i+1)*12]==memory[table+i*12:table+(i+1)*12]
    result.update({'chosen':chosen,'accepted':accepted,'removed':sorted(removed) if accepted else []})
    return result
lengths=(0,1,0x7000,0x7001,0xf000,0xf001,0x1f000,0x1f001)
for driver,flags,fmt,paired,index in itertools.product(range(2),(0,0xf0000000),range(4),(False,True),range(len(lengths))):
    stereo_cases.append(stereo(driver,flags,(0,3,7)[index%3],fmt,lengths[index],paired,(0,127,255)[(index+fmt)%3],(0,7,15)[(index+fmt)%3]))
for driver,flags,kind in itertools.product(range(2),(0,0xf0000000),('missing_left','missing_right')):stereo_cases.append(stereo(driver,flags,7,2,0x8000,True,127,7,kind))
scenarios=('free_first','free_last','free_two','steal_mono','steal_pair','lower','equal','higher','force','zero_priority','word_tie')
for driver,flags,slot,scenario in itertools.product(range(2),(0,0xf0000000),(6,7),scenarios):pool_cases.append(pool(driver,flags,slot,scenario))
for driver in range(2):
    probe=stereo(driver,0xf0000000,3,2,0x8000,True,127,7)
    for direction in ('read','write'):
        for ordinal in (1,probe['reads' if direction=='read' else 'writes']):fault_cases.append(stereo(driver,0xf0000000,3,2,0x8000,True,127,7,fault=(direction,ordinal)))
for driver,scenario,budget in itertools.product(range(2),('steal_mono','steal_pair','lower'),(1,16,64,256,512,1024)):budget_cases.append(pool(driver,0xf0000000,7,scenario,budget))
for driver,budget in itertools.product(range(2),(1,16,64,256,512,1024)):budget_cases.append(stereo(driver,0xf0000000,3,2,0x8000,True,127,7,budget=budget))
names=re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_dispatcher.ps1').read_text())
paths=[Path(__file__),streams_verifier,baseline,P/'verify_arm_sound_reset_services.py',dll,P/'arm_legacy_alignment_reference.py',P/'build_arm_sound_dispatcher.ps1']+[P/'source'/name for name in names]
paths += [P/'source/sa_arm_sound_boot_native.c',P/'build_arm_sound_boot_native.ps1'] if '--connected' in sys.argv else []
report={'stereo_cases_passed':len(stereo_cases),'pool_cases_passed':len(pool_cases),'mmio_failure_cases_passed':len(fault_cases),'budget_cases_passed':len(budget_cases),'stereo_cases':stereo_cases,'pool_cases':pool_cases,'faults':fault_cases,'budgets':budget_cases,
 'sha256':{str(path):sha(path) for path in paths},
 'connected':'--connected' in sys.argv,
 'scope':'Separate dispatcher overlay: direct mono/paired stereo buffer start with format/length/envelope boundaries, missing buffers, and full SOSB ownership-pool free/preemption/priority/full-word ties. Full CPU/RAM/modeled MMIO/ordered-effect equality versus original ARM, no callbacks to ARM. Independent setup/allocation/rejection assertions. No actual sample decode, timing/synthesis/audio output, every-command or main-driver integration proof.'}
(P/('analysis/arm_sound_dispatcher_stereo_connected_verification.json' if '--connected' in sys.argv else 'analysis/arm_sound_dispatcher_stereo_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(stereo_cases),'stereo/buffer cases,',len(pool_cases),'allocation cases,',len(fault_cases),'MMIO faults and',len(budget_cases),'budgets passed')
