"""Whole-state ABI qualification, independent query oracle and frozen old CPU leaf."""
import collections,ctypes,hashlib,itertools,json
from pathlib import Path
import movement_mode_query_fixtures as q
P=q.P;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def configurations():
    for word,mode,t,fd in itertools.product((0,1,0xffffffff,0x80000000,0x39a52e71),range(8),(0,1),(False,True)):
        yield dict(kind='ram',word=word,mode=mode,t=t,fd=fd)
    for target,mode,t in itertools.product((q.SLOT,q.ENTRY,q.ENTRY+2,0x8cfffffc,0xacb40000,0x0cb40000,0x8cb40001),(0,7),(0,1)):
        yield dict(kind='ram_alias',target=target,mode=mode,t=t,store_word=target not in (q.SLOT,q.ENTRY,q.ENTRY+2),word=0)
    for target,mode in itertools.product((0xa05f8000,0xa05f6920,0xff000038,0xffa00020,0xffd8000c),(0,7)):
        yield dict(kind='mmio',target=target,mode=mode,store_word=False)
    for target,mode in itertools.product((0x8b000000,0x8cfffffe,0xffffffff,0),(0,7)):
        yield dict(kind='guard',target=target,mode=mode,store_word=False)
    for pr,mode in itertools.product((0,0x8c010000,0x8cfed000),(0,7)):
        yield dict(kind='root_return',word=1,pr=pr,mode=mode)

def main():
    legacy_source=P/'source/sa_global_mode_nonzero_8c049a4e.c';legacy_proof=P/'analysis/global_mode_nonzero_8c049a4e_verification.json'
    proof=json.loads(legacy_proof.read_text());assert digest(legacy_source)==proof['source_sha256']
    legacy_dll=ROOT/'work/sonic-adventure/build/global_mode_nonzero_8c049a4e.dll';library=ctypes.CDLL(str(legacy_dll))
    legacy=library.sa_global_mode_nonzero_8c049a4e
    ram=q.v.seed_ram();records=[];checks=old_cases=0;seen=set();counts=collections.Counter()
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for i,c in enumerate(configurations()):
        c=dict(c,case_id=i);m,s,f,cfg=q.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s);initial_f=bytes(f)
        r,path,actual,s,f=q.fast.compare(m,s,f,cfg)
        require(r['full_state_match'] and r['ordered_boundaries_match'],'original/native full CPU/FPU/RAM/device/queue and ordered boundaries')
        require([b['target'] for b in r['boundaries']]==[hex(q.ENTRY)],'actual original leaf entry;no game callee results')
        require(actual==before and bytes(f)==initial_f,'query makes no RAM or FPU changes')
        require(s.r[3]==q.get(before,q.SLOT),'literal pointer left in original R3')
        if c['kind']=='guard':
            require(r['status']==-3 and r['stop_pc']=='0x8c049a50','original reached pointer read guard')
            require(s.r[0]==initial.r[0] and s.sr==initial.sr,'failed read leaves R0/T unchanged')
        else:
            require(r['status']==0 and s.pc==s.pr==initial.pr,'ordinary return to captured root PR')
            require(s.r[0] in (0,1) and (s.sr&1)==(s.r[0]^1),'nonzero result and inverse T flag')
            if c['kind']!='mmio':
                target=q.get(before,q.SLOT);value=q.get(before,target)
                require(s.r[0]==int(value!=0) and s.sr==(initial.sr&~1)|int(value==0),'independent raw-word query and preserved SR')
                old=type(initial).from_buffer_copy(initial);old_ram=bytearray(before);view=(ctypes.c_uint8*len(old_ram)).from_buffer(old_ram)
                legacy.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(type(old))];legacy.restype=None;legacy(view,ctypes.byref(old))
                require(bytes(old)==bytes(s) and old_ram==before,'unchanged standalone canonical implementation agrees on ordinary RAM');old_cases+=1
        require(all(s.r[j]==initial.r[j] for j in range(16) if j not in (0,3)),'all other integer registers including stack preserved')
        require(s.pr==initial.pr,'PR preserved')
        seen.update(path);counts[r['status']]+=1
        records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],result=s.r[0],original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('MODE QUERY DIRECT',len(records),dict(counts),flush=True)
    body=q.v.ANALYSIS_ROUTINES[0];wanted={int(a,16) for a in body['reachable_addresses']};assert wanted<=seen and len(wanted)==6 and body['conditional_edges']==[]
    files=[Path(__file__),P/'movement_mode_query_fixtures.py',P/'movement_mode_query_reference.py',P/'write_watch_movement_mode_query.py',q.v.MP,P/'analysis/movement_mode_query_connection.json',q.v.effects.DLL,legacy_source,legacy_proof,legacy_dll]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,unchanged_standalone_CPU_comparisons=old_cases,status_counts=dict(counts),cases=records,
      coverage_by_routine=[dict(address=hex(q.ENTRY),static_rows=6,executed_rows=6,static_outcomes=0,executed_outcomes=0,missing_rows=[],missing_outcomes=[])],
      new_original_routine_credit=0,canonical_duplicate_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},
      scope='Canonical049a4e adapted to current full-state ABI with preserved standalone source/library/proof. Raw zero/nonzero words,all8FPU bank/PR/SZ modes,T/FD states,P0/P1/P2RAM pointers,code/literal/self reads,lastalignedword,unalignedsoftwareRAM,rootPRs,selectedrealMMIOandguards. No stores/callees. Original/native full state and independent RAM predicate/SR/GPR/FPU/footprint oracles;oldstandalone CPU comparisons only for mappedRAM. Physicalalignment/FPU/MMU/exception/devicehardware andgameplayunqualified.')
    (P/'analysis/movement_mode_query_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MODE QUERY DIRECT PASS',len(records),checks,old_cases,dict(counts),flush=True)

if __name__=='__main__':main()
