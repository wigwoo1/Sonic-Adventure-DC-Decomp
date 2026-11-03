"""Test real adapted SDK allocation/release and owned synthetic payloads."""
import argparse,ctypes as C,hashlib,json,random
from pathlib import Path
from native_texture_test_support import Runtime,Info,Record,Name,List,field
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();root=a.workspace.resolve();b=a.build.resolve();build=json.loads((b/'build_verification.json').read_text())
    assert build['status']=='native_texture_runtime_linked_behavior_unverified'
    for n,h in build['authored_source_sha256'].items():assert sha(root/n)==h
    dll=b/'jp_native_textures.dll';assert sha(dll)==build['dll_sha256']
    t=Runtime(dll);checks=0;groups={};rng=random.Random(0x544558)
    def check(value):
        nonlocal checks
        checks+=1;assert value,f'host texture check {checks}'
    def info(width=8,height=8,palette=False):
        v=Info();s=v.texsurface;s.Type=0x05000000 if palette else 0x10000
        s.BitDepth=1;s.PixelFormat=0x28000000 if palette else 0x08000000
        s.nWidth=width;s.nHeight=height;s.TextureSize=width*height*2;s.fSurfaceFlags=4
        payload=rng.randbytes(s.TextureSize);source=C.create_string_buffer(payload)
        check(t.copy(C.byref(v),source,len(payload))==0)
        word=t.surface_word(C.byref(s));check(word!=0)
        check(s.pSurface>0xffffffff and s.pSurface%32==0)
        check(t.surface_bytes(word)==len(payload));check(C.string_at(t.surface_data(word),len(payload))==payload)
        source[0]=b'\x00' if payload[0] else b'\xff'
        check(C.string_at(t.surface_data(word),len(payload))==payload)
        return v,word
    def enter(name,v,gid,flags=2,bank=0xffffffff):
        t.setname(C.byref(name),None,gid,0)
        t.entry(field(name),C.byref(v),gid,flags,bank);check(t.status()==0)
        address=t.resolve(name.texaddr);check(address and address>0xffffffff)
        record=Record.from_address(address)
        check(record.globalIndex==gid and record.bank==bank and record.texaddr==flags)
        check(record.count==bool(flags&6))
        t.param(address);t.palette(address,bank);check(t.status()==0)
        return address,record
    check(t.status()==8);check(t.init(0,8,1024)==1);t.clear();check(t.status()==8)
    check(t.init(4,8,1)==3);check(t.init(4,8,1<<20)==0)
    base_alloc=t.allocated();base=t.record(0)
    check(base>0xffffffff);check(t.count.value==0)
    for i in range(4):check(t.at(i).globalIndex==0xffffffff and t.at(i).count==0)
    v,word=info();names=(Name*2)();address,record=enter(names[0],v,1000)
    listing=List(names,1);check(t.lookup(C.byref(listing),0)==address)
    check(record.texinfo.texsurface.pSurface==v.texsurface.pSurface)
    check(record.tspparambuffer==0);check(record.texparambuffer==0x08000000|word)
    # A global-index field numerically equal to a live handle is not a binding.
    names[1].texaddr=names[0].texaddr;listing.nbTexture=2
    check(t.lookup(C.byref(listing),1) is None);check(t.status()==5);t.clear()
    check(t.store(field(names[1]),address)==0);record.count=2
    check(t.lookup(C.byref(listing),1)==address)
    check(t.select(C.byref(listing))==1);t.current.value=1000
    old_handle=names[0].texaddr
    check(t.release_num(0)==1);check(record.count==1);check(t.live()==1)
    check(t.current.value==1000);check(t.release_num(1)==1)
    check(t.live()==0 and t.allocated()==base_alloc);check(t.current.value==0xffffffff)
    check(t.resolve(old_handle) is None);t.clear()
    check(t.surface_data(word) is None);t.clear()
    check(t.release(old_handle)==-1);t.clear()
    check(t.release_num(99)==-1);t.clear()
    check(t.init(1,1,1)==3);t.clear();check(t.record(0)==base)
    groups['ownership_aliases_stale_values_and_recovery']=checks

    begin=checks
    for flags in [2,3]:
        check(t.init(4,8,1<<20)==0);v,word=info(palette=True)
        names=(Name*2)();a0,r0=enter(names[0],v,2222,flags,0)
        a1,r1=enter(names[1],v,2222,2,16)
        check(a0!=a1);check(t.live()==1)
        check(t.release(names[0].texaddr)==1);check(t.live()==1)
        if flags==3:
            check(r0.count==1 and r0.texaddr==3)
            check(t.release(names[1].texaddr)==1);check(t.live()==1)
            check(t.release(names[0].texaddr)==1);check(t.live()==0)
            check(r0.texaddr==1 and r0.count==0)
            check(t.resolve(names[0].texaddr)==a0)
            listing=List(names,1);check(t.lookup(C.byref(listing),0) is None);t.clear()
            t.free(a0);check(t.status()==0)
        else:
            check(t.release(names[1].texaddr)==1);check(t.live()==0)
        check(t.count.value==0)
    groups['real_palette_shared_surface_and_cached_release']=checks-begin

    begin=checks;check(t.init(3,8,1<<20)==0)
    v,word=info();names=(Name*4)();addresses=[]
    for i in range(3):addresses.append(enter(names[i],v,3000+i)[0])
    before=(t.count.value,bytes(names[3]),bytes(t.at(2)))
    t.entry(field(names[3]),C.byref(v),4000,2,0)
    check(t.status()==2);t.clear();check((t.count.value,bytes(names[3]),bytes(t.at(2)))==before)
    check(t.nextinfo(0) is None);check(t.status()==2);t.clear()
    for i,address in enumerate(addresses):
        check(t.search(t.record(0),3,3000+i)==i)
        check(t.search2(t.record(0),3,address)==i)
        check(t.search3(t.record(0),3,3000+i,0xffffffff)==i)
    check(t.search2(t.record(0),3,addresses[0]+1)==-1)
    check(t.search2(t.record(0),3,addresses[0]+(1<<32)*C.sizeof(Record))==-1)
    for address in addresses:t.free(address)
    check(t.count.value==0)
    check(t.init(2,1,1<<20)==0);v,word=info();names=(Name*2)()
    enter(names[0],v,5000);before=t.count.value
    t.entry(field(names[1]),C.byref(v),5001,2,0)
    check(t.status()==2 and t.count.value==before);t.clear()
    check(t.at(1).globalIndex==0xffffffff)
    # Name reinitialization removes the old field binding before storing an ID.
    names[1].texaddr=names[0].texaddr;t.setname(C.byref(names[0]),None,names[1].texaddr,0)
    listing=List(names,1);check(t.lookup(C.byref(listing),0) is None);t.clear()
    groups['record_binding_exhaustion_search_and_name_phase']=checks-begin

    begin=checks;issued=[]
    for cycle in range(96):
        check(t.init(1,2,8<<20)==0);base_alloc=t.allocated()
        width=8<<(cycle%5);height=8<<((cycle//5)%5)
        v,word=info(width,height);name=Name();address,record=enter(name,v,6000+cycle)
        issued.append(name.texaddr)
        check(record.tspparambuffer==((width.bit_length()-4)<<3)|(height.bit_length()-4))
        check(t.release(name.texaddr)==1);check(t.live()==0);check(t.allocated()==base_alloc)
        check(t.count.value==0)
        check(t.resolve(issued[0]) is None);t.clear()
    check(len(set(issued))==len(issued))
    # Reinitialization also invalidates handles from an earlier live runtime.
    v,word=info();name=Name();enter(name,v,7000);live_handle=name.texaddr
    check(t.init(1,2,8<<20)==0);check(t.resolve(live_handle) is None);t.clear()
    check(t.live()==0)
    budget=t.allocated()+64;check(t.init(1,2,budget)==3);t.clear()
    v=Info();v.texsurface.nWidth=v.texsurface.nHeight=8;v.texsurface.TextureSize=128
    # Reinitialize with a deliberately tiny payload allowance after shutdown.
    cost=t.allocated();t.shutdown();check(t.init(1,2,cost+64)==0)
    payload=C.create_string_buffer(128);before=t.allocated()
    check(t.copy(C.byref(v),payload,128)==3);check(t.allocated()==before and not v.texsurface.pSurface)
    t.clear();t.shutdown();check(t.status()==8 and t.allocated()==0 and t.live()==0)
    groups['dimension_words_slot_reuse_epoch_and_budget']=checks-begin
    begin=checks;check(t.init(4,8,1<<20)==0)
    qinit=t.dll.jp_queue_initialize;qinit.restype=C.c_int;qinit.argtypes=[C.c_size_t]
    qctx=t.dll.jp_queue_set_context;qctx.restype=C.c_int;qctx.argtypes=[C.c_uint,C.POINTER(C.c_uint32)]
    qget=t.dll.njGetkmPointer;qget.restype=None;qget.argtypes=[]
    qparam=t.dll.jp_queue_get_parameter;qparam.restype=C.c_int;qparam.argtypes=[C.POINTER(C.c_uint32)]
    draw_select=t.dll.jp_texture_renderer_select;draw_select.restype=None;draw_select.argtypes=[C.c_uint32]
    check(qinit(1024)==0)
    header=(C.c_uint32*4)(0xa0000002,0xb1234567,0xabcdef80,0xdeaddead)
    check(qctx(0,header)==0);qget();result=(C.c_uint32*4)()
    names=(Name*3)();listing=List(names,3)
    v,word=info(16,32,True);address,r0=enter(names[0],v,8000,2,0)
    address1,r1=enter(names[1],v,8000,2,16)
    v2,word2=info(64,16);address2,r2=enter(names[2],v2,8001)
    check(t.select(C.byref(listing))==1)
    for i in [0,1,2]:check(t.lookup(C.byref(listing),i)==[address,address1,address2][i])
    draw_select(0);check(t.current.value==8000);check(qparam(result)==0)
    check(list(result)==[header[0],header[1],(header[2]&~63)|(r0.tspparambuffer&63),r0.texparambuffer])
    first=list(result);draw_select(1);check(qparam(result)==0)
    check(list(result)==first) # Actual GCC helper intentionally caches globalIndex alone.
    draw_select(2);check(t.current.value==8001);check(qparam(result)==0)
    check(list(result)==[header[0],header[1],(header[2]&~63)|(r2.tspparambuffer&63),r2.texparambuffer])
    collision=Name();collision.texaddr=names[0].texaddr
    check(t.resolve_field(field(collision)) is None);check(t.status()==5);t.clear()
    check(t.resolve_field(field(names[0]))==address)
    check(t.release_list(C.byref(listing))==1);check(t.live()==0)
    t.shutdown();t.dll.jp_queue_shutdown()
    groups['actual_GCC_renderer_selection_and_field_aware_loader_boundary']=checks-begin
    report=dict(status='native_texture_record_and_owned_surface_contracts_verified',checks=checks,groups=groups,
        actual_SDK_record_creation_search_and_release=True,synthetic_payloads_only=True,
        nonrecycled_handle_cases=96,all_issued_handles_unique=True,game_source_linked=True,
        game_containing_artifact_publishable=False,dll_sha256=sha(dll),
        build_report_sha256=sha(b/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        test_support_sha256=sha(Path(__file__).with_name('native_texture_test_support.py')),
        complete_loaders_or_renderer_qualified=False,gameplay_verified=False,
        actual_GCC_renderer_texture_selection_executed=True,
        overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks')
if __name__=='__main__':main()
