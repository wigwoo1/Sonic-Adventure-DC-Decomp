"""Compare genuine recorder/full input calls with the Japanese machine code.

Only original platform boundaries (peripheral lookup, dev-host file save and
debug print) are hooked. Original get_character, stage_end(QUIT), recorder,
pad_scan and complete analog math execute. Native saving uses actual files.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from verify_native_input_backend import Sample,Source,Peripheral
from camera_reference_numeric import service_for

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();p=r/'outputs/sonic-adventure';out=a.output.resolve()
    assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    b=json.loads((d/'build_verification.json').read_bytes());pin=json.loads((p/'jp_port_tools/jp_camera_external_inputs.json').read_bytes())
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    lib=C.CDLL(b['frames_DLL']);sys.path.insert(0,str(p));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    num=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;num.restype=C.c_int;num.argtypes=[C.c_uint32,C.POINTER(Fpu),C.c_uint32];numeric=service_for(num)
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);image=a.binary.read_bytes();ram[0x10000:0x10000+len(image)]=image
    def pos(address):return address&0xffffff
    def put(address,raw):ram[pos(address):pos(address)+len(raw)]=raw
    def read(address,n):return bytes(ram[pos(address):pos(address)+n])
    def word(address,value):put(address,struct.pack('<I',value&0xffffffff))
    def value(address):return struct.unpack('<I',read(address,4))[0]
    def text(address):return read(address,128).split(b'\0')[0].decode('ascii')
    U=C.c_uint32;I=C.c_int;P=C.c_void_p
    checks=calls=full_calls=0;seen=set();saves=[];prints=[];rng=random.Random(0x52454350)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    ports=[0x8cd10000+64*i for i in range(4)]
    def lookup(memory,regs,sr):
        check(regs[4] in [0,6,12,18],'original physical port alias');regs[0]=ports[regs[4]//6];return sr
    def save(memory,regs,sr):saves.append((text(regs[4]),read(regs[5],regs[6])));return sr
    def debug(memory,regs,sr):prints.append((regs[5],regs[6]));return sr
    hooks={table['njGetPeripheral']:lookup,table['host_save_file']:save,table['njPrintH']:debug}
    entries={table[n]:n for n in ['recording_update','get_character','stage_end','pad_scan','pad_read_sticks','atan2f','_in_atan2s']}
    def trace(pc,op,regs,sr,pr,fpu):
        if pc in entries:seen.add(entries[pc])
    def original(name):
        nonlocal calls,full_calls
        f=Fpu();f.fpscr=0x40001|((calls%2)<<21);s=State();s.r[15]=0x8cf00000;s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000
        reference(ram,s,hooks=hooks,fpu=f,vector_instruction=numeric,instruction_budget=40000,trace=trace)
        check(s.r[15]==0x8cf00000,'original caller stack restored');calls+=1
        if name=='pad_scan':full_calls+=1
    n={name:I.in_dll(lib,name) for name in ['recording_on','recording_mode','recording_kind','demo_next','demo_event']}
    n['recording_buttons']=U.in_dll(lib,'recording_buttons');nextp=P.in_dll(lib,'recording_next')
    arrays={name:(U*count).in_dll(lib,name) for name,count in [('pad_sticks',16),('pad_sticks_read',16),('pad_held',4),('pad_press_repeat',4),('pad_held_live',4),('pad_press_live',4)]}
    records=(Peripheral*8).in_dll(lib,'pad_records');per_info=(P*4).in_dll(lib,'per_info')
    samples=(Sample*4)();callback_errors=[]
    @Source
    def source(context,target):
        try:C.memmove(target,samples,C.sizeof(samples));return 1
        except BaseException as error:callback_errors.append(str(error));return 0
    initialize=bind(lib,'jp_input_initialize',I,P,U,P,P);poll=bind(lib,'jp_input_poll',I);record=bind(lib,'jp_input_record',P,U)
    shutdown=bind(lib,'jp_input_shutdown',I);bind_ports=bind(lib,'jp_pad_bind_test_ports',None);reset_pad=bind(lib,'jp_pad_test_reset',None)
    rec_init=bind(lib,'jp_recording_initialize',I,C.c_wchar_p);begin=bind(lib,'jp_recording_begin_record',I);load=bind(lib,'jp_recording_load_memory',I,P,C.c_size_t)
    status=bind(lib,'jp_recording_status',I);clear=bind(lib,'jp_recording_clear_error',None);base=bind(lib,'jp_recording_buffer',P)
    update=bind(lib,'recording_update',None);scan=bind(lib,'pad_scan',None);set_character=bind(lib,'set_character_number',None,C.c_int16)
    stage_end=bind(lib,'jp_recorder_stage_end_kind',I);reset_end=bind(lib,'jp_recorder_stage_reset_end',None)
    root=out/'記録';root.mkdir();check(rec_init(str(root))==0 and initialize(None,0,C.cast(source,P),None)==0,'real native owners initialized')
    storage=base();guest=0x0cbe0000
    def setglobal(name,v):n[name].value=v;word(table[name],v)
    def cursor(offset):nextp.value=storage+offset;word(table['recording_next'],guest+offset)
    def state_equal(label):
        for name,obj in n.items():check((obj.value&0xffffffff)==value(table[name]),(label,name))
        check(nextp.value-storage==((value(table['recording_next'])-guest)&0xffffffff),(label,'native-width cursor'))
        check(stage_end()==int.from_bytes(read(table['stage_end_kind'],2),'little'),(label,'actual original QUIT effect'))
    def physical():
        check(poll()==0 and not callback_errors,'real integrated sampling');bind_ports()
        for i in range(4):
            q=Peripheral.from_address(record(i));put(ports[i],bytes(q)[:36]+struct.pack('<IIII',int(bool(q.name)),int(bool(q.extend)),q.old,int(bool(q.info))))
            word(table['per_info']+4*i,ports[i])
    def fresh(mode=0,active=1):
        clear();check(begin()==0,'owned recording cleared');put(guest,bytes(1572888));cursor(0)
        for name,v in [('recording_on',active),('recording_mode',mode),('recording_kind',0),('demo_next',0),('demo_event',-1),('recording_buttons',0)]:setglobal(name,v)
        reset_end();put(table['stage_end_kind'],b'\0\0');reset_pad();C.memset(C.addressof(records),0,C.sizeof(records));put(table['pad_records'],bytes(8*52))
        put(table['pads_on'],b'\1');put(table['player_pads_on'],b'\1\1');put(table['quit_pressed'],b'\0');put(table['quit_held_scans'],b'\0\0')
        for name in ['pad_held','pad_press_repeat','pad_held_live','pad_press_live','pad_repeat_scans']:put(table[name],bytes(16))
        P.in_dll(lib,'camera_work').value=None;word(table['camera_work'],0);saves.clear();prints.clear()
    # Original early-return gates must not inspect even a deliberately invalid
    # cursor when disabled or when demonstrating an event.
    for active,event in [(0,-1),(1,0),(1,123)]:
        fresh(active=active);setglobal('demo_event',event);cursor(1);original('recording_update');update();state_equal('early gate');check(status()==0,'guard is after original early gates')
    fresh()
    for case in range(256):
        samples[0]=Sample(1,rng.getrandbits(18)&0x307fe,rng.randrange(-128,128),rng.randrange(-128,128),0,0,rng.randrange(256),rng.randrange(256));physical()
        original('recording_update');update();state_equal('original record copy');check(C.string_at(storage,(case+1)*12)==read(guest,(case+1)*12),'serialized original sample prefix')
    # All character branches, including original intentionally unsaved Tikal
    # and Eggman; the stop sample advances twice and retains its unused bytes.
    for character in range(8):
        fresh();set_character(character);put(table['character_current'],struct.pack('<H',character));samples[0]=Sample(1,4,12,-25,0,0,123,231);physical()
        original('recording_update');update();records[0].on=1024;word(table['pad_records']+8,1024)
        original('recording_update');update();state_equal('X stop');check(C.string_at(storage,24)==read(guest,24),'record stop bytes including sentinel padding')
        check(nextp.value-storage==36,'original stop double increment retained')
        if saves:
            check(len(saves)==1,'one original save');name,data=saves[0];check((root/name).read_bytes()==data,'genuine branch wrote original file bytes')
        else:check(character in [1,4],'only original unsaved character branches')
        original('recording_update');update();check(prints==[(guest+36,8)],'original debug pointer');state_equal('DONE stable')
    fresh();cursor(131040);samples[0]=Sample(1,4,127,-128,0,0,255,0);physical()
    original('recording_update');update();state_equal('last record data slot');original('recording_update');update();state_equal('capacity stop')
    check(nextp.value-storage==131076 and C.string_at(storage,131064)==read(guest,131064),'maximum record and sentinel/double increment')
    check((root/saves[-1][0]).read_bytes()==saves[-1][1] and len(saves[-1][1])==131064,'maximum original save bytes')
    # Full replay, original button history, physical Start preservation and
    # stage-end request execute without hooks for any game function.
    fresh(1);data=b''.join(struct.pack('<IHHhh',rng.randrange(1<<18),rng.randrange(256),rng.randrange(256),rng.randrange(-32768,32768),rng.randrange(-32768,32768)) for _ in range(256))+struct.pack('<III',0xffffffff,0,0)
    check(load(C.create_string_buffer(data),len(data))==0,'load actual playback bytes');put(guest,data)
    for case in range(257):
        samples[0]=Sample(1,(8 if case%2 else 4),12,-12,0,0,11,22);physical();original('recording_update');update();state_equal('whole original playback')
        check(bytes(Peripheral.from_address(record(0)))[:36]==read(ports[0],36),'original replay peripheral fields and button edges')
    check(n['recording_on'].value==0 and stage_end()==6,'sentinel requests real stored QUIT state')
    # Native safety repair: reject unloaded/misaligned/outside cursors before
    # touching samples. This deliberately has no unsafe machine-code oracle.
    for offset in [1,24,1572888]:
        fresh(1);data=struct.pack('<III',0xffffffff,0,0);check(load(C.create_string_buffer(data),12)==0,'load bounded case');cursor(offset);update()
        check(status()==6 and not n['recording_on'].value and stage_end()==6,'invalid playback fails closed with quit request')
    # Entire original pad_scan: real port compaction, disconnected records,
    # enable/player masks, eight player records, analog/repeat and quit edges.
    fresh(active=0);enable=bind(lib,'pad_enable',None);disable=bind(lib,'pad_disable',None)
    enable_player=bind(lib,'pad_enable_player',None,C.c_uint8);disable_player=bind(lib,'pad_disable_player',None,C.c_uint8)
    for case in range(256):
        for i in range(4):samples[i]=Sample(int(bool(case&(1<<i))),rng.randrange(1<<11),rng.randrange(-128,128),rng.randrange(-128,128),0,0,rng.randrange(256),rng.randrange(256))
        physical()
        if case%5==0:disable();put(table['pads_on'],b'\0')
        else:enable();put(table['pads_on'],b'\1')
        enable_player(255)
        bits=[int(case%3!=0),int(case%7!=0)]
        for i,bit in enumerate(bits):
            if not bit:disable_player(i)
        put(table['player_pads_on'],bytes(bits));original('pad_scan');scan();state_equal('full scan')
        for i,q in enumerate(records):
            old=read(table['pad_records']+i*52,52)
            check(bytes(q)[:36]==old[:36] and q.old==struct.unpack_from('<I',old,44)[0],('all original player scalar records',case,i))
        for name,array in arrays.items():check(bytes(array)==read(table[name],C.sizeof(array)),('whole original input array',case,name))
        for i,addr in enumerate(per_info):check(bytes(Peripheral.from_address(addr))[:36]==read(value(table['per_info']+4*i),36),('original peripheral compaction',case,i))
    # Complete scan with recorder enabled verifies the original ordering: X
    # ends recording from the previous game scan rather than this raw poll.
    fresh();set_character(0);put(table['character_current'],b'\0\0')
    for case in range(4):
        samples[0]=Sample(1,1024 if case>=1 else 4,0,0,0,0,123,234)
        for i in range(1,4):samples[i]=Sample()
        physical();original('pad_scan');scan();state_equal('full scan recording order')
        check(C.string_at(storage,36)==read(guest,36),'whole-scan recorded bytes')
        check(n['recording_mode'].value==(0 if case<2 else 2),'X stop delayed until previous scanned record')
    check((root/'K_SONIC.BIN').read_bytes()==saves[0][1],'full scan actual save matches original')
    fresh(1);data=b''.join(struct.pack('<IHHhh',rng.randrange(1<<11),rng.randrange(256),rng.randrange(256),rng.randrange(-128,128),rng.randrange(-128,128)) for _ in range(128))+struct.pack('<III',0xffffffff,0,0)
    check(load(C.create_string_buffer(data),len(data))==0,'whole-scan playback load');put(guest,data)
    for case in range(129):
        samples[0]=Sample(0,0,0,0,0,0,0,0)
        samples[1]=Sample(1,8 if case%2 else 4,0,0,0,0,11,22)
        samples[2]=samples[3]=Sample()
        physical();original('pad_scan');scan();state_equal('full scan compacted playback')
        for i,q in enumerate(records):check(bytes(q)[:36]==read(table['pad_records']+i*52,36),('playback copied into game records',case,i))
        for name,array in arrays.items():check(bytes(array)==read(table[name],C.sizeof(array)),('full playback analog/button array',case,name))
    check(stage_end()==6 and not n['recording_on'].value,'whole-scan playback terminates with original quit effect')
    required={'recording_update','get_character','stage_end','pad_scan','pad_read_sticks','atan2f','_in_atan2s'};check(required<=seen,'original dependency entries executed')
    check(shutdown()==0 and bind(lib,'jp_recording_shutdown',I)()==0,'both native owners drained')
    report=dict(status='genuine_recorder_and_complete_pad_scan_verified_against_original_Japanese_execution',checks=checks,original_calls=calls,complete_original_pad_scan_calls=full_calls,original_dependency_entries_seen=sorted(seen),original_game_dependency_hooks_used=False,original_platform_hooks=['njGetPeripheral','host_save_file','njPrintH'],actual_native_saves_compared=True,original_stage_QUIT_effect_compared=True,native_loaded_extent_safety_repair_verified=True,original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),frame_wait_input_trace_replaced=False,physical_keyboard_or_controller_events_verified=False,complete_stage_engine_executed=False,original_logo_step_executed=False,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
