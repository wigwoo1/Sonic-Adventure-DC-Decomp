"""Whole original 2D polygon execution against real host-owned packet streams.

JP original packet/header bodies run without dependency hooks. GCC's added
wide-picture policy is checked by a separate integer Python value oracle;
that policy has no original JP binary counterpart. Test replay observation
is explicit; complete frame replay and rasterization are not exercised.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from primitive_reference import make_reference,exceptional_binary
from camera_reference_numeric import service_for

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def packed(values):return struct.pack('<'+'I'*len(values),*values)
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def fv(u):return struct.unpack('<f',struct.pack('<I',u))[0]

class Point(C.Structure):_fields_=[('x',C.c_float),('y',C.c_float)]
class Poly(C.Structure):_fields_=[('p',C.c_void_p),('col',C.c_void_p),('tex',C.c_void_p),('num',C.c_uint32)]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve()
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    pin_path=p/'jp_port_tools/jp_polygon2d_external_inputs.json';pin=json.loads(pin_path.read_text())
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    build=json.loads((d/'build_verification.json').read_text());assert build['exit_code']==build['diagnostic_bytes']==0
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in build['existing_diagnostic_objects_sha256'].items():assert sha(Path(n))==h,n
    dll=Path(build['DLL']);assert sha(dll)==build['DLL_sha256']
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import add_sub,multiply
    reference,reference_hashes=make_reference(executor)
    lib=C.CDLL(str(dll));U=C.c_uint32;I=C.c_int32;P=C.c_void_p;S=C.c_size_t
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    init=bind('jp_queue_initialize',I,S);reset=bind('jp_queue_reset_frame',I);shutdown=bind('jp_queue_shutdown',None)
    used=bind('jp_queue_committed_bytes',S,U);data=bind('jp_queue_data',P,U);status=bind('jp_queue_status',I)
    clear=bind('jp_queue_clear_error',None);reserve=bind('jp_queue_reserve',I,U,U)
    setctx=bind('jp_queue_set_context',I,U,C.POINTER(U));getctx=bind('jp_queue_get_context',I,U,C.POINTER(U))
    select=bind('jp_queue_select_context',I,U)
    getparam=bind('jp_queue_get_parameter',I,C.POINTER(U));cursor=bind('jp_queue_parameter_cursor',P,U);descriptor=bind('jp_queue_descriptor_cursor',P,U)
    hints=bind('jp_queue_cache_hint_count',C.c_uint64);last_hint=bind('jp_queue_last_cache_hint',P)
    draw=bind('jp_polygon_probe_draw',I,P,I,U,U);rec_calls=bind('jp_polygon_probe_calls',U)
    rec_meta=bind('jp_polygon_probe_meta',P);rec_saved=bind('jp_polygon_probe_saved',P)
    rec_points=bind('jp_polygon_probe_points',P);rec_colors=bind('jp_polygon_probe_colors',P);rec_texs=bind('jp_polygon_probe_texs',P)
    wide=bind('port_wide_x',C.c_float,C.c_float);squeezes=bind('port_wide_squeezes',I);covers=bind('port_wide_covers',I,C.POINTER(C.c_float),I,I)
    mode=bind('njSetkmColorMode',None,U);get=bind('njGetkmPointer',None);put=bind('njSetkmPointer',None)
    control=U.in_dll(lib,'_nj_control_3d_flag_');color=(U*lib.jp_queue_context_count()).in_dll(lib,'_nj_ctx_color_mode_')
    aspect=C.c_float.in_dll(lib,'port_draw_aspect');pillar=I.in_dll(lib,'port_pillarbox');anchor=C.c_float.in_dll(lib,'port_wide_anchor');cover=I.in_dll(lib,'port_wide_cover')
    numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=I;numeric.argtypes=[U,C.POINTER(Fpu),U]
    service=service_for(numeric);rng=random.Random(0x504f4c59);checks=0;packet_words=0;original_calls=0;color_calls=0;sections={};dependencies=set();banks=set();flags_tested=set();counts=set();fm=0
    def check(ok,label):
        nonlocal checks
        checks+=1
        assert ok,label
    class_cases=[('multiply',0,0xff800000,0x7fbfffff),('multiply',0x80000000,0x7f800000,0x7fbfffff),
        ('multiply',0x40000000,0xff800000,0xff800000),('divide',0x40000000,0xff800000,0x80000000),
        ('divide',0x7f800000,0xff800000,0x7fbfffff),('divide',0xff800000,0x80000001,0x7f800000),
        ('add',0x7f800000,0xff800000,0x7fbfffff),('add',0xff800000,0x40000000,0xff800000),
        ('subtract',0x7f800000,0x7f800000,0x7fbfffff),('subtract',0x40000000,0xff800000,0x7f800000)]
    for bank in range(2):
        for operation,x,y,expected in class_cases:
            f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][3]=x;f.banks[bank][6]=y
            exceptional_binary(f,3,6,operation);check(f.banks[bank][3]==expected,'manual exceptional value fixture')
    sections['manual_exceptional_class_fixtures']=len(class_cases)*2
    symbols={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw;BASE=0x8c000000;CTX=0x8cd00000;CURSORS=0x8cd00200;COLOR=0x8cd00a00
    G_POLY=0x8cd01000;G_POINTS=0x8cd02000;G_COLORS=0x8cd03000;G_TEX=0x8cd04000
    guest_bases=[0x8cd10000+i*0x10000 for i in range(5)];indices=[0,2,4,1,3]
    def store(addr,raw):ram[addr-BASE:addr-BASE+len(raw)]=raw
    def iw(addr,values):store(addr,packed(values))
    def read(addr,size):return bytes(ram[addr-BASE:addr-BASE+size])
    def word(addr):return struct.unpack('<I',read(addr,4))[0]
    iw(symbols['_nj_current_ctx_'],[CTX]);iw(symbols['_nj_vertex_buffer_'],[CURSORS]);iw(symbols['_nj_color_mode_'],[COLOR])
    mmio={0xff000038:0,0xff00003c:0};mmio.update({a:0xde000000+(a&31) for b in guest_bases for a in range((b&0x3ffffff)|0xe0000000,((b&0x3ffffff)|0xe0000000)+65536,4)})
    captured=[]
    names=['njDrawPolygon2D','njGetkmPointer','njSetkmPointer','njSetkmColorMode','njStartVertexNonTexOpaque','njStartVertexNonTexTrans','njStartVertexTexOpaque','njStartVertexTexTrans','njEndVertex']
    addresses={symbols[n]:n for n in names}
    def trace(pc,op,regs,sr,pr,fp):
        nonlocal fm
        if pc in addresses:dependencies.add(addresses[pc])
        if op&0xf00f==0xf00e:fm+=1
    def pref(addr):
        assert not addr&31
        captured.append((addr,[mmio[addr+4*i] for i in range(8)]))
    def original(name,bank,poly=G_POLY,n=0,pri=0,attr=0,arg=0):
        s=executor.State();s.pc=symbols[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        s.r[4]=poly if name=='njDrawPolygon2D' else arg;s.r[5]=n;s.r[6]=attr
        f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][4]=pri
        reference(ram,s,fpu=f,mmio=mmio,vector_instruction=service,cache_prefetch=pref,trace=trace,instruction_budget=500000)
    def start_frame():
        check(reset()==0,'frame reset/guards')
        for i,j in enumerate(indices):iw(CURSORS+4*j,[guest_bases[i]])
    def flush(u):return u&0x80000000 if (u&0x7fffffff)<0x800000 else u
    def op(fn,x,y):return flush(fn(flush(x),flush(y),mode=1)[0])
    def wide_expected(x):
        at=fw(320.) if pillar.value else op(add_sub,fw(320.),fw(anchor.value))
        return op(add_sub,at,op(multiply,op(lambda a,b,mode:add_sub(a,b,True,mode),x,at),fw(aspect.value)))
    def scenario(n,attr,bank,case,append=False,policy=0):
        nonlocal original_calls,packet_words
        if not append:start_frame()
        texture=bool(attr&0x80000000);filled=bool(attr&32);li=int(bool(attr&64));counts.add((filled,n));banks.add(bank);flags_tested.add(attr&0x80000060)
        headers=[rng.getrandbits(32) for _ in range(4)];flags=[0,0x400,0x1000,0x1400,rng.getrandbits(32)][case%5]
        check(setctx(0,(U*4)(*headers))==0,'context set');iw(CTX+144,headers);control.value=flags;iw(symbols['_nj_control_3d_flag_'],[flags])
        point_raw=struct.pack('<'+'f'*(2*n),*[rng.uniform(-1000,2000) for _ in range(2*n)])
        if case%11==0:
            point_raw=struct.pack('<'+'f'*(2*n),*[v for i in range(n) for v in (0. if i%2==0 else 640.,float(i*7))])
        if case%17==0:point_raw=bytes(8*n) # zero-length edges, explicit invalid-result packet values
        if case%19==0:point_raw=packed([1,0x80000001]*n) # DN1 input handling, retained raw fill coordinates
        if 200<=case<220:
            extremes=[0x7f7fffff,0xff7fffff,0x00800000,0x80800000,fw(1e-20),fw(-1e-20),fw(1e20),fw(-1e20)]
            point_raw=packed([extremes[(case+i)%len(extremes)] for i in range(2*n)])
        col_raw=rng.randbytes(4*n);uv_raw=rng.randbytes(4*n)
        pts=C.create_string_buffer(b'\xa5'*32+point_raw+b'\x5a'*32);cols=C.create_string_buffer(b'\xa5'*32+col_raw+b'\x5a'*32);texs=C.create_string_buffer(b'\xa5'*32+uv_raw+b'\x5a'*32)
        poly=Poly(C.addressof(pts)+32,C.addressof(cols)+32,C.addressof(texs)+32 if texture else None,rng.getrandbits(32));poly_bytes=bytes(poly)
        pri=[fw(-2.),fw(-1.),fw(-1.0000001192092896),fw(0.),fw(1.),fw(-1000.),0xff800000,0x7fc12345,0xffc12345,1,0x80000001][case%11]
        aspect.value=[1.,0.75,0.5625,1.25][policy%4];pillar.value=int(policy%3==1);anchor.value=[0.,-320.,320.,37.25][(policy//2)%4];cover.value=int(policy%5==3)
        # GCC-wide branch oracle, separate from original-JP packet equivalence.
        expected_squeeze=aspect.value!=1. and (pillar.value or not cover.value)
        check(bool(squeezes())==expected_squeeze,'squeeze predicate')
        xs=list(struct.unpack('<'+'I'*(2*n),point_raw))[::2]
        xvalues=[fv(flush(v)) for v in xs]
        covered=not pillar.value and any(-1.<=x<=1. for x in xvalues) and any(639.<=x<=641. for x in xvalues) and all(-1.<=x<=1. or 639.<=x<=641. for x in xvalues)
        check(bool(covers(C.cast(C.addressof(pts)+32,C.POINTER(C.c_float)),n,2))==covered,'cover predicate')
        squeezed=n<=32 and expected_squeeze and (texture or not covered)
        expected_points=bytearray(point_raw)
        if squeezed:
            for i,x in enumerate(xs):struct.pack_into('<I',expected_points,8*i,wide_expected(x))
        store(G_POINTS,expected_points);store(G_COLORS,col_raw);store(G_TEX,uv_raw);iw(G_POLY,[G_POINTS,G_COLORS,G_TEX if texture else 0,poly.num])
        prior_used=used(li);prior_prefix=C.string_at(data(li),prior_used) if prior_used else b''
        prior_calls=rec_calls();prior_hints=hints();captured.clear();original('njDrawPolygon2D',bank,n=n,pri=pri,attr=attr);original_calls+=1
        check(draw(C.byref(poly),n,pri,attr)==0,('native draw',n,hex(attr),case,policy))
        check(bytes(poly)==poly_bytes,'input pointer record preserved');check(bytes(pts)==b'\xa5'*32+point_raw+b'\x5a'*32+b'\0','point input guards/payload preserved')
        check(bytes(cols)==b'\xa5'*32+col_raw+b'\x5a'*32+b'\0','color guards/payload preserved');check(bytes(texs)==b'\xa5'*32+uv_raw+b'\x5a'*32+b'\0','texture guards/payload preserved')
        check(rec_calls()==prior_calls+1,'one replay observation');check(C.string_at(rec_meta(),20)==packed([3,n,0,pri,attr]),'replay original arguments')
        check(C.string_at(rec_saved(),C.sizeof(poly))==poly_bytes,'replay original full-width pointers')
        check(C.string_at(rec_points(),n*8)==point_raw and C.string_at(rec_colors(),n*4)==col_raw,'replay before squeezing')
        if texture:check(C.string_at(rec_texs(),n*4)==uv_raw,'replay original packed UV')
        expected_blocks=1+(n if filled else 4*n);check(len(captured)==expected_blocks,'original complete batch block count')
        check(used(li)==prior_used+32*expected_blocks,'exact reserved bound consumed')
        check(data(li)>0xffffffff and poly.p>0xffffffff,'full-width queue and input pointers')
        check(C.string_at(data(li),prior_used)==prior_prefix,'append/growth preserves previous packets')
        check(hints()==prior_hints+expected_blocks and last_hint()==data(li)+used(li)-32,'cache hints are host addresses')
        native=struct.unpack('<'+'I'*(expected_blocks*8),C.string_at(data(li)+prior_used,expected_blocks*32))
        for block,(addr,guest_words) in enumerate(captured):
            expected_address=((guest_bases[li]&0x3ffffff)|0xe0000000)+prior_used+32*block
            check(addr==expected_address,'guest packet list/order')
            defined=set(range(4 if texture else 3)) if block==0 else ({0,1,2,3,4,5,6} if texture else {0,1,2,3,6})
            for j in range(8):
                actual=native[block*8+j]
                if j in defined:
                    check(actual==guest_words[j],('packet word',n,hex(attr),case,policy,block,j,hex(actual),hex(guest_words[j])));packet_words+=1
                else:check(actual==0,'undefined native word is deterministic zero')
        out=(U*4)();check(getctx(0,out)==0 and list(out)==list(struct.unpack('<4I',read(CTX+144,16))),'context header state')
        check(color[0]==word(COLOR),'context color mode')
        for i,j in enumerate(indices):
            check(word(CURSORS+4*j)-guest_bases[i]==used(i),'guest/native committed list offset')
            if data(i):check(cursor(i)==descriptor(i)==data(i)+used(i),'native cursor ownership/commit')
    check(init(1<<24)==0,'queue init')
    for bank in range(2):
        for texture in [0,0x80000000]:
            for trans in [0,64]:
                for n in range(3,33):scenario(n,texture|trans|32,bank,n+bank*37,policy=n%20)
                for n in [*range(2,34),63,64,65,129]:scenario(n,texture|trans,bank,n+bank*37,policy=n%20)
    sections['complete_original_polygon_draws']=original_calls
    # Mixed lists, growing allocations and fresh headers across multiple draws.
    start_frame()
    for i in range(48):scenario(3+i%29,(0x80000000 if i%2 else 0)|(64 if i%3 else 0)|(32 if i%4 else 0),i%2,100+i,append=True,policy=i%20)
    sections['append_draws']=48
    for i in range(16):scenario(3+i%5,(0x80000000 if i%2 else 0)|(32 if i%4<2 else 0)|(64 if i%3 else 0),i%2,200+i,policy=i%20)
    sections['finite_coordinate_extreme_draws']=16
    # Every bit in a mode is tested independently, plus arbitrary masks and
    # control flags. These execute the complete original setter as well.
    for i in range(1024):
        start_frame();header=[rng.getrandbits(32) for _ in range(4)];m=(1<<(i%32)) if i<64 else rng.getrandbits(32);flags=[0,0x400,0x1000,0x1400,rng.getrandbits(32)][i%5]
        context=i%len(color);check(select(context)==0,'select context');iw(symbols['_nj_color_mode_'],[COLOR+4*context])
        check(setctx(context,(U*4)(*header))==0,'color context');iw(CTX+144,header);control.value=flags;iw(symbols['_nj_control_3d_flag_'],[flags]);get();original('njGetkmPointer',i%2)
        mode(m);original('njSetkmColorMode',i%2,arg=m);put();original('njSetkmPointer',i%2)
        result=(U*4)();check(getctx(context,result)==0 and list(result)==list(struct.unpack('<4I',read(CTX+144,16))),'complete color mode header contract')
        check(color[context]==word(COLOR+4*context)==m,'complete color mode side effect');color_calls+=1
    sections['original_color_mode_calls']=color_calls
    # Standalone picture helper value check, independent of packets/SH-4.
    for i in range(1024):
        aspect.value=rng.uniform(0.25,2.);anchor.value=rng.uniform(-640.,640.);pillar.value=i%2;raw=fw(rng.uniform(-1000.,2000.))
        check(fw(wide(fv(raw)))==wide_expected(raw),'independent wide-picture RZ value')
    sections['independent_wide_values']=1024
    # Malformed callers and allocation failures cannot emit or record draws.
    start_frame();pts=(Point*33)();cols=(U*33)();tex=(U*33)();poly=Poly(C.addressof(pts),C.addressof(cols),C.addressof(tex),33)
    for ptr,n,attr in [(None,3,32),(C.byref(poly),-1,0),(C.byref(poly),0,0),(C.byref(poly),1,0),(C.byref(poly),2,32),(C.byref(poly),33,32),(C.byref(poly),0x40000000,0),(C.byref(poly),0x20000000,0x80000000)]:
        before=rec_calls();check(draw(ptr,n,fw(-2.),attr)==-1,'argument preflight');check(rec_calls()==before and used(0)==used(1)==0 and status()==0,'invalid input has no effects')
    for field,attr in [('p',0),('col',0),('tex',0x80000000)]:
        old=getattr(poly,field);setattr(poly,field,None);before=rec_calls();check(draw(C.byref(poly),3,0,attr)==-1 and rec_calls()==before,'null array preflight');setattr(poly,field,old)
    shutdown();check(init(128)==0,'small budget');before=rec_calls();check(draw(C.byref(poly),3,0,32)==4,'whole-batch allocation rejection')
    check(used(0)==used(1)==0 and rec_calls()==before,'budget failure has no partial packet/record');clear();check(reset()==0,'budget failure recoverable')
    check(reserve(0,2)==0,'active test reserve');get();begin=bind('njStartVertexNonTexOpaque',P);end=bind('njEndVertex',None,P);v=begin();before=rec_calls()
    check(draw(C.byref(poly),3,0,32)==2 and rec_calls()==before,'cannot interleave raw producer');clear();end(v);put();check(reset()==0,'active-state failure recoverable')
    shutdown();check(names==list(addresses.values()) and dependencies==set(names),'all real dependency bodies executed')
    check(fm==0,'original polygon arithmetic is unfused')
    report=dict(status='complete_native_polygon2d_packet_contract_verified',checks=checks,defined_packet_words_compared=packet_words,original_polygon_calls=original_calls,original_color_mode_calls=color_calls,sections=sections,
        original_function_bodies_executed=sorted(dependencies),original_dependency_hooks_used=False,banks_tested=sorted(banks),mode_combinations_tested=sorted(flags_tested),filled_counts_tested=sorted(n for f,n in counts if f),outline_counts_tested=sorted(n for f,n in counts if not f),
        original_binary_sha256=sha(a.binary),external_symbols_sha256=sha(a.upstream/'config/1st_read/symbols.txt'),numeric_DLL_sha256=sha(a.numeric_dll),DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),reference_hashes=reference_hashes,
        GCC_aspect_policy='independent Python RZ oracle from pinned GCC source; not a JP binary feature',diagnostic_replay_observer=True,production_replay_executed=False,
        raw_input_arrays_preserved=True,full_width_pointers=True,queue_bound_and_guards_checked=True,undefined_packet_words_zeroed=True,
        original_FMAC_instructions=0,numerical_policy='RZ_DN1_exceptions_disabled_value_contract',FPSCR_flags_and_traps_compared=False,hardware_validation=False,
        class_service_manual='https://www.renesas.com/en/document/mas/sh-4-software-manual',class_service_manual_sections=['9.26 FADD','9.30 FDIV','9.39 FMUL','9.45 FSUB'],
        verifier_helper_sha256={n:sha(Path(__file__).with_name(n)) for n in ['primitive_reference.py','camera_reference_numeric.py']},
        rasterization_verified=False,full_frame_executed=False,gameplay_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks;',original_calls,'whole original draws;',color_calls,'original color setter calls')

if __name__=='__main__':main()
