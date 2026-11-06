"""Round-trip every native motion/model record and compare real local JP PRS files."""
import argparse,bisect,ctypes as C,hashlib,json,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_asset_test_support import Assets,Limits
from verify_native_read_queue import Disc
from build_native_motion_data import BANKS,LAYOUTS

class Entry(C.Structure):
    _fields_=[('name',C.c_char_p),('data',C.c_void_p),('host_bytes',C.c_size_t)]+[(n,C.c_uint32) for n in ('address','original_bytes','kind','original_element','host_element')]
class Field(C.Structure):_fields_=[(n,C.c_uint32) for n in ('original','host','bytes','pointer')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','baseline','binary','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();build=a.build.resolve();inputs=a.inputs.resolve();d=a.output.resolve();b=read(build/'build_verification.json');old=read(a.baseline/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_source_sha256'].items():assert sha(build/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(a.baseline/'build_verification.json')==b['baseline_verification_sha256']
    assert sha(a.binary)==read(r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json')['original_binary_sha256']
    lib=C.CDLL(b['DLL']);checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    check(C.sizeof(Entry)==48 and C.sizeof(Field)==16,'native registry ABI widths')
    layout=bind(lib,'jp_motion_layout',C.c_uint32,C.c_uint32,C.c_uint32,C.c_void_p);layouts={}
    for kind,(ctype,(size,expected)) in enumerate(LAYOUTS.items(),1):
        fields=[];count=layout(kind,0,None)
        for i in range(count):
            f=Field();check(layout(kind,i,C.byref(f))==count,'compiled native field layout');fields.append(f)
        check([(f.original,f.bytes,f.pointer) for f in fields]==[(off,bytes,pointer) for off,name,bytes,pointer in expected],'original documented pointer/scalar field coverage')
        layouts[kind]=fields
    entries=[];by_bank={};expected_rows={(row['bank'],row['index']):row for row in b['registry']}
    for bank in BANKS+('models',):
        count=bind(lib,'jp_motion_'+bank+'_count',C.c_uint32)();getter=bind(lib,'jp_motion_'+bank+'_entry',C.c_int,C.c_uint32,C.c_void_p)
        check(count==b['bank_definition_counts'][bank],'all original '+bank+' definitions present');items=[]
        for i in range(count):
            entry=Entry();check(getter(i,C.byref(entry))==0,'actual native registry record');row=expected_rows[bank,i]
            check(entry.name.decode()==row['name'] and entry.address==row['address'] and entry.original_bytes==row['bytes'] and entry.kind==row['kind'],'registry provenance identity')
            check(entry.data and entry.host_element and entry.original_element and entry.original_bytes%entry.original_element==0 and entry.host_bytes==entry.original_bytes//entry.original_element*entry.host_element,'complete original elements with native pointer widening')
            items.append(entry);entries.append(entry)
        before=Entry();C.memset(C.byref(before),0x5a,C.sizeof(before));prior=bytes(before)
        check(getter(count,C.byref(before))==1 and bytes(before)==prior and getter(0,None)==1,'invalid record lookup preserves caller output')
        by_bank[bank]=items
    intervals=sorted((e.data,e.data+e.host_bytes,e) for e in entries);starts=[x[0] for x in intervals]
    check(all(intervals[i-1][1]<=intervals[i][0] for i in range(1,len(intervals))),'native definitions do not alias or overlap')
    pointers=0
    def pointer_word(pointer):
        nonlocal pointers
        if not pointer:return 0
        pointers+=1;at=bisect.bisect_right(starts,pointer)-1;assert at>=0 and pointer<intervals[at][1],('unowned native pointer',hex(pointer))
        start,end,target=intervals[at];offset=pointer-start;index,inner=divmod(offset,target.host_element)
        if not target.kind:return target.address+offset
        if not inner:return target.address+index*target.original_element
        for f in layouts[target.kind]:
            if f.host<=inner<f.host+(8 if f.pointer else f.bytes):
                assert not f.pointer or inner==f.host,('pointer into pointer bytes',target.name)
                return target.address+index*target.original_element+f.original+inner-f.host
        raise AssertionError(('pointer into native padding',target.name,inner))
    raw_first=a.binary.read_bytes();bank_reports={};native_bytes=original_bytes=0
    for bank,items in by_bank.items():
        if bank!='models':
            raw=(inputs/bank/'content.bin').read_bytes();base=min(e.address for e in items)
        else:raw=raw_first;base=0x8c010000
        result=hashlib.sha256();compared=0
        for e in items:
            native=C.string_at(e.data,e.host_bytes);native_bytes+=len(native)
            if not e.kind:packed=native
            else:
                packed=bytearray(e.original_bytes)
                for i in range(e.original_bytes//e.original_element):
                    for f in layouts[e.kind]:
                        old_at=i*e.original_element+f.original;new_at=i*e.host_element+f.host
                        if f.pointer:struct.pack_into('<I',packed,old_at,pointer_word(struct.unpack_from('<Q',native,new_at)[0]))
                        else:packed[old_at:old_at+f.bytes]=native[new_at:new_at+f.bytes]
                packed=bytes(packed)
            expected=raw[e.address-base:e.address-base+e.original_bytes]
            check(len(expected)==e.original_bytes and packed==expected,'complete original record bytes '+bank+':'+e.name.decode())
            result.update(packed);compared+=len(packed);original_bytes+=len(packed)
        bank_reports[bank]=dict(definitions=len(items),original_bytes_compared=compared,complete_record_payload_sha256=result.hexdigest())
    # Extract actual supplied disc files and run the existing bounded native
    # PRS owner. No source-bank fixture stands in for retail input here.
    retail=d/'local-retail-PRS';retail.mkdir();disc=Disc(a.track);files=disc.files();file_hashes={}
    for bank in BANKS:
        name=bank.upper()+'.PRS';fad,size,isdir=files[name];assert not isdir
        payload=disc.sectors(fad,(size+2047)//2048)[:size];(retail/name).write_bytes(payload);file_hashes[name]=hashlib.sha256(payload).hexdigest()
    disc.f.close();frame=C.CDLL(old['frames_DLL']);assets=Assets(frame)
    check(bind(frame,'jp_textures_initialize',C.c_int,C.c_uint32,C.c_uint32,C.c_size_t)(32,1024,32<<20)==0,'existing texture owner prerequisite for PRS/PVM service cleanup')
    check(assets.init(str(retail),C.byref(Limits(8<<20,16<<20,64<<20,0)))==0,'existing real bounded PRS owner with local retail files')
    for bank in BANKS:
        data=C.c_void_p();size=C.c_size_t();check(assets.read((bank.upper()+'.PRS').encode(),C.byref(data),C.byref(size))==0,'real supplied JP PRS decodes '+bank)
        check(C.string_at(data,size.value)==(inputs/bank/'content.bin').read_bytes(),'complete generated bank equals independently loaded supplied JP data '+bank)
        check(assets.release()==0,'actual decoded owner view released '+bank)
    check(assets.shutdown()==0,'existing PRS owner fully retires')
    bind(frame,'jp_textures_shutdown',None)()
    report=dict(status='all_seven_native_motion_banks_and_genuine_models_roundtrip_to_original_JP_bytes_and_retail_PRS',checks=checks,
        native_definitions=len(entries),bank_results=bank_reports,owned_native_pointer_relocations_checked=pointers,original_bytes_compared=original_bytes,native_bytes_inspected=native_bytes,
        actual_retail_PRS_files_checked=7,compressed_retail_PRS_sha256=file_hashes,source_model_original_binary_sha256=sha(a.binary),
        native_registry_pointers_are_full_width=all(e.data>0xffffffff for e in entries),no_retail_assets_or_generated_game_C_publishable=True,
        build_verification_sha256=sha(build/'build_verification.json'),verifier_sha256=sha(__file__),native_motion_evaluation_or_character_loading_or_gameplay_verified=False,
        playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2),flush=True)

if __name__=='__main__':main()
