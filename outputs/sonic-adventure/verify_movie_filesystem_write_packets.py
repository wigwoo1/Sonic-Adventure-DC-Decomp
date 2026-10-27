"""Actual SDK write/commit locks with independent packet-format oracles."""
import itertools,types
from pathlib import Path
from movie_filesystem_write_vectors import *
import movie_channel_request_fixtures as fixtures
import verify_movie_maple as packet
ENTRIES=(0x8c6469f4,0x8c646a28,0x8c660546,0x8c6605b0,0x8c660580,0x8c6605e2)
DATA={0x8c6469f4,0x8c660546,0x8c660580};LOCKED={0x8c6469f4,0x8c646a28,0x8c660546,0x8c6605b0};SDK={0x8c6469f4,0x8c646a28};SRC=0x8cc7d000
def oracle(m,s,entry,stack,words):
    copied=bytearray(m);pseudo=type(s).from_buffer_copy(s);payload=0x8cc7e000;sp=payload+32;command=12 if entry in DATA else 13
    put(copied,payload,s.r[5]);put(copied,payload+4,((s.r[6]<<24)|(s.r[7]<<16)|stack[0])&0xffffffff)
    put(copied,sp,SRC if entry in DATA else 0);put(copied,sp+4,words if entry in DATA else 0)
    pseudo.r[5]=command;pseudo.r[6]=payload;pseudo.r[7]=2;pseudo.r[15]=sp
    fn=types.FunctionType(packet.packet_oracle.__code__,dict(packet.packet_oracle.__globals__,CTX=fixtures.CTX),'independent_write_packet')
    expected,result=fn(copied,pseudo,dict(entry=packet.COMMAND,second_words=words if entry in DATA else 0))
    return expected,result,command
def main():
    assert v.effects.DLL.name=='movie_filesystem_write.dll'
    e=Evidence();ram=v.seed_ram();base=Session(ram)
    def run(entry,unit,mode,sr,lock=0,busy=0,serial=1,order=0,bank=0,padding=0,previous=0,words=32,auto=0):
        m,s,f,c=fresh(base,entry,[unit,0x89abcdef,0xa5a5a5a5,0x12345678],mode,sr);fixtures.configure(m,unit,lock,busy,serial,order,padding,previous,bank,context=fixtures.CTX);put(m,fixtures.CTX+0x10e8,auto);raw(m,SRC,bytes((j*17+9)&255 for j in range(128)))
        if auto:
            # Same declared controller resources as the original Session's
            # constructor fixture. An automatic pump must have a real MMIO
            # base and packet banks, even when the new lock is contended.
            for off,value in ((0xfc0,1),(0x10f4,0xa05f6c00),(0x10a8,fixtures.FRAME),(0x10ac,fixtures.FRAME+0x10000),(0x10b8,order),(0x10b4,order)):put(m,fixtures.CTX+off,value)
            for mapped in (0,1):put(m,fixtures.CTX+0xf90+mapped,1,1)
            put(m,fixtures.CTX+4,0x1f,1);put(m,fixtures.CTX+5,0x1f,1)
            c['startup_overrides']={0x5f6c18:0};c['scan_samples']=[0x1122]
        stack=(0x12345678,SRC,words)
        for i,value in enumerate(stack):put(m,s.r[15]+4*i,value)
        expected,result,command=oracle(m,s,entry,stack,words);before=bytes(m);fp=bytes(f)
        r,a,s,f=e.compare(m,s,f,c,dict(entry=hex(entry),unit=unit,lock=lock,busy=busy,serial=serial,order=order,bank=bank,padding=padding,previous=previous,words=words,auto=auto,mode=mode))
        islock=entry in LOCKED;contended=islock and lock!=0
        e.require(s.r[0]==(0xfffffffe if contended else result),'independent API result')
        e.require(a[fixtures.LOCK&0xffffff]==(lock|0x80 if contended else 0 if islock else lock),'original mutex state')
        e.require(v.sample(a,fixtures.LOCK-3,3)==bytes([0xa5])*3 and v.sample(a,fixtures.LOCK+1,4)==bytes([0xa5])*4,'mutex neighbors')
        e.require(r['cache_purge_sequence']==([fixtures.LOCK] if islock else []),'mandatory new-lock TAS; constructed busy response gate skips preflight TAS')
        targets=[b['target'] for b in r['boundaries']]
        e.require(('0x8c65f348' in targets)==contended,'actual fatal diagnostic on lock contention')
        e.require(('0x8c646654' in targets)==bool(auto and entry in SDK),'actual automatic response pump selection')
        if auto:e.require(v.load(before,fixtures.CTX+0xfc0)==1 and '0x8c65f1aa' in targets,'original response-busy preflight gate and preserved API result')
        if not auto:
            wanted=before if contended else expected
            e.require(v.sample(a,fixtures.CTX,0x2000)==v.sample(wanted,fixtures.CTX,0x2000) and v.sample(a,fixtures.FRAME,512)==v.sample(wanted,fixtures.FRAME,512),'independent exact packet/context/serial/order/bank/padding/data bytes')
            e.require([b['target'] for b in r['boundaries']].count('0x8c65f810')==int(not contended),'actual common packet builder count')
        e.require(v.sample(a,SRC,128)==v.sample(before,SRC,128),'caller data unchanged')
        e.require(bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'] and (s.sr&~1)==(sr&~1),'FPU/unaffected integer status/special state')
    for entry,lock in itertools.product((0x8c660546,0x8c6605b0),range(256)):
        mode,sr=PROFILES[lock%4];run(entry,1,mode,sr,lock=lock)
    print('WRITE LOCK ALLBYTES',len(e.records),flush=True)
    formats=((0,1,0,0,0,0,0),(0,0x7fffffff,1,1,0,0,1),(0,0x12345678,0,0,2,1,32),(0xff,1,1,1,0,0,2))
    for entry,unit,(mode,sr),(busy,serial,order,bank,padding,previous,words) in itertools.product(ENTRIES,(1,20),PROFILES,formats):run(entry,unit,mode,sr,busy=busy,serial=serial,order=order,bank=bank,padding=padding,previous=previous,words=words)
    print('WRITE PACKETS',len(e.records),flush=True)
    for entry,(mode,sr),auto in itertools.product(SDK,PROFILES,(0,1)):run(entry,1,mode,sr,lock=0x80,auto=auto)
    e.report('movie_filesystem_write_packets_verification.json',set(ENTRIES),[Path(__file__),Path(fixtures.__file__),Path(packet.__file__),P/'filesystem_write_reference_cpu.py'],scope='Actual SDK wrappers/mutex guards/data0C andcommit0D builders withreal common65f810 anddiagnostic/autopump callees. Independent packet/context/data/serial/order/bank/padding/result/mutexneighbors/TASpurge oracles;all256mutexbytes eachnewlock,twounits/fourprofiles/zeroone32datawords/busy/serialwrap. Autopump branch exercisedwithreal originalpump andcontendednewlock;fullnative/original state/order checks remainstrict. Verification-only TAS event whitelist addsnew54c/5b6 sites,integer/memoryexecution unchanged. Declared coherentSDKresources,not hardware/threadatomicity orfullwritebackend/callbackdomain qualification.')
if __name__=='__main__':main()
