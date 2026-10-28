"""Explicit mapped device-presence transition input; original code and returns unchanged."""
import ctypes,types
from pathlib import Path
from movie_filesystem_command12_vectors import *
import write_watch_filesystem_command12 as fast
EVENTS=[]

def ref_presence(memory,c):
    addresses=c['close_presence_transition']
    for address in addresses:assert memory[address&0xffffff]==1;memory[address&0xffffff]=0
    EVENTS.append(dict(engine='original',addresses=list(map(hex,addresses))))

def native_presence(mem,c):
    addresses=c['close_presence_transition'];base=ctypes.addressof(mem.contents)
    for address in addresses:
        cell=ctypes.c_uint8.from_address(base+(address&0xffffff));assert cell.value==1;cell.value=0
    EVENTS.append(dict(engine='native',addresses=list(map(hex,addresses))))

def install():
    code=fast.code;marker='  if pc in tracked and not (pc in PACKET_INTERNAL_ENTRY_ROWS'
    assert code.count(marker)==1;code=code.replace(marker,"  if pc==0x8c660dac and pr==0x8c6491f8 and c.get('close_presence_transition'):ref_presence(memory,c)\n"+marker)
    fast.namespace['ref_presence']=ref_presence;exec(compile(code,'mapped_presence_input_original','exec'),fast.namespace)
    wrapper=fast.effects.reference_run;fast.effects.reference_run=types.FunctionType(wrapper.__code__,dict(wrapper.__globals__,raw_reference=fast.namespace['reference_run']),wrapper.__name__,wrapper.__defaults__)
    code=fast.purge_code;marker=' def observe(ctx,target,mem,fp,sr,r,pr):'
    assert code.count(marker)==1;code=code.replace(marker,marker+"\n  if target==0x8c660dac and pr[0]==0x8c6491f8 and c.get('close_presence_transition'):native_presence(mem,c)")
    ns=dict(fast.compare.__globals__,native_presence=native_presence);exec(compile(code,'mapped_presence_input_native_observer','exec'),ns);fast.compare=ns['compare']

def main():
    install();e=Evidence();ram=v.seed_ram();transition_cases=[]
    for channel in (0,7):
        base=Session(ram,channel=channel)
        for mode,sr in PROFILES:
            m,s,f,c=fresh(base,0x8c64919e,[A,1],mode,sr);d=base.descriptor
            raw(m,A,bytes([0x6c])*72);put(m,A+0x28,channel);put(m,A+4,1)
            for off in (2,8,10,11,12):put(m,d+off,1,1)
            put(m,base.d['header'],1,1);put(m,base.d['status'],1,1);put(m,base.d['status']+0xc,0,1);put(m,base.d['info'],2);put(m,0x8c89cf90+channel*4,0);raw(m,v.load(m,d+0x20),b'U'*16+bytes(496))
            c['close_presence_transition']=tuple(dict.fromkeys((base.d['header'],base.d['status'])))
            wanted=bytearray(m);sp=s.r[15];raw(wanted,A,bytes(72))
            for address in c['close_presence_transition']:put(wanted,address,0,1)
            for off in (8,9,10,11,12):put(wanted,d+off,0,1)
            put(wanted,d+4,0);at=len(EVENTS);r,a,s,f=e.compare(m,s,f,c,dict(kind='presence_transition',channel=channel,mode=mode,sr=hex(sr)))
            raw(wanted,sp-128,v.sample(a,sp-128,128));e.require(a==wanted,'exact external presence bytes plus original cache invalidation and72byte handle clear;128byte scratch')
            e.require(s.r[0]==0,'original close result after presence change')
            e.require(EVENTS[at:]==[dict(engine=engine,addresses=list(map(hex,c['close_presence_transition']))) for engine in ('original','native')],'exactly one identical declared external input event in each engine')
            targets=[b['target'] for b in r['boundaries']];e.require(targets.index('0x8c660dec')<targets.index('0x8c660dac')<targets.index('0x8c64850a'),'original readiness then status then invalidation call order')
            e.require(not any('write_request_input' in b for b in r['boundaries']),'no write after lost presence')
            transition_cases.append(dict(channel=channel,mode=mode,events=EVENTS[at:]))
    e.report('movie_filesystem_command12_presence_transition_verification.json',{0x8c64919e},[Path(__file__),P/'write_watch_filesystem_command12.py',P/'analysis/movie_filesystem_command12_audit.json'],'MappedABI external-device-presence inputtimeline:bothengines startpresent,thenonlytwoSDKpresencebytesbecome0 atoriginalclose statuscall660dac/PR6491f8 afterreadiness660dec. Bothoriginalfunctions executeunchanged;no suppliedcalleeoutput,register/frame/ID/PR orcode modifications. FullRAM/CPU/FPU/orderedboundaries stillcompared;exactinput-event count/invalidation/clear/order independentlychecked. Testlocal observer schedulesexternalinput before journaling inbothengines;productionobserver/CPU/bridge unchanged. Thisisdeclaredmappedinputtimeline,NOTrawpacket/pump/hardwareIRQ/naturalhot-unplugacquisition proof.')
if __name__=='__main__':main()
