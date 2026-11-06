"""Execute genuine character changes and compare Chao reset with original SH-4."""
import argparse,ctypes as C,faulthandler,hashlib,json,os,random,re,shutil,struct,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from native_asset_test_support import Assets,Limits
from verify_native_character_textures import Info as TextureInfo
from verify_native_motion_store import Info as MotionInfo,View
from verify_native_asset_character_exit import AssetInfo,CharacterInfo
from verify_native_game_init import FINAL
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
CASES=('ordinary','preflight','missing','abandon','foreign','final','restart')
PLAYABLE=(0,2,3,5,6,7);INDEX={c:i for i,c in enumerate(PLAYABLE)}
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('starts','completions','retirements')]+[(n,U) for n in ('phase','status','native_error','stage')]+[(n,I) for n in ('requested','current','owned_character')]+[(n,U) for n in ('creator_thread','save_slot','reserved')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def exercise(c,case,inputs,binary,upstream):
    d,b,f,system,ex,game=(c[n] for n in ('d','b','f','system','ex','game'));check,snapshot,info,shutdown,clean=(c[n] for n in ('check','snapshot','info','shutdown','clean'))
    motions=C.CDLL(b['motion_DLL']);assets=Assets(f);switch=bind(f,'jp_character_switch',I,I);recover=bind(f,'jp_character_switch_recover',I);retire=bind(f,'jp_character_switch_retire',I)
    snap=lambda:snapshot(f,'jp_character_switch_snapshot',Info);tex=lambda:snapshot(f,'jp_character_textures_snapshot',TextureInfo);mi=lambda:snapshot(motions,'jp_motions_snapshot',MotionInfo)
    acquire=bind(f,'jp_character_textures_acquire',I,U);release=bind(f,'jp_character_textures_release',I,U);can=bind(motions,'jp_motions_can_select',I,I);ma=bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P);mr=bind(motions,'jp_motions_release',I,Q)
    character=C.c_uint16.in_dll(f,'character_current');lives=C.c_int8.in_dll(f,'lives');slot=C.c_int32.in_dll(f,'save_slot');saves=(C.c_uint8*3552).in_dll(f,'saves');saveaddr=C.addressof(saves)
    root=Path(inputs)/'assets'
    if case in ('missing','abandon'):
        copy=d/'private_missing_Amy_effect';shutil.copytree(root,copy);(copy/'AMY_EFF.PRS').unlink();root=copy
    def clear():
        assets.clear();bind(f,'jp_memory_clear_error',None)();bind(f,'jp_textures_clear_error',None)()
    def start():
        check(bind(system,'jp_system_start_motions',I,S,U)(8<<20,16)==0,'same System starts actual original model/motion data owner')
        check(bind(system,'jp_system_start_assets',I,C.c_wchar_p,P)(str(root),C.byref(Limits(4<<20,8<<20,32<<20,512)))==0 and bind(system,'jp_system_start_character_textures',I)()==0,'same System starts actual Assets and genuine Character texture owner')
    check(C.sizeof(Info)==64,'native switch snapshot ABI');start();check(snap().owned_character==-1 and not snap().starts,'new actual character lifetime has no game texture reference')
    field=bind(f,'jp_character_chao_field',P,U);width=bind(f,'jp_character_chao_field_bytes',S,U);names=b['character_switch_Chao_fields'];fields={n:(field(i),width(i)) for i,n in enumerate(names)}
    check(field(len(names)) is None and width(len(names))==0 and len({a for a,n in fields.values()})==10,'actual original private Chao state has distinct storage and bounded diagnostic views')
    sizes={'records_cleared':1,'carried_to':1,'carried':8,'chao_file_data':384,'chao_records':26*512,'icon_count':1,'icon_row_request':4,'animal_icons':120,'dat_8c6c4618':1,'dat_8c6c4619':10}
    check({n:v[1] for n,v in fields.items()}==sizes,'original Chao record/scalar widths and full native carried pointer')
    rng=random.Random(0x53454348)
    def chao_bytes():return {n:C.string_at(a,size) for n,(a,size) in fields.items()}
    def seed_chao():
        for n,(a,size) in fields.items():C.memmove(a,rng.randbytes(size),size)
        C.c_void_p.from_address(fields['carried'][0]).value=0x123456788c6c1190
        return chao_bytes()
    def expected_chao(before):
        want=dict(before);want.update(records_cleared=b'\1',carried_to=b'\xff',carried=bytes(8),icon_count=b'\0',icon_row_request=bytes(4),dat_8c6c4618=b'\0',dat_8c6c4619=b'\xff'*10)
        records=bytearray(26*512)
        for i in range(26):records[i*512]=8
        want['chao_records']=bytes(records);icons=bytearray(before['animal_icons'])
        for i in range(10):icons[i*12]=255;icons[i*12+4:i*12+8]=struct.pack('<f',1.0)
        want['animal_icons']=bytes(icons);return want
    def chao_oracle(before):
        sys.path.insert(0,str(Path(__file__).resolve().parent.parent));from verify_startup import State,reference
        from verify_fpu_cleanup_transfer import Fpu
        table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(Path(upstream)/'config/1st_read/symbols.txt').read_text(),re.M)}
        raw=Path(binary).read_bytes();check(hashlib.sha256(raw).hexdigest()=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf','supplied original Japanese executable hash')
        ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw
        for n,value in before.items():at=table[n]&0xffffff;ram[at:at+(4 if n=='carried' else len(value))]=value[:4] if n=='carried' else value
        s=State();s.r[15]=0x8cf00000;s.pc=table['chao_reset'];s.pr=0x8cfed000;s.sr=0x70000000;fp=Fpu();fp.fpscr=0x40001;seen=set()
        targets={table[n]:n for n in ('chao_reset','chao_clear_records','memset')}
        def trace(pc,op,regs,sr,pr,fpu):
            if pc in targets:seen.add(targets[pc])
        reference(ram,s,fpu=fp,instruction_budget=2500000,trace=trace)
        check(s.r[15]==0x8cf00000 and seen==set(targets.values()),'entire original SH-4 Chao reset executes actual record/memset calls; carried clear is inlined in retail code')
        result={n:bytes(ram[table[n]&0xffffff:(table[n]&0xffffff)+(4 if n=='carried' else size)]) for n,(_,size) in fields.items()};result['carried']+=bytes(4)
        check(result==expected_chao(before),'independent Chao byte oracle matches supplied original machine execution');return result
    def stable():return (character.value,lives.value,slot.value,bytes(saves),tuple(tex().users),mi().generation,tuple(chao_bytes().items()))
    def transition(target,oracle=False):
        old=character.value;before=bytes(saves);old_lives=lives.value;chao=seed_chao();want_chao=chao_oracle(chao) if oracle else expected_chao(chao)
        want=bytearray(before);old_at=slot.value*1184+0x252+INDEX[old];want[old_at]=old_lives&255
        wanted_lives=struct.unpack('b',bytes([want[slot.value*1184+0x252+INDEX[target]]]))[0];wanted_lives=max(4,wanted_lives)
        previous=snap();check(switch(target)==0,'actual complete original set_character succeeds '+str((old,target,slot.value)))
        now=snap();check(now.phase==1 and now.stage==4 and now.completions==previous.completions+1 and now.owned_character==target and character.value==target,'original sequence completed once with truthful shared current and owned texture reference')
        check(bytes(saves)==bytes(want) and lives.value==wanted_lives,'all3552 shared save bytes and signed-life lower bound match original store/load order')
        check(chao_bytes()==want_chao,'complete genuine Chao records/icons/carried state matches expected bytes while file data and icon padding survive')
        check(list(tex().users)==[int(i==target) for i in range(8)] and mi().character==target,'real original motion selection and exact single game texture reference agree')
    if case=='ordinary':
        test=0
        for old in PLAYABLE:
            for target in PLAYABLE:
                for save_slot in range(3):
                    if character.value!=old:transition(old)
                    slot.value=save_slot;C.memmove(saveaddr,rng.randbytes(3552),3552);lives.value=(-128,-1,0,3,4,9,127)[test%7]
                    transition(target,oracle=test<4);test+=1
        check(test==108,'all36 playable old/target pairs execute on each of three actual save slots')
        current=character.value
        check(release(current)==2 and bind(f,'jp_character_textures_release_all',I)()==2 and tex().users[current]==1,'unchecked caller cannot steal the live game texture reference')
        check(acquire(current)==0 and tex().users[current]==2,'real task/renderer consumer can retain an additional reference')
        before=stable();check(switch(PLAYABLE[(INDEX[current]+1)%6])==5 and stable()==before,'retained actual texture consumer blocks before save/character/motion/Chao mutations')
        check(retire()==0 and tex().users[current]==1 and snap().owned_character==-1 and shutdown(5000)==11,'game reference retires while extra actual consumer still protects textures and parent')
        check(release(current)==0,'external real consumer explicitly retires its final reference')
    elif case=='preflight':
        transition(0);seed_chao();before=stable();starts=snap().starts
        for invalid in (-1,1,4,8,65536):check(switch(invalid)==1 and stable()==before and snap().starts==starts,'invalid/unplayable character rejects before actual original operation')
        for invalid in (-1,3):slot.value=invalid;old=stable();check(switch(5)==1 and stable()==old,'invalid signed save slot cannot index original storage')
        slot.value=0;character.value=4;old=stable();check(switch(5)==1 and stable()==old,'invalid old playable save index rejects');character.value=0
        source=C.create_string_buffer(b'actual retained loader work');check(bind(f,'jp_memory_bind_source',I,P,S)(source,len(source))==0,'actual SDK loader view retained');old=stable();check(switch(5)==5 and stable()==old,'borrowed loader work blocks original switch without being unbound');check(bind(f,'jp_memory_unbind_source',I)()==0,'caller retires actual borrowed loader view')
        view=View();check(ma(0,b'sonic_updraft_action',C.byref(view))==0,'actual current-primary motion consumer retained');old=stable();m=bytes(mi())
        check(can(5)==7 and bytes(mi())==m and switch(5)==7 and stable()==old,'pure real motion preflight blocks old-bank users before texture release or save mutation');check(mr(view.token)==0,'actual old action consumer retires')
        model=View();check(ma(7,b'sonic_object',C.byref(model))==0,'actual shared canonical model consumer retained');model_hash=hashlib.sha256(C.string_at(model.data,model.bytes)).hexdigest();transition(5)
        check(hashlib.sha256(C.string_at(model.data,model.bytes)).hexdigest()==model_hash and retire()==0 and shutdown(5000)==11,'shared model is preserved through valid switch and still protects final parent cleanup');check(mr(model.token)==0,'actual shared model consumer retires')
    elif case in ('missing','abandon'):
        transition(0);slot.value=1;lives.value=9;saves[1184+0x252+3]=2;chao=seed_chao();want=chao_oracle(chao);before=bytes(saves);old=mi().selections
        check(switch(5)==6 and snap().phase==2 and snap().stage==3 and snap().owned_character==-1,'real missing second Amy file stops original operation at texture boundary with retained partial work')
        check(character.value==5 and lives.value==9 and saves[1184+0x252]==9 and saves[1184+0x252+3]==2 and chao_bytes()==chao and mi().selections==old+1,'partial state reflects exact original completed writes while lives-load and Chao tail have not run')
        retained=stable();check(switch(5)==2 and stable()==retained,'partial operation cannot restart original prefix and overwrite target saved lives')
        if case=='abandon':
            check(shutdown(5000)==0 and not snapshot(f,'jp_character_exit_snapshot',CharacterInfo).active and not snapshot(f,'jp_assets_exit_snapshot',AssetInfo).initialized,'actual System abandons failed original switch by recovering zero-user partial textures and retiring exact child/providers')
        else:
            slot.value=2;check(recover()==2,'recovery rejects changed caller save slot');slot.value=1
            character.value=0;check(recover()==8,'recovery rejects changed shared character identity');character.value=5
            clear();check(recover()==6 and snap().phase==2 and chao_bytes()==chao,'actual repeated missing-file recovery retains partial responsibility without executing final tail')
            shutil.copyfile(Path(inputs)/'assets/AMY_EFF.PRS',root/'AMY_EFF.PRS');clear();check(recover()==0,'restored actual PRS input resumes retained operation')
            check(lives.value==4 and saves[1184+0x252+3]==2 and saves[1184+0x252]==9 and chao_bytes()==want and snap().phase==1 and snap().owned_character==5,'resumed original tail preserves distinct saved lives and performs genuine Chao reset exactly after successful texture load')
            check(retire()==0,'resumed real game texture reference retires')
    elif case=='foreign':
        transition(0);v=Info();C.memset(C.byref(v),0x5a,C.sizeof(v));sentinel=bytes(v);out=[];before=stable()
        def other():out.extend((switch(5),recover(),retire(),bind(f,'jp_character_switch_snapshot',I,P)(C.byref(v)),can(5)))
        thread=threading.Thread(target=other);thread.start();thread.join();check(out==[3]*5 and bytes(v)==sentinel and stable()==before,'foreign thread cannot switch/recover/retire/inspect owner or select motion; output sentinel unchanged');check(retire()==0,'creator retires actual game reference')
    else:
        transition(7)
        if case=='final':
            final=FINAL|32|33554432|16777216;check(bind(system,'jp_system_bind_exit',I,U)(final)==0,'same final Exit receives actual Asset/Character/Motion ownership while game reference lives')
            before=stable();check(clean()==6 and stable()==before,'final global Exit blocks active game reference before invalidating shared state');check(retire()==0 and clean()==0,'caller retires actual game reference then complete native final cleanup succeeds')
            check(switch(0)==2 and recover()==2 and retire()==2 and clean()==0,'terminal original owner cannot resurrect character operations')
        else:check(retire()==0,'actual game texture reference retires before ordinary shutdown')
    check(shutdown(5000)==0 and not info().parent_owned,'complete actual initializer and owned child graph retire cleanly')
    if case=='restart':
        check(bind(f,'jp_queue_reset_frame',I)()==0,'borrowed frame queue reset for actual restart');c['configure']();check(c['initialize']()==0,'complete original game initializer restarts');start()
        check(snap().starts==0 and snap().owned_character==-1 and character.value==7,'fresh actual owner clears stale switch metadata while original game_init preserves shared current character');transition(3);check(retire()==0 and shutdown(5000)==0,'restarted original character switch and child cleanup succeed')
    check(bind(f,'jp_lcd_shutdown',I)()==0 and bind(f,'jp_vmu_store_shutdown',I)()==0,'caller closes actual borrowed LCD/VMU')
    if case!='final':
        check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'caller closes borrowed Frame owner');bind(f,'jp_render_state_shutdown',None)();bind(f,'jp_textures_shutdown',None)();bind(f,'jp_queue_shutdown',None)()
    faulthandler.disable();c['fault'].close();out=dict(case=case,checks=len(c['labels']),assertions=c['labels'],observations=c['obs'],build_verification_sha256=sha(c['a'].build/'build_verification.json'),verifier_sha256=sha(__file__),original_set_character_and_Chao_reset_executed=True,automatic_3D_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(case,len(c['labels']),'checks',flush=True)
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','track','binary','upstream','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure';b=read(a.build/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if a.case:
        original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1;prefix=raw.split(marker)[0];anchor=b"('jp_textures_initialize',(32,1024,32<<20))";assert prefix.count(anchor)==1;prefix=prefix.replace(anchor,b"('jp_textures_initialize',(1024,4096,128<<20))")
        caller=d/'private_actual_parent_caller.py';caller.write_bytes(prefix+('        from verify_native_character_switch import exercise\n        exercise(locals(), '+repr(a.case)+', '+repr(str(a.inputs.resolve()))+', '+repr(str(a.binary.resolve()))+', '+repr(str(a.upstream.resolve()))+")\nif __name__=='__main__':main()\n").encode())
        cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','normal'];run=subprocess.run(cmd,capture_output=True,timeout=240,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr)
        assert not run.returncode,(a.case,run.returncode,run.stderr.decode(errors='replace')[-6500:]);v=read(d/'run/result.json');v.update(original_parent_verifier_sha256=sha(original),derived_caller_sha256=sha(caller));(d/'result.json').write_text(json.dumps(v,indent=2)+'\n');print(a.case,v['checks'],'checks',flush=True);return
    cases={}
    for case in CASES:
        cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
        for n in ('workspace','build','inputs','track','binary','upstream'):cmd+=['--'+n,str(getattr(a,n).resolve())]
        cmd+=['--case',case,'--output',str(d/case)];run=subprocess.run(cmd,capture_output=True,timeout=300,creationflags=subprocess.CREATE_NO_WINDOW);(d/(case+'.log')).write_bytes(run.stdout+run.stderr);assert not run.returncode,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6500:]);cases[case]=read(d/case/'result.json');print(case,cases[case]['checks'],'checks',flush=True)
    out=dict(status='original_set_character_genuine_Chao_reset_saved_lives_real_texture_motion_consumers_partial_continuation_restart_and_final_Exit_verified',checks=sum(v['checks'] for v in cases.values()),cases=cases,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),native_game_or_provider_functions_replaced=False,original_set_character_and_Chao_reset_executed=True,automatic_3D_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True)
if __name__=='__main__':main()
