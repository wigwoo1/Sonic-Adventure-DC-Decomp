"""Original RTC instructions, real chain delivery, failures and OS-clock reads."""
import argparse,ctypes as C,datetime as D,hashlib,json,random,re,struct,sys,threading,time
from pathlib import Path
from native_packet_test_support import bind
from native_clock_test_support import Clock,Counter
from verify_native_vblank_chain import Record,Snapshot,Callback,Idle
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p
Reader=C.CFUNCTYPE(I,P,C.POINTER(U))
class Date(C.Structure):
    _fields_=[('year',C.c_uint16)]+[(n,C.c_uint8) for n in ('month','day','hour','minute','second','dayofweek','ageofmoon')]
class CardTime(C.Structure):_fields_=Date._fields_[:-1]
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('reads','ticks','updates','failures')]+[('offset',C.c_int64)]+[(n,U) for n in ('count','mode','chain','initialized','status','read_error')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fields(date):return tuple(getattr(date,n) for n,_ in date._fields_)

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['original_source_sha256'].items():assert sha(u/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    for key,row in b['copied_DLLs'].items():assert sha(row['path'])==row['sha256'],key
    assert sha(b['RTC_DLL'])==b['RTC_DLL_sha256'] and sha(b['client_DLL'])==b['client_DLL_sha256']
    C.CDLL(b['copied_DLLs']['frames_DLL']['path']);C.CDLL(b['copied_DLLs']['GPU_DLL']['path'])
    clock=Clock(Path(b['copied_DLLs']['present_DLL']['path']));chain=clock.lib
    lib=C.CDLL(b['RTC_DLL']);client=C.CDLL(b['client_DLL'])
    configure=bind(lib,'jp_rtc_configure',I,P,P);init=bind(lib,'syRtcInit',I);close=bind(lib,'jp_rtc_shutdown',I);status=bind(lib,'jp_rtc_status',I);snapshot=bind(lib,'jp_rtc_snapshot',I,P)
    get=bind(client,'client_get_date',I,P);setdate=bind(client,'client_set_date',I,P);cardtime=bind(client,'client_card_time',None,P)
    mode=bind(lib,'syRtcSetServerMode',None,I);getmode=bind(lib,'syRtcGetStat',I);server=bind(lib,'syRtcExecServer',None)
    todate=bind(lib,'syRtcCountToDate',None,U,P);tocount=bind(lib,'syRtcDateToCount',None,P,P);compare=bind(lib,'syRtcCompareDate',I,P,P)
    wi=bind(chain,'jp_wait_initialize',I,U,U,U,P,P);wc=bind(chain,'jp_wait_shutdown',I);pump=bind(chain,'jp_wait_pump',I,P);ws=bind(chain,'jp_wait_snapshot',I,P)
    add=bind(chain,'syChainAddHandler',U,C.c_int16,P,U,P);delete=bind(chain,'syChainDeleteHandler',None,U);count=bind(chain,'jp_vblank_chain_count',U);describe=bind(chain,'jp_vblank_chain_describe',I,U,P)
    checks=0;groups={};counter=Counter();callback_errors=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def group(name):groups[name]=checks-sum(groups.values())
    def info():
        out=Info();check(snapshot(C.byref(out))==0,'RTC state snapshot');return out
    def native_date(count):out=Date();todate(count,C.byref(out));return out
    check(bind(client,'client_date_size',U)()==C.sizeof(Date)==10 and bind(client,'client_time_size',U)()==C.sizeof(CardTime)==8,'actual original SDK caller agrees on date and card-time ABI')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,reference_hashes=make_reference(executor)
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();check(sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf','preserved Japanese original image hash');ram[0x10000:0x10000+len(binary)]=binary
    calls={};entered={};names=list(b['original_function_sha256'])+['card_get_time'];entries={table[n]:n for n in names}
    def put(at,value):at&=0xffffff;ram[at:at+len(value)]=value
    def read(at,n):at&=0xffffff;return bytes(ram[at:at+n])
    def trace(pc,*unused):
        if pc in entries:entered[entries[pc]]=entered.get(entries[pc],0)+1
    def original(name,*args):
        cpu=executor.State();cpu.pc=table[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
        for i,x in enumerate(args):cpu.r[4+i]=x&0xffffffff
        fpu=Fpu();fpu.fpscr=0x40001;reference(ram,cpu,fpu=fpu,trace=trace,instruction_budget=200000)
        calls[name]=calls.get(name,0)+1;check(cpu.r[15]==0x8cf00000,'complete original instruction call preserves its stack');return cpu.r[0]
    epoch=D.datetime(1950,1,1);rng=random.Random(0x525443)
    counts=[0,1,59,60,86399,86400,0x5cb417d1,0xffffffff]+[rng.randrange(1<<32) for _ in range(96)]
    for year in (1952,1998,2000,2026,2084):
        for month,day in ((1,1),(2,28),(3,1),(12,31)):
            counts.append(int((D.datetime(year,month,day)-epoch).total_seconds()))
    for value in counts:
        at=0x8cf10000;put(at,b'\0'*10+b'\xa5'*6);original('syRtcCountToDate',value,at);out=native_date(value)
        check(bytes(out)[:9]==read(at,9) and read(at+10,6)==b'\xa5'*6,'native complete date/week/moon matches original instructions without hooks')
        expected=epoch+D.timedelta(seconds=value);phase=value-0x5cb417d1
        moon=(phase%0x26ee7f if phase>=0 else 0x26ee7f-((-phase)%0x26ee7f))//86400
        check(fields(out)==(expected.year,expected.month,expected.day,expected.hour,expected.minute,expected.second,(expected.weekday()+1)%7,moon),'independent civil calendar and original moon formula')
        x=U(123);tocount(C.byref(out),C.byref(x));original('syRtcDateToCount',at,at+32)
        check(x.value==value==struct.unpack('<I',read(at+32,4))[0] and status()==0,'bounded native conversion round trips across complete original count range')
        other=native_date((value+7919)&0xffffffff);put(at+64,bytes(other));got=compare(C.byref(out),C.byref(other));old=C.c_int32(original('syRtcCompareDate',at,at+64)).value
        check(got==old==((value> ((value+7919)&0xffffffff))-(value< ((value+7919)&0xffffffff))),'original complete date ordering')
    group('unhooked_original_calendar_and_independent_datetime')
    untouched=U(0x89abcdef)
    for date in [Date(1949,12,31),Date(2087,1,1),Date(2026,0,1),Date(2026,13,1),Date(2026,2,29),Date(2000,2,30),Date(2026,1,0),Date(2026,1,32),Date(2026,1,1,24),Date(2026,1,1,0,60),Date(2026,1,1,0,0,60),Date(2086,12,31)]:
        tocount(C.byref(date),C.byref(untouched));check(status()==8 and untouched.value==0x89abcdef,'invalid/out-of-range native input preserves output')
    date=Date(2000,1,1);before=bytes(date);tocount(C.byref(date),C.byref(date));check(status()==1 and bytes(date)==before,'overlapping date/count output rejects without destroying input')
    tocount(None,C.byref(untouched));check(status()==1 and untouched.value==0x89abcdef,'null calendar input preserves output')
    todate(0,None);check(status()==1,'null date output rejects')
    sentinel=Date();C.memset(C.byref(sentinel),0xa5,C.sizeof(sentinel));before=bytes(sentinel)
    check(get(C.byref(sentinel))==-1 and status()==2 and bytes(sentinel)==before,'uninitialized original-SDK date call preserves output')
    group('native_range_null_and_output_preservation')
    script=[];raw_value=0x90000000;error=0;continuous=False;reenter=False;recursive=[];reader_calls=0
    @Reader
    def reader(context,out):
        nonlocal reader_calls,raw_value
        reader_calls+=1
        if reenter:recursive.extend([close(),configure(None,None)])
        entry=script.pop(0) if script else (error,raw_value)
        rc,value=entry
        if not rc:out[0]=value
        if continuous:raw_value+=1
        return rc
    @Idle
    def idle(context,ms):return 1
    def start(rate=60,real=False):
        check(wc()==0,'previous wait owner released');counter.tick=0
        check((clock.init(None,None,0) if real else counter.start(clock,781250))==0,'actual native clock initialized')
        check(wi(rate,1,4096,C.cast(idle,P),None)==0,'actual original-chain wait owner initialized')
    def advance():
        s=Snapshot();check(ws(C.byref(s))==0,'rational wait deadline');counter.tick=s.next_deadline;done=U(99)
        check(pump(C.byref(done))==0 and done.value==1,'real original chain dispatched one tick')
    check(configure(C.cast(reader,P),None)==0 and init()==-1 and status()==5 and info().initialized==0,'missing real callback owner cannot become successful initialization')
    start();events=[]
    @Callback
    def observe(arg):
        try:
            date=Date();assert get(C.byref(date))==0;events.append((int(arg or 0),fields(date)))
        except BaseException as exc:callback_errors.append(repr(exc))
    handles=[add(0x900,observe,88,None) for _ in range(255)];check(all(handles) and init()==-1 and status()==5 and count()==255 and not info().initialized,'full genuine callback pool fails without orphaning RTC state')
    for h in handles:delete(h)
    check(init()==0 and count()==1,'original-SDK init installs one real RTC callback')
    v=info();record=Record();check(describe(v.chain,C.byref(record))==0 and record.priority==0x8000005c and record.callback>0xffffffff and not record.argument,'native pointer-width typed RTC callback retains original priority and active-record flag')
    check(init()==-1 and status()==10 and count()==1,'duplicate init cannot leak another handler')
    prior=v.count;raw_value+=1;before_h=add(0x900,observe,88,1);after_h=add(0x900,observe,96,2);advance()
    check([x[0] for x in events]==[1,2] and events[0][1]==fields(native_date(prior)) and events[1][1]==fields(native_date(raw_value)) and not callback_errors,'actual chain order gives game priority 88 old cache and priority 96 refreshed cache')
    delete(before_h);delete(after_h);check(info().ticks==1 and info().count==raw_value,'typed callback really executed the original server on the actual logical tick')
    group('real_chain_registration_capacity_priority_and_callback')
    date=Date();read_before=info().reads;raw_value+=17;check(get(C.byref(date))==0 and fields(date)==fields(native_date(prior+1)) and info().reads==read_before,'ordinary original GetDate reads the cached count')
    mode(1);script[:]=[(0,raw_value-1)]+[(0,raw_value)]*4
    check(get(C.byref(date))==1 and fields(date)==fields(native_date(raw_value)) and status()==0 and not script,'bounded direct read refreshes baseline after a second ticks over')
    script[:]=[(-37,0)]+[(0,raw_value)]*4;check(get(C.byref(date))==1 and status()==0 and not script,'bounded force recovers from a transient failed reading once four new readings agree')
    continuous=True;old=bytes(date);n=reader_calls
    check(get(C.byref(date))==-1 and status()==7 and reader_calls-n==16 and bytes(date)==old,'clock that never stabilizes returns after exactly sixteen reads without changing output')
    continuous=False;error=-37;n=reader_calls
    check(get(C.byref(date))==-1 and status()==6 and reader_calls-n==16 and bytes(date)==old and info().read_error==U(-37).value,'persistent provider failure is bounded and exposes the actual provider error')
    error=0;check(get(C.byref(date))==1 and status()==0,'subsequent reliable read recovers normally')
    mode(0);cached=info().count;script[:]=[(0,raw_value+1),(0,raw_value+2)];server();check(info().count==cached and status()==0,'unchanged original two-reading filter holds previous count when reads disagree')
    script[:]=[(-5,0),(0,raw_value+1)];server();check(info().count==cached and status()==6,'failed cached refresh preserves prior count and reports failure')
    mode(2);raw_value+=3;server();check(status()==0 and info().count==raw_value,'original full-read server mode refreshes real state')
    mode(3);raw_value+=1;server();check(get(C.byref(date))==3 and fields(date)==fields(native_date(raw_value)),'combined direct/full mode preserves original selection')
    mode(8);check(status()==9 and getmode()==3,'console-address logging mode explicitly unsupported and retains prior mode')
    mode(0);foreign=[]
    def other():foreign.extend([close(),configure(None,None),get(C.byref(sentinel)),status()])
    thread=threading.Thread(target=other);thread.start();thread.join();check(foreign==[3,3,-1,3] and count()==1 and bytes(sentinel)==before,'foreign thread cannot access, close or overwrite output of RTC owner')
    reenter=True;mode(1);check(get(C.byref(date))==1 and recursive and set(recursive)=={4} and count()==1,'provider cannot recursively close/reconfigure its active owner');reenter=False
    group('original_modes_filter_bounded_force_and_owner_errors')
    mode(0);target=Date(2000,2,29,12,34,56);check(setdate(C.byref(target))==0,'actual SDK setter applies a virtual clock offset')
    out=Date();check(get(C.byref(out))==0 and fields(out)[:6]==fields(target)[:6] and info().offset!=0,'setter changes game-visible date rather than pretending to write hardware')
    offset=info().offset;raw_value+=1;server();check(info().count==int((D.datetime(2000,2,29,12,34,57)-epoch).total_seconds()) and info().offset==offset,'virtual date continues ticking with its underlying host source')
    error=-55;old=info().count;old_offset=info().offset;check(setdate(C.byref(Date(2001,1,1)))==-1 and info().count==old and info().offset==old_offset,'failed setter preserves existing date and offset');error=0
    # Original GetDate and card_get_time execute without an emulated hardware
    # hook: their original cached mode reads the original global count directly.
    for value in counts[::7]:
        raw_value=value;check(close()==0 and configure(C.cast(reader,P),None)==0 and init()==0,'fresh original-compatible clock session for real card timestamp caller')
        put(table['sygRtcCount'],struct.pack('<I',value));put(table['sygRtcStat'],struct.pack('<I',0));at=0x8cf10100;put(at,b'\xa5'*16)
        original('card_get_time',at);out=CardTime();cardtime(C.byref(out))
        check(bytes(out)==read(at,8) and read(at+8,8)==b'\xa5'*8,'unchanged native original card timestamp caller matches complete original GetDate/card instruction path')
    check(close()==0 and count()==0 and info().offset==0,'RTC releases actual chain handler and resets session offset before wait teardown')
    group('virtual_set_date_and_original_card_timestamp_consumer')
    raw_base=0x90000000
    @Reader
    def elapsed(context,out):out[0]=raw_base+counter.tick//781250;return 0
    for rate in (60,120,240):
        start(rate);check(configure(C.cast(elapsed,P),None)==0 and init()==0,'RTC initialized against one elapsed-time source')
        clock.write(0)
        for _ in range(rate*2):advance()
        check(info().count==raw_base+2 and info().ticks==rate*2 and clock.timer()==0,'RTC elapsed time remains two seconds at every logical rate while paused game timer stays zero')
        check(close()==0 and count()==0,'rate test removes RTC from original chain')
    group('clock_domain_and_cadence_independence')
    start(real=True);check(configure(None,None)==0 and init()==0,'default uses actual Windows local clock and actual QPC wait owner')
    before_os=D.datetime.now();out=Date();check(get(C.byref(out))==0,'real OS date reaches original-SDK caller');after_os=D.datetime.now()
    actual=D.datetime(*fields(out)[:6]);check(before_os-D.timedelta(seconds=2)<=actual<=after_os+D.timedelta(seconds=2),'game-visible native date agrees with independently read PC civil time')
    card=CardTime();cardtime(C.byref(card));check(fields(card)==fields(out)[:7],'real original card timestamp caller receives actual PC date')
    time.sleep(.05);done=U();check(pump(C.byref(done))==0 and done.value>=1 and info().ticks>=1,'real QPC-driven callbacks execute RTC server without synthetic time advancement')
    before_os=D.datetime.now();check(setdate(C.byref(Date(2000,1,1)))==0,'real-source virtual setter works');after_os=D.datetime.now()
    check(abs((after_os-before_os).total_seconds())<5 and after_os.year!=2000,'game RTC setter does not modify the Windows clock')
    check(close()==0 and count()==0 and wc()==0,'real RTC shuts down before actual wait owner');clock.shutdown();group('actual_PC_clock_SDK_caller_and_shutdown')
    for n,h in b['original_source_sha256'].items():assert sha(u/n)==h,n
    result=dict(status='original_RTC_calendar_native_PC_clock_chain_and_card_timestamp_verified',checks=checks,groups=groups,
        complete_original_instruction_calls=sum(calls.values()),original_calls=calls,original_entries=entered,original_function_hooks_used=False,
        reference_hashes=reference_hashes,actual_PC_clock_date=actual.isoformat(),actual_PC_QPC_callback_ticks=done.value,
        actual_logical_test_rates=[60,120,240],SDK_date_bytes=C.sizeof(Date),SDK_card_time_bytes=C.sizeof(CardTime),
        OS_clock_modified=False,original_source_preserved=True,full_sbInitSystem_general_exit_audio_or_gameplay_verified=False,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',sum(calls.values()),'original calls',flush=True)
if __name__=='__main__':main()
