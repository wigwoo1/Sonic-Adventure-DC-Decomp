"""Original-byte differential tests plus independent slot/record invariants."""
import ctypes,hashlib,itertools,json,struct
from pathlib import Path
from verify_startup import ROOT,State,reference,signed
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_main_dispatch_active_query import OBSERVE

PROJECT=Path(__file__).resolve().parent
SELECT,REQUEST,FLDI=0x8C093F02,0x8C0940DA,0x8C094102
TABLE,LOW,HIGH=0x8C79E40C,0x8C79EC44,0x8C79EC48
RAM_SIZE=0x1000000
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
binary=BINARY.read_bytes();BASELINE=bytearray(RAM_SIZE);BASELINE[0x10000:0x10000+len(binary)]=binary
DLL=ROOT/'work/sonic-adventure/build/main_dispatch_sound.dll';LIB=ctypes.CDLL(str(DLL))
ARGS=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p]
select=LIB.sa_sound_slot_select_8c093f02;request=LIB.sa_sound_request_8c0940da;connected=LIB.sa_sound_request_connected
for fn in (select,request):fn.argtypes=ARGS+[Call];fn.restype=ctypes.c_int
connected.argtypes=ARGS+[OBSERVE];connected.restype=ctypes.c_int
SPECIAL={0,0x42,0x43,0x7C,0xA1,0xA0,0xEF,0x229,0x22A,0x423,0x40F,0x438,0x29B,0x403,0x405,0x4F9}
IDS=sorted({0xFFFFFFFF,0x80000000,0x7FFFFFFF,0x11,1}|{(a+d)&0xFFFFFFFF for a in SPECIAL for d in (-1,0,1)}|
           set(range(0x308,0x30F))|set(range(0x4BE,0x4C8))|set(range(0x4EF,0x4F9)))


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(m,a):return sum(m[(a&0xFFFFFF)+i]<<(8*i) for i in range(4))
def store(m,a,v):
    for i in range(4):m[(a&0xFFFFFF)+i]=(v>>(8*i))&255
def high_id(i):return i in SPECIAL or 0x309<=i<=0x30D or 0x4BF<=i<=0x4C6 or 0x4F0<=i<=0x4F7


def prepare(c,number):
    m=bytearray(BASELINE);s=State();s.r[:]=[(0x12340000+i*0x10201+number)&0xFFFFFFFF for i in range(16)]
    s.r[15]=0x8CF00000-(number%4)*0x100;s.pr=0x8C000500;s.sr=0x700003F0|(number&1)
    s.pc=SELECT if c['entry']=='select' else REQUEST
    s.r[4]=c.get('priority',0) if c['entry']=='select' else c.get('id',0x11)
    s.r[5]=c.get('id',0x11) if c['entry']=='select' else 0x91827364
    s.r[6]=c.get('priority',0);s.r[7]=0xBADC0FFE
    f=Fpu();f.fpscr=c.get('mode',0)|0x40000|0x3F07C;f.fpul=0xCAFEBABE
    for b in range(2):f.banks[b][:]=[0xAB000000+b*0x10000+i*0x101 for i in range(16)]
    store(m,LOW,c.get('low',0));store(m,HIGH,c.get('high',32))
    for i in range(37):
        for j in range(13):store(m,TABLE+52*i+4*j,0xDD000000|(i<<8)|j)
        store(m,TABLE+52*i,0x7FFFFFFF);store(m,TABLE+52*i+4,1)
    for i in c.get('free',[]):store(m,TABLE+52*i+4,0)
    for i,priority in c.get('priorities',[]):store(m,TABLE+52*i,priority)
    return m,s,f


def model_selection(m,c):
    high=high_id(c.get('id',0x11));cursor=load(m,HIGH if high else LOW)
    selected=-1
    for _ in range(5 if high else 32):
        if load(m,TABLE+52*cursor+4)==0 or signed(load(m,TABLE+52*cursor),32)<=signed(c.get('priority',0),32):selected=cursor
        cursor+=1
        if cursor>=(37 if high else 32):cursor=32 if high else 0
        if selected>=0:break
    after=selected+1
    if after>=(37 if high else 32):after=32 if high else 0
    return selected&0xFFFFFFFF,after,HIGH if high else LOW


def cases():
    for i,pattern,which in itertools.product(IDS,range(4),range(2)):
        high=high_id(i);cursor=(36 if which else 32) if high else (31 if which else 0)
        slots=list(range(32,37) if high else range(32));start=slots.index(cursor)
        target=slots[(start+(len(slots)-1 if pattern==2 else 0))%len(slots)]
        yield {'entry':'select','id':i,'low':0 if high else cursor,'high':cursor if high else 32,
               'free':[target] if pattern in (1,2) else [],'priorities':[(target,0)] if pattern==3 else [],'mode':(i%8)<<19}
    for high,target,priority in itertools.product((False,True),range(37),(0x80000000,0,0x7FFFFFFF,0xFFFFFFFF)):
        if high!=(target>=32):continue
        yield {'entry':'select','id':0 if high else 0x11,'priority':priority,'priorities':[(target,priority)],'mode':(target%8)<<19}
    for cursor in (0,1,31,32,36):
        yield {'entry':'select','id':0,'high':cursor,'free':[cursor]}
    for index,mode,flip,fail in itertools.product((0xFFFFFFFF,0x80000000,0,1,31,32,36),range(8),(False,True),(False,True)):
        yield {'entry':'request','index':index,'mode':mode<<19,'flip':flip,'fail':fail}
    yield {'entry':'request','missing':True}
    for i,mode,full,observed in itertools.product((0,0x11,0x309,0x30E,0x4F7,0x4F8),range(8),(False,True),(False,True)):
        yield {'entry':'connected','id':i,'mode':mode<<19,'free':[] if full else [31,36],'low':31,'high':36,'observed':observed}


def run(c,number,inventory):
    memory,initial,fpu=prepare(c,number);before=bytes(memory)
    em=bytearray(memory);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
    special={'macl':0xFEED1234};path=[];ee=[];errors=[]
    def event(t,r,sr,pr,f):return t,tuple(r),sr,pr,bytes(f)[:132]
    def trace(pc,w,r,sr,pr,_f):
        assert pc in inventory,hex(pc);path.append(pc)
        if pc==SELECT and c['entry']!='select':ee.append(event(pc,r,sr,pr,ef))
    def stub(m,r,sr,pr,f):
        # Deliberately clobber volatile state; request reloads its saved arguments.
        for i in range(8):r[i]=(0xA0000000+i*0x1111)&0xFFFFFFFF
        sr[0]^=1
        if c.get('flip'):f.fpscr^=0x380000
        f.banks[(f.fpscr>>21)&1][5]=0x2468ACE0
        store(m,0x8CEE0000,0x13579BDF);r[0]=c['index']
    def hook(mem,r,sr):
        ee.append(event(SELECT,r,sr,expected.pr,ef));s=[sr];p=[expected.pr];stub(mem,r,s,p,ef);return s[0]
    stop=None;wanted=0
    if c.get('missing'):stop=SELECT;wanted=-2
    elif c.get('fail'):stop=SELECT;wanted=-17
    else:
        result=c.get('index') if c['entry']=='request' else model_selection(memory,c)[0]
        mode=fpu.fpscr^(0x380000 if c.get('flip') else 0)
        if c['entry']!='select' and not result&0x80000000 and mode&0x80000:stop=FLDI;wanted=-1
    reference(em,expected,fpu=ef,special=special,trace=trace,stop_pc=stop,
              hooks={SELECT:hook} if c['entry']=='request' and not stop==SELECT else {},instruction_budget=20000)
    if stop==SELECT:
        ee.append(event(SELECT,list(expected.r),expected.sr,expected.pr,ef))
        if c.get('fail'):
            r=list(expected.r);s=[expected.sr];p=[expected.pr];stub(em,r,s,p,ef);expected.r[:]=r;expected.sr=s[0]
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr);fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    ae=[]
    @Call
    def call(_ctx,t,mem,f,s,r,p):
        try:
            assert t==SELECT;ae.append(event(t,r[:16],s[0],p[0],f.contents));stub(mem,r,s,p,f.contents)
            return -17 if c.get('fail') else 0
        except Exception as exc:errors.append(repr(exc));return -99
    @OBSERVE
    def observe(_ctx,t,mem,f,s,r,p):
        try:ae.append(event(t,r[:16],s[0],p[0],f.contents))
        except Exception as exc:errors.append(repr(exc))
    args=[(ctypes.c_uint8*RAM_SIZE).from_buffer(memory),ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None]
    fn=select if c['entry']=='select' else request if c['entry']=='request' else connected
    cb=observe if c.get('observed',True) else OBSERVE()
    status=fn(*args,cb if c['entry']=='connected' else Call() if c['entry']=='select' or c.get('missing') else call)
    assert status==wanted and not errors,(number,c,status,wanted,errors)
    assert memory==em and list(actual.r)==list(expected.r),(number,c,'RAM/GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and macl.value==special['macl'],(number,c,'FPU/FPUL/MACL differs')
    expected_events=[] if c['entry']=='select' or c.get('missing') or c.get('observed') is False else ee
    assert ae==expected_events,(number,c,'boundary differs')
    invariant=False
    if c['entry']=='select':
        result,cursor,at=model_selection(before,c)
        assert actual.r[0]==result and load(memory,at)==cursor,(number,c,'independent slot invariant')
        independent=bytearray(before);store(independent,at,cursor)
        # Selector writes only its five saved register words and cursor.
        independent[(initial.r[15]&0xFFFFFF)-20:initial.r[15]&0xFFFFFF]=memory[(initial.r[15]&0xFFFFFF)-20:initial.r[15]&0xFFFFFF]
        assert memory==independent
        invariant=True
    elif c['entry']=='connected' and status==0:
        index,cursor,at=model_selection(before,c)
        assert load(memory,at)==cursor
        if index&0x80000000:assert actual.r[0]==0xFFFFFFFF
        else:
            assert actual.r[0]==44
            addr=TABLE+52*index
            assert [load(memory,addr+4*i) for i in range(9)]==[0xFFFFFFFF,120,0x91827364,17,c['id'],0,0xBADC0FFE,0,0]
            bank=(af.fpscr>>21)&1
            assert [load(memory,addr+4*i) for i in (9,10,11)]==[0,0,0]
            assert load(memory,addr+48)==(af.banks[bank][5] if af.fpscr&0x100000 else load(before,addr+48))
        invariant=True
    return path,len(ae),status,invariant


def main():
    translation=json.loads((PROJECT/'analysis/main_dispatch_sound_translation.json').read_text())
    for p,h in translation['sha256'].items():assert digest(Path(p))==h,p
    rows=set();words={};edges=set()
    import csv
    for routine in translation['routines']:
        rows.update(int(a,16) for a in routine['reachable_addresses']);edges.update(tuple(int(a,16) for a in edge) for edge in routine['conditional_edges'])
        for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t'):words[int(r['address'],16)]=int.from_bytes(bytes.fromhex(r['bytes']),'little')
    covered=set();taken=set();counts={};records=[];boundaries=partial=invariants=0
    for number,c in enumerate(cases()):
        path,b,status,check=run(c,number,rows);covered.update(path);boundaries+=b;partial+=status!=0;invariants+=check
        counts[c['entry']]=counts.get(c['entry'],0)+1
        for i,a in enumerate(path):
            if words[a]>>8 in (0x89,0x8B,0x8D,0x8F):
                nxt=i+(2 if words[a]>>8 in (0x8D,0x8F) else 1)
                if nxt<len(path):taken.add((a,path[nxt]))
        records.append({'input':c,'steps':len(path),'status':status,'boundary_comparisons':b,'independent_invariant':check,'full_state_match':True})
        if (number+1)%200==0:print(number+1,'sound comparisons passed',flush=True)
    # R9 is assigned 1 once, preserved, and never changed in this leaf routine.
    impossible={(0x8C093FE2,0x8C09401A),(0x8C093FE8,0x8C09401A),(0x8C094088,0x8C0940BE),(0x8C09408E,0x8C0940BE)}
    assert covered==rows,('uncovered rows',[hex(a) for a in rows-covered])
    assert taken==edges-impossible,('missing feasible edges',[(hex(a),hex(b)) for a,b in edges-impossible-taken])
    files=[Path(__file__),PROJECT/'recover_main_dispatch_sound.py',PROJECT/'analysis/main_dispatch_sound_translation.json',
           PROJECT/'build_main_dispatch_sound.ps1',PROJECT/'source/sa_main_dispatch_sound.c',PROJECT/'source/sa_sound_request_connected.c',
           PROJECT/'verify_startup.py',PROJECT/'verify_fpu_cleanup_transfer.py',PROJECT/'source/fpu_cleanup_transfer.h',BINARY]
    report={'cases_passed':len(records),'counts':counts,'instruction_rows_covered':len(covered),'feasible_conditional_edges_covered':len(taken),
            'infeasible_edges_from_constant_r9':[[hex(a),hex(b)] for a,b in sorted(impossible)],'boundary_comparisons':boundaries,
            'partial_guard_error_cases':partial,'independent_invariants':invariants,'source':translation['source'],
            'source_sha256':digest(PROJECT/translation['source']),'dll_sha256':digest(DLL),'binary_sha256':digest(BINARY),
            'sha256':{str(p):digest(p) for p in files},'routines':translation['routines'],
            'scope':'Original-byte full-state comparisons of slot selection, request wrapper and native connected request; independent selection and record invariants, all rows and feasible branch outcomes.',
            'limitations':['Names/semantics inferred. This writes sound-request records; audio synthesis/driver playback is not recovered here.',
                          'Valid RAM/pointers and selected retail states; MMIO, hardware exception dispatch, concurrency and full-game behavior remain unverified.',
                          'PR=1 stops only at reached FLDI0; SZ=1 preserves overlapping eight-byte FMOV stores and the FR5 companion bits.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_sound_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k in ('cases_passed','counts','instruction_rows_covered','feasible_conditional_edges_covered','boundary_comparisons','partial_guard_error_cases','independent_invariants')},indent=2))


if __name__=='__main__':main()
