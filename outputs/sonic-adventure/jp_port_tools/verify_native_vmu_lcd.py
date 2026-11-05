"""Original-machine LCD/init/draw comparisons plus real native endpoint I/O.
No original function hooks; only fresh cards and locally supplied source/data.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,threading
from pathlib import Path
from native_packet_test_support import bind
from capture_native_boot_progress import png
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;P=C.c_void_p;Q=C.c_uint64;S=C.c_size_t
class Point(C.Structure):_fields_=[('x',C.c_int16),('y',C.c_int16)]
def screen_fields(pointer):
    return [('pixels',C.c_ubyte*1536)]+[(n,C.c_int8) for n in ('ready','direction','port','mode')]+[('wait',C.c_int16),('countdown',I),('frame',C.c_uint16),('timer',C.c_uint16),('anim',pointer)]+[(n,C.c_int8) for n in ('puzzles','slides','blank','cursor','blink')]+[('tiles',C.c_int8*9),('along_row',C.c_int8),('look_wait',C.c_int16),('roam_wait',C.c_int16),('target',Point),('pos',Point),('lcd',U),('held',I),('pressed',I)]
class Screen(C.Structure):_fields_=screen_fields(P)
class Guest(C.Structure):_fields_=screen_fields(U)
class Frame(C.Structure):_fields_=[('bitmap',P),('updates',U)]
class StateInfo(C.Structure):_fields_=[(n,I) for n in ('mode','skip','updates','request')]+[('screen_bytes',U),('frame_bytes',U)]
class LCDInfo(C.Structure):_fields_=[('accepted',Q),('presented',Q)]+[(n,U) for n in ('connected','pending','root','device')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','gpu','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    for kind in ('frames','present'):assert sha(b[kind+'_DLL'])==b[kind+'_DLL_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    out.mkdir();media=out/'media';media.mkdir();lib=C.CDLL(b['frames_DLL']);gpu=C.CDLL(str(a.gpu.resolve()));present=C.CDLL(b['present_DLL'])
    init=bind(lib,'vmu_init',None);get_screen=bind(lib,'jp_vmu_screen',P,U);get_state=bind(lib,'jp_vmu_state_read',I,P)
    lcd_init=bind(lib,'jp_lcd_initialize',I,U);lcd_shutdown=bind(lib,'jp_lcd_shutdown',I);lcd_read=bind(lib,'jp_lcd_read',I,U,P,S,P);connect=bind(lib,'jp_lcd_connectors',I,U,U,U)
    ready=bind(lib,'pdVmsLcdIsReady',I,U);direction=bind(lib,'pdLcdGetDirection',I,U);write=bind(lib,'pdVmsLcdWrite1',I,U,P);blit=bind(lib,'pdVmsLcdBlt',None,P,P,U)
    vsync=bind(present,'frame_vsync',None);bind(present,'frame_set_interval',None,I)(1);bind(present,'jp_wait_probe_countdown',None,I)(1000000)
    store={n:bind(lib,'jp_vmu_store_'+n,ret,*args) for n,ret,args in [('initialize',I,[C.c_wchar_p]),('create',I,[U]),('open',I,[U]),('close',I,[U]),('shutdown',I,[])]}
    checks=[];calls={};entered=set();rng=random.Random(0x4c434456)
    def check(ok,label):assert ok,label;checks.append(label)
    info=StateInfo();assert get_state(C.byref(info))==0
    check((info.screen_bytes,info.frame_bytes)==(C.sizeof(Screen),C.sizeof(Frame))==(1600,16) and C.sizeof(Guest)==1596,'native pointer growth preserves original scalar/on-card widths')
    def state():v=StateInfo();assert get_state(C.byref(v))==0;return v
    def native_screen(port=0):return Screen.from_address(get_screen(port))
    def read(drive):
        data=C.create_string_buffer(192);i=LCDInfo();assert lcd_read(drive,data,192,C.byref(i))==0;return data.raw,i
    # Complete original Japanese instructions execute for pure drawing, SDK
    # blitters and init/probe over a seeded Maple description table in RAM.
    import sys
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,_=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    def put(at,data):at&=0xffffff;ram[at:at+len(data)]=data
    def rd(at,count):return bytes(ram[at&0xffffff:(at&0xffffff)+count])
    traced=['vmu_init','vmu_clear','screens_probe','screen_probe','pdVmsLcdIsReady','pdLcdGetDirection','kdGetDeviceInfo','kdDevGetLCDInfo','kdDevGetFD','mpdrv_get_deviceinfo','pdVmsLcdBltAsm','pdVmsLcdBltAsmFlipH','pdVmsLcdBltAsmFlipV','pdVmsLcdBltAsmFlipHV']
    addresses={table[n]:n for n in traced}
    def trace(pc,*unused):
        if pc in addresses:entered.add(addresses[pc])
    def original(name,args=()):
        cpu=executor.State();cpu.pc=table[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
        for i,v in enumerate(args):cpu.r[4+i]=v&0xffffffff
        fpu=Fpu();fpu.fpscr=0x40001;reference(ram,cpu,fpu=fpu,trace=trace,instruction_budget=1500000)
        check(cpu.r[15]==0x8cf00000,'complete original '+name+' restores stack');calls[name]=calls.get(name,0)+1;return cpu.r[0]
    put(table['_mp'],struct.pack('<I',0x8cc80000));guest_info=original('mpdrv_get_deviceinfo',[0]);next_info=original('mpdrv_get_deviceinfo',[1]);check(next_info-guest_info==120,'original device-info stride derived from actual machine code')
    for device in range(24):put(guest_info+device*120,bytes(120))
    def profile(drive,root=0,child=1,connected=True):
        root_port=drive//2*6;port=root_port+drive%2+1
        data=bytearray(rd(guest_info+root_port*120,120));struct.pack_into('<I',data,0,1);data[16]=1;data[18+drive%2]=root;put(guest_info+root_port*120,data)
        data=bytearray(120)
        if connected:struct.pack_into('<I',data,0,14);data[8:12]=bytes([0,5,16,0]);data[18]=child
        put(guest_info+port*120,data)
    init();check(all(native_screen(i).ready==0 for i in range(4)),'game does not invent LCDs before actual owner setup')
    check(store['initialize'](str(media))==0,'real card owner initialized')
    paths=[]
    for drive in range(8):
        path=media/('VMU_'+chr(65+drive//2)+str(1+drive%2)+'.VMU');paths.append(path)
        check(store['create'](drive)==0 and store['open'](drive)==0,'fresh card attached for actual LCD endpoint');profile(drive)
    card_hashes=[sha(p) for p in paths]
    check(lcd_init(255)==0 and lcd_init(255)==4,'real LCD owner initialized once')
    for root in range(4):
        for child in range(4):
            check(connect(0,root,child)==0,'native connector profile configured');profile(0,root,child)
            # Set actual state rather than substituting any init dependency.
            header=(I*4).from_address(get_screen(0)-16);header[:]=[7,31,47,99];put(table['vmu'],struct.pack('<4i',7,31,47,99))
            for port in range(4):
                native=native_screen(port);guest=Guest();pattern=rng.randbytes(1536)
                native.pixels[:]=pattern;guest.pixels[:]=pattern
                for field in ('wait','countdown','frame','timer','puzzles','slides','blank','cursor','blink','along_row','look_wait','roam_wait'):
                    setattr(native,field,7);setattr(guest,field,7)
                native.anim=None;guest.anim=0;native.tiles[:]=guest.tiles[:]=list(range(9));native.target=guest.target=Point(4,5);native.pos=guest.pos=Point(6,7)
                native.held=guest.held=123;native.pressed=guest.pressed=456
                put(table['vmu']+16+port*C.sizeof(Guest),bytes(guest))
            original('vmu_init');init();check((state().mode,state().skip,state().updates,state().request)==(7,0,0,0),'native original VMU header reset matches intended preserved game mode')
            for port in range(4):
                native=native_screen(port);guest=Guest.from_buffer_copy(rd(table['vmu']+16+port*C.sizeof(Guest),C.sizeof(Guest)))
                for name,typ in Screen._fields_:
                    nv,gv=getattr(native,name),getattr(guest,name)
                    if name=='anim':check(nv is None and gv==0,'init preserves original animation pointer')
                    elif isinstance(nv,(C.Array,C.Structure)):check(bytes(nv)==bytes(gv),'native/original VMU init field '+name)
                    else:check(nv==gv,'native/original VMU init scalar '+name)
    check(traced[:10] and set(traced[:10]).issubset(entered),'complete original init/probe/SDK/Maple memory providers executed without hooks')
    source=0x8cc00000;target=0x8cc10000;guard=b'\xa5'*16
    for case in range(5):
        pixels=rng.randbytes(1536);data=C.create_string_buffer(pixels);put(source,pixels)
        for flag in (0,1,2,3,99):
            put(target-16,guard+bytes(192)+guard);outbuf=C.create_string_buffer(guard+bytes(192)+guard)
            original('pdVmsLcdBlt',[target,source,flag]);blit(C.byref(outbuf,16),data,flag)
            expected=bytearray(192)
            for y in range(32):
                for x in range(48):
                    if pixels[y*48+x]&8:
                        dx=47-x if flag in (1,3) else x;dy=31-y if flag in (2,3) else y;expected[dy*6+dx//8]|=128>>(dx%8)
            check(outbuf.raw[:-1]==guard+bytes(expected)+guard==rd(target-16,224),'all original ASM flip bits and independent bit-3 oracle, with both canaries')
    native=native_screen();native.port=0;header=(I*4).from_address(get_screen(0)-16);header[2]=0;put(table['vmu']+8,bytes(4));guest_addr=0x8cc20000
    for dx,dy in [(0,0),(1,2),(-3,4),(47,31),(-48,0),(0,32),(55,-40)]:
        pixels=rng.randbytes(1536);native.pixels[:]=pixels;guest=Guest();guest.port=0;guest.pixels[:]=pixels;put(guest_addr,bytes(guest));point=Point(dx,dy);put(source,bytes(point))
        original('vmu_shift',[guest_addr,source]);bind(lib,'vmu_shift',None,P,P)(C.byref(native),C.byref(point))
        expected=bytes(pixels[(y-dy)*48+x-dx] if 0<=x-dx<48 and 0<=y-dy<32 else 0 for y in range(32) for x in range(48))
        check(bytes(native.pixels)==expected==rd(guest_addr,1536),'original clipped VMU shift matches every independent output pixel')
    for sx,sy in [(0,4),(1,1),(2,2),(3,5),(7,4),(48,32),(49,2),(-1,3)]:
        pixels=rng.randbytes(1536);native.pixels[:]=pixels;guest=Guest();guest.port=0;guest.pixels[:]=pixels;put(guest_addr,bytes(guest));size=Point(sx,sy);put(source,bytes(size))
        original('vmu_pixelate',[guest_addr,source]);bind(lib,'vmu_pixelate',None,P,P)(C.byref(native),C.byref(size));check(bytes(native.pixels)==rd(guest_addr,1536),'complete original VMU pixelation output matches native bytes')
    # Queue every port; one pending packet owns its source bytes until a real
    # frame service commits it. Another write is genuinely busy.
    for drive in range(8):
        check(connect(drive,0,1)==0,'normal orientation restored')
    init();payloads=[]
    for drive in range(8):
        port=drive//2*6+drive%2+1;payload=rng.randbytes(192);data=C.create_string_buffer(payload);payloads.append(payload)
        check(ready(port)==1 and write(port,data)==0,'original SDK queues a real LCD packet')
        C.memset(data,0,192);check(write(port,data)==-1,'second write cannot overwrite a pending packet')
        raw,info=read(drive);check(raw==bytes(192) and info.pending==1 and info.accepted==1 and info.presented==0,'accepted packet is copied and remains pending before service')
    check(lcd_shutdown()==4,'pending LCD commands prevent silent shutdown/discard');vsync()
    for drive,payload in enumerate(payloads):
        raw,info=read(drive);check(raw==payload and not info.pending and info.presented==1,'real frame_vsync commits each independent LCD packet')
    # Draw an actual locally supplied original Chao picture using unchanged
    # game draw/pack/write bodies, then observe the committed native LCD.
    # This .inc is generated by the matching build, not stored in the checkout.
    # Read the actual picture from the already hash-verified supplied binary.
    picture=rd(table['chao_pictures'],72);assert len(picture)==72 and any(picture)
    bitmap=C.create_string_buffer(picture);animation=(Frame*2)();animation[0].bitmap=C.addressof(bitmap);animation[0].updates=2
    native=native_screen();bind(lib,'vmu_clear',None,P)(C.byref(native));bind(lib,'vmu_set_anim',None,P,P)(C.byref(native),animation);bind(lib,'vmu_draw_chao',None,P)(C.byref(native))
    expected=bytearray(192)
    for y in range(24):
        for x in range(24):
            if picture[y*3+x//8]&(128>>(x%8)):expected[(y+4)*6+(x+12)//8]|=128>>((x+12)%8)
    bind(lib,'vmu_write',None,P)(C.byref(native));check(read(0)[1].pending==1,'original game LCD pack/write queues real owned pixels');vsync();raw,info=read(0);check(raw==bytes(expected),'original game Chao drawing reaches actual LCD endpoint bit for bit')
    rgba=bytearray()
    for y in range(32*8):
        for x in range(48*8):rgba.extend((31,48,32,255) if raw[(y//8)*6+(x//8)//8]&(128>>((x//8)%8)) else (186,200,164,255))
    picture_file=out/'original-chao-lcd-buffer.png';png(picture_file,bytes(rgba),48*8,32*8)
    # Removal discards a pending image before it can appear on a new card.
    check(write(1,C.create_string_buffer(bytes([255])*192))==0 and store['close'](0)==0,'pending LCD packet then actual media removal');vsync();raw,info=read(0);check(not info.connected and not info.pending and raw==bytes(192) and ready(1)==0 and write(1,bitmap)==-2,'removed endpoint has no LCD and cannot accept a write')
    check(store['open'](0)==0,'card reattached');init();check(native_screen().ready==1 and read(0)[0]==bytes(192),'reinserted card does not inherit old pending LCD pixels')
    data=C.create_string_buffer(b'\xa5'*224);meta=LCDInfo();prior=bytes(data)
    check(lcd_read(8,data,192,C.byref(meta))==1 and lcd_read(0,data,191,C.byref(meta))==1 and bytes(data)==prior,'invalid native read range leaves caller bytes untouched')
    foreign=[];t=threading.Thread(target=lambda:foreign.extend([lcd_read(0,data,192,C.byref(meta)),lcd_shutdown()]));t.start();t.join();check(foreign==[3,3] and bytes(data)==prior,'foreign thread cannot read/change LCD owner')
    check([sha(p) for p in paths]==card_hashes,'LCD work never changes any real virtual-card disk bytes')
    check(lcd_shutdown()==0,'LCD owner cleanly shuts down');init();check(all(native_screen(i).ready==0 for i in range(4)),'original init sees no LCD after real display owner shutdown')
    check(store['shutdown']()==0,'real card owner shuts down')
    result=dict(status='original_VMU_init_draw_ASM_bit_semantics_and_frame_driven_native_LCD_endpoints_verified',checks=len(checks),original_machine_calls=calls,total_original_calls=sum(calls.values()),original_entries_seen=sorted(entered),original_function_hooks_used=False,all_four_screens_and_eight_LCD_endpoints=True,all_16_connector_direction_combinations=True,native_pointer_growth_accounted=True,queued_busy_source_lifetime_disconnect_and_owner_thread_verified=True,original_Chao_picture_and_game_pack_write_reach_real_LCD=True,LCD_buffer_capture=dict(path=str(picture_file),sha256=sha(picture_file),source_binary_sha256=sha(a.binary),picture_address=hex(table['chao_pictures']),actual_committed_LCD_bytes=True,native_window_screenshot=False,retail_asset_derived=True,VMU_minigame_running=False),build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),VMU_CPU_program_emulator_or_widget_implemented=False,user_saves_used=False,whole_gameplay_verified=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(checks),'checks',sum(calls.values()),'original calls',flush=True)
if __name__=='__main__':main()
