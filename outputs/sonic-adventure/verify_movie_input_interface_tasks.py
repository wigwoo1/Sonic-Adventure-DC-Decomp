"""Original interface setup, task-created requests, header/block replies and resumes."""
import hashlib,itertools,json,sys
from pathlib import Path
from movie_input_interface_session import Session,v,tasks
P=v.P

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def configurations():
    for channel,command,order,endian,geometry in itertools.product((0,7),(1,18),(0,1),(0,1),(33,511)):
        yield dict(channel=channel,command=command,mode=4 if (channel+command+order+endian)%2 else 0,order=order,endian=endian,geometry=geometry,kind='core')
    for callback in (0,0x8cfef000):yield dict(channel=0,command=1,mode=0,order=0,endian=0,geometry=33,callback=callback,kind='callback')
    for channel in (0,7):yield dict(channel=channel,command=1,mode=0,order=0,endian=0,geometry=511,kind='unplug')
    yield dict(channel=7,command=1,mode=4,order=1,endian=1,geometry=33,kind='retry')
    yield dict(channel=0,command=1,mode=0,order=0,endian=0,geometry=33,kind='bad_header')

def main():
    ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;comparisons=0;new_resume=set();smoke=bool({'--smoke','--extras-smoke'}&set(sys.argv))
    configs=list(configurations())
    if '--extras-smoke' in sys.argv:configs=[c for c in configs if c['kind']!='core']
    elif smoke:configs=[configs[0],next(c for c in configs if c['command']==18 and c['geometry']==511)]
    for config in configs:
        data=dict(config);geometry=data.pop('geometry');kind=data.pop('kind');session=Session(ram,**data);harvested=0;case_checks=0
        def require(condition,label):
            nonlocal checks,case_checks
            assert condition,(config,label)
            checks+=1;case_checks+=1
        def harvest(status=0):
            nonlocal harvested,comparisons
            for i in range(harvested,len(session.phases)):
                r,path=session.phases[i];wanted=status if i==len(session.phases)-1 else 0
                require(r['status']==wanted,'host status');require(r['full_state_match'],'entire machine/devices');require(r['ordered_boundaries_match'],'ordered boundaries')
                comparisons+=1
            harvested=len(session.phases)
            initial=type(session.f).from_buffer_copy(session.initial_fpu)
            # The original SZ0 context code saves one selected bank, not a
            # bank-preserving outer ABI. Do not invent that contract. Every
            # physical-bank value/order remains in the exact machine comparison;
            # the direct helper and context suites have independent FPU oracles.
            if status==0:
                require(session.f.fpscr==initial.fpscr and session.f.fpul==initial.fpul,'returned outer FPSCR and FPUL retained')
            # A reached missing callee has not returned through the scheduler.
            # Its current FPSCR has no outer-call preservation contract; compare
            # it strictly with original execution above, without imposing one.
        def snapshot():
            sp=v.load(session.m,session.task+0x18)
            return sp,v.sample(session.m,sp,session.idle_sp+64-sp),v.sample(session.m,session.owner+0x210,32*0x124)
        def intact(before):
            sp,frame,records=before
            require(v.sample(session.m,sp,len(frame))==frame,'saved task stack untouched by transport')
            require(v.sample(session.m,session.owner+0x210,32*0x124)==records,'task context records untouched by transport')
        def transfer(*,status=8,unplug=False,block_data=None):
            queued=session.tokens()[0];require(queued>0,'request originally queued')
            before=snapshot();session.submit();harvest();intact(before)
            require(session.tokens()[:2]==[0,queued],'original queued->inflight')
            session.observe(geometry,status,unplug,block_data);intact(before)
            session.pump();harvest();intact(before)
            require(session.tokens()[1:]==[0,queued],'original inflight->completed')
            require(v.load(session.m,session.context+0xfc0)==1,'response rearmed submission')
            return queued
        harvest()
        interface=session.interface;reader=0x8c66080c if session.endian else 0x8c660816
        writer=0x8c66081e if session.endian else 0x8c660826
        require(interface==v.load(session.m,0x8c647eb0),'original initializer selected retail interface object')
        require(v.load(session.m,interface+0xc)==reader and v.load(session.m,interface+0x10)==writer,'original selector installed callbacks')
        require(v.load(session.m,interface+8)==0 and v.sample(session.m,session.descriptor+2,1)==b'\x01','original descriptor initialization')
        session.queue();harvest();require(session.command_apis_preserved_task_records,'command APIs left saved task records untouched')
        require(session.tokens()==[1,0,0],'task-created first serial')
        saved=snapshot();require(v.load(session.m,saved[0])==0x8c66091a,'original completion-wait resume PC')
        require(saved[0]==session.idle_sp-(112 if session.command==1 else 108),'original nested stack depth')
        serial_counter=v.load(session.m,session.context+0xfe0)
        session.call('pending_header_wait',v.AI_SCHEDULE);harvest()
        require(snapshot()[0]==saved[0] and session.tokens()==[1,0,0],'pending task neither duplicates packet nor changes frame depth')
        require(v.load(session.m,session.context+0xfe0)==serial_counter,'no serial advance while waiting')
        transfer(status=7 if kind=='bad_header' else 252 if kind=='retry' else 8,unplug=kind=='unplug')
        expected_metadata=session.metadata_expected() if kind not in ('unplug','bad_header','retry') else None
        serial_before=v.load(session.m,session.context+0xfe0)
        r,path=session.resume()
        valid_prefix=geometry==511 and kind=='core' and session.command==1
        harvest(-2 if valid_prefix or session.callback==0x8cfef000 else 0)
        if kind=='retry':
            require(v.load(session.m,v.load(session.m,session.task+0x18))==0x8c66091a,'retry returns to original completion wait')
            require(session.tokens()==[serial_before,0,1],'retry created a new original request serial')
            transfer();expected_metadata=session.metadata_expected();session.resume();harvest()
        if kind=='unplug':
            sp=v.load(session.m,session.task+0x18)
            require(v.load(session.m,sp)==0x8c660932,'device-removal error yields at original continuation')
            session.resume();harvest();new_resume.add('0x8c660932')
            require(0x8c660932 in session.seen and 0x8c648326 in session.seen,'original removal return and caller error conversion execute')
        if expected_metadata is not None:
            require([v.load(session.m,session.output+j*4) for j in range(19)]==expected_metadata,'independent decoded metadata fields')
            target_calls=[b for r,_ in session.phases for b in r['boundaries'] if b['target']==hex(reader)]
            require(len(target_calls)==10,'ten actual 16bit header reads')
            require([b['arguments'][1] for b in target_calls]==[0,2,4,6,8,10,12,16,18,20],'original read offsets')
        if valid_prefix:
            require(session.records[-1]['stop_pc']=='0x8c64ae32','real next filesystem dependency')
            require(v.load(session.m,session.channel_record+8)==1 and v.load(session.m,session.result_word)==0,'valid command remains pending at real missing dependency')
            require(not session.records[-1]['callback_events'],'no fabricated completion callback')
        elif geometry==511 and kind=='core' and session.command==18:
            require(session.tokens()==[serial_before,0,1],'validated header queued actual block request')
            sp=v.load(session.m,session.task+0x18);require(v.load(session.m,sp)==0x8c660b2e,'original block completion-wait continuation')
            block_destination=v.load(session.m,0x8c6484b0);expected=bytearray(v.sample(session.m,block_destination,512))
            for phase in range(2):
                waiting=session.tokens()[0]
                session.call('pending_block_wait',v.AI_SCHEDULE);harvest()
                require(session.tokens()[0]==waiting and v.load(session.m,v.load(session.m,session.task+0x18))==0x8c660b2e,'pending block resumes without duplicate request')
                block=bytes((j*37+phase*17+session.channel)&255 for j in range(256))
                transfer(block_data=block)
                serial=v.load(session.m,session.context+0xfe0)
                session.resume();harvest();expected[phase*256:phase*256+256]=block
                require(v.sample(session.m,block_destination,512)==expected,'original copy assembled block bytes')
                require('0x8c6607fc' in session.records[-1]['targets'],'actual word-copy helper')
                if phase==0:require(session.tokens()[0]==serial,'next sub-block request came from original serial counter')
            new_resume.update(('0x8c660b2e','0x8c648426'))
            require({0x8c660b2e,0x8c648426}<=session.seen,'actual block wait and caller return continuations execute')
            require(v.load(session.m,session.result_word)==0x21,'original successful storage mode result')
        if not valid_prefix:
            # The low-level removal return is -208 (retail66094e); the original
            # negative-header caller maps it to -255 (64831c->648326,648362).
            result=(0xffffff01 if kind in ('unplug','bad_header') else 0x21 if geometry==511 else 0xffffff83)
            if session.callback==0x8cfef000:
                # A real unknown callback is an expected reached boundary.
                # Its host stop is checked separately below; this branch is
                # retained here for the completion-state oracle.
                require(session.records[-1]['stop_pc']==hex(session.callback),'unknown callback stops at actual target')
            require(v.load(session.m,session.result_word)==result,'original command result')
            require(v.sample(session.m,session.channel_record,0x2c)==bytes(0x2c),'original worker cleared pending command')
            callback_events=[b for record in session.records for b in record['callback_events']]
            require(bool(callback_events)==bool(session.callback),'registered completion callback selection')
            if session.callback:
                require(callback_events[-1]['target']==hex(session.callback) and callback_events[-1]['arguments'][:3]==[session.channel,session.command,result],'original completion callback arguments')
            if session.callback!=0x8cfef000:
                session.call('completed_idle',v.AI_SCHEDULE);harvest()
                require(v.load(session.m,session.channel_record+8)==0,'completed worker remains idle')
                session.call('original_owner_cancel',v.OWNER_ALL);harvest()
                require(v.sample(session.m,session.owner,0x2694)==bytes(0x2694),'original owner teardown')
        seen.update(session.seen);edges.update(session.edges)
        records.append(dict(input=config,independent_checks=case_checks,phases=session.records,original_interface=hex(interface),decoded_metadata=expected_metadata,task_frames_never_repaired=True,completed_ids_never_injected=True))
        print('INTERFACE TASK',len(records),config,'comparisons',comparisons,flush=True)
    if smoke:print('INTERFACE TASK SMOKE PASS',len(records),comparisons,checks);return
    bodies=[]
    for module in ('movie_channel_io_translation','movie_channel_decoder_translation'):
        path=P/'analysis'/f'{module}.json';manifest=json.loads(path.read_text())
        bodies.extend(r for r in manifest['routines'] if r['address'] in ('0x8c6608a4','0x8c660a7e'))
    coverage=[]
    for body in bodies:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
    files=[Path(__file__),P/'movie_input_interface_session.py',P/'movie_input_interface_task_fixtures.py',Path(v.__file__),P/'write_watch_input_interface.py',v.MP,P/'analysis/movie_input_interface_connection.json',v.effects.DLL,P/'analysis/movie-input-interface-references/literal_evidence.json',P/'analysis/movie-input-interface-references/references.tsv']
    report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_qualified_nonlocal_routes=sorted(new_resume),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original647e16/647de8 own interface object/callbacks/descriptors; original task constructors/APIs/context saves own every frame. Task-created header request->submit->raw response->actual16bit reads and metadata. Invalid headers complete through original callback/idle/teardown; valid command18 reads/copies two256byte subblocks and returns mode33 through660b2e/648426; valid command1 stops at actual64ae32. Removal low-level-208 becomes original caller error-255. Explicit device busy-clear/raw bytes only, no injected completedID/decoded output/frame/parser/callback result. Existing SDK automatic primary requests may also advance serial counter. Physical Maple DMA/controller/filesystem acquisition, unqualified reader/block branches and complete game remain unfinished.')
    (P/'analysis/movie_input_interface_tasks_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INTERFACE TASK PASS',len(records),comparisons,checks,flush=True)
if __name__=='__main__':main()
