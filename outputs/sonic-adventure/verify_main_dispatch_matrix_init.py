"""Compare native matrix init and its extended caller chain to retail SH4."""
import ctypes,hashlib,itertools,json
from pathlib import Path

import verify_main_dispatch_transform as chain
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT=Path(__file__).resolve().parent
base,active=chain.base,chain.active
load,store=base.load,base.store
START,END,FLDI=0x8C057B40,0x8C057BAC,0x8C057B66
LIMIT=0x8C88F698
PAIRS=((0,0),(0,1),(1,1),(1,2),(8,4),(0xFFFFFFFF,0),
       (0x7FFFFFFF,0x80000000),(0x80000000,0x7FFFFFFF),(0xFFFFFFFF,0xFFFFFFFF),
       (0xFFFFFFFE,0xFFFFFFFF),(0x7FFFFFFE,0x7FFFFFFF),(0x80000000,0x80000000))
PATTERNS=(0,0x80000000,0x3F800000,0xBF800000,0x7F800000,0xFF800000,0x7FA01234,
          0x7FC01234,1,0x7FFFFF,0x800000,0x7F7FFFFF,0xFFFFFFFF,0x12345678,0x87654321,0xDEADBEEF)
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_matrix_init.dll'
LIB=ctypes.CDLL(str(DLL))
init=LIB.sa_main_dispatch_matrix_init_8c057b40
init.argtypes=active.pure_args;init.restype=ctypes.c_int
chain.functions={'transform':LIB.sa_main_dispatch_matrix_transform_connected,
                 'detach':LIB.sa_main_dispatch_matrix_detach_connected,
                 'status':LIB.sa_main_dispatch_matrix_status_connected,
                 'flag':LIB.sa_main_dispatch_matrix_flag_connected}
for fn in chain.functions.values():fn.argtypes=active.pure_args[:-1]+[chain.Call,active.OBSERVE];fn.restype=ctypes.c_int
chain.EXTERNALS=chain.EXTERNALS-{START}
chain.TRACKED=chain.TRACKED|{START}
old_prepare=chain.prepare


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def signed(value):return value-(1<<32) if value&0x80000000 else value


def connected_prepare(c,number):
    memory,state,fpu=old_prepare(c,number)
    store(memory,LIMIT,c.get('limit',8))
    return memory,state,fpu


def connected_guard(c):
    if c.get('failure') or c.get('no_transform'):return None
    if c['entry']!='transform' and c['mode']&0x80000:return 0x8C0B1DA8
    if c['mode']&0x80000 and signed(c.get('depth',2))<signed(c.get('limit',8)):return FLDI
    if c['mode']&0x180000 or c.get('pop_mode'):return chain.POP
    return None


chain.prepare=connected_prepare
chain.guard_target=connected_guard


def direct_cases():
    for pair,mode,rm,flags in itertools.product(PAIRS,range(8),range(4),(0,0x40000,0x7F07C)):
        yield {'entry':'init','depth':pair[0],'limit':pair[1],'mode':mode<<19,'rm':rm,'fpscr_flags':flags}
    for mask,mode in itertools.product(range(8),range(8)):
        yield {'entry':'init','depth':0,'limit':2,'mode':mode<<19,'relocate_mask':mask}
    for alias,mode in itertools.product(('depth_global','stack_global'),(0,0x100000,0x200000,0x300000)):
        yield {'entry':'init','depth':0,'limit':2,'mode':mode,'alias':alias}
    for mode in range(8):
        yield {'entry':'init','depth':0,'limit':2,'mode':mode<<19,'observe':False}


def connected_cases():
    for entry,angles,mode,pair in itertools.product(chain.functions,chain.ANGLES,range(8),((0,8),(8,8),(0xFFFFFFFF,0))):
        yield {'entry':entry,'angles':angles,'mode':mode<<19,'depth':pair[0],'limit':pair[1]}
    for entry,flags,mode in itertools.product(('detach','status','flag'),(0,0x800),range(8)):
        yield {'entry':entry,'flags':flags,'mode':mode<<19,'no_transform':True,'no_external':entry!='status'}
    for entry,variant,bank in itertools.product(chain.functions,('mutate','angles','relocate_matrix','unobserved'),range(2)):
        c={'entry':entry,'angles':(1,2,3),'mode':bank<<21}
        if variant=='mutate':c['mutate']=True
        elif variant=='angles':c.update(mutate_angles=True,mutate=True)
        elif variant=='relocate_matrix':c['relocate_matrix']=True
        else:c['observe']=False
        yield c
    for entry,target,failure,mode in itertools.product(chain.functions,
            (chain.RZ,chain.RX,chain.RY,chain.CALC),('missing','service_error'),(0,0x100000,0x200000,0x300000)):
        yield {'entry':entry,'angles':(1,2,3),'mode':mode,'fail_target':target,'failure':failure,'mutate':True}
    for entry,bank in itertools.product(chain.functions,range(2)):
        yield {'entry':entry,'angles':(0,0,0),'mode':bank<<21,'fail_target':chain.CALC,'failure':'missing','null_external':True}
    for entry,pop_mode,bank in itertools.product(chain.functions,(0x80000,0x100000,0x180000),range(2)):
        yield {'entry':entry,'angles':(1,2,3),'mode':bank<<21,'pop_mode':pop_mode}


def run_direct(c,number,allowed):
    fixture=dict(c,entry='transform')
    memory,initial,fpu=connected_prepare(fixture,number)
    initial.pc=START
    fpu.fpscr=c['mode']|c.get('rm',0)|c.get('fpscr_flags',0x40000)
    for bank in range(2):
        fpu.banks[bank][:]=[PATTERNS[(i+number+bank*3)%16] for i in range(16)]
    pointer_slots=[LIMIT,chain.DEPTH,chain.STACK]
    for i,literal in enumerate((0x8C057B50,0x8C057B54,0x8C057BAC)):
        if c.get('relocate_mask',0)&(1<<i):
            pointer_slots[i]=0x8CE80000+i*0x1000;store(memory,literal,pointer_slots[i])
    store(memory,pointer_slots[0],c['limit']);store(memory,pointer_slots[1],c['depth'])
    data=chain.MATRIX+0x100
    if c.get('alias')=='depth_global':data=pointer_slots[1]-0x20
    elif c.get('alias')=='stack_global':data=pointer_slots[2]-0x20
    store(memory,pointer_slots[2],data)
    em=bytearray(memory);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
    path=[];special={'macl':0xFEED1234}
    push=signed(c['depth'])<signed(c['limit'])
    guard=bool(push and c['mode']&0x80000)
    def trace(pc,word,r,sr,pr,_f):
        assert pc in allowed,hex(pc)
        path.append(pc)
    steps=reference(em,expected,fpu=ef,special=special,trace=trace,stop_pc=FLDI if guard else None,instruction_budget=200)
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    events=[]
    @active.OBSERVE
    def observe(*args):events.append(True)
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    result=init(view,ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,
                observe if c.get('observe',True) else active.OBSERVE())
    assert result==(-1 if guard else 0) and not events,(number,c,result)
    assert memory==em,(number,c,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,c,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and macl.value==special['macl']
    assert pr.value==initial.pr and actual.r[15]==initial.r[15]
    independent=False
    if not guard:
        assert actual.r[0]==int(push)
        if push and not c['mode']&0x100000 and not c.get('alias'):
            original_bank=((fpu.fpscr>>21)&1)^1
            assert [load(memory,data+4*i) for i in range(16)]==list(fpu.banks[original_bank])
            assert [load(memory,data+0x40+4*i) for i in range(16)]==[0x3F800000 if i%5==0 else 0 for i in range(16)]
            assert load(memory,pointer_slots[1])==(c['depth']+1)&0xFFFFFFFF
            assert load(memory,pointer_slots[2])==data+0x40
            independent=True
    return path,steps,guard,independent


def main():
    manifest_path=PROJECT/'analysis/main_dispatch_matrix_init_translation.json'
    manifest=json.loads(manifest_path.read_text())
    for p,h in manifest['sha256'].items():assert digest(Path(p))==h,p
    rows={int(a,16) for a in manifest['reachable_addresses']}
    wanted_edges={tuple(int(a,16) for a in edge) for edge in manifest['conditional_edges']}
    allowed=set(rows)|set(range(chain.POP,0x8C639BD4,2))
    dependencies=['main_dispatch_shared_translation.json','main_dispatch_active_translation.json','main_dispatch_detach_translation.json']
    for name in dependencies:
        for item in json.loads((PROJECT/'analysis'/name).read_text())['routines']:
            allowed.update(int(a,16) for a in item['reachable_addresses'])
    allowed.update(int(a,16) for a in json.loads((PROJECT/'analysis/main_dispatch_transform_translation.json').read_text())['reachable_addresses'])
    allowed.update(int(a,16) for a in json.loads((PROJECT/'analysis/main_dispatch_shared_check_translation.json').read_text())['reachable_addresses'])
    allowed.update(range(active.QUERY,0x8C06CEFE,2));allowed.update(range(base.RESET,0x8C05E7D4,2))
    coverage={k:set() for k in ('init',*chain.functions)};edges={k:set() for k in coverage}
    counts={k:0 for k in coverage};records=[];boundaries=0;guards=0;failures=0;unobserved=0;independent=0;null_cases=0
    for number,c in enumerate(itertools.chain(direct_cases(),connected_cases())):
        if c['entry']=='init':
            path,steps,guard,check=run_direct(c,number,rows);b=0;external={};independent+=check
        else:path,b,steps,guard,external=chain.run_case(c,number,allowed)
        entry=c['entry'];counts[entry]+=1;coverage[entry].update(set(path)&rows)
        boundaries+=b;guards+=bool(guard);failures+=bool(c.get('failure'));unobserved+=not c.get('observe',True)
        null_cases+=bool(c.get('no_external') or c.get('null_external'))
        for i,pc in enumerate(path):
            if pc==0x8C057B4A and i+1<len(path):edges[entry].add((pc,path[i+1]))
        records.append({'input':c,'steps':steps,'boundaries':b,'external_calls':external,
                        'partial_guard':hex(guard) if isinstance(guard,int) and not isinstance(guard,bool) else bool(guard),'full_state_match':True})
        if (number+1)%400==0:print(number+1,'matrix-init/native-chain comparisons passed',flush=True)
    for entry in coverage:
        assert coverage[entry]==rows,(entry,'uncovered init rows',rows-coverage[entry])
        assert edges[entry]==wanted_edges,(entry,'uncovered init edges',wanted_edges-edges[entry])
    files=[Path(__file__),manifest_path,PROJECT/'verify_main_dispatch_transform.py',PROJECT/'verify_main_dispatch_detach.py',
           PROJECT/'verify_main_dispatch_active_query.py',PROJECT/'verify_main_dispatch_shared_helpers.py',
           PROJECT/'verify_startup.py',PROJECT/'verify_fpu_cleanup_transfer.py',PROJECT/'verify_tikal_controller.py',
           PROJECT/'build_main_dispatch_matrix_init.ps1',PROJECT/'analysis/main-dispatch-matrix-init/diagnostics.tsv']
    for name in ('sa_main_dispatch_matrix_connected.c','sa_main_dispatch_transform.c','sa_main_dispatch_detach.c',
                 'sa_main_dispatch_shared_helpers.c','sa_main_dispatch_status_query.c','sa_main_dispatch_shared_check.c',
                 'sa_main_dispatch_active_query.c','sa_main_dispatch_reset.c','model_matrix_stack_pop.c','sh4_float_i32.c',
                 'sh4_float_i32.h','sh4_compare_bits.h','fpu_cleanup_transfer.h'):
        files.append(PROJECT/'source'/name)
    files.extend(PROJECT/'analysis'/name for name in dependencies+['main_dispatch_transform_translation.json','main_dispatch_shared_check_translation.json'])
    report={k:manifest[k] for k in ('name','address','end_exclusive','instruction_bytes','retail_span_sha256','source')}
    report.update({'source_sha256':digest(PROJECT/manifest['source']),'binary_sha256':digest(base.BINARY),
        'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},'cases_passed':len(records),'counts':counts,
        'boundary_comparisons':boundaries,'init_rows_by_entry':{k:len(v) for k,v in coverage.items()},
        'init_conditional_edges_by_entry':{k:len(v) for k,v in edges.items()},
        'independent_save_identity_checks_passed':independent,'partial_precision_guards_passed':guards,
        'partial_service_error_cases_passed':failures,'unobserved_cases_passed':unobserved,'null_external_cases_passed':null_cases,
        'comparison_level':'Complete 16 MiB RAM, GPR/SR/PR, physical FPU banks/FPSCR, separate FPUL/MACL and ordered native/external boundary snapshots against original SH4 instructions; independent raw-save and identity assertions for ordinary successful initialization.',
        'scope':'50-row self-contained push/init directly and through transform/detach/STATUS/FLAG, signed depth/limit boundaries, literal relocation, raw FR/SZ transfers, full-buffer/guard paths, guest-global overlap, native pop and remaining-call partial failures.',
        'limitations':['Three rotations, vector calculation and sound remain external fixtures; full numerical transform behavior is not recovered here.',
                       'Reached push FLDI1 with PR=1 returns host -1 after original depth/pointer changes; a full-buffer return does not impose that restriction.',
                       'Raw initial SZ=1 push semantics are preserved and may save/write partial matrices. Existing native pop supports PR=0/SZ=0 only and returns -1 at entry otherwise.',
                       'Mapped guest-pointer preconditions required; invalid pointer exceptions, MMIO, cache/hardware behavior and all actual game states are unverified. Full game/ports remain unfinished.'],
        'cases':records})
    (PROJECT/'analysis/main_dispatch_matrix_init_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    keys=('cases_passed','counts','boundary_comparisons','init_rows_by_entry','init_conditional_edges_by_entry',
          'independent_save_identity_checks_passed','partial_precision_guards_passed','partial_service_error_cases_passed',
          'unobserved_cases_passed','null_external_cases_passed')
    print(json.dumps({k:report[k] for k in keys},indent=2))


if __name__=='__main__':main()
