"""Original/native full-state comparisons plus independent finite geometry oracles."""
import ast,collections,hashlib,itertools,json,math,struct,sys
from fractions import Fraction
from pathlib import Path
import write_watch_collision_math as fast
v=fast.old;P=v.P;CROSS,LENGTH=0x8c027360,0x8c63a75c
A,B,C,OUT=0x8cc00000,0x8cc00100,0x8cc00200,0x8cc00300

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def raw(m,a,b):m[a&0xffffff:(a&0xffffff)+len(b)]=b
def fwords(xyz):return [bits(x) for x in xyz]

def configurations():
    geometries=[((0,0,0),(1,0,0),(0,1,0)),((1,2,3),(4,2,1),(1,6,5)),((-2,1,3),(1,-3,2),(4,0,-1)),((3,3,3),(3,3,3),(4,5,6)),((1,0,-1),(-1,2,3),(5,-2,0))]
    for bank,rm,dn,xyz,alias in itertools.product(range(2),range(2),range(2),geometries,('separate','out_a','out_b','out_c','out_a4','b_a','c_a','b_c')):
        yield dict(kind='geometry',entry=CROSS,fpscr=(bank<<21)|(dn<<18)|rm,xyz=xyz,alias=alias,view='p1',oracle=True)
    vectors=[(3,4,0),(0,0,0),(1,0,0),(1,2,2),(2,3,6),(1,1,1),(.5,.5,.5),(-3,4,0)]
    for bank,rm,dn,xyz in itertools.product(range(2),range(2),range(2),vectors):
        yield dict(kind='norm',entry=LENGTH,fpscr=(bank<<21)|(dn<<18)|rm,xyz=(xyz,(3,0,0),(0,4,0)),alias='separate',view='p1',oracle=True)
    for entry,bank,sz,pr,dn,rm,en in itertools.product((CROSS,LENGTH),range(2),range(2),range(2),range(2),range(2),(0,0x80,0xf80)):
        yield dict(kind='mode',entry=entry,fpscr=(bank<<21)|(sz<<20)|(pr<<19)|(dn<<18)|rm|en|0x3f07c,xyz=((3,4,0),(5,-2,1),(-1,2,3)),alias='separate',view='p1',oracle=False)
    values=(0,0x80000000,0x3f800000,0xbf800000,1,0x80000001,0x7fffff,0x800000,0x7f7fffff,0x7f800000,0xff800000,0x7fbfffff,0x7fc00001,0x800001,0x40000000,0x12345678)
    for entry,bank,dn,rm,x in itertools.product((CROSS,LENGTH),range(2),range(2),range(2),values):
        yield dict(kind='edge',entry=entry,fpscr=(bank<<21)|(dn<<18)|rm,raw_xyz=((x,bits(1),bits(2)),(bits(3),x,bits(-1)),(bits(.5),bits(-2),x)),alias='separate',view='p1',oracle=False)
    for entry,bank,sz,guard in itertools.product((CROSS,LENGTH),range(2),range(2),('bad_a','edge_a','bad_b','bad_c','bad_out','edge_out')):
        if entry==LENGTH and guard not in ('bad_a','edge_a'):continue
        yield dict(kind='guard',entry=entry,fpscr=(bank<<21)|(sz<<20)|0x40000,xyz=((3,4,0),(5,-2,1),(-1,2,3)),alias='separate',view='p1',guard=guard,oracle=False)
    for entry,bank,rm,view in itertools.product((CROSS,LENGTH),range(2),range(2),('p0','p1','p2')):
        yield dict(kind='view',entry=entry,fpscr=(bank<<21)|0x40000|rm,xyz=((3,4,0),(5,-2,1),(-1,2,3)),alias='separate',view=view,oracle=True)
    for entry,rm in itertools.product((CROSS,LENGTH),(2,3)):
        yield dict(kind='reserved_rm',entry=entry,fpscr=0x40000|rm,xyz=((3,4,0),(5,-2,1),(-1,2,3)),alias='separate',view='p1',oracle=False)

def main():
    assert v.effects.DLL.name=='collision_math.dll'
    configs=[dict(c,case_id=i) for i,c in enumerate(configurations())]
    shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
    if shard is not None:assert shard in range(3);configs=[c for i,c in enumerate(configs) if i%3==shard]
    case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
    if case_id is not None:configs=[c for c in configs if c['case_id']==case_id];assert len(configs)==1
    smoke='--smoke' in sys.argv
    if smoke:configs=[next(c for c in configurations() if c['entry']==entry and c['oracle'])|dict(case_id=-entry) for entry in (CROSS,LENGTH)]
    # Extract only the independent rational sqrt oracle definitions. Importing
    # that historical script would rerun it and rewrite its old proof file.
    oracle_path=P/'verify_native_sh4_sqrt.py';tree=ast.parse(oracle_path.read_text());nodes=[n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name in ('value','oracle')];assert len(nodes)==2
    ns=dict(Fraction=Fraction,math=math,struct=struct);exec(compile(ast.Module(body=nodes,type_ignores=[]),str(oracle_path),'exec'),ns);sqrt_oracle=ns['oracle']
    ram=v.seed_ram();records=[];seen=set();counts=collections.Counter();checks=independent_cross=independent_norm=0
    for config in configs:
        case_checks=0
        def require(ok,label):
            nonlocal checks,case_checks
            assert ok,(config,label);checks+=1;case_checks+=1
        entry=config['entry'];fpscr=config['fpscr'];mode=(fpscr>>19)&7
        cfg=v.Input(dict(entry=entry,mode=mode,probe=True,sr=0x400000a0|(config['case_id']&1),mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,collision_math_arguments=[A,B,C,OUT]))
        m,s,f=v.prepare(ram,cfg)
        for j in range(16):s.r[j]=(0x12345678^((j+1)*0x01030507))&0xffffffff
        s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=entry;s.sr=cfg['sr']
        for bank in range(2):
            for j in range(16):f.banks[bank][j]=bits(.5+(bank*16+j)/16)
        f.fpscr=fpscr;f.fpul=0xc0ffee42;cfg['fpul']=f.fpul
        xyz=config.get('raw_xyz') or [fwords(x) for x in config['xyz']]
        for address,words in zip((A,B,C),xyz):raw(m,address-16,bytes([0x6d])*128);raw(m,address,b''.join(x.to_bytes(4,'little') for x in words)*4)
        raw(m,OUT-16,bytes([0xa7])*128)
        addresses=[A,B,C,OUT];alias=config['alias']
        if alias.startswith('out_'):addresses[3]={'out_a':A,'out_b':B,'out_c':C,'out_a4':A+4}[alias]
        if alias=='b_a':addresses[1]=A
        if alias=='c_a':addresses[2]=A
        if alias=='b_c':addresses[1]=C
        guard=config.get('guard')
        if guard:
            slot={'bad_a':0,'edge_a':0,'bad_b':1,'bad_c':2,'bad_out':3,'edge_out':3}[guard];addresses[slot]=0x8cfffffc if guard.startswith('edge_') else 0x8b000000
        top={'p0':0,'p1':0x80000000,'p2':0xa0000000}[config['view']]
        if not guard:addresses=[(a&0x1fffffff)|top for a in addresses]
        s.r[4:8]=addresses
        initial_s=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f);before=bytes(m)
        points=[[Fraction.from_float(number(v.load(m,a+j*4))) for j in range(3)] for a in addresses[:3]] if config['oracle'] else None
        r,path,a,s,f=fast.compare(m,s,f,cfg)
        require(r['full_state_match'],'entire original/native CPU/FPU/RAM and device state');require(r['ordered_boundaries_match'],'ordered original/native boundaries');require(r['status'] in (0,1,-1,-3),'completed/trapped/unsupported/mapped-memory status')
        require([b['target'] for b in r['boundaries']]==[hex(entry)],'leaf entry only;no substituted original game services')
        require(s.pr==initial_s.pr and s.r[15]==initial_s.r[15] and s.sr==initial_s.sr and f.fpul==initial_f.fpul,'unaffected PR/SP/SR/FPUL')
        require((r['final_mach'],r['final_macl'],r['final_gbr'])==(cfg['mach'],cfg['macl'],cfg['gbr']),'unaffected integer special state')
        wanted=bytearray(before)
        if entry==CROSS and addresses[3]&0x1f000000==0x0c000000:
            offset=addresses[3]&0xffffff;size=min(16 if fpscr&0x100000 else 12,len(wanted)-offset)
            wanted[offset:offset+size]=a[offset:offset+size]
        require(a==wanted,'only permitted original output footprint;all other RAM unchanged')
        if config['oracle']:
            require(r['status']==0,'ordinary independent finite geometry completes')
            bank=(fpscr>>21)&1;regs=list(initial_s.r)
            if entry==CROSS:
                p,q,z=points;u=[q[j]-p[j] for j in range(3)];w=[z[j]-p[j] for j in range(3)];expected=(u[1]*w[2]-u[2]*w[1],u[2]*w[0]-u[0]*w[2],u[0]*w[1]-u[1]*w[0])
                got=[Fraction.from_float(number(v.load(a,addresses[3]+j*4))) for j in range(3)];require(tuple(got)==expected,'independent exact small-dyadic geometric cross product,including aliases')
                regs[0]=8
                for j in (4,5,6):regs[j]+=8
                changed={2,3,4,5,6,7,8,9};independent_cross+=1
            else:
                square=sum((x*x for x in points[0]),Fraction());square_bits=bits(float(square));require(Fraction.from_float(number(square_bits))==square,'independent chosen squared norm is exactly representable')
                expected,inexact=sqrt_oracle(square_bits,fpscr&3);require(f.banks[bank][0]==expected,'independent rational squared-bracket/midpoint length oracle')
                require(f.banks[bank][7]==expected,'original result also remains in scratchFR7');regs[4]+=12;changed={0,4,5,6,7};independent_norm+=1
            require(list(s.r)==regs,'independent exact integer register effects')
            require(all(f.banks[b][j]==initial_f.banks[b][j] for b in range(2) for j in range(16) if b!=bank or j not in changed),'unaffected physical FP registers/bank')
            require((f.fpscr&~0x3f07c)==(fpscr&~0x3f07c),'floating control/bank/mode bits preserved')
        if guard:require(r['status']==-3,'independent reached mapped-RAM guard')
        if config['kind']=='reserved_rm':require(r['status']==-1,'existing reserved-rounding unsupported policy')
        seen.update(path);counts[r['status']]+=1;records.append(dict(configuration=config,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),full_state_match=r['full_state_match'],ordered_boundaries_match=r['ordered_boundaries_match'],independent_checks=case_checks))
        if len(records)%32==0:print('COLLISION MATH',shard,len(records),dict(counts),flush=True)
    coverage=[]
    for body in v.ANALYSIS_ROUTINES:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=0,missing_rows=sorted(missing),missing_outcomes=[]))
    files=[Path(__file__),P/'collision_math_reference.py',P/'write_watch_collision_math.py',v.MP,P/'analysis/collision_math_connection.json',v.effects.DLL,oracle_path,P/'analysis/native_sh4_sqrt_verification.json',P/'analysis/sh4_vector_numeric_verification.json',P/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h']
    report=dict(existing_numeric_probe_mode=True,original_native_comparisons=len(records),independent_checks=checks,independent_cross_oracle_cases=independent_cross,independent_length_oracle_cases=independent_norm,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Both original leaf functions,fullCPU/FPU/RAM/ordered-state comparisons. Independent bounded dyadic cross geometry and exact-rational sqrt length cases,aliases,bothphysicalbanks/SZ/PR/DN,RM0/1plusreservedguards,selectedenable/flagcombinations,NaN/infinity/denormal/zero/overflow inputs,MMU-off mappedP0/P1/P2 views andRAMguard prefixes. Existing numerical backend stop/trap policies retained;unsupported inputs are not fabricated completions. SR.FD isclear bydeclaredcallABI;physicalFPUdisabled/exception/address-error/MMUdelivery unqualified. FIPR instruction plumbing uses pinned shared vendor core;independent finite oracles doNOTprove arbitraryhardwarebitpatterns. No upstream game implementation imported.')
    suffix='_'+str(shard) if shard is not None else '';dest=P/'analysis'/('collision_math_direct'+suffix+'_verification.json')
    if smoke:dest=P.parents[1]/'work/sonic-adventure/collision_math_direct_smoke.json'
    if case_id is not None:dest=P.parents[1]/'work/sonic-adventure'/('collision_math_case_'+str(case_id)+'.json')
    dest.write_text(json.dumps(report,indent=2)+'\n');print('COLLISION MATH DIRECT PASS',len(records),checks,dict(counts),flush=True)
if __name__=='__main__':main()
