"""Actual full bootstrap, card/font claims and ordered global/native exit.
Fresh VMU images only; genuine SDK files/fibers/driver and original callbacks.
"""
import argparse,ctypes as C,faulthandler,hashlib,json,struct,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_aica import Options as AicaOptions
from verify_native_sound_clock import Options as ClockOptions
from verify_native_input_backend import Source,Sample
from verify_native_card_lifecycle import Card
from verify_native_vmu_fs import FiberInfo,Time
from verify_native_exit_logo import ExitInfo
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
CORE,SOUND,CARD,PRINT=245762,524288,1048576,2097152
PARENT=CORE|SOUND|CARD|PRINT;FINAL=PARENT|64|128|512|1024
class Life(C.Structure):
    _fields_=[(n,Q) for n in ('next_token','token','identity','generation')]+[(n,U) for n in ('terminal','closing','finished','status','creator_thread','native_error')]
CASES=('local_restart','early','final','missing_mask','late_claim_rejection','pending_save','close_timeout','denied_write','sticky_print','reentrant_card','reentrant_print','foreign','retained_frame','process_exit','process_failure')
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','build','track','output'):ap.add_argument('--'+name,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for name,h in b['authored_source_sha256'].items():assert sha(r/name)==h,name
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for key in ('workspace','build','track'):cmd+=['--'+key,str(getattr(a,key).resolve())]
            cmd+=['--case',name,'--output',str(d/name)];run=subprocess.run(cmd,capture_output=True,timeout=120,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr);expected=70 if name=='process_failure' else 0
            assert run.returncode==expected,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6000:])
            cases[name]=read(d/name/('progress.json' if name.startswith('process_') else 'result.json'));cases[name]['actual_exit_code']=run.returncode
            print(name,cases[name]['checks'],'checks;',run.returncode,'exit',flush=True)
        result=dict(status='actual_boot_card_font_System_early_final_exit_and_native_claim_lifetimes_scoped_verification',checks=sum(row['checks'] for row in cases.values()),cases=cases,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),borrowed_VMU_media_and_LCD_remain_caller_owned=True,full_gameplay_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True)
    f=C.CDLL(b['frames_DLL']);system=C.CDLL(b['System_DLL']);ex=C.CDLL(b['authored_exit_DLL']);boot=C.CDLL(b['main_boot_DLL']);sound=C.CDLL(b['game_sound_DLL']);labels=[];obs={};keep=[]
    is_fiber=bind(C.WinDLL('kernel32'),'IsThreadAFiber',I);fiber_before=bool(is_fiber())
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label);(d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels),indent=2)+'\n')
    def snap(lib,name,kind,*args):
        out=kind();check(bind(lib,name,I,*([U]*len(args)),P)(*args,C.byref(out))==0,'actual '+name);return out
    def life(kind):return snap(f,'jp_boot_lifetime_snapshot',Life,kind)
    def sysinfo():return snap(system,'jp_system_snapshot',SystemInfo)
    def clean():return bind(ex,'jp_exit_cleanup',I)()
    def settle():
        for _ in range(10000):
            bind(f,'buExecServer',None)()
            if bind(f,'buGetLastError',I,I)(0)!=-1:return
        raise AssertionError('actual original card fibers did not settle')
    @Source
    def source(ctx,samples):C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
    keep.append(source)
    for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
        check(bind(f,name,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'borrowed prerequisite '+name)
    buffer=C.addressof((C.c_ubyte*0x40000).in_dll(f,'texture_buffer'));check(bind(f,'jp_render_state_register_buffer',I,P,S,U)(buffer,0x40000,4096)==0,'registered actual main buffer')
    media=d/'media';media.mkdir();check(bind(f,'jp_vmu_store_initialize',I,C.c_wchar_p)(str(media))==0,'borrowed actual media owner')
    check(bind(f,'jp_vmu_store_create',I,U)(0)==0 and bind(f,'jp_vmu_store_open',I,U)(0)==0 and bind(f,'jp_lcd_initialize',I,U)(1)==0,'fresh real media image and borrowed LCD')
    path=C.create_unicode_buffer(str(a.track.resolve()));keep.append(path)
    options=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(path),2352,3,5000,0,C.cast(source,P).value,None,None,None)
    sound_options=SoundOptions(56,0,2<<20,5000,AicaOptions(32,1,16384,40,5000,1,1,0),None);clock=ClockOptions(56,441,8192,5,None,None,0,None,None)
    def start():
        check(bind(system,'jp_system_configure',I,P)(C.byref(options))==0,'actual core configuration')
        check(bind(boot,'jp_main_boot_configure',I,P,P)(C.byref(sound_options),C.byref(clock))==0,'actual parent configuration')
        check(bind(boot,'jp_main_boot_initialize',I)()==0,'complete original successful parent order')
        check(sysinfo().resources==PARENT,'one existing System registration owns core sound card and font')
    start();claims=[life(0),life(1)];tokens=[v.token for v in claims]
    check(all(v.token and v.identity and not v.terminal and not v.closing for v in claims),'actual live providers claimed exactly once')
    check(bind(f,'buExit',I)()==-1,'legacy independent card destruction rejected while System owns it')
    font_before=bind(f,'jp_textures_live_surfaces',U)();bind(f,'njExitPrint',None)();check(bind(f,'jp_textures_live_surfaces',U)()==font_before==1,'legacy independent font destruction rejected')
    for kind in range(2):
        out=Q(0x1234);check(bind(f,'jp_boot_lifetime_claim',I,U,P)(kind,C.byref(out))==2 and out.value==0x1234,'second owner cannot acquire actual claimed provider')
        check(bind(f,'jp_boot_lifetime_cancel',I,U,Q)(kind,tokens[kind]+1)==2,'wrong cancellation authority rejected')
    if a.case=='foreign':
        results=[]
        def other():
            for kind in range(2):results.extend((bind(f,'jp_boot_lifetime_close',I,U,Q,U)(kind,tokens[kind],0),bind(f,'jp_boot_lifetime_snapshot',I,U,P)(kind,C.byref(Life()))))
        thread=threading.Thread(target=other);thread.start();thread.join();check(results==[3]*4,'foreign-thread lifetime operations preserve real state')
        check([bytes(life(k)) for k in range(2)]==[bytes(v) for v in claims],'foreign attempts leave claim snapshots unchanged')
    if a.case=='local_restart':
        check(bind(boot,'jp_main_boot_shutdown',I,U)(5000)==0,'ordinary parent cleanup delegates real claims to System')
        check(sysinfo().resources==0 and not life(0).token and not life(1).token,'nonterminal close releases claims for restart')
        check(bind(f,'jp_queue_reset_frame',I)()==0,'empty queue for second constructor');start()
        check(life(0).token>tokens[0] and life(1).token>tokens[1],'restarted real resources use nonrecycled lifetime tokens')
        tokens=[life(k).token for k in range(2)]
    save_cases=('pending_save','close_timeout','reentrant_card','denied_write')
    if a.case in save_cases:
        settle();date=Time(2026,11,6,12,0,0,5);volume=C.create_string_buffer(b'BOOT EXIT QUALIFICATION'.ljust(32,b'\0'),32);keep.extend([date,volume])
        check(bind(f,'buFormatDisk',I,I,P,I,P,U)(0,volume,0,C.byref(date),0)==0,'real original card format queued');settle()
        check(bind(f,'buGetLastError',I,I)(0)==0,'actual fresh card formatted through original filesystem')
        payload=bytes((i*37+11)&255 for i in range(5120));data=C.create_string_buffer(payload);keep.append(data);events=[]
        Complete=C.CFUNCTYPE(I,I,I,I,U)
        original_complete=bind(f,'card_complete',I,I,I,I,U)
        @Complete
        def completed(drive,op,status,param):
            if a.case=='reentrant_card':events.append((clean(),life(0).terminal,life(1).terminal))
            if a.case=='denied_write':events.append(status)
            return original_complete(drive,op,status,param)
        if a.case in ('reentrant_card','denied_write'):keep.append(completed);bind(f,'buSetCompleteCallback',None,Complete)(completed)
        blocker=None
        if a.case=='denied_write':
            prior_image=sha(media/'VMU_A1.VMU');kernel=C.WinDLL('kernel32',use_last_error=True)
            blocker=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P)(str(media/'VMU_A1.VMU'),0x80000000,1,None,3,0x80,None)
            check(blocker and blocker!=P(-1).value,'real Windows handle denies atomic image replacement while allowing reads')
        check(bind(f,'buSaveFile',I,I,C.c_char_p,P,U,P,U)(0,b'BOOTTEST',data,10,C.byref(date),0)==0,'real ten-block save queued with borrowed data kept alive')
        check(bind(f,'buGetLastError',I,I)(0)==-1,'queued save is genuinely pending')
        if a.case=='close_timeout':
            check(bind(f,'jp_boot_lifetime_close',I,U,Q,U)(0,tokens[0],0)==6,'bounded real card close times out before pending work finishes')
            check(sysinfo().resources==PARENT and life(0).token==tokens[0] and life(0).closing and not life(0).finished,'timeout retains actual claim and all core resources for retry')
        elif a.case=='reentrant_card':
            settle();check(events and all(v==(4,0,0) for v in events),'actual original save completion cannot commit nested global exit')
    if a.case=='reentrant_print':
        packet=bind(f,'jp_print_open',P,U)(0);check(packet,'actual print packet allocation active')
        check(clean()==4 and not life(0).terminal and not life(1).terminal,'active original print transport rejects exit before terminal mutation')
        bind(f,'jp_print_close',None,P,P)(packet,(U*4)(0x80000000,0,0,claims[1].identity));bind(f,'jp_print_clear_error',None)()
        check(bind(f,'jp_boot_lifetime_can_close',I,U,Q)(1,tokens[1])==0,'real print packet retired')
        check(bind(f,'jp_queue_reset_frame',I)()==0,'producer discards empty diagnostic recording before final bind')
    captured=None
    if a.case=='retained_frame':
        bind(f,'njPrintC',None,U,C.c_char_p)(0x00020002,b'OWNED EXIT TEST');captured=Q();check(bind(f,'jp_frames_submit',I,P)(C.byref(captured))==0,'actual original font captured')
        count=S();surface=bind(f,'jp_frame_surface',P,Q,U,P)(captured.value,claims[1].identity,C.byref(count));check(surface and count.value==32768,'captured real font payload retained')
        before=hashlib.sha256(C.string_at(surface,count.value)).hexdigest()
    if a.case in ('sticky_print','process_failure'):
        check(bind(f,'jp_texture_sdk_limits',I,U,S)(1024,1)==0,'actual SDK table budget constrained')
        table=C.addressof((C.c_ubyte*2048).in_dll(f,'texmemlist'));bind(f,'njInitTexture',None,P,U)(table,2048)
        check(bind(f,'jp_textures_status',I)()!=0,'genuine SDK replacement reports its actual budget failure')
    if a.case in ('final','missing_mask','late_claim_rejection','sticky_print','reentrant_card','reentrant_print','process_failure','process_exit'):
        if a.case=='missing_mask':
            check(bind(system,'jp_system_bind_exit',I,U)(CORE|SOUND)==11,'final mask omitting actual card/font claims rejected')
            check(sysinfo().resources==PARENT and all(not life(k).terminal for k in range(2)),'rejected mask preserves existing bootstrap authority')
        if a.case=='late_claim_rejection':
            check(bind(f,'jp_queue_reserve',I,U,U)(0,4)==0 and bind(f,'njStartVertexNonTexOpaque',P)(),'actual open recording makes later device claim reject')
            check(bind(system,'jp_system_bind_exit',I,U)(FINAL)==11,'actual later claim rejection rolls back only new final claims')
            check(sysinfo().resources==PARENT and [life(k).token for k in range(2)]==tokens,'existing card/font/bootstrap claims survive rejected final bind')
            check(bind(f,'jp_queue_discard_active',I)()==0 and bind(f,'jp_queue_reset_frame',I)()==0,'original producer retires actual conflicting recording')
        check(bind(system,'jp_system_bind_exit',I,U)(FINAL)==0,'complete final handoff adopts same actual tokens')
    if a.case.startswith('process_'):
        bind(ex,'syBtExit',None)();raise AssertionError('actual terminal process exit returned')
    if a.case=='sticky_print':
        check(clean()==6,'real sticky font-release failure reported by global Exit')
        state=snap(ex,'jp_exit_snapshot',ExitInfo);check(state.remaining&PRINT and state.failed_resource==PRINT,'global Exit retains exact failed print resource')
        check(life(1).token==tokens[1] and life(1).terminal and life(1).closing and not life(1).finished,'failed terminal print cleanup retains irreversible authority')
        check(bind(f,'jp_texture_sdk_limits',I,U,S)(1024,32<<20)==0,'caller corrects actual budget');bind(f,'jp_textures_clear_error',None)()
    if captured:
        check(clean()==6,'borrowed live capture blocks actual early device retirement')
        check(sysinfo().resources&PRINT and not life(1).finished,'blocked early exit retains actual font obligation')
        count=S();surface=bind(f,'jp_frame_surface',P,Q,U,P)(captured.value,claims[1].identity,C.byref(count));check(surface and hashlib.sha256(C.string_at(surface,count.value)).hexdigest()==before,'borrowed capture remains valid while early device exit waits for its owner')
        check(bind(f,'jp_frame_release',I,Q)(captured.value)==0,'caller releases actual retained capture before retry');captured=None
    if a.case=='denied_write':
        code=clean();obs['initial_denied_write_exit_result']=code
        check(code in (0,6),'real write denial either completes or reaches the bounded global deadline')
        if code:
            check(sysinfo().resources==PARENT and life(0).token==tokens[0] and life(0).terminal and not life(0).finished,'denied-write timeout retains card and dependent core resources')
            # The original SDK retries writes and then scans other free blocks.
            # Keep the real denial in place until its original completion runs;
            # never turn a timed-out save into synthetic success.
            settle()
    check(clean()==0,'actual early or final global exit completes ordered resource retirement')
    check(sysinfo().phase==6 and sysinfo().resources==0,'System observes actual completed terminal exit')
    check(all(life(k).terminal and life(k).finished and life(k).token==tokens[k] for k in range(2)),'real card/font terminal claims finish exactly once')
    check(snap(f,'jp_rf_snapshot',FiberInfo).servers==0 and snap(f,'jp_rf_snapshot',FiberInfo).fibers==0 and bool(is_fiber())==fiber_before,'actual card fibers and Windows fiber conversion retired')
    check(bind(f,'jp_textures_live_surfaces',U)()==0,'actual font surface retired')
    for kind in range(2):check(bind(f,'jp_boot_lifetime_cancel',I,U,Q)(kind,tokens[kind])==2,'terminal authority cannot be cancelled for restart')
    if a.case=='denied_write':
        check(events and events[-1]<0,'actual original SDK completion reports denied save instead of false success')
        card=Card();check(bind(f,'jp_card_state_read',I,U,P)(0,C.byref(card))==0 and card.error==events[-1],'genuine original game callback retains actual write failure')
        check(sha(media/'VMU_A1.VMU')==prior_image,'real denied replacement preserves the complete previous VMU image')
        check(bind(kernel,'CloseHandle',I,P)(blocker)!=0,'caller releases real Windows denial handle')
        bind(f,'jp_vmu_store_clear_error',None)()
    elif a.case in save_cases:
        image=(media/'VMU_A1.VMU').read_bytes();fat=struct.unpack_from('<256H',image,254*512);found=False
        for block in range(253,240,-1):
            for offset in range(block*512,(block+1)*512,32):
                entry=image[offset:offset+32]
                if entry[0] and entry[4:16].rstrip(b'\0')==b'BOOTTEST':
                    at=struct.unpack_from('<H',entry,2)[0];n=struct.unpack_from('<H',entry,24)[0];check(n==10,'real directory contains exact saved block count');out=bytearray();seen=set()
                    for _ in range(n):check(at<200 and at not in seen,'independent actual FAT chain bounded');seen.add(at);out+=image[at*512:(at+1)*512];at=fat[at]
                    check(at==0xfffa and bytes(out)==payload,'full original ten-block save drained before exit, exact FAT and payload');found=True
        check(found,'actual saved file found in real VMU image')
    check(bind(f,'jp_vmu_store_status',I)()==0,'borrowed VMU media remains caller-owned after terminal exit')
    check(bind(f,'jp_lcd_shutdown',I)()==0 and bind(f,'jp_vmu_store_shutdown',I)()==0,'caller explicitly closes borrowed LCD and VMU media')
    check(bind(f,'jp_vmu_store_initialize',I,C.c_wchar_p)(str(media))==0 and bind(f,'jp_vmu_store_open',I,U)(0)==0,'actual image lock can be reacquired after caller release');check(bind(f,'jp_vmu_store_shutdown',I)()==0,'close reacquired image')
    # Early bootstrap exits leave these render prerequisites caller-owned.
    if a.case not in ('final','missing_mask','late_claim_rejection','sticky_print','reentrant_card','reentrant_print'):
        check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'caller retires borrowed queue and frame storage')
        bind(f,'jp_render_state_shutdown',None)();bind(f,'jp_textures_shutdown',None)();bind(f,'jp_queue_shutdown',None)()
    faulthandler.disable();fault.close();result=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
