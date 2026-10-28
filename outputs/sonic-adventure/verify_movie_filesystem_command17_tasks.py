"""Original opcode17 multiple-name deletion with independent metadata/persistence oracles."""
import itertools,json,sys
from pathlib import Path
from movie_filesystem_command17_vectors import Session,v,raw,put,digest,P
import movie_channel_request_fixtures as backend

def configurations():
    for i,(channel,order,endian) in enumerate(itertools.product((0,7),(0,1),(0,1))):
        yield dict(kind='core',channel=channel,order=order,endian=endian,count=i%3,start=1 if i==7 else 0)
    for i,kind in enumerate(('bad_data','bad_commit','special','unplug','abort_unplug','abort_bad','retry_success','retry_exhaust','live_infinite','protocol_data','protocol_commit','mutex_data','mutex_commit','retry_unplug','bad_chain')):
        yield dict(kind=kind,channel=7 if i&1 else 0,order=i&1,endian=(i>>1)&1,count=1,start=0)

def main():
    assert v.effects.DLL.name=='movie_filesystem_command17.dll'
    if '--merge' in sys.argv:return merge()
    configs=list(configurations());smoke='--smoke' in sys.argv
    shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
    if shard is not None:
        assert shard in range(3)
        configs=[c for i,c in enumerate(configs) if i%3==shard]
    if smoke:configs=configs[:1]
    if '--case' in sys.argv:configs=[c for c in configurations() if c['kind']==sys.argv[sys.argv.index('--case')+1]]
    records=[];comparisons=checks=0;seen=set();edges=set();ram=v.seed_ram();routes=set()
    for config in configs:
        kind=config['kind'];channel=config['channel'];order=config['order'];endian=config['endian'];count=config['count'];start=config['start']
        budget=1 if kind in ('retry_success','retry_exhaust','retry_unplug') else 0
        s=Session(ram,channel=channel,order=order,endian=endian,mode=4 if (channel+order+endian)&1 else 0,command=1,retry_budget=budget)
        done=0;case_checks=0
        def require(ok,label):
            nonlocal checks,case_checks
            assert ok,(config,label);checks+=1;case_checks+=1
        def harvest():
            nonlocal done,comparisons
            for r,path in s.phases[done:]:
                require(r['status']==0,'host execution completed');require(r['full_state_match'],'all original/native state');require(r['ordered_boundaries_match'],'ordered original/native effects');comparisons+=1
            done=len(s.phases)
        def frame_pc():return v.load(s.m,v.load(s.m,s.task+0x18))
        def transfer(data=None,status=8,unplug=False,ack=False):
            queued=s.tokens()[0];require(queued>0,'actual queued request ID');sp=v.load(s.m,s.task+0x18);frame=v.sample(s.m,sp,s.idle_sp+64-sp);contexts=v.sample(s.m,s.owner+0x210,32*0x124)
            def intact():require(v.sample(s.m,sp,len(frame))==frame and v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'transport preserves original savedframes/contexts')
            s.submit();harvest();intact();require(s.tokens()[:2]==[0,queued],'actual inflight request ID')
            s.observe(511,status=status,unplug=unplug,ack=ack,block_data=data,metadata_words={6:509,8:2,10:507,12:2,16:32,18:0} if data is None and not ack else None);intact();s.pump();harvest();intact();require(s.tokens()[1:]==[0,queued],'actual completed request ID')
        harvest();s.queue();harvest();transfer();s.resume();harvest()
        byte_order='big' if endian else 'little';root=bytearray(b'U'*16+bytes(496));fat=bytearray(1024)
        for off,value in ((0x46,509),(0x48,2),(0x4a,507),(0x4c,2)):root[off:off+2]=value.to_bytes(2,byte_order)
        for index,value in ((507,506),(506,0xfffa),(20,20 if kind=='bad_chain' else 21),(21,0xfffa)):fat[index*2:index*2+2]=value.to_bytes(2,byte_order)
        def entry(type_,label):
            b=bytearray(32);b[0]=type_;b[1]=0xa5;b[2:4]=(20).to_bytes(2,byte_order);b[4:16]=(label+bytes(12))[:12];b[16:24]=bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,3));b[24:26]=(2).to_bytes(2,byte_order);b[26:28]=(1).to_bytes(2,byte_order);return bytes(b)
        for reserved in (22,509,510,511):fat[reserved*2:reserved*2+2]=(0xfffa).to_bytes(2,byte_order)
        directory=bytearray(1024);directory[:32]=entry(0xcc,b'SONIC');other=bytearray(entry(0x33,b'DATA'));other[2:4]=(22).to_bytes(2,byte_order);other[24:26]=(1).to_bytes(2,byte_order);directory[512:544]=other
        destinations=[v.load(s.m,s.descriptor+off) for off in (0x20,0x24,0x28)]
        plan=[(511,destinations[0],root),(509,destinations[1],bytes(fat[:512])),(510,destinations[1]+512,bytes(fat[512:])),(507,destinations[2],directory[:512]),(506,destinations[2]+512,directory[512:])]
        for block,dest,data in plan:
            for part in range(2):transfer(data[part*256:(part+1)*256]);s.resume();harvest()
            require(v.sample(s.m,dest,512)==data,'independent raw initialization bytes')
        require(v.load(s.m,s.result_word)==0 and v.sample(s.m,s.descriptor+10,3)==bytes((1,1,1)),'actual initialization completion')
        buffers=[v.sample(s.m,a,n) for a,n in zip(destinations,(512,1024,1024))]
        initial_reads=[b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e']
        require(initial_reads==[[channel,b,d] for b,d,_ in plan],'independent original initialization read plan')

        array=0x8cdb0000;strings=0x8cdb0100;caller=bytearray([0x6d])*0x500;labels=[x.encode() for x in config['labels']]
        for j,label in enumerate(labels):caller[j*4:j*4+4]=(strings+j*64).to_bytes(4,'little');caller[0x100+j*64:0x100+j*64+len(label)+1]=label+b'\0'
        raw(s.m,array,caller);pool=v.sample(s.m,0x8c8a1210,8*72);metadata=v.sample(s.m,s.output,76)
        oracle_root=bytearray(root);oracle_fat=bytearray(fat);oracle_dir=bytearray(directory);oracle_dirty=bytearray(v.sample(s.m,s.descriptor+0x48,32));delete_results=[]
        for label in labels[:max(0,count)]:
            found=next((j for j in (0,16) if oracle_dir[j*32] and oracle_dir[j*32+4:j*32+16].split(b'\0',1)[0]==label),None)
            if found is None:delete_results.append(-251);continue
            if found==0 and kind!='bad_chain':oracle_fat[40:44]=(0xfffc).to_bytes(2,byte_order)*2
            if found==16:oracle_fat[44:46]=(0xfffc).to_bytes(2,byte_order)
            oracle_dir[found*32:(found+1)*32]=bytes(32);oracle_dirty[found//16]=1;delete_results.append(0)
        expected_plan=[dict(role='dir',block=507 if j==0 else 506,index=j,error=None) for j in (0,1) if oracle_dirty[j]]+[dict(role='fat',block=509+j,index=j,error=None) for j in (0,1)]
        result=-246 if any(x<0 for x in delete_results) else 0
        if config['error']:
            role,index,phase,status=config['error'];at=next(i for i,item in enumerate(expected_plan) if item['role']==role and item['index']==index);expected_plan=expected_plan[:at+1];expected_plan[-1]['error']=[phase,status]
            result=-255 if status==8 else -252 if role=='dir' else -192
        require(oracle_dirty[:2]==bytes((int(any(x['role']=='dir' and x['index']==0 for x in expected_plan)),int(any(x['role']=='dir' and x['index']==1 for x in expected_plan)))),'independent original dirty sectors')
        require(v.load(s.m,v.load(s.m,0x8c660e38))==0,'constructor NULL optional write hook');put(s.m,0x8c66d3a0,0)
        counter_base=v.load(s.m,v.load(s.m,0x8c649eec)+v.load(s.m,s.descriptor+4)*80)
        first=len(s.phases);s.call('original_batch_arguments_API',0x8c647cc4,[channel,channel,array,count],(0,0,0));harvest();s.call('original_batch_opcode_API',0x8c647cb4,[channel,17]);harvest();s.resume();harvest()
        cursor=first;packets=[];progress=[];events=[];postyield=set();failures=0;ledger={block:bytes(data) for block,dest,data in plan}
        def check_buffers():
            require(v.sample(s.m,destinations[0],512)==oracle_root,'entire independent RAM root')
            require(v.sample(s.m,destinations[1],1024)==oracle_fat,'entire independent RAM FAT')
            require(v.sample(s.m,destinations[2],1024)==oracle_dir and v.sample(s.m,s.descriptor+0x48,32)==oracle_dirty,'entire independent directory and dirty flags')
            require(v.sample(s.m,array,len(caller))==caller and v.sample(s.m,0x8c8a1210,8*72)==pool and v.sample(s.m,s.output,76)==metadata,'caller names/array/guards,handlepool,decoded metadata unchanged')
        check_buffers()
        for tx,item in enumerate(expected_plan):
            staged=bytearray(512);role=item['role'];offset=item['index']*512
            for phase in range(5):
                require(s.tokens()[0]>0,'actual original queued request')
                inputs=[b['write_request_input'] for rr,path in s.phases[cursor:] for b in rr['boundaries'] if 'write_request_input' in b];cursor=len(s.phases);require(len(inputs)==1,'one original packet per literal phase');req=inputs[0];commit=phase==4
                payload=oracle_dir[offset:offset+512] if role=='dir' else oracle_fat[offset:offset+512];ptr=destinations[2 if role=='dir' else 1]+offset
                require(req['mapped_unit']==s.mapped and req['command']==(13 if commit else 12) and req['parameters']==[2,(phase<<16)|item['block']],'independent unit/command/block/phase')
                if commit:require(req['words']==0 and req['data']==[],'empty original commit')
                else:require(req['words']==32 and int(req['source'],16)==ptr+phase*128 and req['data']==list(payload[phase*128:(phase+1)*128]),'exact independent128-byte payload/pointer');staged[phase*128:(phase+1)*128]=bytes(req['data'])
                require(frame_pc()==(0x8c660cf6 if commit else 0x8c660c7c),'actual original response-wait frame')
                failed=bool(item['error'] and phase==item['error'][0]);status=item['error'][1] if failed else 7
                packets.append(dict(transaction=tx,role=role,block=item['block'],phase=phase,status=status,payload_sha256=__import__('hashlib').sha256(bytes(req['data'])).hexdigest()))
                transfer(status=status,ack=True)
                if commit or failed:
                    if failed:failures+=1
                    if commit and not failed:require(staged==payload,'complete independently acknowledged block');ledger[item['block']]=bytes(staged)
                    if role=='dir':
                        if failed and status==251:oracle_fat[item['block']*2:item['block']*2+2]=(0xffff).to_bytes(2,byte_order)
                        if not failed:oracle_dirty[item['index']]=0
                    else:oracle_fat[item['block']*2:item['block']*2+2]=(0xffff if failed else 0xfffa).to_bytes(2,byte_order)
                    progress.append([channel,17,len(progress)+1,counter_base+failures]);events.append(dict(role=role,block=item['block'],failed=failed))
                rr,path=s.resume();harvest();postyield.update(path);check_buffers()
                if failed:break
        bs=[b for rr,path in s.phases[first:] for b in rr['boundaries']];cb=[b for b in bs if b['pr']=='0x8c647bb4']
        require(s.tokens()[0]==0 and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'worker complete,idle,no extra request')
        require(v.load(s.m,s.result_word)==result&0xffffffff and len(cb)==1 and cb[0]['arguments'][:3]==[channel,17,result&0xffffffff],'independent result/callback precedence')
        pointers=[strings+j*64 for j in range(max(0,count))]
        require([b['arguments'][:2] for b in bs if b['target']=='0x8c64b2dc']==[[channel,p] for p in pointers],'complete preflight pass in caller order')
        require([b['arguments'][:2] for b in bs if b['target']=='0x8c649c68']==[[channel,p] for p in pointers],'complete delete pass continues after missing/duplicate names')
        require([b['target'] for b in bs if b['target'] in ('0x8c64ab36','0x8c64afe0')]==(['0x8c64ab36'] if config['error'] and config['error'][0]=='dir' else ['0x8c64ab36','0x8c64afe0']),'original directory-then-FAT persistence and early error stop')
        require([b['arguments'] for b in bs if b['target']=='0x8c647bde']==progress and [v.load(s.m,s.descriptor+off) for off in (0x30,0x34)]==[1+len(events),counter_base+failures],'independent progress/final counters')
        require([b['arguments'][:3] for rr,path in s.phases for b in rr['boundaries'] if b['target']=='0x8c660a7e']==initial_reads,'no hidden readback')
        s.resume();harvest();s.call('original_completed_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'actual original owner teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,result=hex(result&0xffffffff),delete_results=delete_results,transactions=events,packets=packets,progress=progress,postyield_parent_pcs=sorted(hex(pc) for pc in postyield if pc in ROUTES),phases=s.records,independent_checks=case_checks,ack_ledger_not_physical_durability=True,failed_block_storage_state_proven=False))
        print('COMMAND17 TASK',kind,'PASS',comparisons,checks,flush=True)
    coverage=[]
    for body in v.ANALYSIS_ROUTINES:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges;coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'movie_filesystem_command17_vectors.py',P/'movie_filesystem_command17_reference.py',P/'movie_filesystem_command17_session.py',P/'movie_filesystem_command17_task_fixtures.py',P/'write_watch_filesystem_command17.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_command17_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for c in records for pc in c['postyield_parent_pcs']}),dll_sha256=digest(v.effects.DLL),sha256={str(f):digest(f) for f in files},scope='Actual initialized opcode17 batch deletion via original raw metadata/root/FAT/linkedDIR,argument/opcode APIs/scheduler/transport/pump. Independent whole buffers/caller preservation,payload/block/phase/commit plan,preflight then all deletions including missing/duplicate,DIR thenFAT persistence/error precedence,progress/counters/callback/idle/teardown. OriginalconstructorNULLhook,retry0API/explicitabort0;no suppliedcalleeoutput/parsedmetadata/frame/ID/PR. ACKledger notphysicalflash/durability/atomicity;failedblockcontentsunproved.')
    suffix='_'+str(shard) if shard is not None else '';dest=P/'analysis'/('movie_filesystem_command17_tasks'+suffix+'_verification.json')
    if '--case' in sys.argv or smoke:dest=P.parents[1]/'work/sonic-adventure'/('command17_case_'+configs[0]['kind']+'.json')
    dest.write_text(json.dumps(report,indent=2)+'\n');print('COMMAND17 TASKS PASS',comparisons,checks,flush=True)

ROUTES={0x8c647b4e,0x8c649e46,0x8c649e50,0x8c649e5a,0x8c649e76,0x8c649e92,0x8c649eae,0x8c649ec0,0x8c649ecc}

def configurations():
    items=[('one',['SONIC'],None,None),('two',['SONIC','DATA'],None,None),('reverse',['DATA','SONIC'],None,None),('missing',['MISSING'],None,None),('mixed',['SONIC','MISSING','DATA'],None,None),('duplicate',['SONIC','SONIC'],None,None),('missing_first',['MISSING','DATA'],None,None),('zero',[],0,None),('negative',[],-1,None),('bad_chain',['SONIC'],None,None),('dir_abort',['SONIC'],None,['dir',0,0,8]),('later_dir_abort',['SONIC','DATA'],None,['dir',1,0,8]),('dir_no_space',['SONIC'],None,['dir',0,4,251]),('fat_abort',['SONIC'],None,['fat',0,0,8]),('fat_late_abort',['SONIC'],None,['fat',1,0,8]),('fat_no_space',['SONIC'],None,['fat',0,4,251]),('missing_dir_abort',['MISSING','SONIC'],None,['dir',0,0,8]),('missing_fat_abort',['SONIC','MISSING'],None,['fat',0,0,8])]
    for i,(kind,labels,count,error) in enumerate(items):
        channel,order,endian=((0,0,0),(7,1,1),(0,1,0),(7,0,1))[i%4];yield dict(kind=kind,channel=channel,order=order,endian=endian,count=len(labels) if count is None else count,start=0,labels=labels,error=error)
if __name__=='__main__':main()
