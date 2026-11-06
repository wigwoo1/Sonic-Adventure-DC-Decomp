"""Real JP music/voice, exact original prefixes and independent complete PCM."""
import argparse,ctypes as C,hashlib,json,re,struct,sys,wave
from pathlib import Path
from adx_media_fixtures import Disc
from camera_reference_numeric import service_for
from model_reference import make_reference
from verify_native_audio import bind
I=C.c_int32;U=C.c_uint32;P=C.c_void_p;H=C.c_int16
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def bits(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def signed(u):return u-(1<<32) if u>>31 else u

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','check','binary','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=read(a.build/'build_verification.json')
    proof=read(a.check/'verification.json');assert proof['build_verification_sha256']==sha(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'];frame=C.CDLL(b['frames_DLL']);lib=C.CDLL(b['game_sound_DLL'])
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import multiply,multiply_add,truncate_i32
    ref,refhash=make_reference(executor);numeric=bind(C.CDLL(str(r/'work/sonic-adventure/build/sh4_vector_numeric.dll')),'sa_sh4_vector_numeric',I,U,C.POINTER(Fpu),U);service=service_for(numeric)
    raw=a.binary.read_bytes();assert hashlib.sha256(raw).hexdigest()=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw
    symbols={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    checks=0;calls={};records=[]
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def put(at,data):at&=0xffffff;ram[at:at+len(data)]=data
    def get(at,n):at&=0xffffff;return bytes(ram[at:at+n])
    def original(name,*args):
        cpu=executor.State();cpu.pc=symbols[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
        for i,value in enumerate(args):
            if i<4:cpu.r[4+i]=value&0xffffffff
            else:put(cpu.r[15]+4*(i-4),struct.pack('<I',value&0xffffffff))
        f=Fpu();f.fpscr=0x40001;ref(ram,cpu,fpu=f,vector_instruction=service,instruction_budget=100000)
        check(cpu.r[15]==0x8cf00000 and cpu.pc==0x8cfed000,'complete original retail-prefix function returns normally');calls[name]=calls.get(name,0)+1
        return I(cpu.r[0]).value
    def oracle(payload,nch,k):
        output=[[] for _ in range(nch)];new=[0]*nch;old=[0]*nch;c0=bits(k[0]/4096);c1=bits(k[1]/4096)
        for i in range(len(payload)//18):
            scale=struct.unpack_from('>H',payload,i*18)[0];assert not scale&0x8000;gain=(scale&8191)+1;ch=i%nch
            for byte in payload[i*18+2:i*18+18]:
                for code in (byte>>4,byte&15):
                    step=code if code<8 else code-16
                    if nch==1:
                        predictor=signed((k[0]*new[ch]+k[1]*old[ch])&0xffffffff)>>12
                        sample=max(-32768,min(32767,predictor+gain*step));old[ch],new[ch]=new[ch],sample
                    else:
                        x=multiply_add(bits(gain),bits(step*65536),multiply_add(c0,new[ch],multiply(c1,old[ch],1)[0],1)[0],1)[0]
                        sample=signed(truncate_i32(x)[0])>>16;old[ch],new[ch]=new[ch],x
                    output[ch].append(sample)
        return output
    coefficient=bind(lib,'ADX_GetCoefficient',None,I,I,P,P);create=bind(lib,'ADXPD_Create',P);destroy=bind(lib,'ADXPD_Destroy',None,P)
    bind(lib,'ADXPD_Init',None)();disc=Disc(a.track)
    try:
        for index,item in enumerate(disc.fixtures()):
            filename=item['archive'] or item['name'];base=item['offset'];header=disc.read(filename,base,64)
            assert header[:2]==b'\x80\0' and header[4]==3 and header[5]==18 and header[6]==4 and header[7] in (1,2)
            offset=struct.unpack_from('>H',header,2)[0]+4;nch=header[7];rate,total=struct.unpack_from('>II',header,8);cutoff=struct.unpack_from('>H',header,16)[0]
            check(0<rate<=48000 and total>0 and 0<=cutoff<rate//2,'actual retail ADX format within original decoder domain')
            frames=min(total,rate*2 if index==0 else 4096);blocks=((frames+31)//32)*nch
            payload=disc.read(filename,base+offset,blocks*18);source=C.create_string_buffer(payload);coeff=(H*2)();coefficient(cutoff,rate,coeff,C.byref(coeff,2));k=list(coeff)
            put(0x8cd50000,b'\xa5'*16);original('ADX_GetCoefficient',cutoff,rate,0x8cd50000,0x8cd50004)
            check(k==[struct.unpack('<h',get(0x8cd50000,2))[0],struct.unpack('<h',get(0x8cd50004,2))[0]],'real header-derived coefficients match complete original runtime')
            left=(H*(blocks*32//nch))();right=(H*(blocks*32//nch))();obj=create();check(bool(obj) and obj>0xffffffff,'actual native expander allocated')
            try:
                bind(lib,'ADXPD_SetCoef',None,P,I,I)(obj,rate,cutoff)
                if nch==1:entry=bind(lib,'ADXPD_EntryMono',I,P,P,I,P)(obj,source,blocks,left)
                else:entry=bind(lib,'ADXPD_EntrySte',I,P,P,I,P,P)(obj,source,blocks,left,right)
                check(entry==1,'real expander accepts actual retail encoded input');bind(lib,'ADXPD_Start',None,P)(obj);bind(lib,'ADXPD_ExecServer',None)()
                check(bind(lib,'ADXPD_GetStat',I,P)(obj)==3 and bind(lib,'ADXPD_GetNumBlk',I,P)(obj)==blocks,'actual original decoder server consumes complete retail chunk')
                wanted=oracle(payload,nch,k);check(list(left)==wanted[0] and (nch==1 or list(right)==wanted[1]),'every actual decoded sample matches independent exact recurrence')
                prefix=min(blocks,4 if nch==2 else 2);prefix-=prefix%nch;lh=(H*2)();rh=(H*2)();pl=(H*128)();pr=(H*128)()
                put(0x8cd10000,payload[:prefix*18]);put(0x8cd20000,bytes(256));put(0x8cd30000,bytes(256));put(0x8cd40000,bytes(16))
                if nch==1:
                    expected=original('ADX_DecodeMono2',0x8cd10000,prefix,0x8cd20000,0x8cd40000,*k);actual=bind(lib,'ADX_DecodeMono2',I,P,I,P,P,H,H)(source,prefix,pl,lh,*k)
                else:
                    expected=original('ADX_DecodeSteFloat',0x8cd10000,prefix,0x8cd20000,0x8cd40000,0x8cd30000,0x8cd40008,*k);actual=bind(lib,'ADX_DecodeSteFloat',I,P,I,P,P,P,P,H,H)(source,prefix,pl,lh,pr,rh,*k)
                check(actual==expected==prefix and bytes(pl)==get(0x8cd20000,256) and bytes(pr)==get(0x8cd30000,256) and bytes(lh)==get(0x8cd40000,4) and bytes(rh)==get(0x8cd40008,4),'retail prefix all PCM/history bytes match complete original instructions')
                count=prefix*32//nch;check(list(pl)[:count]==list(left)[:count] and (nch==1 or list(pr)[:count]==list(right)[:count]),'native expander output includes the verified original prefix')
                interleaved=bytearray()
                for i in range(frames):interleaved+=struct.pack('<'+'h'*nch,*([left[i],right[i]] if nch==2 else [left[i]]))
                if index==0:
                    wav=d/'native-JP-title-ADX-decoder-demo.wav'
                    with wave.open(str(wav),'wb') as f:f.setnchannels(nch);f.setsampwidth(2);f.setframerate(rate);f.writeframes(interleaved)
                records.append(dict(**item,channels=nch,rate=rate,cutoff=cutoff,coefficients=k,decoded_blocks=blocks,frames=frames,
                    header_sha256=hashlib.sha256(header).hexdigest(),encoded_chunk_sha256=hashlib.sha256(payload).hexdigest(),PCM_sha256=hashlib.sha256(interleaved).hexdigest(),
                    every_sample_independent_recurrence_verified=True,complete_original_prefix_blocks=prefix))
                print(item['name'],frames,'frames',nch,'channels verified',flush=True)
            finally:destroy(obj)
    finally:disc.close()
    result=dict(status='real_Japanese_music_voice_ADX_PCM_native_expander_original_prefixes_and_independent_full_chunks_verified',runtime_checks=checks,assets=records,
        frames_verified=sum(x['frames'] for x in records),complete_original_entry_calls=sum(calls.values()),original_entry_calls=calls,
        original_function_hooks=False,original_track_sha256=disc.sha256,voice_archive_file_count=disc.voice_archive_file_count,build_verification_sha256=sha(a.build/'build_verification.json'),
        core_proof_sha256=sha(a.check/'verification.json'),verifier_sha256=sha(__file__),fixture_reader_sha256=sha(Path(__file__).parent/'adx_media_fixtures.py'),reference_hashes=refhash,
        private_decoder_demo_WAV=str(wav),private_decoder_demo_WAV_sha256=sha(wav),
        real_GDFS_live_stream_scheduling_Windows_playback_or_main_gameplay_verified=False,retail_assets_published=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
