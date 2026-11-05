"""Verify original backup envelopes against SH-4 and an independent CRC oracle.
No game/SDK function hooks; no retail assets or user saves are written.
"""
import argparse,binascii,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from model_reference import make_reference
P=C.c_void_p;U=C.c_uint32;I=C.c_int32;H=C.c_uint16;S=C.c_size_t
class Header(C.Structure):
    _fields_=[('vms_comment',C.c_char*18),('btr_comment',C.c_char*34),('game_name',C.c_ubyte*16),('icon_palette',P),('icon_data',P),('icon_num',H),('icon_speed',H),('visual_data',P),('visual_type',H),('reserved',H),('save_data',P),('save_size',U)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','game-build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure');out.mkdir();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(a.upstream/'sdk/shinobi/vmsutil.c')==b['original_source_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    assert C.sizeof(Header)==128 and Header.icon_palette.offset==72 and Header.save_data.offset==112
    lib=C.CDLL(b['DLL']);build=bind(lib,'jp_backup_image_build',I,P,S,C.POINTER(Header));analyze=bind(lib,'jp_backup_image_analyze',I,C.POINTER(Header),P,S);size=bind(lib,'buCalcBackupFileSize',I,U,U,U);crc=bind(lib,'buCalcCRC',H,P,U)
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,refhash=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    def put(at,data):at&=0xffffff;ram[at:at+len(data)]=data
    def read(at,n):return bytes(ram[at&0xffffff:(at&0xffffff)+n])
    def iw(at,n):put(at,struct.pack('<I',n&0xffffffff))
    checks=0;calls={};entered={};rng=random.Random(0x42555049);VIS=[0,8064,4544,2048]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    entries={table[n]:n for n in [*b['original_functions_sha256'],'memcpy','memset']}
    def trace(pc,*unused):
        if pc in entries:entered[entries[pc]]=entered.get(entries[pc],0)+1
    def original(name,args):
        state=executor.State();state.pc=table[name];state.pr=0x8cfed000;state.r[15]=0x8cf00000;state.sr=0x70000000
        for i,value in enumerate(args):state.r[4+i]=value&0xffffffff
        fpu=Fpu();fpu.fpscr=0x40001;reference(ram,state,fpu=fpu,trace=trace,instruction_budget=4000000);calls[name]=calls.get(name,0)+1
        check(state.r[15]==0x8cf00000,'whole original stack restored');return C.c_int32(state.r[0]).value
    A=0x8cc00000;O=0x8cc10000;PAL=0x8cc40000;ICON=0x8cc41000;VISUAL=0x8cc43000;DATA=0x8cc48000
    def output_header(h,guest,image_addr):
        for name,offset,n in [('vms_comment',0,18),('btr_comment',18,34),('game_name',52,16)]:check(C.string_at(C.addressof(h)+getattr(Header,name).offset,n)==guest[offset:offset+n],('original parsed string/padding bytes',name))
        for name,offset in [('icon_num',76),('icon_speed',78),('visual_type',84),('reserved',86)]:check(getattr(h,name)==struct.unpack_from('<H',guest,offset)[0],('original parsed value',name))
        check(h.save_size==struct.unpack_from('<I',guest,92)[0],'original parsed data length')
        for name,offset in [('icon_palette',68),('icon_data',72),('visual_data',80),('save_data',88)]:
            gp=struct.unpack_from('<I',guest,offset)[0];expected=image_addr+gp-O if gp else None;check(getattr(h,name)==expected,('real native-width borrowed pointer relocation',name))
    # Include actual default Japanese records in the game's 10-block envelope,
    # using generated icons rather than redistributing retail image data.
    game=json.loads((a.game_build/'build_verification.json').read_bytes());assert sha(game['frames_DLL'])==game['frames_DLL_sha256'];gl=C.CDLL(game['frames_DLL']);bind(gl,'save_reset_all',None)();save_records=bytes((C.c_ubyte*(3*0x4a0)).in_dll(gl,'saves'));game_payload=save_records+bytes(3968-len(save_records))
    cases=[(icons,visual,[0,31,513][(icons+visual)%3]) for icons in [1,2,3] for visual in range(4)]+[(2,0,game_payload)]
    last_image=None;last_header=None
    for index,(icons,visual,payload) in enumerate(cases):
        payload=payload if isinstance(payload,bytes) else rng.randbytes(payload);palette=rng.randbytes(32);icon=rng.randbytes(icons*512);picture=rng.randbytes(VIS[visual]);speed=index*17
        comment=(b'VMU '+str(index).encode()) if index%2 else b'ABCDEFGHIJKLMNOPQR';boot=b'Native backup test'+bytes([0])+b'ignored tail';name=b'SONICADV_TEST001'
        h=Header();h.vms_comment=comment;h.btr_comment=boot;h.game_name[:]=name;h.icon_num=icons;h.icon_speed=speed;h.visual_type=visual;h.save_size=len(payload)
        keep=[C.create_string_buffer(x) for x in [palette,icon,picture,payload]];h.icon_palette=C.addressof(keep[0]);h.icon_data=C.addressof(keep[1]);h.visual_data=C.addressof(keep[2]) if picture else None;h.save_data=C.addressof(keep[3]) if payload else None
        total=128+len(icon)+len(picture)+len(payload);padded=(total+511)//512*512
        expected=bytearray(128);expected[:16]=comment.split(b'\0')[0][:16].ljust(16,b' ');expected[16:48]=boot.split(b'\0')[0][:32].ljust(32,b' ');expected[48:64]=name.split(b'\0')[0][:16].ljust(16,b'\0');struct.pack_into('<HHHHI',expected,64,icons,speed,visual,0,len(payload));expected[96:128]=palette;expected+=icon+picture+payload;struct.pack_into('<H',expected,70,binascii.crc_hqx(expected,0));expected+=bytes(padded-total)
        check(size(icons,visual,len(payload))==original('buCalcBackupFileSize',[icons,visual,len(payload)])==padded//512,'whole original size and independent block arithmetic')
        guest=bytearray(96);guest[:18]=C.string_at(C.addressof(h),18);guest[18:52]=C.string_at(C.addressof(h)+18,34);guest[52:68]=name;struct.pack_into('<IIHHIHHII',guest,68,PAL,ICON,icons,speed,VISUAL if picture else 0,visual,0,DATA if payload else 0,len(payload))
        put(A,guest);put(PAL,palette);put(ICON,icon);put(VISUAL,picture);put(DATA,payload);put(O,b'\xa5'*(padded+32))
        check(original('buMakeBackupFileImage',[O,A])==padded//512 and read(O,padded)==expected,'entire original image including CRC, strings, icons, visual, data and padding')
        memory=C.create_string_buffer(b'\xa5'*(padded+64));address=C.addressof(memory)+32
        check(build(address,padded,C.byref(h))==padded//512 and C.string_at(address,padded)==expected,'all actual native serialized bytes equal independent and original images')
        check(C.string_at(address-32,32)==C.string_at(address+padded,32)==b'\xa5'*32,'output canaries intact')
        parsed=Header();C.memset(C.addressof(parsed),0xa5,C.sizeof(parsed));put(A,b'\xa5'*96)
        check(original('buAnalyzeBackupFileImage',[A,O])==0,'whole original analysis');check(analyze(C.byref(parsed),address,padded)==0 and C.string_at(address,padded)==expected,'native analysis leaves all source bytes unchanged');output_header(parsed,read(A,96),address)
        last_image=bytes(expected);last_header=h
        print('Verified original image case',index+1,'of',len(cases),flush=True)
    check(len(last_image)==5120 and last_image[1152:1152+len(save_records)]==save_records,'actual Japanese save records survive exact ten-block VMU envelope')
    # CRC is independent from the game's record CRC; check all byte values and
    # odd lengths against binascii and original instructions.
    for payload in [b'',bytes(range(256)),bytes(range(255,-1,-1)),b'123456789']+[rng.randbytes(n) for n in [1,2,3,17,31,512,1023]]:
        buf=C.create_string_buffer(payload);put(DATA,payload);want=binascii.crc_hqx(payload,0)
        check(crc(buf,len(payload))==original('buCalcCRC',[DATA,len(payload)])&0xffff==want,'original signed-byte CRC and independent polynomial oracle')
    bad=bytearray(last_image);bad[-513]^=1;src=C.create_string_buffer(bytes(bad));parsed=Header();C.memset(C.addressof(parsed),0xa5,C.sizeof(parsed));put(O,bad);put(A,b'\xa5'*96)
    check(original('buAnalyzeBackupFileImage',[A,O])==-224 and analyze(C.byref(parsed),src,len(bad))==-224,'CRC error preserves original header-filled return semantics');output_header(parsed,read(A,96),C.addressof(src));check(src.raw[:-1]==bad,'bad-CRC input remains untouched')
    # Malformed/truncated buffers are rejected before the original unbounded
    # analyzer can read their declared payload or write a header.
    for length in [0,1,127,128,len(last_image)-1]:
        short=C.create_string_buffer(last_image[:length]);parsed=Header();C.memset(C.addressof(parsed),0xa5,C.sizeof(parsed));prior=bytes(parsed)
        check(analyze(C.byref(parsed),short,length)==-256 and bytes(parsed)==prior,'truncated input rejects atomically')
    for offset,value,width in [(64,0,2),(64,4,2),(68,4,2),(72,0xffffffff,4)]:
        malformed=bytearray(last_image);malformed[offset:offset+width]=value.to_bytes(width,'little');buf=C.create_string_buffer(bytes(malformed));parsed=Header();C.memset(C.addressof(parsed),0xa5,C.sizeof(parsed));prior=bytes(parsed)
        check(analyze(C.byref(parsed),buf,len(malformed))==-223 and bytes(parsed)==prior and buf.raw[:-1]==malformed,'illegal shape rejects without mutation')
    valid=C.create_string_buffer(last_image);parsed=Header();check(analyze(C.byref(parsed),valid,len(last_image))==0,'valid image for bounds checks')
    for kind in ['short','null-output','null-palette','null-icons','null-save','wrapped-source','aliased-source']:
        h=Header.from_buffer_copy(last_header);mem=C.create_string_buffer(b'\xa5'*(5120+64));dest=C.addressof(mem)+32;capacity=5119 if kind=='short' else 5120
        if kind=='null-output':dest=None
        if kind=='null-palette':h.icon_palette=None
        if kind=='null-icons':h.icon_data=None
        if kind=='null-save':h.save_data=None
        if kind=='wrapped-source':h.icon_palette=(1<<64)-16
        if kind=='aliased-source':h.icon_palette=dest+96
        check(build(dest,capacity,C.byref(h))==-256 and mem.raw[:-1]==b'\xa5'*(5120+64),'invalid native output/source span rejects before writing')
    check(analyze(C.byref(parsed),P((1<<64)-64),128)==-256,'wrapped image span rejected before access');check(analyze(C.cast(valid,C.POINTER(Header)),valid,len(last_image))==-256,'header/input alias rejected')
    # Original analysis temporarily zeros CRC. Prove the bounded native API
    # instead works on a truly read-only mapping, not just restored bytes.
    kernel=C.WinDLL('kernel32',use_last_error=True);alloc=kernel.VirtualAlloc;alloc.restype=P;alloc.argtypes=[P,S,U,U];protect=kernel.VirtualProtect;protect.restype=I;protect.argtypes=[P,S,U,C.POINTER(U)];free=kernel.VirtualFree;free.restype=I;free.argtypes=[P,S,U]
    address=alloc(None,len(last_image),0x3000,4);check(bool(address),'actual memory mapping');C.memmove(address,last_image,len(last_image));old=U();check(protect(address,len(last_image),2,C.byref(old))!=0,'actual read-only protection')
    check(analyze(C.byref(parsed),address,len(last_image))==0 and C.string_at(address,len(last_image))==last_image,'real read-only input accepted with original CRC intact');check(free(address,0,0x8000)!=0,'mapping released')
    check(set(b['original_functions_sha256'])<=set(entered),'all four original bodies and their real CRC code entered')
    result=dict(status='original_backup_containers_CRC_native_bounds_and_readonly_analysis_verified',checks=checks,original_entry_calls=sum(calls.values()),original_calls_by_function=calls,original_functions_entered=entered,original_game_or_SDK_function_hooks_used=False,image_cases=len(cases),all_icon_and_visual_shapes_tested=True,Japanese_three_record_payload_in_ten_block_envelope_verified=True,native_64bit_borrowed_pointers_verified=True,read_only_OS_mapping_verified=True,malformed_alias_and_truncated_buffers_rejected=True,retail_icons_used=False,user_save_files_written=False,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_binary_sha256=sha(a.binary),reference_hashes=refhash,card_filesystem_IO_or_buInit_verified=False,VMU_minigame_emulator_implemented=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',sum(calls.values()),'original calls',flush=True)
if __name__=='__main__':main()
