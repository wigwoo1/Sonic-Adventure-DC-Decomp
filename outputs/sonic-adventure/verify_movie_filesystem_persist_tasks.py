"""Original command7 persistence with independent request plans and ACK ledger."""
import hashlib,itertools,json,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];parent=P/'verify_movie_filesystem_write_tasks.py'

def configuration_list():
    names=['happy_a','happy_b','happy_c','happy_d','dir_abort','dir_reloc_first','dir_reloc_second','dir_nospace','dir_fat_abort','dir_root_abort','fat_abort','fat_reloc','fat_nospace','fat_root_ignored','fat_remove_ignored','fat_reloc_remove_ignored','two_dirty_second_reloc']
    result=[]
    for i,name in enumerate(names):
        ch,order,endian=((0,0,0),(7,1,1),(0,1,0),(7,0,1))[i%4]
        result.append(dict(kind=name,channel=ch,order=order,endian=endian,count=1,start=0,index=16 if name in ('happy_b','happy_d','dir_reloc_second') else 0,pool_count=0 if name in ('dir_nospace','fat_nospace') else 8,primary_free=name in ('dir_reloc_first','dir_fat_abort','dir_root_abort'),two_dirty=name=='two_dirty_second_reloc'))
    if '--case' in sys.argv:return [c for c in result if c['kind']==sys.argv[sys.argv.index('--case')+1]]
    return result

def tx(role,block,index=0,error=None,after=None):return dict(role=role,block=block,index=index,error=error,after=after)

def transaction_plan(c):
    kind=c['kind'];di=c['index']//16;block=507 if not di else 506
    d=tx('dir',block,di);f=[tx('fat',509,0),tx('fat',510,1)];root=tx('root',511)
    if kind.startswith('happy_'):return [d]+f,0,0
    if kind=='dir_abort':return [tx('dir',block,di,'data8')],-255,0
    if kind=='dir_nospace':return [tx('dir',507,0,'commit251')],-252,0
    if kind in ('dir_reloc_first','dir_reloc_second','dir_fat_abort','dir_root_abort','two_dirty_second_reloc'):
        di=1 if kind in ('dir_reloc_second','two_dirty_second_reloc') else 0;block=506 if di else 507;new=508 if c['primary_free'] else 39
        first=tx('dir',block,di,'commit251',('dir_reloc',di,block,new));plan=([tx('dir',507,0)] if c['two_dirty'] else [])+[first,tx('dir',new,di)]
        if kind=='dir_fat_abort':return plan+[tx('fat',509,0,'data8')],-255,0
        plan+=f
        if kind=='dir_root_abort':return plan+[tx('root',511,0,'data8')],-255,0
        return plan+[root]+f,0,0
    if kind=='fat_abort':return [d,tx('fat',509,0,'data8')],-255,0
    if kind=='fat_nospace':return [d,tx('fat',509,0,'commit251')],-192,0
    if kind=='fat_remove_ignored':return [d,tx('fat',509,0,'unplug')],0,0
    first=tx('fat',509,0,'commit251',('fat_reloc',38))
    if kind=='fat_reloc_remove_ignored':return [d,first,tx('fat',38,0,'unplug')],0,1
    plan=[d,first,tx('fat',38,0),tx('fat',39,1)]
    return plan+[tx('root',511,0,'data8' if kind=='fat_root_ignored' else None)],0,0

source=parent.read_text().split('        def command(label,begin,total):')[0].replace('movie_filesystem_write_vectors','movie_filesystem_persist_vectors').replace("'movie_filesystem_write.dll'","'movie_filesystem_persist.dll'")
source=source.replace('{6:509,8:2,10:507,12:2,16:32}',"{6:509,8:2,10:507,12:2,16:32,18:config['pool_count']}")
marker="        byte_order='big' if endian else 'little';root=b'U'*16+bytes(496);fat=bytearray(1024)";assert source.count(marker)==1
source=source.replace(marker,marker+"\n        root=bytearray(root)\n        for off,value in ((0x46,509),(0x48,2),(0x4a,507),(0x4c,2)):root[off:off+2]=value.to_bytes(2,byte_order)")
marker='        def entry(type_,label):';assert source.count(marker)==1
source=source.replace(marker,"        for free in range(32,32+config['pool_count']):fat[free*2:free*2+2]=(0xfffc).to_bytes(2,byte_order)\n        for reserved in (22,509,510,511):fat[reserved*2:reserved*2+2]=(0xfffa).to_bytes(2,byte_order)\n        if config['primary_free']:fat[1016:1018]=(0xfffc).to_bytes(2,byte_order)\n"+marker)
marker="        directory=entry(0xcc,b'SONIC')+entry(0x33,b'DATA')+bytes(960)";assert source.count(marker)==1
source=source.replace(marker,"        directory=bytearray(1024);index=config['index'];directory[index*32:(index+1)*32]=entry(0xcc,b'SONIC')\n        if config['two_dirty']:\n            other=bytearray(entry(0xcc,b'DATA'));other[2:4]=(22).to_bytes(2,byte_order);other[24:26]=(1).to_bytes(2,byte_order);directory[512:544]=other")
source+='''
        expected_plan,result_code,wanted_notready=transaction_plan(config);result_code&=0xffffffff
        oracle_root=bytearray(root);oracle_fat=bytearray(fat);oracle_dir=bytearray(directory);oracle_dirty=bytearray(v.sample(s.m,s.descriptor+0x48,32));fat_base=509;dir_map=[507,506]
        require(oracle_dirty[:2]==bytes(2),'original initialization clears directory dirty bytes')
        require(v.load(s.m,0x8c67c27c)==0,'original constructor NULL writehook')
        # Explicit coherent software-policy input, same declared domain as prior write tests.
        # Original constructor does not acquire/clear this policy; do not call it a default.
        put(s.m,0x8c66d3a0,0)
        if config['two_dirty']:
            raw(s.m,0x8cdb0000,b'DATA'+bytes(9));s.call('original_secondary_RAM_removal',0x8c649c68,[channel,0x8cdb0000]);harvest();oracle_dir[512:544]=bytes(32);oracle_fat[44:46]=(0xfffc).to_bytes(2,byte_order);oracle_dirty[1]=1
        index=config['index'];oracle_dir[index*32:(index+1)*32]=bytes(32);oracle_fat[40:44]=(0xfffc).to_bytes(2,byte_order)*2;oracle_dirty[index>>4]=1
        counter_base=v.load(s.m,v.load(s.m,0x8c649de4)+v.load(s.m,s.descriptor+4)*80)
        raw(s.m,s.channel_record+0x2c,b'SONIC'+bytes(8));first=len(s.phases);s.call('persist_args_api',0x8c647cc4,[channel,channel,0,0],(0,0,0));harvest();s.call('persist_opcode_api',0x8c647cb4,[channel,7]);harvest();s.resume();harvest()
        tx_at=phase=failures=notready=0;cursor=first;staged=bytearray(512);requests=[];events=[];expected_progress=[];nonlocal_seen=set();nonlocal_edges=set();ack_images={block:bytes(data) for block,dest,data in plan}
        def check_buffers():
            require(v.sample(s.m,destinations[0],512)==oracle_root,'independent root bytes')
            require(v.sample(s.m,destinations[1],1024)==oracle_fat,'independent FAT bytes including original bad/reserved markers')
            require(v.sample(s.m,destinations[2],1024)==oracle_dir and v.sample(s.m,s.descriptor+0x48,32)==oracle_dirty,'independent directory/dirty bytes')
            require(v.load(s.m,s.output+0xc)==fat_base,'original cached FAT base')
        check_buffers()
        for step in range(140):
            if not v.load(s.m,s.channel_record+8):break
            if not s.tokens()[0]:
                pc=frame_pc();require(pc==0x8c660be6 and notready<wanted_notready,'only genuine declared not-ready yield')
                notready+=1;failures+=1;expected_progress.append([channel,7,len(expected_progress)+1,counter_base+failures]);r,trace=s.resume();harvest();nonlocal_seen.update(trace);continue
            require(tx_at<len(expected_plan),'no extra native write transaction');item=expected_plan[tx_at];queued=s.tokens()[0]
            inputs=[b['write_request_input'] for r,trace in s.phases[cursor:] for b in r['boundaries'] if 'write_request_input' in b];cursor=len(s.phases);require(len(inputs)==1,'one original queued request input')
            req=inputs[0];commit=phase==4;require(req['mapped_unit']==s.mapped and req['command']==(13 if commit else 12) and req['parameters']==[2,(phase<<16)|item['block']],'independent unit/data-or-commit/block/phase descriptor')
            role=item['role'];offset=item['index']*512;payload=oracle_dir[offset:offset+512] if role=='dir' else oracle_fat[offset:offset+512] if role=='fat' else oracle_root
            if not commit:
                ptr=destinations[2]+offset if role=='dir' else destinations[1]+offset if role=='fat' else destinations[0]
                require(req['words']==32 and int(req['source'],16)==ptr+phase*128 and req['data']==list(payload[phase*128:(phase+1)*128]),'independent source pointer and exact128byte queued payload')
                staged[phase*128:(phase+1)*128]=bytes(req['data'])
            else:require(req['words']==0 and req['data']==[],'original empty commit payload')
            error=item['error'];failed=(error in ('data8','unplug') and phase==0) or (error=='commit251' and commit)
            status=251 if failed and error=='commit251' else 8 if failed and error=='data8' else 7;unplug=failed and error=='unplug'
            saved_pc=frame_pc();require(saved_pc==(0x8c660cf6 if commit else 0x8c660c7c),'actual saved backend response wait')
            requests.append(dict(transaction=tx_at,role=role,block=item['block'],phase=phase,request_id=queued,command=req['command'],payload_sha256=__import__('hashlib').sha256(bytes(req['data'])).hexdigest(),status=status,unplug=unplug))
            transfer(status=status,unplug=unplug,ack=True)
            if commit or failed:
                if failed:failures+=1
                if commit and not failed:
                    require(staged==payload,'complete acknowledged512byte transaction')
                    ack_images[item['block']]=bytes(staged)
                if role=='dir':
                    if failed and error!='data8':oracle_fat[item['block']*2:item['block']*2+2]=(0xffff).to_bytes(2,byte_order)
                    if not failed:oracle_dirty[item['index']]=0
                if role=='fat':oracle_fat[item['block']*2:item['block']*2+2]=(0xffff if failed else 0xfffa).to_bytes(2,byte_order)
                if item['after']:
                    action=item['after']
                    if action[0]=='dir_reloc':
                        _,at,old,new=action;link=int.from_bytes(oracle_fat[old*2:old*2+2],byte_order)
                        # The old link was captured before marking it bad.
                        link=506 if at==0 else 0xfffa
                        oracle_fat[new*2:new*2+2]=link.to_bytes(2,byte_order)
                        if at==0:oracle_root[0x4a:0x4c]=new.to_bytes(2,byte_order)
                        else:oracle_fat[dir_map[at-1]*2:dir_map[at-1]*2+2]=new.to_bytes(2,byte_order)
                        dir_map[at]=new
                    else:
                        _,fat_base=action;oracle_root[0x46:0x48]=fat_base.to_bytes(2,byte_order)
                        for block in range(fat_base,fat_base+2):oracle_fat[block*2:block*2+2]=(0xfffa).to_bytes(2,byte_order)
                events.append(dict(transaction=tx_at,role=role,block=item['block'],failed=failed,error=error if failed else None));expected_progress.append([channel,7,len(expected_progress)+1,counter_base+failures]);tx_at+=1;phase=0;staged=bytearray(512)
            else:phase+=1
            r,trace=s.resume();harvest();nonlocal_seen.update(trace);check_buffers()
        require(tx_at==len(expected_plan) and phase==0 and notready==wanted_notready,'entire independent transaction plan completed')
        require(v.load(s.m,s.channel_record+8)==0 and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c) and v.load(s.m,s.result_word)==result_code,'independent original result/idle state')
        boundaries=[b for r,trace in s.phases[first:] for b in r['boundaries']];callbacks=[b for b in boundaries if b['pr']=='0x8c647bb4'];require(len(callbacks)==1 and callbacks[0]['arguments'][:3]==[channel,7,result_code],'one actual completion callback')
        attempts=[b for b in boundaries if b['target']=='0x8c6476e8'];progress=[b['arguments'] for b in boundaries if b['target']=='0x8c647bde'];require(len(attempts)==len(expected_plan)+wanted_notready and progress==expected_progress,'independent block-attempt/progress order and failure counters')
        require(v.load(s.m,s.descriptor+0x30)==1+len(attempts) and v.load(s.m,s.descriptor+0x34)==counter_base+failures,'independent final progress counters')
        require([b['arguments'][:3] for r,trace in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e']==initial_reads,'no hidden rereads')
        require(v.load(s.m,0x8c67c27c)==0,'original writehook remains NULL')
        if not result_code and not kind.endswith('_ignored'):
            for at,block in enumerate(dir_map):require(ack_images[block]==bytes(oracle_dir[at*512:(at+1)*512]),'acknowledged directory image matches RAM')
            for at in range(2):require(ack_images[fat_base+at]==bytes(oracle_fat[at*512:(at+1)*512]),'acknowledged FAT image matches RAM')
            require(ack_images[511]==bytes(oracle_root),'acknowledged root image matches RAM')
        s.resume();harvest();s.call('original_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'actual completed task teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,result=hex(result_code),plan=expected_plan,requests=requests,transactions=events,notready_yields=notready,progress=progress,phases=s.records,nonlocal_observed_pcs=sorted(hex(pc) for pc in nonlocal_seen if pc in ROUTES),independent_checks=case_checks,ack_ledger_not_physical_flash_proof=True))
        print('PERSIST TASK',len(records),kind,'PASS',comparisons,flush=True)
    coverage=[]
    for body in v.manifest['routines']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),parent,P/'movie_filesystem_persist_vectors.py',P/'movie_filesystem_persist_session.py',P/'movie_filesystem_persist_reference.py',P/'write_watch_filesystem_persist.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_persist_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for c in records for pc in c['nonlocal_observed_pcs']}),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual initializedcommand7 plusoriginalsecondaryRAMmutator forone two-dirtycase. Independent literaltransactionplans/sourcebytes/metadata/FAT/DIR/dirty/counters/callbacks andACKledger;rawSDKsubmit/pump/resumeonly. ACKledger recordscompletedacknowledgedimages,not physicalDMA/flashatomicity orpowerloss proof. OriginalignoredFAT/rooterrors retained. No parsedmetadata,callbackreturn,ID,PR,frameorcalleeoutputsupplied. Constructor-clearedNULLwritehook and explicit coherent initialabortpolicy0 inputdomain; natural policyacquisition notproved;fullfilesystem/gameunfinished.')
    name='movie_filesystem_persist_tasks'+('' if shard is None else '_'+str(shard))+'_verification.json'
    out=ROOT/'work/sonic-adventure'/('persist_case_'+records[0]['configuration']['kind']+'.json') if '--case' in sys.argv else P/'analysis'/name
    out.write_text(json.dumps(report,indent=2)+'\\n');print('PERSIST TASK PASS',comparisons,checks,flush=True)
'''
ROUTES={0x8c64ab88,0x8c64af0c,0x8c64b00a,0x8c64b06c,0x8c64ac38,0x8c64ac42,0x8c649db6,0x8c647a70}
ns=dict(__name__='persist_task_replay',__file__=str(Path(__file__)),transaction_plan=transaction_plan,ROUTES=ROUTES,parent=parent,ROOT=ROOT);exec(compile(source,str(parent),'exec'),ns);ns['configurations']=configuration_list

def merge():
    paths=[P/f'analysis/movie_filesystem_persist_tasks_{i}_verification.json' for i in range(3)];reports=[json.loads(p.read_text()) for p in paths];pins={}
    for r in reports:
        for pin,h in r['sha256'].items():assert ns['digest'](pin)==h,pin;pins[pin]=h
    expected=configuration_list();cases=[c for r in reports for c in r['cases']];assert len(cases)==len(expected)==17 and {c['configuration']['kind'] for c in cases}=={c['kind'] for c in expected}
    coverage=[]
    for first in reports[0]['coverage_by_routine']:
        cvs=[next(c for c in r['coverage_by_routine'] if c['address']==first['address']) for r in reports];missing=set.intersection(*(set(c['missing_rows']) for c in cvs));edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
        coverage.append(dict(address=first['address'],static_rows=first['static_rows'],executed_rows=first['static_rows']-len(missing),executed_outcomes=first['executed_outcomes']+len(first['missing_outcomes'])-len(edges),missing_rows=sorted(missing),missing_outcomes=sorted(edges)))
    for p in paths:pins[str(p)]=ns['digest'](p)
    result=dict(reports[0],original_task_sessions=17,original_native_comparisons=sum(r['original_native_comparisons'] for r in reports),independent_checks=sum(r['independent_checks'] for r in reports),cases=sorted(cases,key=lambda c:[x['kind'] for x in expected].index(c['configuration']['kind'])),coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for r in reports for pc in r['newly_observed_nonlocal_parent_routes']}),independent_process_shards=3,sha256=pins)
    (P/'analysis/movie_filesystem_persist_tasks_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('PERSIST TASK MERGED PASS',result['original_native_comparisons'],result['independent_checks'],flush=True)
ns['merge']=merge
if __name__=='__main__':ns['main']()