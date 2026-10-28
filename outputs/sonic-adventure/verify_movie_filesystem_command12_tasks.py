"""Original command20 writes with raw device ACK/error/removal inputs only."""
import itertools,json,sys
from pathlib import Path
from movie_filesystem_command12_vectors import Session,v,raw,put,digest,P
import movie_channel_request_fixtures as backend
ROOT=P.parents[1]

def configurations():
    for i,(channel,order,endian) in enumerate(itertools.product((0,7),(0,1),(0,1))):
        yield dict(kind='core',channel=channel,order=order,endian=endian,count=i%3,start=1 if i==7 else 0)
    for i,kind in enumerate(('bad_data','bad_commit','special','unplug','abort_unplug','abort_bad','retry_success','retry_exhaust','live_infinite','protocol_data','protocol_commit','mutex_data','mutex_commit','retry_unplug','bad_chain')):
        yield dict(kind=kind,channel=7 if i&1 else 0,order=i&1,endian=(i>>1)&1,count=1,start=0)

def main():
    assert v.effects.DLL.name=='movie_filesystem_command12.dll'
    if '--merge' in sys.argv:raise SystemExit('Use dedicated reviewed merge script')
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
            s.observe(511,status=status,unplug=unplug,ack=ack,block_data=data,metadata_words={6:509,8:2,10:507,12:2,16:32,18:config['pool_count']} if data is None and not ack else None);intact();s.pump();harvest();intact();require(s.tokens()[1:]==[0,queued],'actual completed request ID')
        harvest();s.queue();harvest();transfer();s.resume();harvest()
        byte_order='big' if endian else 'little';root=b'U'*16+bytes(496);fat=bytearray(1024)
        for index,value in ((507,506),(506,0xfffa),(20,20 if kind=='bad_chain' else 21),(21,0xfffa)):fat[index*2:index*2+2]=value.to_bytes(2,byte_order)
        root=bytearray(root)
        for off,value in ((0x46,509),(0x48,2),(0x4a,507),(0x4c,2)):root[off:off+2]=value.to_bytes(2,byte_order)
        for free in list(range(24,24+config['free_count']))+list(range(32,32+config['pool_count'])):fat[free*2:free*2+2]=(0xfffc).to_bytes(2,byte_order)
        for reserved in (22,509,510,511):fat[reserved*2:reserved*2+2]=(0xfffa).to_bytes(2,byte_order)
        if config['primary_free']:fat[1016:1018]=(0xfffc).to_bytes(2,byte_order)
        def entry(type_,label):
            b=bytearray(32);b[0]=type_;b[1]=0xa5;b[2:4]=(20).to_bytes(2,byte_order);b[4:16]=(label+bytes(12))[:12];b[16:24]=bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,3));b[24:26]=(2).to_bytes(2,byte_order);b[26:28]=(1).to_bytes(2,byte_order);return bytes(b)
        directory=bytearray(1024);index=config['index'];directory[index*32:(index+1)*32]=entry(0xcc,b'SONIC');other=bytearray(entry(0x33,b'DATA'));other[2:4]=(22).to_bytes(2,byte_order);other[24:26]=(1).to_bytes(2,byte_order);other_index=0 if index else 1;directory[other_index*32:(other_index+1)*32]=other
        if index:
            for j in range(1,index):
                rec=bytearray(entry(0x33,('F%02d'%j).encode()));rec[2:4]=j.to_bytes(2,byte_order);rec[24:26]=(1).to_bytes(2,byte_order);directory[j*32:(j+1)*32]=rec;fat[j*2:j*2+2]=(0xfffa).to_bytes(2,byte_order)
        if kind=='full_directory':
            for j in range(32):
                rec=bytearray(entry(0x33,('F%02d'%j).encode()));rec[2:4]=j.to_bytes(2,byte_order);rec[24:26]=(1).to_bytes(2,byte_order);directory[j*32:(j+1)*32]=rec;fat[j*2:j*2+2]=(0xfffa).to_bytes(2,byte_order)
        destinations=[v.load(s.m,s.descriptor+off) for off in (0x20,0x24,0x28)]
        plan=[(511,destinations[0],root),(509,destinations[1],bytes(fat[:512])),(510,destinations[1]+512,bytes(fat[512:])),(507,destinations[2],directory[:512]),(506,destinations[2]+512,directory[512:])]
        for block,dest,data in plan:
            for part in range(2):transfer(data[part*256:(part+1)*256]);s.resume();harvest()
            require(v.sample(s.m,dest,512)==data,'independent raw initialization bytes')
        require(v.load(s.m,s.result_word)==0 and v.sample(s.m,s.descriptor+10,3)==bytes((1,1,1)),'actual initialization completion')
        buffers=[v.sample(s.m,a,n) for a,n in zip(destinations,(512,1024,1024))]
        initial_reads=[b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e']
        require(initial_reads==[[channel,b,d] for b,d,_ in plan],'independent original initialization read plan')

        out=0x8cdb0000;data=bytes((j*19+31+config['seed'])&255 for j in range(2048));raw(s.m,out,data)
        stamp=(2026).to_bytes(2,'little')+bytes((10,28,11,22,33,3));raw(s.m,s.channel_record+0x6c,stamp);encoded_stamp=bytes.fromhex('2026102811223302')
        label=b'NEWFILE' if kind in ('new_file','empty_new','full_directory') else b'SONIC';n=config['write_count'];flags=config['flags'];verify=bool(flags&0x80000000)
        require(v.load(s.m,0x8c66d3a4)==0,'original retail free-count policy0 retained after construction/acquisition');put(s.m,0x8c66d3a0,0)
        acquired_pool=v.sample(s.m,0x8c8a1210,8*72);require(acquired_pool==bytes(8*72),'original eight empty handles');allocated=[]
        if kind=='pool_full':
            for j in range(8):s.call('original_allocator_pool_fill',0x8c64b2a6,[channel]);harvest();allocated.append(s.s.r[0]);require(allocated[-1]==0x8c8a1210+j*72,'real first-free handle acquisition')
        pool_before=v.sample(s.m,0x8c8a1210,8*72);prior_counters=[v.load(s.m,s.descriptor+off) for off in (0x30,0x34)]
        counter_base=v.load(s.m,v.load(s.m,0x8c64b820)+v.load(s.m,s.descriptor+4)*80)
        oracle_root=bytearray(root);oracle_fat=bytearray(fat);oracle_dir=bytearray(directory);oracle_dirty=bytearray(v.sample(s.m,s.descriptor+0x48,32));fat_base=509;dir_map=[507,506]
        early=kind in ('capacity_fail','full_directory','pool_full');result_code=(-252 if kind=='capacity_fail' else -248 if kind=='full_directory' else -249 if kind=='pool_full' else -255 if kind=='data_abort' else -252 if kind=='data_nospace' else -240 if kind=='verify_mismatch' else -255 if kind=='verify_read_error' else -191 if kind=='verify_empty' else 0)&0xffffffff
        old_exists=label==b'SONIC'
        if old_exists and kind!='capacity_fail':
            oracle_dir[index*32:(index+1)*32]=bytes(32);oracle_dirty[index//16]=1
            for block in (20,21):oracle_fat[block*2:block*2+2]=(0xfffc).to_bytes(2,byte_order)
        new_index=index if old_exists else 2
        di=new_index//16;db=507 if di==0 else 506
        def tx(role,block,index=0,error=None,action=None):return dict(role=role,block=block,index=index,error=error,action=action)
        dat=[] if early else [tx('data',31-j,j) for j in range(n)]
        if kind=='data_abort':dat=[tx('data',31,0,'data8')]
        if kind=='data_bad':dat=[tx('data',31,0,'commit251'),tx('data',30,0)]
        if kind=='data_retry_abort':dat=[tx('data',31,0,'commit251'),tx('data',30,0,'data8'),tx('data',29,0)]
        if kind=='data_nospace':dat=[tx('data',21,0,'commit251'),tx('data',20,0,'commit251')]
        d=[tx('dir',db,di)];fs=[tx('fat',509,0),tx('fat',510,1)];rt=tx('root',511)
        if kind=='dir_abort':d=[tx('dir',db,di,'data8')];fs=[]
        if kind=='dir_nospace':d=[tx('dir',db,di,'commit251')];fs=[]
        if kind in ('dir_reloc','dir_reloc_second','dir_fat_abort','dir_root_abort'):
            d=[tx('dir',db,di,'commit251',('dir_reloc',di,db,508)),tx('dir',508,di)]
            if kind=='dir_fat_abort':fs=[tx('fat',509,0,'data8')]
            elif kind=='dir_root_abort':fs=fs+[tx('root',511,0,'data8')]
            else:fs=fs+[rt]+[tx('fat',509,0),tx('fat',510,1)]
        if kind=='fat_abort':fs=[tx('fat',509,0,'data8')]
        if kind in ('fat_reloc','fat_root_ignored'):fs=[tx('fat',509,0,'commit251',('fat_reloc',38)),tx('fat',38,0),tx('fat',39,1),tx('root',511,0,'data8' if kind=='fat_root_ignored' else None)]
        plan_expected=[] if early else dat+d+fs;successful=[]
        def serialize():
            record=bytearray(32);record[0]=0x33;record[1]=flags&255;record[2:4]=(successful[0] if successful else 0xffff).to_bytes(2,byte_order);record[4:16]=(label+bytes(12))[:12];record[16:24]=encoded_stamp;record[24:26]=len(successful).to_bytes(2,byte_order)
            oracle_dir[new_index*32:(new_index+1)*32]=record;oracle_dirty[di]=1
        if not early and not dat:serialize()
        raw(s.m,s.channel_record+0x2c,label+bytes(13-len(label)));first=len(s.phases);s.call('original_write_arguments_API',0x8c647cc4,[channel,channel,0,out],(n,0,flags));harvest();s.call('original_write_opcode_API',0x8c647cb4,[channel,12]);harvest();s.resume();harvest()
        tx_at=phase=failures=0;cursor=first;staged=bytearray(512);ledger={block:bytes(blob) for block,dest,blob in plan};events=[];packets=[];expected_progress=[];postyield_seen=set()
        def buffers():
            require(v.sample(s.m,destinations[0],512)==oracle_root,'independent complete RAM root')
            require(v.sample(s.m,destinations[1],1024)==oracle_fat,'independent complete FAT and original reserved/bad/free markers')
            require(v.sample(s.m,destinations[2],1024)==oracle_dir and v.sample(s.m,s.descriptor+0x48,32)==oracle_dirty,'independent complete directory and dirty bits')
            require(v.sample(s.m,out,2048)==data,'caller data unchanged')
            require(v.load(s.m,s.output+0xc)==fat_base,'independent cached FAT base')
        buffers()
        while tx_at<len(plan_expected):
            item=plan_expected[tx_at];require(s.tokens()[0]>0,'actual queued write request for literal plan');inputs=[b['write_request_input'] for rr,path in s.phases[cursor:] for b in rr['boundaries'] if 'write_request_input' in b];cursor=len(s.phases);require(len(inputs)==1,'one original write packet');req=inputs[0];commit=phase==4;role=item['role'];offset=item['index']*512
            payload=data[offset:offset+512] if role=='data' else oracle_dir[offset:offset+512] if role=='dir' else oracle_fat[offset:offset+512] if role=='fat' else oracle_root
            ptr=out+offset if role=='data' else destinations[2]+offset if role=='dir' else destinations[1]+offset if role=='fat' else destinations[0]
            require(req['mapped_unit']==s.mapped and req['command']==(13 if commit else 12) and req['parameters']==[2,(phase<<16)|item['block']],'independent unit/command/block/phase')
            if not commit:require(req['words']==32 and int(req['source'],16)==ptr+phase*128 and req['data']==list(payload[phase*128:(phase+1)*128]),'independent caller/metadata pointer and128-byte payload');staged[phase*128:(phase+1)*128]=bytes(req['data'])
            else:require(req['words']==0 and req['data']==[],'empty original commit')
            failed=(item['error']=='data8' and phase==0) or (item['error']=='commit251' and commit);status=8 if failed and item['error']=='data8' else 251 if failed else 7
            require(frame_pc()==(0x8c660cf6 if commit else 0x8c660c7c),'actual backend response-wait frame')
            packets.append(dict(transaction=tx_at,role=role,block=item['block'],phase=phase,status=status,payload_sha256=__import__('hashlib').sha256(bytes(req['data'])).hexdigest()));transfer(status=status,ack=True)
            if commit or failed:
                if failed:failures+=1
                if commit and not failed:require(staged==payload,'complete acknowledged512byte transaction');ledger[item['block']]=bytes(staged)
                if role=='data':
                    if failed and not (kind=='data_abort'):oracle_fat[item['block']*2:item['block']*2+2]=(0xffff).to_bytes(2,byte_order)
                    if not failed:
                        if successful:oracle_fat[successful[-1]*2:successful[-1]*2+2]=item['block'].to_bytes(2,byte_order)
                        oracle_fat[item['block']*2:item['block']*2+2]=(0xfffa).to_bytes(2,byte_order);successful.append(item['block'])
                    if tx_at+1==len(dat):serialize()
                if role=='dir':
                    if failed and item['error']!='data8':oracle_fat[item['block']*2:item['block']*2+2]=(0xffff).to_bytes(2,byte_order)
                    if not failed:oracle_dirty[item['index']]=0
                if role=='fat':oracle_fat[item['block']*2:item['block']*2+2]=(0xffff if failed else 0xfffa).to_bytes(2,byte_order)
                action=item['action']
                if action:
                    if action[0]=='dir_reloc':
                        _,at,old,new=action;oracle_fat[new*2:new*2+2]=(506 if at==0 else 0xfffa).to_bytes(2,byte_order)
                        if at==0:oracle_root[0x4a:0x4c]=new.to_bytes(2,byte_order)
                        else:oracle_fat[dir_map[at-1]*2:dir_map[at-1]*2+2]=new.to_bytes(2,byte_order)
                        dir_map[at]=new
                    else:
                        fat_base=action[1];oracle_root[0x46:0x48]=fat_base.to_bytes(2,byte_order)
                        for block in range(fat_base,fat_base+2):oracle_fat[block*2:block*2+2]=(0xfffa).to_bytes(2,byte_order)
                expected_progress.append([channel,12,len(expected_progress)+1,counter_base+(2*n if verify else n)+failures]);events.append(dict(role=role,block=item['block'],failed=failed));tx_at+=1;phase=0;staged=bytearray(512)
            else:phase+=1
            r,path=s.resume();harvest();postyield_seen.update(path);buffers()
        reads_expected=[];read_events=[]
        if verify and kind!='verify_empty':
            for j,block in enumerate(successful):
                if kind in ('verify_mismatch','verify_read_error') and j:break
                scratch=0x8c89cfb0+channel*0x800;reads_expected.append([channel,block,scratch])
                actual=[b['arguments'][:3] for rr,path in s.phases[first:] for b in rr['boundaries'] if b['target']=='0x8c660a7e'];require(actual==reads_expected,'independent actual FAT-followed readback plan')
                if kind=='verify_read_error':
                    transfer(status=7,ack=True);failures+=1;r,path=s.resume();harvest();postyield_seen.update(path);expected_progress.append([channel,12,len(expected_progress)+1,counter_base+2*n+failures]);read_events.append(dict(block=block,error=-255));break
                block_data=bytearray(data[j*512:(j+1)*512])
                if kind=='verify_mismatch':block_data[0]^=255
                for half in range(2):transfer(bytes(block_data[half*256:(half+1)*256]));r,path=s.resume();harvest();postyield_seen.update(path)
                require(v.sample(s.m,scratch,512)==block_data,'independent complete original readback scratch bytes');expected_progress.append([channel,12,len(expected_progress)+1,counter_base+2*n+failures]);read_events.append(dict(block=block,mismatch=kind=='verify_mismatch'))
        require(not s.tokens()[0] and v.load(s.m,s.channel_record+8)==0,'literal transaction/readback plan reaches original idle')
        require(v.load(s.m,s.result_word)==result_code and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'independent original completion result')
        bs=[b for rr,path in s.phases[first:] for b in rr['boundaries']];cb=[b for b in bs if b['pr']=='0x8c647bb4'];require(len(cb)==1 and cb[0]['arguments'][:3]==[channel,12,result_code],'one original completion callback')
        require([b['arguments'] for b in bs if b['target']=='0x8c647bde']==expected_progress,'independent original progress sequence')
        require(v.sample(s.m,0x8c8a1210,8*72)==pool_before,'all write/read handles cleared;prior busy records retained')
        require([v.load(s.m,s.descriptor+off) for off in (0x30,0x34)]==(prior_counters if early else [1+len(events)+len(read_events),counter_base+(2*n if verify else n)+failures]),'independent final counters')
        require([b['arguments'][:3] for rr,path in s.phases for b in rr['boundaries'] if b['target']=='0x8c660a7e']==initial_reads+reads_expected,'no hidden reads')
        buffers()
        for handle in allocated:s.call('original_allocator_release',0x8c64b2d4,[handle]);harvest()
        s.resume();harvest();s.call('original_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'original owner teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,result=hex(result_code),plan=plan_expected,transactions=events,packets=packets,readback=read_events,progress=expected_progress,postyield_parent_pcs=sorted(hex(pc) for pc in postyield_seen if pc in ROUTES),phases=s.records,independent_checks=case_checks,ack_ledger_is_not_flash_durability=True))
        print('COMMAND12 TASK',len(records),kind,'PASS',comparisons,flush=True)
    coverage=[]
    for body in v.ANALYSIS_ROUTINES:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'verify_movie_filesystem_write_tasks.py',P/'movie_filesystem_command12_vectors.py',P/'movie_filesystem_command12_session.py',P/'movie_filesystem_command12_reference.py',P/'write_watch_filesystem_command12.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_command12_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for rec in records for pc in rec['postyield_parent_pcs']}),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original initializedcommand12 normal/verifiedwrite path. Literal independentDATA/DIR/FAT/ROOT writeplans,exact128bytepayloads,callerdata/timestamp/rawresponse inputs only,fullRAMmetadata/counters/callback/handleclear/idle/ownerteardown. Explicitabortpolicy0;freecountpolicyretail0 retained. No suppliedcalleeoutputs/parsedmetadata/savedframes/IDs/PR. Readbackrawbytes/mismatch/errordeclaredexternalinputs. ACKledger notphysicalDMA/flashdurability;originalignoredclosereturn/FATerrors preserved.')
    name='movie_filesystem_command12_tasks'+('' if shard is None else '_'+str(shard))+'_verification.json'
    if '--case' in sys.argv:name='command12_case_'+configs[0]['kind']+'.json';dest=ROOT/'work/sonic-adventure'/name
    else:dest=P/'analysis'/name
    dest.write_text(json.dumps(report,indent=2)+'\n');print('COMMAND12 TASK PASS',comparisons,checks,flush=True)

ROUTES={0x8c649434,0x8c6494a0,0x8c6492e0,0x8c6492ec,0x8c64b7ea,0x8c64b802,0x8c64b898,0x8c64b8ae,0x8c64b8de,0x8c647b02}
def configurations():
    names=['normal_le','normal_be','new_file','empty_existing','empty_new','verify_write','verify_mismatch','verify_read_error','verify_empty','capacity_fail','full_directory','pool_full','data_abort','data_bad','data_retry_abort','data_nospace','dir_abort','dir_nospace','dir_reloc','dir_reloc_second','dir_fat_abort','dir_root_abort','fat_abort','fat_reloc','fat_root_ignored']
    for i,kind in enumerate(names):
        yield dict(kind=kind,channel=7 if i&1 else 0,order=i&1,endian=i&1,count=1,start=0,write_count=0 if kind in ('empty_existing','empty_new','verify_empty','full_directory') else 3 if kind=='capacity_fail' else 2 if kind in ('normal_be','new_file','verify_write') else 1,flags=(0x80000000 if kind.startswith('verify_') else 0)|(0xa5 if i&1 else 0x5a),index=16 if kind in ('normal_be','dir_reloc_second') else 0,free_count=0 if kind in ('capacity_fail','data_nospace') else 8,pool_count=0 if kind=='dir_nospace' else 8,primary_free=kind in ('dir_reloc','dir_reloc_second','dir_fat_abort','dir_root_abort'),seed=i*7)
if __name__=='__main__':main()
