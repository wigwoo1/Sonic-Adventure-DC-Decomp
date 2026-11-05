"""Compare full native save images/reset/check family to Japanese machine code.
The original memset/memcpy/default/CRC dependencies execute without hooks.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
P=C.c_void_p;I=C.c_int;U=C.c_uint32
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists();b=json.loads((d/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256']
    fb=json.loads((Path(b['frame_build'])/'build_verification.json').read_bytes());assert sha(b['frames_DLL'])==fb['frames_DLL_sha256']
    lib=C.CDLL(b['frames_DLL']);sys.path.insert(0,str(r/'outputs/sonic-adventure'));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    raw=a.binary.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw
    def put(addr,data):ram[addr&0xffffff:(addr&0xffffff)+len(data)]=data
    def read(addr,n):return bytes(ram[addr&0xffffff:(addr&0xffffff)+n])
    checks=calls=0;seen=set();rng=random.Random(0x53415645)
    trace_names=['save_reset_all','save_reset','save_checksum','save_check_all','reset_shooting_scores','reset_mole_scores','reset_trial_times','board_reset_icecap_scores','board_reset_sunaboard_scores','cart_reset_times','ring_reset']
    addresses={table[n]:n for n in trace_names}
    def trace(pc,op,regs,sr,pr,fpu):
        if pc in addresses:seen.add(addresses[pc])
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def original(name,arg=0):
        nonlocal calls
        f=Fpu();f.fpscr=0x40001|((calls%2)<<21);s=State();s.r[15]=0x8cf00000;s.r[4]=arg;s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000
        reference(ram,s,fpu=f,instruction_budget=2500000,trace=trace)
        check(s.r[15]==0x8cf00000,'original stack restored');calls+=1;return s.r[0]
    size=bind(lib,'jp_save_bytes',U)();count=bind(lib,'jp_save_count',U)();check((size,count)==(0x4a0,3),'Japanese serialized dimensions')
    saves=(C.c_ubyte*(size*count)).in_dll(lib,'saves');slot=I.in_dll(lib,'save_slot');rings=C.c_int16.in_dll(lib,'rings');playing=I.in_dll(lib,'demo_playing')
    reset_all=bind(lib,'save_reset_all',None);reset=bind(lib,'save_reset',None);valid=bind(lib,'save_check_all',I,P);crc=bind(lib,'jp_save_checksum',U,P)
    guard_before=b'BEFORE_SAVE_ARRAY_GUARD_1234567890';guard_after=b'AFTER_SAVE_ARRAY_GUARD__1234567890'
    put(table['saves']-len(guard_before),guard_before);put(table['saves']+size*count,guard_after)
    for case in range(3):
        initial=rng.randbytes(size*count);C.memmove(saves,initial,len(initial));put(table['saves'],initial)
        slot.value=case;put(table['save_slot'],struct.pack('<i',case));rings.value=-12345;put(table['rings'],struct.pack('<h',-12345));playing.value=1
        original('save_reset_all');reset_all();check(bytes(saves)==read(table['saves'],size*count),'all three full default images match original')
        check(rings.value==0 and read(table['rings'],2)==b'\0\0' and slot.value==case and playing.value==1,'reset only its actual intended state')
        check(read(table['saves']-len(guard_before),len(guard_before))==guard_before and read(table['saves']+size*count,len(guard_after))==guard_after,'original output extent agrees with native type')
    defaults=bytes(saves);check(defaults[:size]==defaults[size:size*2]==defaults[size*2:],'three defaults identical')
    check(original('save_check_all',table['saves'])==valid(C.addressof(saves))==1,'whole original default checksum validator')
    for selected in range(3):
        initial=rng.randbytes(size*count);C.memmove(saves,initial,len(initial));put(table['saves'],initial);slot.value=selected;put(table['save_slot'],struct.pack('<i',selected));rings.value=319;put(table['rings'],struct.pack('<h',319))
        original('save_reset');reset();actual=bytes(saves);check(actual==read(table['saves'],size*count),'selected original full save image matches')
        check(actual[selected*size:(selected+1)*size]==defaults[:size] and all(actual[i*size:(i+1)*size]==initial[i*size:(i+1)*size] for i in range(3) if i!=selected) and rings.value==0,'only selected slot resets, with genuine ring effect')
    # Independent table-driven reflected CCITT, unlike the game's bit loop.
    lut=[]
    for initial in range(256):
        value=initial
        for _ in range(8):value=(value>>1)^(0x8408 if value&1 else 0)
        lut.append(value)
    def independent(data):
        value=0xffff
        for byte in data[4:]:value=(value>>8)^lut[(value^byte)&255]
        return value^0xffff
    samples=[bytes(size),b'\xff'*size,bytes(range(256))*(size//256)+bytes(range(size%256))]+[rng.randbytes(size) for _ in range(253)]
    scratch=0x8cd10000
    for index,data in enumerate(samples):
        buffer=C.create_string_buffer(data);want=independent(data);check(crc(buffer)==want,'independent whole-record CRC')
        check(buffer.raw[:size]==data,'native CRC is read-only')
        if index<16:put(scratch,data);check(original('save_checksum',scratch)==want,'full original checksum and dependencies, both register banks')
    for selected in range(3):
        records=[]
        for _ in range(3):
            data=bytearray(rng.randbytes(size));struct.pack_into('<I',data,0,independent(data)|(rng.getrandbits(16)<<16));records.append(data)
        full=b''.join(records);buffer=C.create_string_buffer(full);put(scratch,full)
        check(original('save_check_all',scratch)==valid(buffer)==1,'original validator accepts valid CRC low halves and ignores stored upper halves')
        for offset in [4,0x260,0x49f]:
            corrupt=bytearray(full);corrupt[selected*size+offset]^=1;cb=C.create_string_buffer(bytes(corrupt));check(valid(cb)==0,'native validator rejects payload corruption in every slot/section')
            if offset==0x260:put(scratch,corrupt);check(original('save_check_all',scratch)==0,'original corruption result and early slot return')
    check(set(trace_names)<=seen,'all eleven genuine default/reset/CRC/ring dependencies executed without hooks')
    result=dict(status='original_Japanese_save_defaults_reset_full_record_layout_and_CRC_verified',checks=checks,original_calls=calls,original_dependencies_seen=sorted(seen),original_dependency_hooks_used=False,serialized_record_bytes=size,save_count=count,default_all_slots_sha256=hashlib.sha256(defaults).hexdigest(),default_single_slot_sha256=hashlib.sha256(defaults[:size]).hexdigest(),native_independent_CRC_records=256,original_CRC_parent_comparisons=16,low_16_checksum_comparison_preserved=True,save_store_load_or_disk_persistence_verified=False,original_boot_init_executed=False,original_binary_sha256=sha(a.binary),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
if __name__=='__main__':main()
