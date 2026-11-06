"""Verify real character definitions, native original loops and JP file loads."""
import argparse,ctypes as C,faulthandler,hashlib,json,shutil,struct,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,List,Name,Record
from native_asset_test_support import Assets,Limits
from native_pvm_test_support import PVM,index_archive
from native_video_test_support import Video
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('starts','acquires','releases','recoveries')]+[(n,U) for n in ('phase','status','native_error','creator_thread')]+[('partial_character',I),('loaded_lists',U),('users',C.c_int8*8)]
class Definition(C.Structure):
    _fields_=[('name',C.c_char_p),('list',P)]+[(n,U) for n in ('address','character','slot','bank')]
CASES=('ordinary','missing','capacity','foreign')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');iv=read(a.inputs/'inputs.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(a.inputs/'inputs.json')==b['character_texture_input_manifest_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','inputs','binary'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--output',str(d/name)];run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW);(d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6000:]);cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        out=dict(status='genuine_all_eight_character_definitions_original_reference_loops_real_JP_loads_and_partial_recovery_verified',checks=sum(v['checks'] for v in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),full_set_character_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True);return
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True);labels=[];obs={}
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label);(d/'progress.json').write_text(json.dumps(dict(checks=len(labels),assertions=labels),indent=2)+'\n')
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;assets=Assets(lib);pvm=PVM(lib);video=Video(lib)
    get=bind(lib,'jp_character_texture_definition',I,U,P);count=bind(lib,'jp_character_texture_definition_count',U);snapshot=bind(lib,'jp_character_textures_snapshot',I,P)
    initialize=bind(lib,'jp_character_textures_initialize',I);acquire=bind(lib,'jp_character_textures_acquire',I,U);release=bind(lib,'jp_character_textures_release',I,U);all_release=bind(lib,'jp_character_textures_release_all',I);recover=bind(lib,'jp_character_textures_recover',I);shutdown=bind(lib,'jp_character_textures_shutdown',I);listing=bind(lib,'jp_character_textures_list',I,U,U,P)
    def info():v=Info();check(snapshot(C.byref(v))==0,'actual original-users owner snapshot');return v
    def clear():assets.clear();pvm.clear();t.clear();video.clear()
    check(C.sizeof(Info)==64 and C.sizeof(Definition)==32 and C.sizeof(Name)==16 and C.sizeof(List)==16,'actual native pointer-width ABIs')
    check(count()==17 and initialize()==6,'seventeen genuine lists and missing real borrowed provider rejected')
    decode=bind(lib,'prskit_decompress',I,P,S,P,S,P,P)
    def decoded(name):
        compressed=a.inputs/'assets'/name;assert sha(compressed)==iv['PRS_files'][name]['sha256'];raw=compressed.read_bytes();n=S();at=S()
        check(decode(raw,len(raw),None,0,C.byref(n),C.byref(at))==0 and at.value==len(raw) and n.value<=8<<20,'actual bounded retail PRS size '+name)
        buffer=C.create_string_buffer(n.value);check(decode(raw,len(raw),buffer,len(buffer),C.byref(n),C.byref(at))==0,'actual retail PRS decode '+name);return buffer.raw[:n.value]
    module=decoded('TIKAL_PROG.PRS');originals={0:(0x8c010000,a.binary.read_bytes()),1:(0x0cb00000,module)}
    definitions=[];prior={};by_character={i:[] for i in range(8)};expected_files={};formats={}
    for index,row in enumerate(b['character_texture_registry']):
        desc=Definition();check(get(index,C.byref(desc))==0 and desc.name.decode()==row['name'] and desc.address==row['address'] and desc.character==row['character'] and desc.slot==row['slot'] and desc.bank==row['bank'],'source-backed original list identity')
        base,raw=originals[desc.bank];old_array,old_count=struct.unpack_from('<II',raw,desc.address-base);native=List.from_address(desc.list)
        check(desc.list>0xffffffff and C.cast(native.textures,P).value>0xffffffff and native.nbTexture==old_count and 0<old_count<=512,'complete original count and full-width native list pointers')
        names=[]
        for i in range(old_count):
            pointer,attr,texture=struct.unpack_from('<III',raw,old_array-base+12*i);name=native.textures[i]
            check(base<=pointer<base+len(raw),'original texture filename pointer inside actual supplied image')
            expected=raw[pointer-base:raw.index(b'\0',pointer-base)];actual=C.string_at(name.filename)
            check(actual==expected and name.attr==attr and name.texaddr==texture,'entire original filename and initial texture fields')
            names.append((name.filename,actual))
        definitions.append(desc);by_character[desc.character].append(desc);prior[desc.name.decode()]=names
    sentinel=Definition();C.memset(C.byref(sentinel),0x5a,C.sizeof(sentinel));before=bytes(sentinel)
    check(get(17,C.byref(sentinel))==1 and bytes(sentinel)==before and get(0,None)==1,'invalid diagnostic lookups preserve output')
    source=(a.inputs/'original_texture.c').read_text()
    # Descriptor order is the exact original character_pvms array order.
    import re
    for c,items in by_character.items():
        names=[]
        for desc in items:
            symbol=desc.name.decode();match=re.search(r'\{"([A-Z0-9_]+)", &'+re.escape(symbol)+r'\}',source);assert match,symbol;filename=match[1]+'.PRS';expected_files[symbol]=filename
            payload=decoded(filename);flags,rows,images=index_archive(payload);check(len(rows)>=List.from_address(desc.list).nbTexture and flags&8,'actual retail PVM supplies every original list entry')
            for image in images:formats[str(image['kind'])]=formats.get(str(image['kind']),0)+1
    obs.update(list_count=17,original_texture_entries=sum(List.from_address(x.list).nbTexture for x in definitions),retail_texture_formats=formats)
    root=d/'assets';shutil.copytree(a.inputs/'assets',root)
    if a.case=='missing':(root/'SON_EFF.PRS').unlink()
    check(bind(lib,'jp_queue_initialize',I,S)(8<<20)==0 and bind(lib,'jp_frames_initialize',I,U,U,S)(8,128,64<<20)==0 and video.init(64,64,0xff123456,1)==0,'actual frame queue video and Windows-independent transport owners initialized')
    check(t.init(2 if a.case=='capacity' else 1024,4096,128<<20)==0 and assets.init(str(root),C.byref(Limits(4<<20,8<<20,32<<20,512)))==0 and initialize()==0,'actual texture/PRS/file providers and character owner initialized')
    if a.case in ('missing','capacity'):
        check(acquire(0)==6 and info().phase==2 and info().partial_character==0 and not info().users[0],'real partial character load fails before logical user increment')
        check((info().loaded_lists!=0 if a.case=='missing' else assets.snapshot().pending_refs!=0) and t.live()>0,'actual successfully loaded references remain owned for recovery')
        before=info().loaded_lists;check(acquire(2)==2 and release(0)==2 and shutdown()==2 and recover()==6 and info().loaded_lists==before,'failed phase retains actual references and requires explicit reported-error recovery')
        clear();check(recover()==0 and not info().loaded_lists and not assets.snapshot().pending_refs and not any(info().users),'real partial bindings and pending original PVM references retired')
        check(assets.evict()==0 and t.live()==0,'real partial-recovery cleanup leaves no orphaned surface')
        if a.case=='missing':shutil.copyfile(a.inputs/'assets/SON_EFF.PRS',root/'SON_EFF.PRS')
        else:
            check(shutdown()==0 and t.init(1024,4096,128<<20)==0 and initialize()==0,'recovered owner restarts on real sufficient texture table')
    if a.case=='foreign':
        check(acquire(0)==0,'actual character acquired before foreign-thread test');out=Info();C.memset(C.byref(out),0x5a,C.sizeof(out));old=bytes(out);values=[];saved=info();view=P(0xabcdef)
        def worker():values.extend((initialize(),acquire(0),release(0),all_release(),recover(),shutdown(),snapshot(C.byref(out)),listing(0,0,C.byref(view))))
        thread=threading.Thread(target=worker);thread.start();thread.join();check(values==[3]*8 and bytes(out)==old and view.value==0xabcdef and list(info().users)==list(saved.users),'foreign thread cannot alter data ownership or caller outputs')
        check(release(0)==0,'creator releases actual foreign-test data')
    for character in range(8):
        before=assets.snapshot();check(acquire(character)==0 and info().users[character]==1,'original character load executes on real private retail files '+str(character))
        pointers=[]
        for desc in by_character[character]:
            output=P();check(listing(character,desc.slot,C.byref(output))==0 and output.value==desc.list,'checked original character list view')
            native=List.from_address(output.value);check(pvm.loaded(C.byref(native))==native.nbTexture,'all actual named texture bindings present')
            for i in range(native.nbTexture):
                name=native.textures[i];pointer=t.lookup(C.byref(native),i);check(pointer and name.filename==prior[desc.name.decode()][i][0] and C.string_at(name.filename)==prior[desc.name.decode()][i][1],'actual bound texture preserves original filename pointer/content')
                record=Record.from_address(pointer);check(record.texinfo.texsurface.pSurface>0xffffffff and record.count>0,'actual owned texture payload and native reference')
                pointers.append((pointer,record.count))
        after=assets.snapshot();check(after.files_read-before.files_read==len(by_character[character]) and after.pvm_loads-before.pvm_loads==len(by_character[character]),'real file reads and PVM loads match original per-character calls')
        check(acquire(character)==0 and info().users[character]==2 and assets.snapshot().files_read==after.files_read and all(Record.from_address(at).count==n for at,n in pointers),'second logical original user shares existing texture bindings without reload')
        check(shutdown()==8 and release(character)==0 and info().users[character]==1 and all(Record.from_address(at).count==n for at,n in pointers),'live original user blocks owner shutdown and intermediate release retains SDK refs')
        check(release(character)==0 and not info().users[character],'last original user releases actual list bindings')
        output=P(0xabcdef);check(listing(character,0,C.byref(output))==2 and output.value==0xabcdef,'retired caller cannot obtain new logical list view')
        check(assets.evict()==0 and t.live()==0 and not info().loaded_lists,'all actual list and resident references retired')
    for _ in range(127):check(acquire(2)==0,'bounded original signed-byte logical user acquired')
    before=assets.snapshot().files_read;check(acquire(2)==5 and info().users[2]==127 and assets.snapshot().files_read==before,'signed reference overflow rejects before original state or files mutate')
    check(all_release()==0 and not any(info().users),'genuine original release-all retires all logical references')
    for c in range(8):check(acquire(c)==0,'actual simultaneous character texture acquisition '+str(c))
    check(all(info().users) and all_release()==0 and not any(info().users) and not info().loaded_lists,'original release-all handles all eight actual characters simultaneously')
    output=P(0xabcdef);check(acquire(8)==1 and release(8)==1 and listing(8,0,C.byref(output))==1 and output.value==0xabcdef,'invalid character/output does not index original arrays')
    check(shutdown()==0 and info().phase==3 and acquire(0)==2 and initialize()==0 and acquire(0)==0 and release(0)==0 and shutdown()==0,'actual ordinary shutdown and fresh character owner restart')
    clear();check(assets.recover()==0 and assets.evict()==0 and assets.shutdown()==0 and t.live()==0,'caller drains actual borrowed asset provider and remaining resident surfaces')
    check(bind(lib,'jp_queue_reset_frame',I)()==0 and bind(lib,'jp_frames_shutdown',I)()==0 and pvm.shutdown()==0,'caller closes actual borrowed frame and memory owners');video.shutdown();t.shutdown();bind(lib,'jp_queue_shutdown',None)()
    faulthandler.disable();fault.close();out=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
