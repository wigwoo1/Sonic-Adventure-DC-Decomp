"""Real driver/bank through the owned core pump into actual muted WASAPI."""
import argparse,ctypes as C,hashlib,json,struct,time
from pathlib import Path
from verify_native_aica import Options,Info,bind,read,sha
from verify_native_audio import Info as AudioInfo
P=C.c_void_p;U=C.c_uint32;I=C.c_int32

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','audit','probe_check','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');audit=read(a.audit/'dependency_inventory.json');probe=read(a.probe_check/'verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for name,h in b['authored_source_sha256'].items():assert sha(r/name)==h,name
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(b['audio_DLL'])==b['audio_DLL_sha256']
    lib=C.CDLL(b['DLL']);audio=C.CDLL(b['audio_DLL']);open_=bind(lib,'jp_aica_open',I,P);close=bind(lib,'jp_aica_close',I,U);put=bind(lib,'jp_aica_write',I,U,P,U);get=bind(lib,'jp_aica_read',I,U,P,U)
    reset=bind(lib,'jp_aica_set_arm_reset',I,U);render=bind(lib,'jp_aica_render',I,U,P,C.c_uint64);pump=bind(lib,'jp_aica_pump',I,U);drain=bind(lib,'jp_aica_drain',I,U);snapshot=bind(lib,'jp_aica_snapshot',I,P);audio_snapshot=bind(audio,'jp_audio_snapshot',I,P)
    labels=[];waits=0
    def check(ok,label):assert ok,label;labels.append(label)
    def write(at,raw):
        data=C.create_string_buffer(raw);check(put(at,data,len(raw))==0,'copied real driver/bank/command RAM upload')
    def word(at):
        v=U();check(get(at,C.byref(v),4)==0,'actual driver shared-memory word');return v.value
    def setword(at,v):write(at,struct.pack('<I',v))
    options=Options(C.sizeof(Options),1,16384,40,5000,1,1,0);check(open_(C.byref(options))==0,'owned native core and real muted Windows output open')
    try:
        write(0,b'SEGA'*((2<<20)//4));program=(a.audit/'private_inputs/MASARU.arm.bin').read_bytes();check(hashlib.sha256(program).hexdigest()==audit['actual_Japanese_driver']['program_sha256'],'actual original ARM program fingerprint');write(0,program);check(reset(0)==0,'release actual driver processor')
        remaining=44100
        while remaining:
            n=min(4093,remaining);pcm=C.create_string_buffer(n*4);check(render(n,pcm,n*4)==0,'actual driver offline boot while output session remains real and independent');remaining-=n
        status=word(0xc8);host=word(0xd0);mapping=word(0xcc);check(word(status)==0,'real driver command service ready')
        bank=next(x for x in audit['actual_Japanese_multi_units'] if x['name']=='EMERALDCOAST.MLT');sectors=(bank['bytes']+2047)//2048
        with Path(audit['media_inputs']['data_track']).open('rb') as f:f.seek((bank['fad']-45150)*2352);disc=f.read(sectors*2352)
        raw=b''.join(disc[j*2352+16:j*2352+2064] for j in range(sectors))[:bank['bytes']];check(hashlib.sha256(raw).hexdigest()==probe['cases']['physical_split']['bank_sha256'],'exact original Japanese bank for real output test')
        offsets={'SMSB':0,'SMPB':0x80,'SOSB':0x100,'SFPB':0x200,'SFOB':0x280,'SFPW':0x288}
        for row in bank['bank_records']:
            if row['allocation_bytes']>=0:
                at=mapping+offsets[row['type']]+row['number']*8;setword(at,row['sound_memory_offset']);setword(at+4,row['allocation_bytes'])
            if row['data_bytes']>0:write(row['sound_memory_offset'],raw[row['data_offset']:row['data_offset']+row['data_bytes']])
        def command(kind,*params):return bytes([kind,0,*params])+bytes(14-len(params))
        def midi(kind,x,y=0):return command(0x86,0,0,y,x,0,(kind>>4)&7)
        commands=[command(0x8a,0),command(0x84,0),command(0x82,0),midi(0xb0,121),midi(0xb0,32),midi(0xc0,0),midi(0xb0,10,64),midi(0xe0,0,64),midi(0xb0,1,127),midi(0xb0,7,106),midi(0x90,60,127)]
        write(host,b''.join(commands)+bytes(16));setword(status,(word(status)&0xffffff00)|1)
        remaining=88200
        while remaining:
            n=min(4093,remaining);deadline=time.monotonic()+5
            while True:
                rc=pump(n)
                if rc!=8:break
                assert time.monotonic()<deadline;waits+=1;time.sleep(.002)
            check(rc==0,'real original driver audio generated and copied by owned PCM pump');remaining-=n
        check(drain(5000)==0,'actual Windows stream clock drains all original game-bank frames')
        out=AudioInfo();ci=Info();check(audio_snapshot(C.byref(out))==0 and snapshot(C.byref(ci))==0,'actual shared core and output snapshots')
        check(ci.submitted==out.submitted==out.consumed==88200 and out.queued_frames==out.discarded==0,'real driver frames delivered once without drops')
        expected=probe['real_Windows_delivery']['source_hash'];check(out.source_hash==expected and expected!=14695981039346656037,'independent qualified nonzero game-note float hash matches every output packet')
        check(ci.frames==132300 and ci.cycles==132300*128 and out.session_muted==1 and out.events>0 and word(status)&255==0,'actual emulation progress, queried session mute, render events and command acknowledgement')
        result=dict(status='actual_game_driver_bank_owned_AICA_pump_and_muted_Windows_nonzero_PCM_delivery_verified',checks=len(labels),labels=labels,frames=88200,source_hash=out.source_hash,queue_wait_observations=waits,actual_render_events=out.events,
            build_verification_sha256=sha(a.build/'build_verification.json'),probe_reference_sha256=sha(a.probe_check/'verification.json'),verifier_sha256=sha(__file__),original_SDK_commands_supplied_by_harness=True,audible_speaker_or_physical_waveform_verified=False,retail_audio_private=True,overall_completion_percent=10)
    finally:check(close(5000)==0,'owned driver/output actual worker and core retire together')
    closed=Info();check(snapshot(C.byref(closed))==0 and closed.phase==4 and not closed.resources,'actual factory fully closed after nonzero output');result['checks']=len(labels)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True)

if __name__=='__main__':main()
