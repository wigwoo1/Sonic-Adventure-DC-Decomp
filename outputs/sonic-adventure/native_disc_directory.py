"""Recovered root-directory loader and lookup using a native host sector service.
No extraction manifest is used for file locations. This is not full game mounting.
"""
import ctypes,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_request_abort_8c6013d2 import Dispatch,Read as TimerRead,Observe,MMIO
from verify_interrupt_constructor import IO as InterruptIO,Read as InterruptRead,Write as InterruptWrite
PROJECT=ROOT/'outputs/sonic-adventure';SIZE=16*1024*1024;U=ctypes.c_uint32;BP=ctypes.POINTER(ctypes.c_uint8)

def discover_root_files(lib,read,offset,sectors,base,stride,names,boot_directory=None):
    init=lib.sa_disc_queue_init
    submit=lib.sa_disc_queue_submit;submit.argtypes=[ctypes.c_void_p,U,U,BP,ctypes.c_uint64,ctypes.POINTER(U)]
    step=lib.sa_disc_queue_step;step.argtypes=[ctypes.c_void_p]
    release=lib.sa_disc_queue_release;release.argtypes=[ctypes.c_void_p,U]
    q=ctypes.create_string_buffer(lib.sa_disc_queue_size())
    assert init(q,read,None,offset,sectors,base,stride)==0
    def sectors_at(lba,count):
        out=(ctypes.c_uint8*(count*2048))();handle=U()
        assert submit(q,lba,count,out,len(out),ctypes.byref(handle))==0
        for _ in range(count):assert step(q)==0
        assert release(q,handle.value)==0
        return bytes(out)
    volume=sectors_at(base+16,1);assert volume[:7]==b'\x01CD001\x01'
    lba=struct.unpack_from('<I',volume,158)[0];size=struct.unpack_from('<I',volume,166)[0]
    assert 0<size<=1024*1024
    directory=sectors_at(lba,(size+2047)//2048)
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary
    if boot_directory is not None:
        boot_name=boot_directory.encode('ascii');assert 0<len(boot_name)<=12
        ram[0x80f0:0x80fe]=boot_name.ljust(12,b' ')+b'\x01\x00'
    for at,value in ((0xbc,0x8c740000),(0x673414,0),(0x67c004,0x8c7d0000)):
        struct.pack_into('<I',ram,at,value)
    abi=[BP,ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
    specs={0x8c6454fe:'sa_file_directory_load',0x8c645468:'sa_file_directory_record_parse',0x8c64535c:'sa_file_directory_bytes_copy',
      0x8c645348:'sa_file_name_upper',0x8c6453c0:'sa_file_name_equal',0x8c6453e2:'sa_file_name_equal_folded',
      0x8c645442:'sa_file_name_checksum',0x8c6455ce:'sa_file_filename_lookup'}
    specs.update({0x8c5ffc98:'sa_file_mount_sector_read',0x8c600d82:'sa_file_reserved_handle_acquire',
      0x8c600f1e:'sa_file_read_sectors',0x8c601084:'sa_file_read_aligned',0x8c644ff6:'sa_file_device_read',
      0x8c10d938:'sa_file_bios_request',0x8c10d960:'sa_file_bios_execute'})
    specs.update({0x8c600038:'sa_file_context_initialize',0x8c600dd0:'sa_file_directory_storage_initialize',
      0x8c645328:'sa_file_device_interface_get',0x8c644fe8:'sa_file_device_initialize',0x8c10d974:'sa_file_device_low_initialize',
      0x8c600464:'sa_file_handle_open_initialize'})
    specs.update({0x8c5ffd28:'sa_file_mount',0x8c5ff9e6:'sa_file_volume_signature_check',0x8c5ff9c0:'sa_file_volume_word_get',
      0x8c5ffcfe:'sa_file_directory_parent_sector_adjust',0x8c60041a:'sa_file_directory_select',
      0x8c64135a:'sa_interrupt_handler_primary',0x8c6412c2:'sa_interrupt_handler_set',0x8c658c20:'sa_mount_driver_enable',
      0x8c6451c8:'sa_file_device_disc_request',0x8c6451fc:'sa_file_device_disc_info',0x8c10d988:'sa_file_bios_disc_info',
      0x8c64530a:'sa_file_device_version_request'})
    functions={a:getattr(lib,n) for a,n in specs.items()}
    for fn in functions.values():fn.argtypes=abi
    service=lib.sa_disc_service;service.argtypes=[ctypes.c_void_p,BP,U,U,U,ctypes.POINTER(U)]
    finalizer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/stream_cleanup_disc_native.dll')).sa_request_finalize_native
    finalizer.argtypes=[BP,ctypes.POINTER(State),ctypes.c_void_p,TimerRead,Dispatch,Observe,U]
    Pump=ctypes.CFUNCTYPE(None,BP,ctypes.POINTER(State))
    pump=lib.sa_callback_service_call_return_one_8c6452d6
    pump.argtypes=[BP,ctypes.POINTER(State),Pump];pump.restype=None
    constructor=lib.sa_interrupt_callback_create;constructor.argtypes=abi[:7]+[ctypes.POINTER(InterruptIO)]+abi[7:]
    errors=[];comparisons=0;sector_calls=[];mount_calls=[]
    def execute(entry,args):
        nonlocal ram,comparisons
        if entry==0x8c600038:struct.pack_into('<I',ram,0xf00000,0x8c66c4b4)
        expected_ram=bytearray(ram);view=(ctypes.c_uint8*SIZE).from_buffer(ram)
        expected_view=(ctypes.c_uint8*SIZE).from_buffer(expected_ram)
        expected_queue=ctypes.create_string_buffer(lib.sa_disc_queue_size())
        assert init(q,read,None,offset,sectors,base,stride)==0
        assert init(expected_queue,read,None,offset,sectors,base,stride)==0
        hardware={0xa05f6920:0,0xa05f6924:0,MMIO:0xffffffff};native_hardware=dict(hardware)
        @InterruptRead
        def read_interrupt(ctx,address,value):
            if address not in native_hardware:errors.append(hex(address));return -3
            value[0]=native_hardware[address];return 0
        @InterruptWrite
        def write_interrupt(ctx,address,value):
            if address not in native_hardware:errors.append(hex(address));return -3
            native_hardware[address]=value;return 0
        interrupt=InterruptIO(0x8c7e0000,0,None,read_interrupt,write_interrupt)
        state=State();state.r[:]=[0x22340000+i for i in range(16)]
        for i,v in enumerate(args):state.r[4+i]=v
        state.r[15]=0x8cf00000;state.sr=0x70000063;state.pr=0x8c000100;state.pc=entry
        r=(U*16)(*state.r);sr=U(state.sr);pr=U(state.pr);fpul=U();macl=U();f=Fpu();nf=Fpu.from_buffer_copy(f)
        def expected_service(memory,regs,status):
            value=U();assert regs[6]==0
            assert service(expected_queue,expected_view,regs[7],regs[4],regs[5],ctypes.byref(value))==0
            regs[0]=value.value;return status
        def coherent_cache(memory,regs,status):regs[0]=0;return status
        @TimerRead
        def timer(user,address):assert address==MMIO;return 0xffffffff
        @Dispatch
        def device(memory,cpu,target,ctx):
            if target!=0x8c740000 or cpu.contents.r[6]!=0:return -8
            value=U();rc=service(q,memory,cpu.contents.r[7],cpu.contents.r[4],cpu.contents.r[5],ctypes.byref(value))
            if not rc:cpu.contents.r[0]=value.value;cpu.contents.pc=cpu.contents.pr
            return rc
        @Call
        def call(ctx,target,m,fp,status,regs,ret):
            try:
                if target==0x8c5ffc98:
                    sector_calls.append((regs[4],regs[5],regs[6]))
                if target==0x8c5ffd28:mount_calls.append(tuple(regs[i] for i in range(4,8)))
                if target==0x8c64594c:return constructor(m,fp,status,regs,ret,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(interrupt),None,call)
                if target in (0x8c5fc0d8,0x8c5fc336):regs[0]=0;return 0
                if target==0x8c740000:
                    value=U();assert regs[6]==0
                    rc=service(q,m,regs[7],regs[4],regs[5],ctypes.byref(value));regs[0]=value.value;return rc
                if target in (0x8c600cf0,0x8c6452d6):
                    cpu=State();cpu.r[:]=[regs[i] for i in range(16)];cpu.sr,cpu.pr,cpu.pc=status[0],ret[0],target
                    if target==0x8c600cf0:rc=finalizer(m,ctypes.byref(cpu),None,timer,device,Observe(),100000)
                    else:
                        @Pump
                        def lower(memory,s):
                            rr=(U*16)(*s.contents.r);ss=U(s.contents.sr);pp=U(s.contents.pr)
                            assert call(None,s.contents.pc,m,fp,ctypes.byref(ss),rr,ctypes.byref(pp))==0
                            s.contents.r[:]=rr;s.contents.sr,s.contents.pr,s.contents.pc=ss.value,pp.value,pp.value
                        pump(m,ctypes.byref(cpu),lower);rc=0
                    for i in range(16):regs[i]=cpu.r[i]
                    status[0],ret[0]=cpu.sr,cpu.pr;return rc
                return functions[target](m,fp,status,regs,ret,ctypes.byref(fpul),ctypes.byref(macl),None,call)
            except BaseException as exc:errors.append(repr(exc));return -9
        reference(expected_ram,state,fpu=f,hooks={0x8c740000:expected_service,0x8c5fc0d8:coherent_cache,0x8c5fc336:coherent_cache},mmio=hardware,vbr=interrupt.vbr)
        assert functions[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        assert not errors,errors
        assert ram==expected_ram and list(r)==list(state.r) and sr.value==state.sr and pr.value==state.pr and bytes(f)==bytes(nf)
        assert hardware==native_hardware
        comparisons+=1;return r[0]
    root_lba,root_size=lba,size
    mount_result=execute(0x8c600038,(2,0x8c7a0000,4096,0x8cb00000))
    if mount_result:
        return [None for _ in names],{'boot_directory':boot_directory,'mount_result':mount_result,
          'mounted_flag':struct.unpack_from('<H',ram,0x7a0038)[0],'native_comparisons':comparisons,
          'mount_calls':mount_calls,'loader_sector_calls':sector_calls,'scope':'Full retail/native context/mount failure comparison; no synthetic success or directory fallback.'}
    assert struct.unpack_from('<I',ram,0x673414)[0]==0x8c7a0000
    assert struct.unpack_from('<I',ram,0x7a000c)[0]==0x8c66d240
    assert struct.unpack_from('<I',ram,0x7a003c)[0]==0x8c66c4b4
    assert struct.unpack_from('<I',ram,0x7a0014)[0]==0x8c7a1080
    assert struct.unpack_from('<H',ram,0x7a10cc)[0]==1
    assert len(mount_calls)==1
    assert struct.unpack_from('<H',ram,0x7a0038)[0]==1
    if boot_directory is not None:
        position=0;selected=None
        while position<root_size:
            length=directory[position]
            if not length:position=(position//2048+1)*2048;continue
            record=directory[position:position+length]
            raw=record[33:33+record[32]]
            if record[25]&2 and raw.upper()==boot_directory.upper().encode('ascii'):
                selected=struct.unpack_from('<I',record,2)[0],struct.unpack_from('<I',record,10)[0];break
            position+=length
        assert selected is not None
        lba,size=selected;directory=sectors_at(lba,(size+2047)//2048)
    assert execute(0x8c6454fe,(0xaca00000,lba+150,0x8cb00000,size))==0
    count=struct.unpack_from('<I',ram,0xb00000)[0]
    assert 0<count<4096
    # Independent read-only oracle; this walk does not supply loader state.
    oracle=[];position=0
    while position<size:
        length=directory[position]
        if not length:position=(position//2048+1)*2048;continue
        record=directory[position:position+length];assert len(record)==length and length>=34
        raw=record[33:33+record[32]]
        name=(b'.' if raw==b'\0' else b'..' if raw==b'\1' else raw) if record[25]&2 else raw[:-2]
        oracle.append((struct.unpack_from('<I',record,2)[0],struct.unpack_from('<I',record,10)[0],name))
        position+=length
    assert len(oracle)==count
    seed=struct.unpack_from('<h',binary,0x8c64554e-0x8c010000)[0]
    for index,(extent,length,name) in enumerate(oracle):
        if index==1:extent=oracle[0][0] # Original parent-sector adjustment.
        at=0xb00010+index*44
        assert struct.unpack_from('<II',ram,at)==(extent,length)
        assert bytes(ram[at+10:at+11+len(name)])==name+b'\0'
        assert ram[at+9]==((seed+sum(name.upper()))&0xffffffff)>>2&255
    expected_reads=[base+16+150]+list(range(root_lba+150,root_lba+150+(root_size+2047)//2048))
    if boot_directory is not None:expected_reads+=list(range(lba+150,lba+150+(size+2047)//2048))
    assert [x[0] for x in sector_calls]==expected_reads
    before_calls=len(sector_calls)
    table_before=bytes(ram[0xb00000:0xb00010+count*44])
    assert execute(0x8c6454fe,(0xaca00000,lba+150,0x8cb00000,size))==0
    assert len(sector_calls)==before_calls and bytes(ram[0xb00000:0xb00010+count*44])==table_before
    resolved=[];lookups=[]
    for query in names:
        encoded=query.encode('ascii');ram[0xc00000:0xc00000+len(encoded)+1]=encoded+b'\0'
        found=execute(0x8c6455ce,(0x8cc00000,0x8cb00000,0))
        lookups.append({'query':query,'index':found})
        if not found:resolved.append(None);continue
        at=0xb00010+(found-1)*44
        extent,length=struct.unpack_from('<II',ram,at)
        actual_name=bytes(ram[at+10:at+44]).split(b'\0')[0].decode('ascii')
        resolved.append({'path':actual_name,'lba':extent,'size':length})
    evidence={'boot_directory':boot_directory,'mount_result':0,'directory_lba':lba,'directory_fad':lba+150,'directory_size':size,'records':count,'independent_records_checked':len(oracle),'native_comparisons':comparisons,'cached_reload_skips_io':True,'context_initialized_natively':True,'mount_calls':mount_calls,'mount_completed_natively':True,
      'volume_sha256':hashlib.sha256(volume).hexdigest(),'directory_sha256':hashlib.sha256(directory).hexdigest(),'lookups':lookups,'loader_sector_calls':sector_calls,
      'resolved':resolved,'scope':'Original context initializer including complete mount tailcall, callback construction/primary installation, INIT/drive/version, volume signature/field decoding, directory loader/read/wait/completion and lookup. Full retail RAM/CPU/FPU/MMIO comparison. Host PVD inspection is an independent oracle; context storage, interrupt bootstrap and root boot selector are platform inputs. Cache services are coherent host no-ops.'}
    return resolved,evidence
