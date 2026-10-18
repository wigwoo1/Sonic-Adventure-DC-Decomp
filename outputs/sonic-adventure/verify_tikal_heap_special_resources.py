"""Original Tikal cleanup and all three immediate services execute; nested services hooked."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
Invoke=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*5),ctypes.POINTER(ctypes.c_uint32))
def main():
    main_binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(main_binary)]=main_binary;old[0xb00000:0xb00000+len(module)]=module
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();events=[];original=[];errors=[]
    def effect(memory,service,args,status):
        if mutation=='task' and service==0x8c098b42:
            struct.pack_into('<I',memory,0xa01020,0x8ca03000);memory[0xa02008]=owner^255
        if mutation=='fields':
            if service==0x8c079476:struct.pack_into('<I',memory,0xa05144,0)
            if service==0x8c098b42 and args[0]==0x8ca07100:struct.pack_into('<I',memory,0xa06000,0x8ca07020)
        if mutation=='counter' and service==0x8c64ddc0:memory[0x1c24b8]=255
        if mutation=='descriptor' and service==0x8c64ddc0:
            struct.pack_into('<I',memory,0xa08004,0)
        result=(service^args[0])&0xffffffff
        if service==0x8c64ddc0 and flags&16:result=1
        return status^(0x01000031 if mutation!='none' else 0),result
    @Invoke
    def invoke(ctx,service,argc,a,b,c,status):
        if ctx!=0x1234:errors.append('context')
        args=(a,b,c) if argc==3 else (a,)
        if argc not in [1,3]:errors.append('argc')
        events.append((service,args,status[0]));status[0],result=effect(native,service,args,status[0]);return result
    def hook(service):
        def callback(memory,regs,status):
            args=tuple(regs[4:7]) if service==0x8c07a55c else (regs[4],)
            original.append((service,args,status));status,regs[0]=effect(memory,service,args,status);return status
        return callback
    hooks={target:hook(target) for target in [0x8c079476,0x8c07a55c]}
    dll_path=ROOT/'work/sonic-adventure/build/tikal_heap_resource_runtime.dll';fn=ctypes.CDLL(str(dll_path)).sa_tikal_cleanup_with_heap_resources
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32,ctypes.c_void_p,Invoke];fn.restype=ctypes.c_uint32
    cases=0
    for owner,flags,count,mutation,initial_sr,free_order in itertools.product([0,7,255],[0,15,16,31],[0,1,2,128],['none','fields'],[0,0x700083f3],[(),(3,4),(4,3),(5,3,4)]):
        for memory in (old,native):
            memory[0xa01000:0xa08000]=bytes(0x7000);memory[0x78c600:0x78cc00]=bytes(0x600)
            struct.pack_into('<2I',memory,0xa01020,0x8ca02000,0x12345678);memory[0xa02008]=owner
            for base,value in [(0x78c608,0x8ca09000),(0x78c628,0x8ca06000),(0x78c648,0x87654321),(0x78c668,0x8ca05000)]:struct.pack_into('<I',memory,base+owner*4,value)
            for offset,bit,value in [(0xa05064,1,0x8c880020),(0xa05148,2,0x1234),(0xa05144,4,0x1234),(0xa06000,8,0x8c8800a0)]:struct.pack_into('<I',memory,offset,value if flags&bit else 0)
            struct.pack_into('<I',memory,0xa05078,0x8c880060 if flags&16 else 0)
            for i in range(38):struct.pack_into('<3I',memory,0x79e410+i*52,0x56780000+i,0x8ca02000 if i%3==0 else 0x8ca03000,0x87654321)
            struct.pack_into('<I',memory,0x1c2594,0x8ca04000);memory[0x1c24b8]=count
            struct.pack_into('<6I',memory,0xa04000,0x1234,0x8ca08000,0x1234,0x8ca08010,0,0)
        for memory in (old,native):
            memory[0xa08000:0xa08900]=bytes(0x900)
            struct.pack_into('<2I',memory,0xa08000,0x8ca08100,3)
            struct.pack_into('<2I',memory,0xa08010,0x8ca08200,2)
            for base in [0xa08100,0xa08200]:
                for i in range(3):struct.pack_into('<3I',memory,base+i*12,0x1234,0x5678,0x8ca08300+i*0x80)
            struct.pack_into('<I',memory,0x88f7a8,0xdeadbeef)
        for memory in (old,native):
            memory[0x8a83e0:0x8c03e8]=b'\xa5'*0x18008;memory[0x67c2d0:0x67c308]=bytes(56)
            struct.pack_into('<2I',memory,0x67c2d4,0x400000,0x400000)
            records=[0x8c8a83e4+i*24 for i in range(3)]
            struct.pack_into('<5I',memory,0x67c2e0,records[0],records[-1],0,0,0x1000)
            for i,record in enumerate(records):
                struct.pack_into('<H',memory,record&0xffffff,4)
                struct.pack_into('<4I',memory,(record&0xffffff)+4,records[i-1] if i else 0,records[i+1] if i<2 else 0,1000+i*100,100)
                resource=0xa08300+i*0x80
                memory[resource:resource+0x80]=bytes(128)
                struct.pack_into('<I',memory,resource,0x1234+i);struct.pack_into('<I',memory,resource+0x10,2+((flags>>3)&1))
                struct.pack_into('<I',memory,resource+0x34,1000+i*100);struct.pack_into('<H',memory,resource+0x40,1)
            struct.pack_into('<3I',memory,0x673484,0x8ca08300,0,0x1234);struct.pack_into('<I',memory,0x88f6a0,0);struct.pack_into('<I',memory,0x673d18,0)
            struct.pack_into('<I',memory,0x88f7b0,0xabcdef12)
            memory[0x8c03e0:0x8d33e8]=b'\xa5'*0x13008
            blocks=[0x8c8c03e4+i*76 for i in range(3)]
            struct.pack_into('<I',memory,0x67c308,blocks[0])
            for i,block in enumerate(blocks):
                struct.pack_into('<H',memory,block&0xffffff,0x1001)
                struct.pack_into('<2I',memory,(block&0xffffff)+4,blocks[i+1] if i<2 else 0,1000+i*100)
                for slot in range(16):struct.pack_into('<I',memory,(block&0xffffff)+12+slot*4,1 if slot==0 else 0)
                resource=0xa08300+i*0x80
                struct.pack_into('<I',memory,resource+0x18,3);struct.pack_into('<I',memory,resource+0x24,8)
                struct.pack_into('<I',memory,resource+0x30,0)
        for memory in (old,native):
            memory[0x880000:0x880200]=bytes(512)
            positions=[16+i*64 for i in range(6)]+[400]
            for i,block in enumerate(positions):
                struct.pack_into('<4I',memory,0x880000+block,0,0,0x8c880000+positions[i+1] if i<6 else 0,0x8c880000+positions[i-1] if i else 0)
            for i,index in enumerate(free_order):
                struct.pack_into('<2I',memory,0x880000+positions[index],0x8c880000+positions[free_order[i+1]] if i+1<len(free_order) else 0,0x8c880000+positions[free_order[i-1]] if i else 0)
            struct.pack_into('<I',memory,0x79f03c,0x8c880000+positions[free_order[0]] if free_order else 0)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=0x8ca01000;regs.r[15]=0x8cf00000;regs.pc=0x0cb00780;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear();reference(old,regs,hooks)
        result=fn(view,ctypes.byref(sr),0x8ca01000,0x8cf00000,0x1234,invoke)
        context=(owner,flags,count,mutation,initial_sr,free_order)
        assert events==original and result==regs.r[0] and sr.value==regs.sr,(context,events,original)
        for start,end in [(0xa01000,0xa08900),(0x8a83e0,0x8d33e8),(0x67c2d0,0x67c310),(0x673480,0x673d20),(0x88f6a0,0x88f6a4),(0x88f7b0,0x88f7b4),(0x88f7a4,0x88f7b0),(0x78c600,0x78cc00),(0x79e40c,0x79ebc4),(0x1c24b4,0x1c25a4)]:assert old[start:end]==native[start:end],context
        assert old[0x880000:0x880200]==native[0x880000:0x880200] and old[0x79f03c:0x79f040]==native[0x79f03c:0x79f040],context
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/tikal_heap_resource_runtime.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'original_bodies_connected':22,'scope':'Original Tikal cleanup, owner cleanup, owner table release reference table release, descriptor release and entity allocation release all executed. Resource-release linked-block payload path and dependencies execute; heap release executes with varied existing free-list order; only owner helper/detach services hooked, including three-argument detach. Compare branch/count combinations, callback mutation, cached work pointer, table/entity/task RAM, return and SR. Tikal state machine and rendering unrecovered.'}
    (out/'analysis/tikal_heap_special_resource_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} Tikal cleanup call-chain comparisons passed')
if __name__=='__main__':main()
