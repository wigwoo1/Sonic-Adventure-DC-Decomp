"""Original delete gateway guard tree, with declared coherent ABI inputs."""
import itertools,json
from pathlib import Path
from movie_filesystem_command8_vectors import *
GATEWAY=0x8c649f1e

def main():
    e=Evidence();ram=v.seed_ram();parent=P/'analysis/movie_filesystem_command8_translation.json';manifest=json.loads(parent.read_text());body=next(b for b in manifest['routines'] if b['address']==hex(GATEWAY))
    for channel in (0,7):
        base=Session(ram,channel=channel)
        for kind,(mode,sr) in itertools.product(('inactive','absent','function_off','busy','cache_off','bad_root','latched_root'),PROFILES):
            m,s,f,c=fresh(base,GATEWAY,[channel,OUT,OUT+32],mode,sr);raw(m,OUT,b'MISSING'+bytes(6));raw(m,OUT+32,b'NEW'+bytes(10))
            descriptor=base.descriptor;root=v.load(m,descriptor+0x20)
            # Coherent mapped helper inputs, not claims of natural acquisition.
            put(m,descriptor+2,int(kind!='inactive'),1);put(m,descriptor+8,int(kind!='cache_off'),1)
            put(m,base.d['header'],int(kind!='absent'),1);put(m,base.d['status'],int(kind!='absent'),1)
            put(m,base.d['status']+0xc,int(kind=='busy'),1);put(m,base.d['info'],0 if kind=='function_off' else 2)
            put(m,0x8c89cf90+channel*4,int(kind=='latched_root'))
            data=bytearray(512);data[:16]=b'U'*16
            if kind=='bad_root':data[7]=0
            raw(m,root,data)
            # Sentinel cache bytes expose exactly the original invalidation.
            for off,val in ((9,0x5a),(10,0xa5),(11,0x33),(12,0x77)):put(m,descriptor+off,val,1)
            before=bytearray(m);sp=s.r[15];regs=list(s.r);initial_fpu=bytes(f)
            counter=v.load(m,v.load(m,0x8c649ff8)+v.load(m,descriptor+4)*80)
            wanted=bytearray(before);put(wanted,descriptor+0x30,1);put(wanted,descriptor+0x34,counter)
            cleanup=kind in ('absent','function_off')
            if cleanup:
                for off in (8,9,10,11,12):put(wanted,descriptor+off,0,1)
                put(wanted,descriptor+4,0)
            if kind=='bad_root':put(wanted,descriptor+11,0,1);put(wanted,descriptor+12,0,1)
            result=-253 if kind in ('bad_root','latched_root') else -255
            r,a,s,f=e.compare(m,s,f,c,dict(kind=kind,channel=channel,mode=mode,sr=hex(sr)))
            # Existing original/native comparison still checks the entire stack.
            # This independent projection bounds only legitimate ABI scratch.
            raw(wanted,sp-132,v.sample(a,sp-132,132));e.require(a==wanted,'exact nonstack RAM/cache/counters and132byte nested ABI frame bound')
            e.require(s.r[0]==result&0xffffffff,'independent guard error')
            e.require(bytes(f)==initial_fpu and list(s.r[8:])==regs[8:],'all FPU/callee GPR state')
            targets=[b['target'] for b in r['boundaries'] if b['target'] in ('0x8c649f1e','0x8c660dac','0x8c660dec','0x8c64850a','0x8c602e9c','0x8c64a4b2')]
            expected=['0x8c649f1e']+([] if kind=='inactive' else ['0x8c660dac'])+(['0x8c64850a'] if cleanup else [] if kind=='inactive' else ['0x8c660dec'])+(['0x8c602e9c','0x8c64a4b2','0x8c660dac','0x8c660dec'] if kind in ('bad_root','latched_root') else [])
            e.require(targets==expected,'exact guard/canonical-root call order')
            e.require(not any('write_request_input' in b for b in r['boundaries']) and not r['cache_purge_sequence'],'no I/O after rejected delete')
        print('COMMAND8 DIRECT',channel,len(e.records),flush=True)
    missing=set(body['reachable_addresses'])-{hex(pc) for pc in e.seen};outcomes=set(map(tuple,body['conditional_edges']))-e.edges
    report=dict(original_native_comparisons=len(e.records),independent_checks=e.checks,cases=e.records,coverage_by_routine=[dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes))],dll_sha256=digest(v.effects.DLL),scope='Direct gateway ABI only;coherent explicitlydeclared admission/presence/function/busy/cache/root/latch inputs,not natural acquisition. Original constructors andallstatus/readiness/invalidation/root callees execute;no suppliedcalleeoutput. ExactnonstackRAM/counters/callorder/132byte maxABIframe/FPU/calleeGPR checks plusoriginal/nativefullstate comparison. New234-source native rename with unchanged guard callees;not full rename-path coverage alone.',sha256={str(p):digest(p) for p in (Path(__file__),parent,P/'movie_filesystem_command8_vectors.py',P/'analysis/movie_filesystem_command8_connection.json',P/'analysis/movie_filesystem_command8_unbounded.json',v.effects.DLL)})
    (P/'analysis/movie_filesystem_command8_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND8 DIRECT PASS',len(e.records),e.checks,flush=True)
if __name__=='__main__':main()
