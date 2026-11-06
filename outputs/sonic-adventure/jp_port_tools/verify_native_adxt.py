"""Original ADXT pool/lifetime, audio-frame schedule and real retail playback.

Disc fixture is an independent read-only oracle, never a production I/O path.
Retail samples/captures remain private. Whole-game playability is not asserted.
"""
import argparse,ctypes as C,hashlib,json,random,struct,subprocess,sys,threading,time,wave
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from adx_media_fixtures import Disc
from adx_step_oracle import StereoADX
from rna_test_runtime import Runtime,Joints
from verify_native_adxstm import Ring,Info as StmInfo
from verify_native_audio import bind
from verify_native_gdfs import Info as FSInfo
from verify_native_g2_audio import Info as G2Info
from verify_native_adx_joints import Ring as OriginalRing
I,U,Q,P=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p
CASES=('lifecycle','pending','voice_cpu','voice_dma','music_cpu','music_dma','timed')
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('initializations','shutdowns','service_calls','owned_bytes','last_core_frame','next_core_frame')]+[(n,U) for n in ('active','stopping','live_players','owned_work','status')]+[('native_error',I)]
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
class MonoADX:
    def __init__(self,coefficients):self.k=coefficients;self.newer=self.older=0
    def decode_step(self,payload):
        assert payload and len(payload)%18==0
        result=[]
        for at in range(0,len(payload),18):
            scale=struct.unpack_from('>H',payload,at)[0];assert scale<0x8000
            for byte in payload[at+2:at+18]:
                for code in (byte>>4,byte&15):
                    step=code if code<8 else code-16
                    value=(scale+1)*step+((self.k[0]*self.newer+self.k[1]*self.older)>>12)
                    value=max(-32768,min(32767,value));result.append(value);self.older,self.newer=self.newer,value
        return [struct.pack('<'+'h'*len(result),*result)]

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);ap.add_argument('--reference',type=Path);a=ap.parse_args()
    r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,key in [('external_adxt.c','adxt_generated_source_sha256'),('sound_owner_adxt.c','adxt_owner_generated_source_sha256'),('external_game_sound.c','game_sound_generated_source_sha256')]:assert sha(a.build/n)==b[key],n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'];d.mkdir()
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--output',str(d/name),'--reference',str(d/name.replace('_dma','_cpu'))]
            run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-5000:])
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_ADXT_native_frontend_scoped_runtime_verified',runtime_checks=sum(x['checks'] for x in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),retail_data_published=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    labels=[];rt=None;errors=[];obs={};groups=[];rings=[]
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    try:
        rt=Runtime(b,a.track,check,timed=a.case in ('pending','timed'));lib=rt.lib;fs=C.CDLL(b['GDFS_DLL'])
        check(C.sizeof(Info)==72,'native ADXT snapshot ABI')
        check(bind(lib,'jp_adxt_initialize',I)()==2,'frontend rejects missing actual file/archive owners')
        check(bind(lib,'jp_adxstm_initialize',I)()==0 and bind(lib,'jp_adxf_initialize',I)()==0,'existing original file/archive owners ready')
        check(bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'/SONICADV')==0,'actual retail audio directory')
        ERROR=C.CFUNCTYPE(None,P,P)
        @ERROR
        def error(ctx,msg):errors.append((ctx,C.string_at(msg).decode(errors='replace')))
        rt.keep.append(error);bind(lib,'ADXERR_EntryErrFunc',None,P,P)(error,0x123456789)
        bind(lib,'mwRnaErrEntryFunc',None,P,P)(error,0xabcdef012)
        def info():
            v=Info();check(bind(lib,'jp_adxt_snapshot',I,P)(C.byref(v))==0,'actual original ADXT pool and owned work snapshot');return v
        def state(h):
            out=(Q*33)();check(bind(lib,'peer_adxt_state',I,P,P)(h,out)==0,'actual complete native ADXT record');return list(out)
        def service():check(bind(lib,'jp_adxt_service',I)()==0,'original complete frontend through existing sound service')
        def create(n=1):
            h=P();check(bind(lib,'jp_adxt_create',I,U,U,P)(n,0,C.byref(h))==0 and bool(h.value),'real native owned player construction');return h.value
        def finish():
            check(bind(lib,'jp_adxt_shutdown',I)()==0,'actual frontend retires before parent owners')
            v=info();check(not v.active and not v.live_players and not v.owned_work and not v.owned_bytes,'no live original player or owned work survives frontend shutdown')
        carry=None
        if a.case=='lifecycle':
            group=Joints(rt,1);groups.append(group);renderer=rt.create(group.handles)
            ring=Ring(rt);rings.append(ring);stream=bind(lib,'ADXSTM_OpenFname',P,C.c_char_p,P)(b'TITLE.ADX',ring.handle)
            check(bool(renderer) and bool(stream),'actual unrelated live renderer and file stream precede ADXT initialization');carry=(renderer,stream,group,ring)
        check(bind(lib,'jp_adxt_initialize',I)()==0,'original frontend joins the same active sound owner')
        if a.case=='lifecycle':
            renderer,stream,group,ring=carry
            check(rt.state(renderer)[0]==1 and bind(lib,'jp_adxstm_snapshot',I,P)(C.byref(StmInfo()))==0,'initialization preserves actual preexisting lower records')
            finish();check(rt.state(renderer)[0]==1 and bind(lib,'ADXSTM_GetStat',I,P)(stream)==1,'frontend shutdown preserves unrelated lower renderer and file stream')
            bind(lib,'mwRnaDestroy',None,P)(renderer);bind(lib,'ADXSTM_Close',None,P)(stream);group.retired();group.close();ring.retired();ring.close();groups.clear();rings.clear()
            check(bind(lib,'jp_adxt_initialize',I)()==0,'frontend restarts after preserving unrelated lower owners')
            bind(lib,'ADXERR_CallErrFunc',None,C.c_char_p)(b'preserved callback')
            check(errors==[(0x123456789,'preserved callback')],'initialization preserves actual preexisting error callback');errors.clear()
            check(bind(lib,'jp_adxt_initialize',I)()==2,'second initialization rejected without pool reset')
            for parent in ('jp_rna_shutdown','jp_adxstm_shutdown','jp_adxf_shutdown'):
                check(bind(lib,parent,I)()==2,'parent refuses to invalidate a live ADXT owner: '+parent)
            h=P(123);check(bind(lib,'jp_adxt_create',I,U,U,P)(0,0,C.byref(h))==1 and bind(lib,'jp_adxt_create',I,U,U,P)(3,0,C.byref(h))==1,'invalid channel counts rejected')
            check(bind(lib,'jp_adxt_create',I,U,U,P)(2,100,C.byref(h))==1 and not h.value,'undersized work rejected before constructor')
            handles=[create() for _ in range(4)];before=info()
            check(before.live_players==before.owned_work==4 and before.owned_bytes==4*55428,'actual four hardware ports and matching work allocations')
            check(bind(lib,'jp_adxt_create',I,U,U,P)(1,0,C.byref(h))==9 and not h.value,'real sound port exhaustion rolls back partial player')
            check(info().live_players==4 and info().owned_work==4,'failed constructor leaves no extra decoder/joints/work')
            for h in handles:
                v=state(h);check(v[11]>0xffffffff and v[14]>0xffffffff and v[14]%32==0 and v[12]==19*2048,'native full-width original voice work layout')
                check(bind(lib,'jp_adxt_destroy',I,P)(h)==0,'each actual player releases its subordinate sound/decoder/joints')
            check(not info().live_players and not info().owned_work,'all successful and failed constructor storage retired')
            errors.clear() # Exhausted original hardware-port allocation reports its original diagnostic.
            raw=C.create_string_buffer(32*320+64);base=(C.addressof(raw)+31)&~31
            joints=[bind(lib,'SJRBF_Create',P,P,I,I)(base+i*320,256,64) for i in range(32)]
            check(all(joints),'actual complete ring-joint pool occupied')
            for available in range(3):
                if available:bind(lib,'SJRBF_Destroy',None,P)(joints.pop())
                failed=P();check(bind(lib,'jp_adxt_create',I,U,U,P)(2,0,C.byref(failed))==9 and not failed.value,'actual constructor rolls back at input or either output joint allocation')
                check(not info().live_players and not info().owned_work and not rt.info().live_renderers,'partial joint allocation leaves no native work or subordinate renderer')
            for joint in joints:bind(lib,'SJRBF_Destroy',None,P)(joint)
            timer=bind(lib,'syTmrDiffCount',U,U,U);rng=random.Random(0x544d52)
            for first,second in [(0,0),(0xffffffff,0),(0,0xffffffff),(0x80000000,0x7fffffff)]+[(rng.getrandbits(32),rng.getrandbits(32)) for _ in range(4096)]:
                check(timer(first,second)==(second-first)&0xffffffff,'original shared timer difference including wrap')
            service();n=info().service_calls
            for _ in range(12):service()
            check(info().service_calls==n,'repeated polling without audio time does not accelerate frontend')
            for quantum in (441,294,147):
                before=info();start=before.last_core_frame
                for _ in range(10):rt.render(quantum);service()
                after=info();check(after.next_core_frame-before.next_core_frame==735*(after.service_calls-before.service_calls),'every logical server step stays on actual audio-frame cadence')
            result=[];t=threading.Thread(target=lambda:result.append(bind(lib,'jp_adxt_shutdown',I)()));t.start();t.join(5)
            check(not t.is_alive() and result==[3] and info().active,'foreign-thread teardown rejected without changing owner')
            @ERROR
            def temporary(ctx,msg):errors.append(('temporary',C.string_at(msg).decode()))
            rt.keep.append(temporary);bind(lib,'ADXT_EntryErrFunc',None,P,P)(temporary,123)
            finish();bind(lib,'ADXERR_CallErrFunc',None,C.c_char_p)(b'restored callback')
            check(errors==[(0x123456789,'restored callback')],'frontend restores its borrowed global error channel after owned registration');errors.clear()
            check(bind(lib,'jp_adxt_initialize',I)()==0,'clean frontend can restart');finish()
            obs.update(original_timer_boundary_random_cases=4100,partial_constructor_real_port_exhaustion=True)
        elif a.case=='pending':
            h=create(2);check(bind(lib,'jp_adxt_start_fname',I,P,C.c_char_p)(h,b'TITLE.ADX')==0,'real original filename playback starts')
            service();v=state(h);si=StmInfo();check(bind(lib,'jp_adxstm_snapshot',I,P)(C.byref(si))==0 and si.pending_reads==1,'actual pending retail read retains original borrowed input chunk')
            before=info();check(bind(rt.g2,'jp_g2_gate_set',I,I)(15)==0,'hold actual completion gate')
            check(bind(lib,'jp_adxt_shutdown',I)()==7 and info().owned_bytes==before.owned_bytes and state(h)[5]==v[5],'gated teardown retains file, player and work')
            check(bind(rt.g2,'jp_g2_gate_set',I,I)(0)==0,'release actual completion gate');rt.close()
            check(not info().active and not info().live_players and not info().owned_bytes,'real global System cleanup retires pending ADXT before lower file/core owners')
            obs.update(pending_read_System_global_cleanup=True)
        else:
            voice=a.case.startswith('voice') or a.case=='timed';nch=1 if voice else 2;h=create(nch)
            check(bind(lib,'sdMemBlkSetTransferMode',I,I)(int(a.case.endswith('_dma')))==0,'selected real sound SDK transfer mode')
            disc=Disc(a.track.resolve())
            try:
                if voice:
                    check(bind(lib,'jp_adxf_partition_load',I,U,C.c_char_p,U)(0,b'EVENT_ADX.AFS',2088)==0,'original real voice archive parser')
                    afs,_=disc.files['EVENT_ADX.AFS'];offset,size=struct.unpack('<II',disc.read('EVENT_ADX.AFS',8,8));data=disc.sectors(afs+offset//2048,(size+2047)//2048)[:size]
                    check(bind(lib,'jp_adxt_start_afs',I,P,U,U)(h,0,0)==0,'actual unmodified Japanese voice starts through original ADXT/AFS')
                    filename='voice0'
                else:
                    filename='FISHMISS.ADX';data=disc.read(filename,0,disc.files[filename][1]);check(bind(lib,'jp_adxt_start_fname',I,P,C.c_char_p)(h,filename.encode())==0,'actual complete retail music starts through original filename player')
            finally:disc.close()
            total,rate=struct.unpack_from('>I',data,12)[0],struct.unpack_from('>I',data,8)[0]
            payload=data[struct.unpack_from('>H',data,2)[0]+4:];coeff=(6569,-2634) if voice else (7334,-3283)
            oracle=MonoADX(coeff) if voice else StereoADX(coeff);expected=[bytearray() for _ in range(nch)]
            output=bytearray();states=[];deadline=time.monotonic()+100;dec=0;last=0;steps=[];sent=0;copy_signature=None
            before_dma=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(before_dma))==0,'actual transfer counters before full player run')
            for iteration in range(10000):
                if a.case=='timed':
                    window=bind(rt.device,'jp_device_window',P)();check(bind(rt.present,'jp_wait_window_idle',I,P,U)(window,5)==1,'existing native wait advances actual timed frontend')
                else:service()
                v=state(h);status=v[1]
                if not states or states[-1]!=status:states.append(status)
                now=bind(lib,'ADXSJD_GetDecNumSmpl',I,P)(v[4]);used=bind(lib,'ADXSJD_GetDecDtLen',I,P)(v[4])
                check(dec<=now<=total and last<=used<=len(data),'real original decoder bounds and monotonic input progress')
                if now>dec:
                    values=oracle.decode_step(payload[last:used]);count=now-dec;check(all(len(x)>=count*2 for x in values),'independent ADX recurrence covers actual decode step')
                    for ch in range(nch):
                        sample=values[ch][:count*2];expected[ch]+=sample
                        # Automatic service may perform several decoder steps
                        # during one native wait. Earlier ring bytes have then
                        # legitimately wrapped; compare every retained sample.
                        retained=min(count,8192);compare=sample[-retained*2:]
                        at=(now-retained)%8192*2;size=min(len(compare),16384-at);base=v[14]+ch*16448
                        actual=C.string_at(base+at,size)+C.string_at(base,len(compare)-size)
                        if actual!=compare:
                            ring=OriginalRing.from_address(v[9+ch]);mismatch=next(i for i,(x,y) in enumerate(zip(actual,sample)) if x!=y)
                            (d/'private_PCM_diagnostic.json').write_text(json.dumps(dict(iteration=iteration,previous_decoded=dec,decoded=now,previous_input=last,input_used=used,count=count,state=v,owner={n:getattr(info(),n) for n,_ in Info._fields_},ring={n:getattr(ring,n) for n,_ in ring._fields_},mismatch_byte=mismatch,actual=actual[max(0,mismatch-16):mismatch+32].hex(),expected=sample[max(0,mismatch-16):mismatch+32].hex(),errors=errors),indent=2)+'\n')
                        check(actual==compare,'all retained actual decoded retail PCM samples match independent recurrence in original output ring')
                    steps.append(count)
                dec,last=now,used
                rv=rt.state(v[6]);signature=tuple(rv[12:16])+tuple(rv[19:24])+tuple(rv[39:40])
                if rv[5] and rv[23] and signature!=copy_signature and status in (2,3):
                    if a.case=='timed':sent=rv[39]
                    count=rv[23];limit=(total+15)&~15
                    if sent+count<=limit:
                        for ch in range(nch):
                            expected_copy=(bytes(expected[ch])+b'\0'*30)[sent*2:(sent+count)*2]
                            check(len(expected_copy)==count*2,'original sound transfer never runs ahead of real decoded samples')
                            check(C.string_at(rv[12+ch],count*2)==expected_copy,'actual RNA borrowed PCM chunk matches independent retail recurrence and final zero padding')
                            check(rt.memory(rv[16+ch]+rv[19]*2,count*2)==expected_copy,'actual ARM sound RAM receives every selected RNA copy unchanged')
                        sent+=count
                    copy_signature=signature
                if status==4 and rv[39]>sent and a.case!='timed':
                    count=rv[39]-sent;check(count<=8192-sent%8192,'final original transfer remains inside one actual sound-ring segment')
                    for ch in range(nch):
                        tail=(bytes(expected[ch])+b'\0'*30)[sent*2:rv[39]*2]
                        check(len(tail)==count*2 and rt.memory(rv[16+ch]+sent%8192*2,count*2)==tail,'complete final PCM transfer and exact zero padding reached actual sound RAM')
                    sent=rv[39]
                if status==5:break
                if status==0:raise AssertionError(('unexpected original recovery stop',iteration,v,errors))
                if a.case!='timed':output+=rt.render(128);time.sleep(.0001)
                check(time.monotonic()<deadline,'actual full retail playback makes bounded progress')
            else:raise AssertionError(('retail ADXT playback did not finish',states,v,errors))
            n=I();scale=I();bind(lib,'ADXT_GetTime',None,P,P,P)(h,C.byref(n),C.byref(scale))
            check(dec==total and n.value==total and scale.value==rate,'actual full declared retail stream reaches PLAYEND with original sample time')
            check(rv[39]==((total+15)&~15) and (a.case=='timed' or sent==rv[39]),'all declared retail PCM and at most15 final zero samples transfer before PLAYEND')
            check((a.case=='timed' or all(x in states for x in (2,3,4))) and v[31]==1,'original play/end path and finite tail padding observed; offline runs include every intermediate state')
            check(not errors,('valid playback emits no original errors',errors))
            after_dma=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(after_dma))==0,'actual transfer counters after full player run')
            dma_count=after_dma.dma_started-before_dma.dma_started
            check(dma_count>0 if a.case.endswith('_dma') else dma_count==0,'selected path uses real DMA or solely real CPU transfers')
            if a.case!='timed':
                check(any(output),'actual supplied ARM firmware produces nonzero mixed PCM')
                (d/'private_output.s16le').write_bytes(output)
                if a.case.endswith('_dma'):check(output==(a.reference/'private_output.s16le').read_bytes(),'every mixed PCM byte agrees between real CPU/DMA paths')
                wav=d/('native-JP-ADXT-'+filename+'.wav')
                with wave.open(str(wav),'wb') as f:f.setnchannels(2);f.setsampwidth(2);f.setframerate(44100);f.writeframes(output)
                obs.update(private_WAV=str(wav),PCM_sha256=hashlib.sha256(output).hexdigest(),firmware_frames=len(output)//4)
            check(bind(lib,'jp_adxt_destroy',I,P)(h)==0,'finished original player and native work retire');finish()
            obs.update(unmodified_retail_header=True,declared_samples=total,sample_rate=rate,complete_retail_stream=True,states=states,decoder_steps=steps,timed_automatic=a.case=='timed',independent_decoded_PCM_sha256=[hashlib.sha256(x).hexdigest() for x in expected],new_actual_DMA_transfers=dma_count)
        check(not errors,('all remaining original errors accounted for',errors))
        if rt.live:
            if info().active:finish()
            rt.close()
    finally:
        if rt and rt.live:
            bind(rt.g2,'jp_g2_gate_set',I,I)(0);bind(rt.lib,'jp_adxt_shutdown',I)();rt.close()
    result=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)

if __name__=='__main__':main()
