"""Original block relocation with independent literal plans and exact data/metadata."""
import json,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];parent=P/'verify_movie_filesystem_write_tasks.py';WORK=0x8cdb0000

def configs():
    names=['move_a','move_b','no_data','no_high_free','read_abort','write_abort','dir_abort','fat_abort','has_CC','bad_link']
    result=[]
    for i,kind in enumerate(names):
        channel,order,endian=((0,0,0),(7,1,1),(0,1,0),(7,0,1))[i%4]
        result.append(dict(kind=kind,channel=channel,order=order,endian=endian,count=1,start=0))
    if '--case' in sys.argv:return [c for c in result if c['kind']==sys.argv[sys.argv.index('--case')+1]]
    return result

def transaction_plan(kind):
    def t(role,block,index=0,error=False,after=None):return dict(role=role,block=block,index=index,error=error,after=after)
    metadata=[t('dir',507,0),t('dir',506,1),t('fat',509,0),t('fat',510,1)]
    if kind in ('has_CC','bad_link'):return [],-245
    if kind=='read_abort':return [t('read',20,error=True)],-255
    if kind=='write_abort':return [t('read',20),t('data',31,error=True)],-255
    if kind=='dir_abort':return [t('dir',507,error=True)],-255
    if kind=='fat_abort':return metadata[:2]+[t('fat',509,error=True)],-255
    if kind in ('no_data','no_high_free'):return metadata,0
    return [t('read',20),t('data',31,after='head'),t('read',21),t('data',30,after='previous')]+metadata,0

def block_bytes(block):return bytes((block*17+i*3+5)&255 for i in range(512))
source=parent.read_text().split('        def command(label,begin,total):')[0].replace('movie_filesystem_write_vectors','movie_filesystem_command9_vectors').replace("'movie_filesystem_write.dll'","'movie_filesystem_command9.dll'")
source=source.replace('fat=bytearray(1024)',"fat=bytearray((0xfffc).to_bytes(2,byte_order)*512)")
marker='        def entry(type_,label):';assert source.count(marker)==1
source=source.replace(marker,"        for reserved in (509,510,511):fat[reserved*2:reserved*2+2]=(0xfffa).to_bytes(2,byte_order)\n        if kind in ('no_data','no_high_free','dir_abort','fat_abort'):\n            fat[40:44]=(0xfffc).to_bytes(2,byte_order)*2\n        if kind=='no_high_free':fat[60:64]=(31).to_bytes(2,byte_order)+(0xfffa).to_bytes(2,byte_order)\n        if kind=='bad_link':fat[0:2]=(32).to_bytes(2,byte_order)\n".replace('\n','\n')+marker)
# Escape handling above is normalized before compilation below.
source=source.replace("directory=entry(0xcc,b'SONIC')+entry(0x33,b'DATA')+bytes(960)","directory=bytearray(1024)\n        if kind not in ('no_data','dir_abort','fat_abort'):\n            file=bytearray(entry(0xcc if kind=='has_CC' else 0x33,b'SONIC'));file[2:4]=(30 if kind=='no_high_free' else 20).to_bytes(2,byte_order);directory[:32]=file")
source=source.replace('\n','\n')
source+='''
        require(v.load(s.m,s.output+0x30)==32 and v.load(s.m,s.output+0x38)==128,'independently decoded data/entry limit and packet capability')
        initial_counters=[v.load(s.m,s.descriptor+x) for x in (0x30,0x34)];oracle_root=bytearray(root);oracle_fat=bytearray(fat);oracle_dir=bytearray(directory);oracle_dirty=bytearray(v.sample(s.m,s.descriptor+0x48,256));require(oracle_dirty==bytes(256),'original cleared256byte dirty table')
        raw(s.m,WORK,bytes([0xa5])*512);oracle_work=bytearray(bytes([0xa5])*512);put(s.m,0x8c66d3a0,0)
        expected_plan,result_code=transaction_plan(kind);result_code&=0xffffffff
        if expected_plan and expected_plan[0]['role']!='read':oracle_dirty[:]=bytes([1])*256
        first=len(s.phases);s.call('command9_arguments_API',0x8c647cc4,[channel,channel,WORK,0],(0,0,0));harvest();s.call('command9_opcode_API',0x8c647cb4,[channel,9]);harvest();s.resume();harvest()
        at=part=failures=0;cursor=first;requests=[];events=[];expected_progress=[];routes=set();ack_images={block:bytes(data) for block,dest,data in plan}
        def buffers():
            require(v.sample(s.m,destinations[0],512)==oracle_root,'independent root bytes unchanged')
            require(v.sample(s.m,destinations[1],1024)==oracle_fat,'independent exact FAT relocation links')
            require(v.sample(s.m,destinations[2],1024)==oracle_dir and v.sample(s.m,s.descriptor+0x48,256)==oracle_dirty,'independent directory heads and all256dirtybytes')
            require(v.sample(s.m,WORK,512)==oracle_work,'independent caller workspace assembly')
        buffers()
        for step in range(100):
            if not v.load(s.m,s.channel_record+8):break
            require(at<len(expected_plan) and s.tokens()[0]>0,'no inventedextra transaction oryield');item=expected_plan[at];role=item['role'];saved=v.load(s.m,v.load(s.m,s.task+0x18));failed=item['error'] and part==0
            if role=='read':
                require(saved==0x8c660b2e,'actual saved read response wait');calls=[b for r,_ in s.phases[first:] for b in r['boundaries'] if b['target']=='0x8c660a7e'];require(calls[-1]['arguments'][:3]==[channel,item['block'],WORK],'independent physical read source and workspace')
                data=block_bytes(item['block']);status=7 if failed else 8;transfer(None if failed else data[part*256:(part+1)*256],status=status)
                if not failed:oracle_work[part*256:(part+1)*256]=data[part*256:(part+1)*256]
                complete=failed or part==1
                requests.append(dict(role=role,block=item['block'],part=part,status=status,queued_request_id=s.tokens()[2]))
            else:
                commit=part==4;require(saved==(0x8c660cf6 if commit else 0x8c660c7c),'actual saved write data/commit wait')
                inputs=[b['write_request_input'] for r,_ in s.phases[cursor:] for b in r['boundaries'] if 'write_request_input' in b];cursor=len(s.phases);require(len(inputs)==1,'one actual queuedwrite input');req=inputs[0]
                offset=item['index']*512;payload=oracle_work if role=='data' else oracle_dir[offset:offset+512] if role=='dir' else oracle_fat[offset:offset+512]
                ptr=WORK if role=='data' else destinations[2]+offset if role=='dir' else destinations[1]+offset
                require(req['mapped_unit']==s.mapped and req['command']==(13 if commit else 12) and req['parameters']==[2,(part<<16)|item['block']],'independent write unit/command/block/phase')
                require((req['words']==0 and req['data']==[]) if commit else (req['words']==32 and int(req['source'],16)==ptr+part*128 and req['data']==list(payload[part*128:(part+1)*128])),'independent exact write source and128bytes')
                status=8 if failed else 7;transfer(status=status,ack=True);complete=failed or commit
                requests.append(dict(role=role,block=item['block'],part=part,status=status,payload_sha256=digest_bytes(bytes(req['data']))))
                if complete and not failed:ack_images[item['block']]=bytes(payload)
                if complete and role=='dir' and not failed:oracle_dirty[item['index']]=0
                if complete and role=='fat':oracle_fat[item['block']*2:item['block']*2+2]=(0xffff if failed else 0xfffa).to_bytes(2,byte_order)
                if complete and not failed and item['after']=='head':oracle_dir[2:4]=(31).to_bytes(2,byte_order);oracle_fat[62:64]=(21).to_bytes(2,byte_order);oracle_fat[40:42]=(0xfffc).to_bytes(2,byte_order)
                if complete and not failed and item['after']=='previous':oracle_fat[62:64]=(30).to_bytes(2,byte_order);oracle_fat[60:62]=(0xfffa).to_bytes(2,byte_order);oracle_fat[42:44]=(0xfffc).to_bytes(2,byte_order);oracle_dirty[:]=bytes([1])*256
            if complete:
                failures+=int(failed);expected_progress.append([channel,9,initial_counters[0]+len(events),initial_counters[1]+failures]);events.append(dict(role=role,block=item['block'],failed=failed));at+=1;part=0
            else:part+=1
            r,path=s.resume();harvest();routes.update(hex(pc) for pc in path if pc in ROUTES);buffers()
        require(at==len(expected_plan) and part==0,'complete literal relocation plan')
        require(v.load(s.m,s.result_word)==result_code and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'independent result andidle')
        boundaries=[b for r,_ in s.phases[first:] for b in r['boundaries']];cb=[b for b in boundaries if b['pr']=='0x8c647bb4'];require(len(cb)==1 and cb[0]['arguments'][:3]==[channel,9,result_code],'one original callback')
        progress=[b['arguments'] for b in boundaries if b['target']=='0x8c647bde'];require(progress==expected_progress,('exact progress/counters',progress,expected_progress))
        require([v.load(s.m,s.descriptor+x) for x in (0x30,0x34)]==[initial_counters[0]+len(events),initial_counters[1]+failures],'preserved initial counters plusactualattempts')
        reads=[b['arguments'][:3] for b in boundaries if b['target']=='0x8c660a7e'];require(reads==[[channel,t['block'],WORK] for t in expected_plan if t['role']=='read'],'no hidden file reads')
        if result_code==0:
            for t in expected_plan:
                if t['role']=='data':require(ack_images[t['block']]==block_bytes(20 if t['block']==31 else 21),'acknowledged destination hasoriginalsource bytes')
            for j,block in enumerate((507,506)):require(ack_images[block]==bytes(oracle_dir[j*512:(j+1)*512]),'acknowledged directory image')
            for j,block in enumerate((509,510)):require(ack_images[block]==bytes(oracle_fat[j*512:(j+1)*512]),'acknowledged FAT image')
        s.resume();harvest();s.call('original_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'actual completed owner teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,result=hex(result_code),requests=requests,transactions=events,progress=progress,original_initial_counters=initial_counters,nonlocal_observed_pcs=sorted(routes),phases=s.records,independent_checks=case_checks))
        print('COMMAND9 TASK',len(records),kind,comparisons,flush=True)
    coverage=[]
    for body in v.manifest['routines']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),parent,P/'movie_filesystem_command9_vectors.py',P/'movie_filesystem_command9_session.py',P/'movie_filesystem_command9_reference.py',P/'write_watch_filesystem_command9.py',v.MP,P/'analysis/movie_filesystem_command9_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for c in records for pc in c['nonlocal_observed_pcs']}),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original rawacquisition thenactualcommand9 APIs/scheduler,valid callerworkspace,rawreadbytes/writeACKs only. Independent literalmove20->31/21->30 andmetadata plans,exact source/destination/data/links/heads/all256dirtybytes/counters/callback/idle/ownerteardown. TypeCCfound rejects(-245) whilemissingtypeCCallowsrelocation. ACKledger notphysicalDMA/flashdurability/powerloss proof. No suppliedparsedmetadata/calleeoutputs/frames/IDs/PR/results. Explicitabortpolicy0/NULLhook domain;fullfilesystem/gameunfinished.')
    name='movie_filesystem_command9_tasks'+('' if shard is None else '_'+str(shard))+'_verification.json';out=ROOT/'work/sonic-adventure'/('command9_case_'+records[0]['configuration']['kind']+'.json') if '--case' in sys.argv else P/'analysis'/name
    out.write_text(json.dumps(report,indent=2)+'\\n');print('COMMAND9 TASK PASS',comparisons,checks,flush=True)
'''
ROUTES={0x8c64a8ee,0x8c64a8fe,0x8c64a988,0x8c64a992,0x8c647aac}
ns=dict(__name__='command9_task_replay',__file__=str(Path(__file__)),transaction_plan=transaction_plan,block_bytes=block_bytes,WORK=WORK,ROUTES=ROUTES,ROOT=ROOT,parent=parent,digest_bytes=lambda raw:__import__('hashlib').sha256(raw).hexdigest());exec(compile(source,str(Path(__file__)),'exec'),ns);ns['configurations']=configs
if __name__=='__main__':ns['main']()
