"""Compare valid SDK record lifecycles to Japanese machine code.
Original kmFreeTexture is an explicit observed service boundary. Its allocator
is not emulated: native actual payload release is independently host-tested.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_texture_test_support import Runtime,Info,Record,Name,field
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();root=a.workspace.resolve();p=root/'outputs/sonic-adventure'
    sys.path.insert(0,str(p));from verify_startup import State,reference
    historical=json.loads((p/'analysis/jp_ninja_math_v2_verification.json').read_text())
    assert sha(a.binary)==historical['original_binary_sha256']
    symbols=a.upstream/'config/1st_read/symbols.txt';assert sha(symbols)==historical['original_symbols_sha256']
    table={name:int(address,16) for address,name in re.findall(r'^(0x[0-9a-fA-F]+)\s+(\S+)',symbols.read_text(),re.M)}
    build=json.loads((a.build/'build_verification.json').read_text())
    for n,h in build['authored_source_sha256'].items():assert sha(root/n)==h
    dll=a.build/'jp_native_textures.dll';assert sha(dll)==build['dll_sha256'];t=Runtime(dll)
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    BASE=0x8c000000;RECORDS=0x8cd00000;INFO=0x8cd01000;NAMES=0x8cd02000
    def store(addr,values):struct.pack_into('<'+'I'*len(values),ram,addr-BASE,*values)
    def word(addr):return struct.unpack_from('<I',ram,addr-BASE)[0]
    calls={};comparisons=0;freed=[];reached=set();rng=random.Random(0x1007a11)
    def check(value,why):
        nonlocal comparisons
        comparisons+=1;assert value,why
    def free_service(memory,regs,sr):
        desc=regs[4];freed.append(word(desc+28)>>3);regs[0]=0;return sr
    def trace(pc,*rest):reached.add(pc)
    def call(name,*args):
        s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[15]=0x8cf00000
        for i,value in enumerate(args[:4]):s.r[i+4]=value&0xffffffff
        if len(args)>4:store(s.r[15],[v&0xffffffff for v in args[4:]])
        s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000
        reference(ram,s,hooks={table['kmFreeTexture']:free_service},trace=trace,instruction_budget=20000)
        calls[name]=calls.get(name,0)+1;return C.c_int32(s.r[0]).value
    def compare_records():
        check(t.count.value==C.c_int32(word(table['_nj_tex_count'])).value,'record count')
        check(t.current.value==word(table['_nj_current_texture']),'current texture cache')
        for i in range(4):
            rec=t.at(i);address=RECORDS+i*68
            check([rec.globalIndex,rec.bank,rec.tspparambuffer,rec.texparambuffer,rec.texaddr]==
                  [word(address+4*j) for j in range(5)],'record fields')
            s=rec.texinfo.texsurface
            check([s.Type,s.BitDepth,s.PixelFormat,s.nWidth,s.nHeight,s.TextureSize,s.fSurfaceFlags]==
                  [word(address+24+4*j) for j in range(7)],'surface metadata')
            check(rec.count==struct.unpack_from('<H',ram,address-BASE+64)[0],'reference count')
            check(rec.dummy==struct.unpack_from('<H',ram,address-BASE+66)[0],'record dummy')
            check(bool(rec.texinfo.texaddr)==bool(word(address+20)),'info pointer presence')
            guest_surface=word(address+52)
            if guest_surface and (guest_surface>>3) not in freed:
                check(t.surface_word(C.byref(s))==guest_surface>>3,'owned surface identity')
            elif not guest_surface:
                check(not s.pSurface,'freed record surface')
    def release(name,handle):
        before=t.live();observed=len(freed)
        result=call('njReleaseTextureLN',handle)
        check(t.release(name.texaddr)==result,'release return')
        check(before-t.live()==len(freed)-observed,'actual native frees versus original free requests')
        compare_records()
    for iteration in range(64):
        check(t.init(4,8,8<<20)==0,'initialize')
        ram[RECORDS-BASE:RECORDS-BASE+4*68]=bytes(4*68)
        call('njInitTexMemList',RECORDS,4)
        store(table['_nj_current_texture'],[0xffffffff]);freed.clear()
        compare_records()
        palette=bool(iteration&1);flags=3 if iteration&2 else 2
        width=8<<(iteration%5);height=8<<((iteration//5)%5)
        v=Info();s=v.texsurface
        s.Type=0x05000000 if palette else 0x10000;s.BitDepth=1
        s.PixelFormat=0x28000000 if palette else 0x08000000
        s.nWidth=width;s.nHeight=height;s.TextureSize=width*height*2
        # Exercise all mipmap/VQ/twiddle/stride flag combinations as header
        # bit contracts. These opaque buffers are not decoded image fixtures.
        s.fSurfaceFlags=iteration%32
        payload=C.create_string_buffer(rng.randbytes(s.TextureSize))
        check(t.copy(C.byref(v),payload,s.TextureSize)==0,'copy synthetic payload')
        resource=t.surface_word(C.byref(s))
        # Guest pSurface is an eight-byte-unit hardware offset. Native uses
        # this same address-field ID to identify actual host-owned bytes.
        store(INFO,[0,s.Type,s.BitDepth,s.PixelFormat,width,height,s.TextureSize,s.fSurfaceFlags,
                    resource*8,resource*8,resource*8])
        gid=10000+iteration;names=(Name*2)();guest_handles=[]
        for i in range(2 if palette else 1):
            naddr=NAMES+i*12;bank=i*16 if palette else 0xffffffff
            call('njSetTextureName',naddr,0,gid,0);t.setname(C.byref(names[i]),None,gid,0)
            check(names[i].attr==word(naddr+4) and names[i].texaddr==word(naddr+8),'name global-index phase')
            call('njEntryTexMemList',naddr+8,INFO,gid,flags if i==0 else 2,bank)
            t.entry(field(names[i]),C.byref(v),gid,flags if i==0 else 2,bank)
            check(t.status()==0,'entry status');guest_handle=word(naddr+8);guest_handles.append(guest_handle)
            native=t.resolve(names[i].texaddr)
            check((native-t.record(0))//C.sizeof(Record)==(guest_handle-RECORDS)//68,'handle-to-slot mapping')
            compare_records()
            call('njSetTextureParam',guest_handle);t.param(native);compare_records()
            call('njSetPaletteBankLN',guest_handle,bank);t.palette(native,C.c_int32(bank).value);compare_records()
        t.current.value=gid;store(table['_nj_current_texture'],[gid])
        for query in [gid,gid+1,0xffffffff]:
            check(t.search(t.record(0),4,query)==call('njSearchTexList',RECORDS,4,query),'search index')
            for bank in [0,16,0xffffffff]:
                check(t.search3(t.record(0),4,query,bank)==call('njSearchTexList3',RECORDS,4,query,bank),'banked search index')
        if palette:
            release(names[0],guest_handles[0]);release(names[1],guest_handles[1])
            if flags==3:release(names[0],guest_handles[0])
        else:
            # Vary reference counts without pretending to execute a loader.
            count=1+iteration%3;Record.from_address(t.resolve(names[0].texaddr)).count=count
            struct.pack_into('<H',ram,guest_handles[0]-BASE+64,count)
            for _ in range(count):release(names[0],guest_handles[0])
        check(t.live()==0,'last live surface freed')
        # Cached-only records remain in the SDK table until explicitly freed.
        for i in range(len(guest_handles)):
            if word(guest_handles[i])!=0xffffffff:
                native=t.resolve(names[i].texaddr)
                call('njFreeTexMemList',guest_handles[i]);t.free(native);compare_records()
    t.shutdown()
    report=dict(status='native_texture_SDK_Japanese_lifecycle_contracts_verified',
        comparisons=comparisons,original_calls=sum(calls.values()),calls_by_function=calls,
        original_instruction_addresses_reached=len(reached),scenarios=64,
        original_binary_sha256=sha(a.binary),original_symbols_sha256=sha(symbols),
        historical_executor_sha256=sha(p/'verify_startup.py'),historical_executor_modified=False,
        original_kmFreeTexture_boundary='observe descriptor and return success; no guest allocator or GPU emulation',
        compared_scope='record fields, surface metadata, reference counts, handle-slot relations, searches, free-request counts',
        deliberate_native_policy='clear freed host descriptor pointers; guest stale descriptor pointers are not asserted equal',
        DLL_sha256=sha(dll),build_report_sha256=sha(a.build/'build_verification.json'),
        verifier_sha256=sha(Path(__file__)),test_support_sha256=sha(Path(__file__).with_name('native_texture_test_support.py')),
        full_loaders_or_renderer_qualified=False,hardware_allocator_validated=False,gameplay_verified=False,
        header_surface_flag_combinations=32,encoded_texture_payload_decoding_verified=False,
        overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],comparisons,'comparisons;',sum(calls.values()),'original calls')
if __name__=='__main__':main()
