"""Native notification/creation/query/gate paths versus original instructions."""
import ctypes,gc,hashlib,itertools,json
from pathlib import Path
import verify_main_dispatch_prefix_gate as previous
import sh4_binary32
from verify_startup import State,signed
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call
from verify_main_dispatch_active_query import OBSERVE

PROJECT=Path(__file__).resolve().parent;ROOT=previous.ROOT
NOTIFY,CREATE,READY=0x8C109FDE,0x8C109E50,0x8C64BED8
DISPOSE=0x8C09865C
DISABLE,MAP,PAIRS=0x8C1BFE20,0x8C754E9C,0x8C15FFDC
DEVICE,TASK,NEXTTASK,ARENA=0x8CE40000,0x8CE50000,0x8CE50100,0x8CE60000
load,store=previous.load,previous.store
binary=previous.sound.binary
def literal(a):return int.from_bytes(binary[a-0x8C010000:a-0x8C010000+4],'little')
DEVICE_SLOT=literal(0x8C65F3D8)
DEVICE_OFFSET=signed(int.from_bytes(binary[0x8C65F3CA-0x8C010000:0x8C65F3CC-0x8C010000],'little'),16)
FREE_TASK=literal(0x8C0986C0);LEVELS=literal(0x8C0986C8);FREE_HEAP=literal(0x8C098B60)
ERROR_SLOT=literal(0x8C10CB08)
DLL=ROOT/'work/sonic-adventure/build/main_dispatch_notification.dll';LIB=ctypes.CDLL(str(DLL))
FUNCTIONS={k:getattr(LIB,name) for k,name in (
    ('notify','sa_notification_connected'),('create','sa_notification_create_connected'),('ready','sa_notification_ready_connected'),
    ('gate','sa_main_dispatch_notified_prefix_gate_connected'))}
for fn in FUNCTIONS.values():fn.argtypes=previous.ARGS+[OBSERVE];fn.restype=ctypes.c_int
EXTERNAL={DISPOSE,previous.SPAWN,previous.CANCEL,previous.SET}
TRACKED={NOTIFY,CREATE,READY,0x8C604370,0x8C646868,0x8C65F3A0,0x8C10CA5C,
         0x8C09852E,0x8C0985EC,0x8C098A80,0x8C098B0C,previous.sound.REQUEST,previous.sound.SELECT}|EXTERNAL


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def signed_div(a,b):
    a,b=signed(a&0xFFFFFFFF,32),signed(b&0xFFFFFFFF,32)
    if not b:return 0
    return ((abs(a)//abs(b))*(-1 if (a<0)!=(b<0) else 1))&0xFFFFFFFF


def prepare(c,number):
    memory,state,fpu=previous.prepare(dict(c,flags=4,character=c.get('id',0xFF)),number)
    entry=c.get('entry','notify')
    state.pc={'notify':NOTIFY,'create':CREATE,'ready':READY,'gate':previous.START}[entry]
    if entry=='notify':state.r[4]=c.get('id',0xFF);state.r[5]=c.get('axis',7)&0xFFFFFFFF;state.r[6]=c.get('duration',59)&0xFFFFFFFF;state.r[7]=c.get('scale',6)&0xFFFFFFFF
    elif entry=='create':
        state.r[4]=c.get('payload',0xA1B2C3D4);state.r[5]=c.get('axis',0x80)&0xFFFFFFFF
        state.r[6]=c.get('duration',59)&0xFFFFFFFF;state.r[7]=c.get('scale',6)&0xFFFFFFFF
        store(memory,state.r[15],c.get('fifth',0xFEDCBA98))
    elif entry=='ready':state.r[4]=c.get('candidate',1)
    store(memory,DISABLE,c.get('disabled',0));store(memory,MAP+c.get('id',0xFF)*4,0xFFFFFFFF if c.get('unmapped') else 3)
    store(memory,PAIRS+24,1);store(memory,PAIRS+28,2)
    store(memory,DEVICE_SLOT,DEVICE)
    for candidate in range(8):store(memory,DEVICE+DEVICE_OFFSET+candidate*0x78,0x100 if c.get('available',1)&(1<<candidate) else 0)
    # The useful default makes the first candidate available.
    if 'available' not in c:store(memory,DEVICE+DEVICE_OFFSET+0x78,0x100)
    store(memory,FREE_TASK,0 if c.get('task_empty') else TASK)
    for i in range(8):store(memory,LEVELS+i*4,0)
    for task in (TASK,NEXTTASK):
        for i in range(12):store(memory,task+4*i,0xDD000000+i)
    store(memory,TASK,NEXTTASK);store(memory,NEXTTASK,0)
    size=c.get('heap',128);store(memory,FREE_HEAP,ARENA if size else 0)
    for at in range(0,0x210,4):store(memory,ARENA+at,0xA5A5A5A5)
    if size:
        for i,v in enumerate((ARENA+size,0,ARENA+size,0)):store(memory,ARENA+4*i,v)
        for i,v in enumerate((0,ARENA,0,ARENA)):store(memory,ARENA+size+4*i,v)
    store(memory,ERROR_SLOT,0xABCDEF01)
    return memory,state,fpu


def cases():
    for axis,duration,scale in itertools.product((-0x80000000,-5,-4,-2,-1,0,1,2,4,5,0x7FFFFFFF),(-0x80000000,0,6,7,59,60,0x7FFFFFFF),(0,1,6,-1,0x80000000,0x7FFFFFFF)):
        yield {'entry':'notify','axis':axis,'duration':duration,'scale':scale,'mode':((axis+duration+scale)%8)<<19}
    for availability,unmapped,disabled,id_,mode in itertools.product((0,2,4,6),(False,True),(0,1),(0,0x80,0xFF),range(2)):
        yield {'entry':'notify','available':availability,'unmapped':unmapped,'disabled':disabled,'id':id_,'mode':mode<<21}
    for entry,heap,empty,mode in itertools.product(('notify','create'),(0,16,32,48,64,128),(False,True),range(8)):
        yield {'entry':entry,'heap':heap,'task_empty':empty,'mode':mode<<19}
    for payload,axis,duration,scale,fifth in itertools.product((0,0xFFFFFFFF),(0,0xFF,0x100),(0x80,0x1FF),(0xFFFFFFFF,0x100),(0,0x123456FF)):
        yield {'entry':'create','payload':payload,'axis':axis,'duration':duration,'scale':scale,'fifth':fifth}
    for candidate,flags,mode in itertools.product((0,1,2,7),(0,1),range(8)):
        yield {'entry':'ready','candidate':candidate,'available':flags<<candidate,'mode':mode<<19}
    for state,available,mode in itertools.product((0,0x2C,0x2D,0x30,0x40,0x80,0xFF),(0,2,4),range(8)):
        yield {'entry':'gate','state':state,'available':available,'mode':mode<<19,'id':0xFF}
    for entry,mode in itertools.product(('notify','create','ready'),range(8)):
        yield {'entry':entry,'mode':mode<<19,'null_external':True}
    for entry,heap,mode in itertools.product(('notify','create'),(16,32),range(8)):
        yield {'entry':entry,'heap':heap,'mode':mode<<19,'null_external':True}
    for state,mode in itertools.product((0x30,0x2C,0x40),range(8)):
        yield {'entry':'gate','state':state,'mode':mode<<19,'flip':True,'observe':False}


class Stopped(Exception):pass


def run(c,number,allowed):
    memory,initial,fpu=prepare(c,number);em=bytearray(memory)
    expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu);special={'macl':0xFEED1234}
    path=[];ee=[];wanted=0;errors=[];calls=[]
    def event(t,r,sr,pr,f,fpul,macl):return t,tuple(r),sr,pr,bytes(f)[:132],fpul,macl
    def stop(pc,r,sr,pr,status):
        nonlocal wanted
        expected.r[:]=r;expected.sr=sr;expected.pr=pr;expected.pc=pc;wanted=status;raise Stopped()
    def trace(pc,w,r,sr,pr,_f):
        assert pc in allowed,hex(pc);path.append(pc)
        if pc in TRACKED and pc!=initial.pc:ee.append(event(pc,r,sr,pr,ef,ef.fpul,special['macl']))
        if pc in (previous.FNEG,previous.sound.FLDI) and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)
        if pc==previous.FMUL:
            probe=Fpu.from_buffer_copy(ef);rc=previous.multiply_expected(probe,2,3)
            if rc:ctypes.memmove(ctypes.byref(ef),ctypes.byref(probe),ctypes.sizeof(Fpu));stop(pc,r,sr,pr,rc)
    def service(t,mem,r,sr,pr,f):
        if t==DISPOSE:
            assert r[4]==TASK
            store(mem,TASK+0x18,0xD15F05ED) # explicit disposal fixture, not native cleanup
        elif t==previous.SPAWN:assert r[4]==previous.OBJECT
        elif t==previous.SET:assert r[4]==0 and r[5]==8
        else:assert t==previous.CANCEL
        for i in range(8):r[i]=0xA0000000+i*0x1111+(t&0xFFFF)
        sr[0]^=1
        if c.get('flip'):f.fpscr^=0x380000
    def hook_for(t):
        def hook(mem,r,sr):
            ee.append(event(t,r,sr,expected.pr,ef,ef.fpul,special['macl']));calls.append(t)
            if c.get('null_external'):stop(t,r,sr,expected.pr,-2)
            s=[sr];p=[expected.pr];service(t,mem,r,s,p,ef)
            if t==c.get('fail_target'):stop(t,r,s[0],p[0],-17)
            return s[0]
        return hook
    try:previous.reference(em,expected,fpu=ef,special=special,trace=trace,hooks={t:hook_for(t) for t in EXTERNAL},instruction_budget=8000)
    except Stopped:pass
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr);fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234);ae=[];ac=[]
    @Call
    def call(_ctx,t,mem,f,s,r,p):
        try:
            assert t in EXTERNAL;ac.append(t);service(t,mem,r,s,p,f.contents)
            return -17 if t==c.get('fail_target') else 0
        except Exception as exc:errors.append(repr(exc));return -99
    @OBSERVE
    def observe(_ctx,t,mem,f,s,r,p):
        try:ae.append(event(t,r[:16],s[0],p[0],f.contents,fpul.value,macl.value))
        except Exception as exc:errors.append(repr(exc))
    result=FUNCTIONS[c['entry']]((ctypes.c_uint8*previous.RAM_SIZE).from_buffer(memory),ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,
        Call() if c.get('null_external') else call,observe if c.get('observe',True) else OBSERVE())
    assert result==wanted and not errors,(number,c,result,wanted,errors)
    assert memory==em and list(actual.r)==list(expected.r),(number,c,'RAM/GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and macl.value==special['macl'],(number,c,'FPU/FPUL/MACL differs')
    assert ae==(ee if c.get('observe',True) else []),(number,c,'boundary differs')
    assert ac==([] if c.get('null_external') else calls),(number,c,'external calls differ')
    invariant=False
    if wanted==0:
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        assert list(actual.r)[8:15]==list(initial.r)[8:15]
        entry=c['entry']
        if entry=='ready':assert actual.r[0]==int(bool(c.get('available',2)&(1<<c.get('candidate',1))));invariant=True
        if entry in ('notify','create'):
            selected=None
            if entry=='create':selected=c.get('payload',0xA1B2C3D4)
            elif not c.get('disabled') and not c.get('unmapped'):
                available=c.get('available',2);selected=1 if available&2 else 2 if available&4 else None
            created=selected is not None and not c.get('task_empty') and c.get('heap',128)>=64
            if created:
                assert load(memory,TASK+0x28)==ARENA+48 and load(memory,TASK+0x2C)==ARENA+16
                assert load(memory,ARENA+16)==selected
                if entry=='create':fields=(c.get('axis',0x80),c.get('duration',59),c.get('scale',6),c.get('fifth',0xFEDCBA98))
                else:
                    axis=max(-4,min(4,c.get('axis',7)));axis={1:2,-1:-2}.get(axis,axis)
                    duration=max(7,min(59,c.get('duration',59)));first=signed_div((duration*(c.get('scale',6)&0xFFFFFFFF))&0xFFFFFFFF,(axis<<2)&0xFFFFFFFF)
                    first=max(signed(first,32),1);last=signed_div(255,first)
                    fields=(128,axis,duration,last)
                    if axis==0:assert load(memory,ERROR_SLOT)==0x44E
                assert [load(memory,ARENA+48+i,1) for i in range(5)]==[1,*[v&255 for v in fields]],(number,c,'independent payload')
                assert [load(memory,ARENA+48+i,1) for i in (5,6,7)]==[0xA5]*3
            invariant=True
    return path,len(ae),wanted,invariant,ac


def main():
    sh4_binary32.execute_multiply=previous.multiply_expected
    manifest_path=PROJECT/'analysis/main_dispatch_notification_translation.json';manifest=json.loads(manifest_path.read_text())
    for p,h in manifest['sha256'].items():assert digest(Path(p))==h,p
    new_rows=set().union(*({int(a,16) for a in r['reachable_addresses']} for r in manifest['routines']));allowed=new_rows.copy()
    ranges=((previous.START,previous.END),(0x8C604370,0x8C604376),(0x8C646868,0x8C646876),(0x8C65F3A0,0x8C65F3B2),
        (0x8C10CA5C,0x8C10CB06),(0x8C09852E,0x8C0985EC),(0x8C0985EC,0x8C09865C),(0x8C098A80,0x8C098B0C),(0x8C098B0C,0x8C098B42))
    for start,end in ranges:allowed.update(range(start,end,2))
    for r in json.loads((PROJECT/'analysis/main_dispatch_sound_translation.json').read_text())['routines']:allowed.update(int(a,16) for a in r['reachable_addresses'])
    rows=set();edges=set();calls=set();records=[];counts=dict.fromkeys(FUNCTIONS,0);boundaries=partial=invariants=closed=0;failures={}
    def check(c,number):
        nonlocal boundaries,partial,invariants,closed
        path,b,status,inv,external=run(c,number,allowed);rows.update(a for a in path if a in new_rows)
        for i,a in enumerate(path):
            if a not in new_rows:continue
            w=int.from_bytes(binary[a-0x8C010000:a-0x8C010000+2],'little')
            if w>>8 in (0x89,0x8B,0x8D,0x8F):
                nxt=i+(2 if w>>8 in (0x8D,0x8F) else 1)
                if nxt<len(path):edges.add((a,path[nxt]))
            if w>>12==0xB or w&0xF0FF in (0x400B,0x402B):calls.add(a)
        counts[c['entry']]+=1;boundaries+=b;partial+=status!=0;invariants+=inv;closed+=status==0 and not external
        if status==0:
            for t in external:failures.setdefault(t,c)
        records.append({'input':c,'steps':len(path),'boundary_comparisons':b,'status':status,'independent_invariant':inv,'external_calls':[hex(t) for t in external],'full_state_match':True})
    for number,c in enumerate(cases()):
        check(c,number)
        if (number+1)%200==0:print(number+1,'notification comparisons passed',flush=True)
        if (number+1)%25==0:gc.collect()
    assert rows==new_rows,('uncovered rows',[hex(a) for a in new_rows-rows])
    assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']},('edge coverage',len(edges))
    assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
    for target,c in sorted(failures.items()):check(dict(c,fail_target=target),len(records))
    files=[Path(__file__),Path(previous.__file__),manifest_path,PROJECT/'build_main_dispatch_notification.ps1',PROJECT/'source/sa_main_dispatch_notification.c',PROJECT/'source/sa_main_dispatch_notification_connected.c']
    for name in ('sa_input_channel_info_pointer.c','sa_input_device_record_index_helpers.c','callback_tail_thunks.c','sa_signed_division.c','sa_task_create.c','sa_task_create_base.c','sa_heap_allocate.c','sa_heap_allocate_zeroed.c','sa_main_dispatch_prefix_gate.c','sa_main_dispatch_sound.c','sh4_multiply_extended.c','sh4_multiply.c','sh4_fmac.c','fpu_cleanup_transfer.h'):
        files.append(PROJECT/'source'/name)
    files.extend((PROJECT/'verify_startup.py',PROJECT/'analysis/main_dispatch_prefix_gate_verification.json'))
    report={'source':manifest['source'],'source_sha256':digest(PROJECT/manifest['source']),'routines':manifest['routines'],
        'cases_passed':len(records),'counts':counts,'rows_covered':len(rows),'conditional_edges_covered':len(edges),'call_sites_covered':len(calls),
        'boundary_comparisons':boundaries,'partial_guard_error_cases':partial,'independent_invariants':invariants,'completed_without_external_calls':closed,
        'service_error_cases':len(failures),'sha256':{str(p):digest(p) for p in files},'dll_sha256':digest(DLL),'binary_sha256':digest(previous.BINARY),
        'scope':'Three original integer routines with native input pointer/query, division, task creation/base, heap allocation/zero allocation, sound and prefix gate integration. Full RAM/CPU/FPU/FPUL/MACL, ordered boundaries and independent clamp/division/payload invariants.',
        'limitations':['Task disposal 8c09865c and prefix spawn/cancel/setter remain explicit service fixtures; no claim of their complete RAM ABI recovery.',
                      'The prefix retains existing arithmetic precision/rounding limits. Valid pointer/table/heap metadata preconditions; no MMIO, hardware exception dispatch, concurrency or physical notification/audio validation.',
                      'Full dispatcher/gameplay/native ports are unfinished; descriptive names and field meanings are inferred.'],
        'cases':records}
    (PROJECT/'analysis/main_dispatch_notification_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered','boundary_comparisons','partial_guard_error_cases','independent_invariants','completed_without_external_calls','service_error_cases')},indent=2))


if __name__=='__main__':
    try:main()
    finally:sh4_binary32.execute_multiply=previous.scalar.finite_execute
