"""Exercise real native selection/restoration/leases and trace the JP selector."""
import argparse,ctypes as C,hashlib,json,re,struct,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from verify_native_motion_data import Entry
from build_native_motion_data import BANKS

class Info(C.Structure):
    _fields_=[(n,C.c_uint64) for n in ('starts','selections','board_resets','acquires','releases','generation','resident_bytes','allocated_bytes')]
    _fields_ += [(n,C.c_uint32) for n in ('phase','status','creator_thread','selected_bank')]+[('character',C.c_int32)]+[(n,C.c_uint32) for n in ('live_leases','ready_mask','reserved')]
class View(C.Structure):
    _fields_=[('token',C.c_uint64),('generation',C.c_uint64),('data',C.c_void_p),('bytes',C.c_size_t)]+[(n,C.c_uint32) for n in ('bank','index','kind','reserved')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();build=a.build.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure';b=read(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_source_sha256'].items():assert sha(build/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(a.binary)==read(p/'jp_port_tools/jp_camera_external_inputs.json')['original_binary_sha256']
    lib=C.CDLL(b['DLL']);checks=0;labels=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label);labels.append(label)
    init=bind(lib,'jp_motions_initialize',C.c_int,C.c_size_t,C.c_uint32);select=bind(lib,'jp_motions_select',C.c_int,C.c_int32)
    board=bind(lib,'jp_motions_reset_board',C.c_int,C.c_uint32);acquire=bind(lib,'jp_motions_acquire',C.c_int,C.c_uint32,C.c_char_p,C.c_void_p)
    release=bind(lib,'jp_motions_release',C.c_int,C.c_uint64);shutdown=bind(lib,'jp_motions_shutdown',C.c_int);snapshot=bind(lib,'jp_motions_snapshot',C.c_int,C.c_void_p)
    def info():
        v=Info();assert snapshot(C.byref(v))==0;return v
    def stable(v):return tuple(getattr(v,n) for n,_ in Info._fields_ if n!='status')
    def blank():v=View();C.memset(C.byref(v),0x5a,C.sizeof(v));return v
    check(C.sizeof(Info)==96 and C.sizeof(View)==48,'native owner/view ABIs')
    data={};all_entries=[]
    for bank,name in enumerate(BANKS+('models',)):
        count=bind(lib,'jp_motion_'+name+'_count',C.c_uint32)();get=bind(lib,'jp_motion_'+name+'_entry',C.c_int,C.c_uint32,C.c_void_p);items=[]
        for i in range(count):e=Entry();assert get(i,C.byref(e))==0;items.append(e);all_entries.append(e)
        data[bank]=items
    def digest(bank):
        h=hashlib.sha256()
        for e in data[bank]:h.update(C.string_at(e.data,e.host_bytes))
        return h.hexdigest()
    defaults={bank:digest(bank) for bank in range(8)};resident=sum(e.host_bytes for e in all_entries);capacity=4;exact=resident+len(all_entries)*56+capacity*24
    check(info().phase==0 and info().selected_bank==0xffffffff and info().character==-1,'cold snapshot has no selected bank')
    out=blank();before=bytes(out);check(select(0)==2 and acquire(7,b'sonic_object',C.byref(out))==2 and bytes(out)==before,'cold operations preserve caller output')
    for budget,count in ((0,4),(129<<20,4),(8<<20,0),(8<<20,65537)):check(init(budget,count)==1 and not info().allocated_bytes,'invalid budgets/capacity reject before allocation')
    check(init(exact-1,capacity)==5 and not info().allocated_bytes and all(digest(i)==defaults[i] for i in range(8)),'one-byte-short budget preserves all actual data')
    check(init(exact,capacity)==0 and info().allocated_bytes==exact and info().resident_bytes==resident and info().ready_mask==128,'exact budget captures complete real bank/model defaults')
    before=stable(info());check(init(exact,capacity)==2 and stable(info())==before,'duplicate initialization retains actual owner')
    check(acquire(0,data[0][0].name,C.byref(out))==2 and bytes(out)==bytes(blank()),'inactive ordinary bank cannot be acquired')
    model=next(e for e in data[7] if e.name==b'sonic_object');model_view=View()
    check(acquire(7,b'sonic_object',C.byref(model_view))==0 and model_view.data==model.data and model_view.kind==1,'canonical shared model acquired before character selection')
    C.c_float.from_address(model.data+16).value=123.25;modified_models=digest(7)
    check(modified_models!=defaults[7],'real shared model position changed through native view')
    # Trace real Japanese instructions only up to the loader call. We do not
    # replace its I/O dependency or claim the original full disc load executes.
    sys.path.insert(0,str(p));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    raw=a.binary.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw
    class ReachedLoader(Exception):pass
    traces={}
    for character in (0,2,7,5,3,6):
        observed=[]
        def trace(pc,op,regs,sr,pr,fpu):
            if pc==table['loader_read_prs']:
                at=regs[4]&0xffffff;end=ram.index(0,at,at+128);observed.append((bytes(ram[at:end]).decode(),regs[5]));raise ReachedLoader()
        s=State();s.r[15]=0x8cf00000;s.r[4]=character;s.pc=table['loader_read_character_motions'];s.pr=0x8cfed000;fpu=Fpu();fpu.fpscr=0x40001
        try:reference(ram,s,fpu=fpu,instruction_budget=10000,trace=trace)
        except ReachedLoader:pass
        check(len(observed)==1 and observed[0][1]==0x0cc00000,'actual original selector reaches real file-load call')
        filename,area=observed[0];bank=BANKS.index(filename.split('.')[0].lower());traces[str(character)]=dict(filename=filename,source_area=area,bank=bank)
        # Change every actual record of the target bank, including pointers.
        # The selector never traverses those payload pointers: it restores them
        # from its true captured defaults before consumers can acquire a view.
        for e in data[bank]:C.memset(e.data,0xa7,e.host_bytes)
        generation=info().generation;check(select(character)==0,'actual original C selector succeeds with real native bank restore')
        state=info();check(state.selected_bank==bank and state.character==character and state.generation>generation and state.ready_mask==128|(1<<bank),'native choice agrees with original Japanese instructions')
        check(digest(bank)==defaults[bank] and digest(7)==modified_models,'whole target bank restored while shared model mutation persists')
    before=stable(info());whole={i:digest(i) for i in range(8)}
    for character in (-2147483648,-1,1,4,8,2147483647):
        check(select(character)==9 and stable(info())==before and all(digest(i)==whole[i] for i in range(8)),'invalid character is checked failure without original infinite loop or data mutation')
    check(select(0)==0,'Sonic bank selected for lifetime cases')
    action=next(e for e in data[0] if e.kind==5);view=View();check(acquire(0,action.name,C.byref(view))==0 and view.data==action.data and view.bytes==action.host_bytes,'real original action acquired')
    C.memset(action.data,0x5c,action.host_bytes);held_hash=digest(0);before=stable(info())
    check(select(2)==7 and select(5)==7 and stable(info())==before and digest(0)==held_hash,'same-bank and different-bank reload reject live old-bank action without partial restore')
    check(board(5)==0 and info().selected_bank==0 and digest(0)==held_hash,'independent Sonic board resets while ordinary action is live')
    board_entry=next(e for e in data[5] if e.kind==5);bv=View();check(acquire(5,board_entry.name,C.byref(bv))==0,'real board action acquired')
    C.memset(board_entry.data,0x4d,board_entry.host_bytes);board_hash=digest(5);before=stable(info())
    check(board(5)==7 and stable(info())==before and digest(5)==board_hash,'board reload rejects its own live view')
    check(board(6)==0 and digest(5)==board_hash and digest(7)==modified_models,'other board area remains independent')
    before=stable(info());check(shutdown()==7 and stable(info())==before and info().allocated_bytes==exact,'shutdown preserves all data and allocations while any view is live')
    check(release(view.token)==0 and select(2)==0 and digest(0)==defaults[0] and digest(5)==board_hash,'ordinary reload succeeds after its lease retires while independent board/model views remain')
    check(release(bv.token)==0 and board(5)==0 and digest(5)==defaults[5],'board defaults restore only after its view retires')
    check(release(model_view.token)==0 and release(model_view.token)==2 and release(0)==1,'lease release rejects stale and zero tokens')
    held=[]
    for i in range(capacity):
        v=View();check(acquire(0,action.name,C.byref(v))==0 and v.data==action.data,'multiple leases retain one actual action');held.append(v)
    check(len({v.token for v in held})==capacity,'every live view has a distinct monotonic token')
    out=blank();before=bytes(out);check(acquire(0,action.name,C.byref(out))==7 and bytes(out)==before,'full lease table rejects without overwriting caller output')
    sent=Info();C.memset(C.byref(sent),0x5a,C.sizeof(sent));sent_before=bytes(sent);out=blank();out_before=bytes(out);before=bytes(info());foreign=[]
    def other():foreign.extend((init(exact,4),select(0),board(5),acquire(0,action.name,C.byref(out)),release(held[0].token),shutdown(),snapshot(C.byref(sent))))
    t=threading.Thread(target=other);t.start();t.join()
    check(foreign==[3]*7 and bytes(out)==out_before and bytes(sent)==sent_before and bytes(info())==before,'foreign thread cannot mutate selection views owner or output')
    for v in held:check(release(v.token)==0,'actual lease retires before further use')
    for bank,name,code in ((8,b'x',1),(0,None,1),(0,b'',1),(0,b'x'*128,1),(0,b'NOT_A_SYMBOL',8),(1,b'amy_object',2)):
        out=blank();before=bytes(out);check(acquire(bank,name,C.byref(out))==code and bytes(out)==before,'invalid/inactive/missing acquisition preserves output')
    check(acquire(0,action.name,None)==1 and snapshot(None)==1,'null output rejected')
    object_entry=next(e for e in data[0] if e.host_bytes>=96);before=C.string_at(object_entry.data,object_entry.host_bytes)
    check(acquire(0,action.name,object_entry.data)==1 and snapshot(object_entry.data)==1 and C.string_at(object_entry.data,object_entry.host_bytes)==before,'owner output cannot overwrite actual motion payload')
    # Bind every actual definition through the public game-facing view API.
    acquired=0
    character_for_bank={0:0,1:5,2:7,3:6,4:3}
    for bank in range(8):
        if bank<5:check(select(character_for_bank[bank])==0,'select source bank for complete name binding')
        elif bank<7:check(board(bank)==0,'initialize independent board for complete name binding')
        for index,e in enumerate(data[bank]):
            v=View();check(acquire(bank,e.name,C.byref(v))==0 and (v.data,v.bytes,v.bank,v.index,v.kind)==(e.data,e.host_bytes,bank,index,e.kind),'whole actual bank name/record binding')
            check(release(v.token)==0,'complete binding lease released');acquired+=1
    previous_token=v.token;generation=info().generation
    check(shutdown()==0 and info().phase==2 and not info().allocated_bytes and all(digest(i)==defaults[i] for i in range(8)),'full shutdown restores every actual bank and canonical model record before freeing allocations')
    check(shutdown()==0,'repeat shutdown is idempotent')
    check(init(exact,capacity)==0 and info().generation>generation,'new owner starts with pristine state and a fresh model generation')
    v=View();check(acquire(7,b'sonic_object',C.byref(v))==0 and v.token>previous_token and release(previous_token)==2 and info().live_leases==1,'old session token cannot retire a new live model view')
    check(release(v.token)==0 and shutdown()==0,'second owner closes cleanly')
    report=dict(status='original_character_selection_native_bank_reload_shared_models_independent_boards_and_view_lifetimes_verified',checks=checks,
        original_machine_selection_traces=traces,original_machine_trace_stops_before_disc_loader=True,original_full_disc_loader_executed=False,
        all_native_record_bindings_checked=acquired,whole_bank_default_restores_verified=True,shared_model_state_preserved_across_selection=True,
        actual_live_view_protection_same_and_different_bank_verified=True,independent_board_areas_verified=True,all_record_defaults_restored_on_shutdown=True,
        exact_owner_allocation_budget=exact,resident_native_record_bytes=resident,foreign_thread_rejection_verified=True,monotonic_tokens_across_sessions_verified=True,
        forced_allocator_failure_or_counter_exhaustion_or_reentrant_callback_verified=False,
        build_verification_sha256=sha(build/'build_verification.json'),verifier_sha256=sha(__file__),
        full_set_character_or_animation_playback_or_System_Exit_handoff_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2),flush=True)

if __name__=='__main__':main()
