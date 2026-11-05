"""Full store/update/load state compared to original Japanese instructions.
All game and library dependencies execute without hooks. Caller domains match
the original contracts (three slots, eight character numbers).
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
    image=a.binary.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(image)]=image
    def put(address,data):at=address&0xffffff;ram[at:at+len(data)]=data
    def read(address,n):at=address&0xffffff;return bytes(ram[at:at+n])
    counts={'checks':0,'original_calls':0};seen=set();rng=random.Random(0x53544154)
    names=['save_store','save_update','save_load','save_checksum','pack_event_flags','pack_events_played','store_unk_2e0','store_story','store_stage_clears','unpack_event_flags','unpack_events_played','load_unk_2e0','load_story','load_stage_clears','get_character','set_character_number'];entries={table[n]:n for n in names}
    def trace(pc,op,regs,sr,pr,fpu):
        if pc in entries:seen.add(entries[pc])
    def check(ok,label):counts['checks']+=1;assert ok,(counts['checks'],label)
    def original(name):
        f=Fpu();f.fpscr=0x40001|((counts['original_calls']%2)<<21);s=State();s.pc=table[name];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000
        reference(ram,s,fpu=f,instruction_budget=2500000,trace=trace);check(s.r[15]==0x8cf00000,'original stack restored');counts['original_calls']+=1
    size=bind(lib,'jp_save_bytes',U)();count=bind(lib,'jp_save_count',U)();check((size,count)==(1184,3),'original serialized record dimensions')
    sizes={'saves':3552,'event_flags':512,'events_played':512,'story_records':96,'dat_8c7a2718':8,'stage_clears':344,'lives':1,'play_time':4,'stage_last_cleared':4,'save_slot':4,'character_current':2,'rings':2}
    arrays={n:(C.c_ubyte*s).in_dll(lib,n) for n,s in sizes.items()}
    def assign(n,data):assert len(data)==sizes[n];C.memmove(arrays[n],data,len(data));put(table[n],data)
    def compare(label):
        for n,array in arrays.items():check(bytes(array)==read(table[n],sizes[n]),(label,n))
    def seed(slot,character):
        for n,s in sizes.items():assign(n,rng.randbytes(s))
        assign('save_slot',struct.pack('<i',slot));assign('character_current',struct.pack('<H',character))
        values=[0,1,2,255];assign('event_flags',bytes(values[(i+character)%4] for i in range(512)))
        values=[128,255,0,1,2,127];assign('events_played',bytes(values[(i+character)%6] for i in range(512)))
        assign('lives',bytes([[128,255,0,1,3,4,127,42][character]]))
    store=bind(lib,'save_store',None);update=bind(lib,'save_update',None);load=bind(lib,'save_load',None);crc=bind(lib,'jp_save_checksum',U,P)
    lut=[]
    for n in range(256):
        x=n
        for _ in range(8):x=(x>>1)^(0x8408 if x&1 else 0)
        lut.append(x)
    def independent(data):
        x=0xffff
        for byte in data[4:]:x=(x>>8)^lut[(x^byte)&255]
        return x^0xffff
    for slot in range(3):
        for character in range(8):
            seed(slot,character);before=bytes(arrays['saves']);old_unknown=before[slot*size+0x2e8+1:slot*size+0x348:12]
            original('save_store');store();compare('complete original store')
            data=bytes(arrays['saves']);saved=data[slot*size:(slot+1)*size]
            check(all(data[i*size:(i+1)*size]==before[i*size:(i+1)*size] for i in range(3) if i!=slot),'other save slots untouched by store')
            check(saved[0x2e8+1:0x348:12]==old_unknown,'original store preserves unknown story bytes')
            check(struct.unpack_from('<I',saved)[0]==independent(saved),'stored CRC agrees with independent implementation')
            expected_flags=bytes(sum((arrays['event_flags'][i*8+bit]==1)<<bit for bit in range(8)) for i in range(64))
            check(saved[0x260:0x2a0]==expected_flags,'only exact flag value 1 packs, not arbitrary nonzero values')
            # Update changes only time, character, stage and checksum, even
            # while unrelated live progress has become dirty.
            previous=saved
            for n in ['event_flags','events_played','story_records','stage_clears','dat_8c7a2718','lives','play_time','stage_last_cleared']:assign(n,rng.randbytes(sizes[n]))
            next_character=(character+3)%8;assign('character_current',struct.pack('<H',next_character))
            original('save_update');update();compare('complete original update');saved=bytes(arrays['saves'])[slot*size:(slot+1)*size]
            allowed=set(range(8))|{0x258,0x25c,0x25d};check(all(x==y for i,(x,y) in enumerate(zip(previous,saved)) if i not in allowed),'update preserves every unrelated serialized byte')
            # Load must retain each runtime story unk_01 rather than copying
            # the serialized unknown byte, and must canonicalize flag arrays.
            for n in ['event_flags','events_played','story_records','stage_clears','dat_8c7a2718','lives','play_time','stage_last_cleared','character_current']:assign(n,rng.randbytes(sizes[n]))
            runtime_unknown=bytes(arrays['story_records'])[1::12]
            original('save_load');load();compare('complete original load')
            check(bytes(arrays['story_records'])[1::12]==runtime_unknown,'original load preserves runtime unknown story bytes')
            check(set(bytes(arrays['event_flags']))<=set([0,1]) and set(bytes(arrays['events_played']))<=set([0,1]),'loaded flag arrays are canonical zero/one')
            check(struct.unpack('<H',bytes(arrays['character_current']))[0]==next_character,'actual original character-number setter reached shared owner')
    check(set(names)<=seen,'all real store/load/update/private packing and character dependencies executed without hooks')
    # Extra deterministic native/independent-CRC cases broaden the value domain
    # without pretending that every one was executed by the original CPU.
    for case in range(128):
        slot=case%3;character=case%8;seed(slot,character);store();data=bytes(arrays['saves'])[slot*size:(slot+1)*size]
        check(struct.unpack_from('<I',data)[0]==independent(data),'native random store independent CRC')
        before=bytes(arrays['saves']);load();check(bytes(arrays['saves'])==before,'native load is read-only on all serialized slots')
        check(crc(C.create_string_buffer(data))==independent(data),'original native checksum helper belongs to full save implementation')
    report=dict(status='entire_original_save_store_load_update_and_shared_progress_state_verified',checks=counts['checks'],original_calls=counts['original_calls'],original_dependency_hooks_used=False,original_dependencies_seen=sorted(seen),all_three_slots_and_eight_characters_tested=True,original_store_cases=24,original_update_cases=24,original_load_cases=24,native_extra_state_cases=128,serialized_save_bytes=size,save_count=count,exact_one_flag_pack_semantics_preserved=True,unknown_story_bytes_preserved=True,other_slots_preserved=True,original_binary_sha256=sha(a.binary),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),disk_persistence_verified=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
