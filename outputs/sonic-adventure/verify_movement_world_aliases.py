"""Actual original identity-loader overlap ordering, including paired transfers."""
import hashlib,itertools,json
from pathlib import Path
import movement_world_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=q.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for mode,offset in itertools.product(range(8),(-32,0,16,32,64)):
        c=dict(entry=0x8c63a8e0,explicit=True,mode=mode,kind='identity_alias',offset=offset)
        m,s,f,cfg=q.prepare(ram,c);s.r[4]=q.IDENTITY+offset
        # Output is a caller pointer; bytes are authored before execution. The
        # source identity table is the original RAM image, not injected results.
        expected=bytearray(m);cursor=q.IDENTITY+offset;source=q.IDENTITY
        size=8 if f.fpscr&0x100000 else 4
        # Two fully latched half-blocks, each stored in descending address order.
        for _ in range(2):
            cursor+=32;latched=[]
            for j in range(8):
                a=(source+j*size)&0xffffff;latched.append(bytes(expected[a:a+size]))
            source+=8*size
            for raw in reversed(latched):
                cursor-=size;a=cursor&0xffffff;expected[a:a+size]=raw
            if _==0:cursor+=32
        before_s=type(s).from_buffer_copy(s);before_f=type(f).from_buffer_copy(f)
        r,path,actual,s,f=q.fast.compare(m,s,f,cfg)
        require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'complete original/native state and ordered boundaries')
        require(actual==expected,'independent two-half-block memory ordering under overlap')
        require(s.pc==s.pr==before_s.pr and s.r[15]==before_s.r[15],'return/stack')
        require(f.fpscr==before_f.fpscr,'raw transfers preserve FPSCR');
        records.append(dict(configuration=c,status=r['status'],steps=len(path),boundaries=r['boundaries']))
    files=[Path(__file__),P/'movement_world_fixtures.py',q.v.MP,P/'analysis/movement_world_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='40 identity-loader output/source overlap cases,all8PR/SZ/FRprofiles. Independent full-memory two-half-block ordering oracle;caller provides only output pointer. Raw transfer semantics,not geometric all-modes matrix identity. No supplied callee outputs/frames;no physical memory/exception proof.')
    (P/'analysis/movement_world_aliases_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('WORLD ALIASES PASS',len(records),checks,flush=True)

if __name__=='__main__':main()
