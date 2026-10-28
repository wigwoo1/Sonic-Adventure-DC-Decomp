"""Original command20 writes with raw device ACK/error/removal inputs only."""
import itertools,json,sys
from pathlib import Path
from movie_filesystem_command12_vectors import Session,v,raw,put,digest,P
import movie_channel_request_fixtures as backend

def configurations():
    for i,(channel,order,endian) in enumerate(itertools.product((0,7),(0,1),(0,1))):
        yield dict(kind='core',channel=channel,order=order,endian=endian,count=i%3,start=1 if i==7 else 0)
    for i,kind in enumerate(('bad_data','bad_commit','special','unplug','abort_unplug','abort_bad','retry_success','retry_exhaust','live_infinite','protocol_data','protocol_commit','mutex_data','mutex_commit','retry_unplug','bad_chain')):
        yield dict(kind=kind,channel=7 if i&1 else 0,order=i&1,endian=(i>>1)&1,count=1,start=0)

def main():
    assert v.effects.DLL.name=='movie_filesystem_command12.dll'
    if '--merge' in sys.argv:raise SystemExit('Use reviewed reports')
    configs=list(configurations());smoke='--smoke' in sys.argv
    shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
    if shard is not None:
        assert shard in range(3)
        configs=[c for i,c in enumerate(configs) if i%3==shard]
    if smoke:configs=configs[:1]
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
            s.observe(511,status=status,unplug=unplug,ack=ack,block_data=data,metadata_words={6:509,8:2,10:507,12:2,16:32,18:8} if data is None and not ack else None);intact();s.pump();harvest();intact();require(s.tokens()[1:]==[0,queued],'actual completed request ID')
        harvest();s.queue();harvest();transfer();s.resume();harvest()
        byte_order='big' if endian else 'little';root=b'U'*16+bytes(496);fat=bytearray(1024)
        for index,value in ((507,506),(506,0xfffa),(20,20 if kind=='bad_chain' else 21),(21,0xfffa)):fat[index*2:index*2+2]=value.to_bytes(2,byte_order)
        root=bytearray(root)
        for off,value in ((0x46,509),(0x48,2),(0x4a,507),(0x4c,2)):root[off:off+2]=value.to_bytes(2,byte_order)
        for free in range(24,40):fat[free*2:free*2+2]=(0xfffc).to_bytes(2,byte_order)
        for reserved in (22,509,510,511):fat[reserved*2:reserved*2+2]=(0xfffa).to_bytes(2,byte_order)
        def entry(type_,label):
            b=bytearray(32);b[0]=type_;b[1]=0xa5;b[2:4]=(20).to_bytes(2,byte_order);b[4:16]=(label+bytes(12))[:12];b[16:24]=bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,3));b[24:26]=(2).to_bytes(2,byte_order);b[26:28]=(1).to_bytes(2,byte_order);return bytes(b)
        other=bytearray(entry(0x33,b'DATA'));other[2:4]=(22).to_bytes(2,byte_order);other[24:26]=(1).to_bytes(2,byte_order);directory=entry(0xcc,b'SONIC')+other+bytes(960)
        destinations=[v.load(s.m,s.descriptor+off) for off in (0x20,0x24,0x28)]
        plan=[(511,destinations[0],root),(509,destinations[1],bytes(fat[:512])),(510,destinations[1]+512,bytes(fat[512:])),(507,destinations[2],directory[:512]),(506,destinations[2]+512,directory[512:])]
        for block,dest,data in plan:
            for part in range(2):transfer(data[part*256:(part+1)*256]);s.resume();harvest()
            require(v.sample(s.m,dest,512)==data,'independent raw initialization bytes')
        require(v.load(s.m,s.result_word)==0 and v.sample(s.m,s.descriptor+10,3)==bytes((1,1,1)),'actual initialization completion')
        buffers=[v.sample(s.m,a,n) for a,n in zip(destinations,(512,1024,1024))]
        initial_reads=[b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e']
        require(initial_reads==[[channel,b,d] for b,d,_ in plan],'independent original initialization read plan')


        require(v.sample(s.m,0x8c8a1210,8*72)==bytes(8*72),'original empty handle pool');s.call('original_caller_handle_allocate',0x8c64b2a6,[channel]);harvest();handle=s.s.r[0];require(handle==0x8c8a1210,'original firstfree caller handle')
        name=bytes.fromhex(config['name_hex']);stamp=bytes.fromhex('2026102811223302');h=bytearray(72);h[0]=1;h[1]=config['byte'];h[2:4]=config['word'].to_bytes(2,'little');h[4:8]=(1).to_bytes(4,'little');h[8:12]=config['type'].to_bytes(4,'little');h[16:28]=name;h[32:40]=stamp
        for off,value in ((0x28,channel),(0x2c,1),(0x30,1),(0x38,22),(0x3c,22)):h[off:off+4]=value.to_bytes(4,'little')
        raw(s.m,handle,h);prior=[v.load(s.m,s.descriptor+off) for off in (0x30,0x34)];base_counter=v.load(s.m,v.load(s.m,0x8c64926c)+v.load(s.m,s.descriptor+4)*80);initial=[1,base_counter] if config['reset'] else prior
        oracle_root=bytes(root);oracle_fat=bytearray(fat);oracle_dir=bytearray(directory);oracle_dirty=bytearray(v.sample(s.m,s.descriptor+0x48,32));entry=bytearray(32);entry[0]=0xcc if config['type']==1 else 0x33;entry[1]=config['byte'];entry[2:4]=(22).to_bytes(2,byte_order);entry[4:16]=name;entry[16:24]=stamp;entry[24:26]=(1).to_bytes(2,byte_order);entry[26:28]=config['word'].to_bytes(2,byte_order);oracle_dir[32:64]=entry;oracle_dirty[0]=1
        plan_expected=[dict(role='dir',block=507,index=0,error='data8' if kind=='dir_abort' else None)]
        if kind!='dir_abort':plan_expected+=[dict(role='fat',block=509,index=0,error='data8' if kind=='fat_abort' else None)]
        if kind not in ('dir_abort','fat_abort'):plan_expected+=[dict(role='fat',block=510,index=1,error=None)]
        first=len(s.phases);s.call('original_close_arguments_API',0x8c647cc4,[channel,handle,config['reset'],0],(0,0,0));harvest();s.call('original_close_opcode_API',0x8c647cb4,[channel,4]);harvest();s.resume();harvest()
        cursor=first;phase=tx_at=failures=0;events=[];packets=[];postyield=set();progress=[]
        def buffers():
            require(v.sample(s.m,destinations[0],512)==oracle_root and v.sample(s.m,destinations[1],1024)==oracle_fat,'independent full root/FAT')
            require(v.sample(s.m,destinations[2],1024)==oracle_dir and v.sample(s.m,s.descriptor+0x48,32)==oracle_dirty,'independent exact type/byte/head/name12/timestamp8/size16/attribute16 and dirtybits')
        buffers()
        while tx_at<len(plan_expected):
            item=plan_expected[tx_at];require(s.tokens()[0]>0,'actual queued close request');items=[b['write_request_input'] for rr,path in s.phases[cursor:] for b in rr['boundaries'] if 'write_request_input' in b];cursor=len(s.phases);require(len(items)==1,'one original close packet');req=items[0];commit=phase==4;off=item['index']*512;payload=oracle_dir[off:off+512] if item['role']=='dir' else oracle_fat[off:off+512];ptr=(destinations[2] if item['role']=='dir' else destinations[1])+off
            require(req['mapped_unit']==s.mapped and req['command']==(13 if commit else 12) and req['parameters']==[2,(phase<<16)|item['block']],'independent close unit/block/phase/data-or-commit')
            require((req['words']==0 and req['data']==[]) if commit else (req['words']==32 and int(req['source'],16)==ptr+phase*128 and req['data']==list(payload[phase*128:(phase+1)*128])),'independent full original metadata payload/source')
            failed=item['error']=='data8' and phase==0;status=8 if failed else 7;packets.append(dict(role=item['role'],block=item['block'],phase=phase,status=status));transfer(status=status,ack=True)
            if commit or failed:
                if failed:failures+=1
                if item['role']=='dir' and not failed:oracle_dirty[0]=0
                if item['role']=='fat':oracle_fat[item['block']*2:item['block']*2+2]=(0xffff if failed else 0xfffa).to_bytes(2,byte_order)
                progress.append([channel,4,initial[0]+len(events),initial[1]+failures]);events.append(dict(role=item['role'],block=item['block'],failed=failed));tx_at+=1;phase=0
            else:phase+=1
            r,path=s.resume();harvest();postyield.update(path);buffers()
        result=-255&0xffffffff if kind=='dir_abort' else 0
        require(v.load(s.m,s.result_word)==result and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'original standalone close callback/idle including propagatedDIR error/ignoredFAT error')
        bs=[b for rr,path in s.phases[first:] for b in rr['boundaries']];cb=[b for b in bs if b['pr']=='0x8c647bb4'];require(len(cb)==1 and cb[0]['arguments'][:3]==[channel,4,result],'one actual opcode4 callback')
        require([b['arguments'] for b in bs if b['target']=='0x8c647bde']==progress,'independent reset-or-retained progress sequence')
        require([v.load(s.m,s.descriptor+off) for off in (0x30,0x34)]==[initial[0]+len(events),initial[1]+failures],'independent reset-or-retained final counters')
        require(v.sample(s.m,handle,72)==bytes(72) and v.sample(s.m,0x8c8a1210,8*72)==bytes(8*72),'original full72byte release and otherpoolrecords preserved')
        require([b['arguments'][:3] for rr,path in s.phases for b in rr['boundaries'] if b['target']=='0x8c660a7e']==initial_reads,'no hidden file-data reads')
        s.resume();harvest();s.call('original_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'original owner teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,result=hex(result),transactions=events,packets=packets,progress=progress,postyield_parent_pcs=sorted(hex(pc) for pc in postyield if pc in ROUTES),phases=s.records,independent_checks=case_checks))
        print('COMMAND12 CLOSEWORKER',len(records),kind,channel,config['type'],config['reset'],'PASS',comparisons,flush=True)
    coverage=[]
    for body in v.ANALYSIS_ROUTINES:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'verify_movie_filesystem_write_tasks.py',P/'movie_filesystem_command12_vectors.py',P/'movie_filesystem_command12_session.py',P/'movie_filesystem_command12_reference.py',P/'write_watch_filesystem_command12.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_command12_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for rec in records for pc in rec['postyield_parent_pcs']}),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original rawinitializedfilesystem plusgenuineopcode4 close throughoriginalargument/opcode APIs/scheduler. Declaredcaller-ownedhandleABIcontents;originalallocatorallocatestherecord,fieldpayloadexplicitinput ratherthanfalselyclaimedwrite-openacquisition. ExactCC/33type,byte/head/full12nametimestamp8/size16/signed-low16attribute/dirty/counterreset,realDIR/FATACK/error/clear/callback/idle/ownerteardown. No suppliedtaskframes/IDs/PR/calleeoutputs;notphysicalDMA/flashdurability/fullfilesystem.')
    name='movie_filesystem_command12_close_worker'+('' if shard is None else '_'+str(shard))+'_verification.json';(P/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n');print('COMMAND12 CLOSEWORKER PASS',comparisons,checks,flush=True)

ROUTES={0x8c6492e0,0x8c6492ec,0x8c647a36}
def configurations():
    for channel in (0,7):
        for i,(type_,reset) in enumerate(((0,0),(0,1),(1,0),(1,1))):yield dict(kind='normal',channel=channel,order=int(channel==7),endian=int(channel==7),count=1,start=0,type=type_,reset=reset,byte=0x80 if type_ else 0xff,word=0x8001 if type_ else 0xffff,name_hex=(b'ABCDEFGHIJKL' if reset else b'DATA\0XYZ1234').hex())
    for kind in ('dir_abort','fat_abort'):yield dict(kind=kind,channel=0,order=0,endian=0,count=1,start=0,type=1,reset=1,byte=0,word=0x7fff,name_hex=(b'DATA'+bytes(8)).hex())
if __name__=='__main__':main()
