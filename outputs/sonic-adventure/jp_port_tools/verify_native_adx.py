"""Complete original ADX coefficients/blocks/expander calls, no routine hooks."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from model_reference import make_reference
from camera_reference_numeric import service_for
from verify_native_audio import bind
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;H=C.c_int16
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['ADX_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    assert sha(a.build/'external_adx.c')==b['ADX_generated_source_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,refhash=make_reference(executor);numeric_path=r/'work/sonic-adventure/build/sh4_vector_numeric.dll'
    assert sha(numeric_path)=='2e56428fadb1903c319da319cf6fa3731839eab1b16163e4e820821d6e4c2db2'
    numeric=bind(C.CDLL(str(numeric_path)),'sa_sh4_vector_numeric',I,U,C.POINTER(Fpu),U);service=service_for(numeric)
    frame=C.CDLL(b['frames_DLL']);lib=C.CDLL(b['game_sound_DLL'])
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw
    calls={};seen=set();sections={};checks=0;rng=random.Random(0x4144584e);cache_hints=[0]
    tracked={table[n]:n for n in ('ADX_GetCoefficient','cos','_in_coss','_app_coss','_app_sins','_in_fmods','ADX_DecodeMono2','ADX_DecodeSte2','ADX_DecodeSteFloat')}
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def put(at,data):offset=at&0xffffff;ram[offset:offset+len(data)]=data
    def get(at,n):offset=at&0xffffff;return bytes(ram[offset:offset+n])
    def trace(pc,*unused):
        if pc in tracked:seen.add(tracked[pc])
    def hint(at):cache_hints[0]+=1
    def original(name,*args,bank=0):
        cpu=executor.State();cpu.pc=table[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
        for i in range(8,15):cpu.r[i]=0x12340000+i
        saved=list(cpu.r[8:])
        for i,value in enumerate(args):
            if i<4:cpu.r[4+i]=value&0xffffffff
            else:put(cpu.r[15]+4*(i-4),struct.pack('<I',value&0xffffffff))
        f=Fpu();f.fpscr=0x40001|(bank<<21)
        reference(ram,cpu,fpu=f,vector_instruction=service,cache_prefetch=hint,trace=trace,instruction_budget=100000)
        check(cpu.pc==0x8cfed000 and list(cpu.r[8:])==saved and f.fpscr&0x3c0003==0x40001|(bank<<21),('complete original callee stack/CPU/FPU policy',name))
        calls[name]=calls.get(name,0)+1;return I(cpu.r[0]).value
    regions={at:(C.c_ubyte*size)() for at,size in ((0x8cd10000,2048),(0x8cd20000,4096),(0x8cd30000,4096),(0x8cd40000,128),(0x8cd50000,128))}
    def reset():
        for at,buf in regions.items():
            data=rng.randbytes(C.sizeof(buf));C.memmove(buf,data,len(data));put(at,data)
    def host(at):
        for base,buf in regions.items():
            if base<=at<base+C.sizeof(buf):return C.addressof(buf)+at-base
        raise AssertionError(hex(at))
    def seed(at,data):data=bytes(data);put(at,data);C.memmove(host(at),data,len(data))
    def equal(label):
        for at,buf in regions.items():check(bytes(buf)==get(at,C.sizeof(buf)),(label,'whole input/output/history guards',hex(at)))
    coefficient=bind(lib,'ADX_GetCoefficient',None,I,I,P,P)
    def coeff(cutoff,rate,bank=0):
        reset();first=0x8cd50010;second=0x8cd50016
        original('ADX_GetCoefficient',cutoff,rate,first,second,bank=bank);coefficient(cutoff,rate,host(first),host(second));equal(('coefficients',cutoff,rate,bank))
        return struct.unpack('<h',get(first,2))[0],struct.unpack('<h',get(second,2))[0]
    start=checks;default=coeff(500,44100)
    pairs=[(0,44100),(500,22050),(500,32000),(500,48000),(-500,44100)]
    pairs += [(rng.randrange(0,rate//2),rate) for rate in (8000,11025,16000,22050,32000,44100,48000) for _ in range(16)]
    for cutoff,rate in pairs:
        for bank in range(2):coeff(cutoff,rate,bank)
    sections['original_coefficients_complete_runtime_cos_sqrt_chain']=checks-start;print('coefficients',checks-start,'checks; default',default,flush=True)
    start=checks
    for case in range(160):
        reset();header=bytearray(rng.randbytes(64));struct.pack_into('>HH',header,0,0x8000 if case%7 else 0x1234,[12,16,44,60,0xffff][case%5])
        header[4:8]=bytes((3,18,[1,2,4,8][case%4],case%2+1));struct.pack_into('>IIH',header,8,44100,123456,500)
        struct.pack_into('>hh',header,20,-123,case%3);seed(0x8cd10040,header)
        size=[0,15,16,17,18,47,48,64][case%8]
        for name,count in (('ADX_DecodeInfo',8),('ADX_DecodeInfoExADPCM2',1),('ADX_DecodeInfoExLoop',7)):
            outputs=[0x8cd50010+i*8 for i in range(count)]
            code=original(name,0x8cd10040,size,*outputs,bank=case%2)
            fn=bind(lib,name,I,P,I,*([P]*count));actual=fn(host(0x8cd10040),size,*[host(x) for x in outputs])
            check(actual==code,(name,'header size/mark/loop status',case));equal((name,case))
    sections['original_header_info_coefficients_loop_errors_and_write_guards']=checks-start;print('headers',checks-start,'checks',flush=True)
    mono=bind(lib,'ADX_DecodeMono2',I,P,I,P,P,H,H)
    stereo_int=bind(lib,'ADX_DecodeSte2',I,P,I,P,P,P,P,H,H)
    stereo_float=bind(lib,'ADX_DecodeSteFloat',I,P,I,P,P,P,P,H,H)
    def decode(name,count,payload,k,history,bank=0):
        reset();src=0x8cd10040;left=0x8cd20040;right=0x8cd30040;lh=0x8cd40020;rh=0x8cd40030
        seed(src,payload);seed(lh,struct.pack('<2h',*history[:2]));seed(rh,struct.pack('<2h',*history[2:]))
        if name=='ADX_DecodeMono2':
            code=original(name,src,count,left,lh,*k,bank=bank);actual=mono(host(src),count,host(left),host(lh),*k)
        else:
            code=original(name,src,count,left,lh,right,rh,*k,bank=bank);fn=stereo_float if name.endswith('Float') else stereo_int
            actual=fn(host(src),count,host(left),host(lh),host(right),host(rh),*k)
        check(actual==code,(name,count,'original exact decoded block count'));equal((name,count,bank))
    start=checks
    scales=[0,1,7,255,4095,8191,8192,16384,32767]
    for case in range(160):
        count=case%8-1;blocks=max(count,2);payload=bytearray()
        for block in range(blocks):payload+=struct.pack('>H',scales[(case+block)%len(scales)])+rng.randbytes(16)
        if case%4==0 and count>0:struct.pack_into('>H',payload,(case%count)*18,0x8000)
        k=default if case%3 else (rng.randrange(-32768,32768),rng.randrange(-32768,32768))
        history=tuple(rng.randrange(-32768,32768) for _ in range(4))
        for name in ('ADX_DecodeMono2','ADX_DecodeSte2','ADX_DecodeSteFloat'):
            for bank in range(2):decode(name,count,payload,k,history,bank)
    sections['complete_original_decoders_scales_counts_history_and_early_end']=checks-start;print('block_decoders',checks-start,'checks',flush=True)
    start=checks
    for byte in range(256):
        payload=struct.pack('>H',byte*31)+bytes([byte])*16
        for name in ('ADX_DecodeMono2','ADX_DecodeSteFloat'):decode(name,1 if name.endswith('Mono2') else 2,payload*2,default,(123,-456,789,-321),byte%2)
    sections['all_256_packed_code_bytes']=checks-start
    # Independent zero-predictor recurrence exercises every legal scale. Input
    # includes all high/low nibbles and both clipping directions.
    start=checks;codes=bytes(range(0,256,16));src=C.create_string_buffer(bytes(18));out=(H*32)();delay=(H*2)()
    for scale in range(8192):
        C.memmove(src,struct.pack('>H',scale)+codes,18);delay[:]=[11,-12]
        check(mono(src,1,out,delay,0,0)==1,'every legal scale decoded by actual mono routine')
        samples=[]
        for byte in codes:
            for nibble in (byte>>4,byte&15):
                step=nibble if nibble<8 else nibble-16;samples.append(max(-32768,min(32767,(scale+1)*step)))
        check(list(out)==samples and list(delay)==[samples[-1],samples[-2]],'independent exact nibble/gain/clipping/history for every scale')
    sections['independent_all_8192_scale_mono_recurrence']=checks-start
    start=checks
    init=bind(lib,'ADXPD_Init',None);create=bind(lib,'ADXPD_Create',P);destroy=bind(lib,'ADXPD_Destroy',None,P)
    native_handles=[];guest_handles=[];layout=(U*7)()
    check(bind(lib,'peer_adx_layout',I,P)(layout)==0 and list(layout)==[72,24,32,40,48,56,64],'native expander width, guest width52 differs only in native pointers/alignment')
    original('ADXPD_Init');init()
    for i in range(9):
        g=original('ADXPD_Create');n=create();check(bool(n)==bool(g),'original eight-slot pool capacity')
        if n:native_handles.append(n);guest_handles.append(g&0xffffffff)
    addresses={g:n for g,n in zip(guest_handles,native_handles)}
    def state(g,n):
        raw=get(g,52);v=list(struct.unpack('<10i4h2h',raw));expected=[x&0xffffffffffffffff for x in v[:6]]
        expected += [0 if v[6]==0 else host(v[6]&0xffffffff),v[7]&0xffffffffffffffff,0 if v[8]==0 else host(v[8]&0xffffffff),0 if v[9]==0 else host(v[9]&0xffffffff)]
        expected += [x&0xffffffffffffffff for x in v[10:]]
        out=(Q*16)();check(bind(lib,'peer_adx_state',I,P,P)(n,out)==0 and list(out)==expected,'all actual original expander fields with native pointer translation')
    for g,n in addresses.items():state(g,n)
    reset();g=guest_handles[0];n=native_handles[0]
    seed(0x8cd10040,struct.pack('>H',7)+bytes(range(16))+struct.pack('>H',13)+bytes(range(16,32)))
    for name,args in [('ADXPD_EntrySte',(0x8cd10040,2,0x8cd20040,0x8cd30040)),('ADXPD_Start',()),('ADXPD_ExecHndl',()),('ADXPD_Stop',()),('ADXPD_Reset',()),('ADXPD_SetMode',(-123,))]:
        code=original(name,g,*args);native_args=[host(x) if x in (0x8cd10040,0x8cd20040,0x8cd30040) else x for x in args]
        signature=[P]+[P if x in (0x8cd10040,0x8cd20040,0x8cd30040) else I for x in args]
        fn=bind(lib,name,I if name.startswith('ADXPD_Entry') else None,*signature);value=fn(n,*native_args)
        if name.startswith('ADXPD_Entry'):check(value==code,'original expander entry result')
        state(g,n);equal(name)
    # Populate all eight real expanders with distinct native buffers. The
    # original server must traverse its 52-byte guest records; native records
    # have real 64-bit pointers and are72 bytes. Odd stereo counts preserve
    # the original internal-error path, not an invented success count.
    for i,(g,n) in enumerate(addresses.items()):
        for name,args in (('ADXPD_Stop',()),('ADXPD_Reset',()),('ADXPD_SetCoef',(22050 if i%2 else 44100,500)),('ADXPD_SetMode',(i,))):
            original(name,g,*args);bind(lib,name,None,P,*([I]*len(args)))(n,*args);state(g,n)
        src=0x8cd10040+i*72;left=0x8cd20040+i*128;right=0x8cd30040+i*128
        seed(src,struct.pack('>H',i+1)+rng.randbytes(16)+struct.pack('>H',2*i+3)+rng.randbytes(16)+bytes(18))
        name='ADXPD_EntrySte' if i%2 else 'ADXPD_EntryMono';count=3 if i==7 else 2
        args=(src,count,left,right) if i%2 else (src,count,left);types=[P,I,P]+([P] if i%2 else [])
        expected=original(name,g,*args);actual=bind(lib,name,I,P,*types)(n,host(src),count,host(left),*([host(right)] if i%2 else []))
        check(actual==expected,'all8 original entry requests accepted');state(g,n)
        original('ADXPD_Start',g);bind(lib,'ADXPD_Start',None,P)(n);state(g,n)
        check(bind(lib,name,I,P,*types)(n,host(src),count,host(left),*([host(right)] if i%2 else []))==original(name,g,*args)==0,'entry while PREP rejected without replacing request');state(g,n)
    original('ADXPD_ExecServer');bind(lib,'ADXPD_ExecServer',None)();equal('eight-real-expander-server')
    for g,n in addresses.items():
        state(g,n)
        for name in ('ADXPD_GetStat','ADXPD_GetNumBlk'):
            check(bind(lib,name,I,P)(n)==original(name,g),'original expander getter after real decoder server')
    check(bind(lib,'peer_adx_error',I)()==struct.unpack('<i',get(table['adxpd_internal_error'],4))[0]==1,'original odd stereo block result sets actual internal-error flag')
    for g,n in addresses.items():original('ADXPD_Destroy',g);destroy(n)
    original('ADXPD_Destroy',0);destroy(None)
    sections['actual_expander_pool_and_real_nested_decode_lifecycle']=checks-start
    check({'ADX_GetCoefficient','cos','_in_coss','_in_fmods','_app_coss','_app_sins','ADX_DecodeMono2','ADX_DecodeSte2','ADX_DecodeSteFloat'}<=seen,'actual complete original math/decode dependency chains executed')
    result=dict(status='original_ADX_coefficients_header_block_expander_native_contracts_verified',runtime_checks=checks,sections=sections,
        complete_original_entry_calls=sum(calls.values()),original_entry_calls=calls,actual_original_dependencies=sorted(seen),original_function_hooks=False,
        default_500Hz_44100Hz_coefficients=list(default),all_8192_mono_scales_independent=True,all_256_code_bytes=True,native_expander_pointer_bits=64,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),reference_hashes=refhash,
        shared_vector_model_new_hardware_measurements=False,retail_PCM_GDFS_streaming_or_playable_game_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
