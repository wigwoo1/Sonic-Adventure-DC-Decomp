"""Actual raw-response initialization followed by original command20 APIs."""
import hashlib,itertools,json,sys
from pathlib import Path
from movie_filesystem_command_vectors import Session,v,raw,put,digest,P
from verify_movie_filesystem_command_info import information
def main():
    assert v.effects.DLL.name=='movie_filesystem_command.dll'
    configs=list(itertools.product((0,7),(0,1),(0,1)));smoke='--smoke' in sys.argv
    if smoke:configs=configs[:1]
    records=[];comparisons=checks=0;seen=set();edges=set();ram=v.seed_ram()
    for channel,order,endian in configs:
        s=Session(ram,channel=channel,order=order,endian=endian,mode=4 if (channel+order+endian)&1 else 0,command=1);done=0;case_checks=0
        def require(ok,label):
            nonlocal checks,case_checks
            assert ok,(channel,order,endian,label);checks+=1;case_checks+=1
        def harvest(last_status=0):
            nonlocal done,comparisons
            for i in range(done,len(s.phases)):
                r,path=s.phases[i];require(r['status']==(last_status if i==len(s.phases)-1 else 0),'host status');require(r['full_state_match'],'full original/native state');require(r['ordered_boundaries_match'],'ordered original/native boundaries');comparisons+=1
            done=len(s.phases)
        def transfer(data=None):
            queued=s.tokens()[0];require(queued>0,'original request queued');sp=v.load(s.m,s.task+0x18);frame=v.sample(s.m,sp,s.idle_sp+64-sp);contexts=v.sample(s.m,s.owner+0x210,32*0x124)
            def intact():require(v.sample(s.m,sp,len(frame))==frame and v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'transport preserves original savedframes/contexts')
            s.submit();harvest();intact();require(s.tokens()[:2]==[0,queued],'original inflight ID')
            s.observe(511,block_data=data,metadata_words={6:509,8:2,10:507,12:2,16:32} if data is None else None);intact();s.pump();harvest();intact();require(s.tokens()[1:]==[0,queued],'original completed ID')
        harvest();s.queue();harvest();transfer();s.resume();harvest()
        require(v.load(s.m,s.output+0x1c)==v.load(s.m,s.output+0x30)==32,'original code copies raw header lookup limit')
        order_name='big' if endian else 'little';root=b'U'*16+bytes(496);fat=bytearray(1024)
        for index,value in ((507,506),(506,0xfffa),(20,21),(21,0xfffa)):fat[index*2:index*2+2]=value.to_bytes(2,order_name)
        def file_entry(kind,label):
            data=bytearray(32);data[0]=kind;data[1]=0xa5;data[2:4]=(20).to_bytes(2,order_name);data[4:16]=(label+bytes(12))[:12];data[16:24]=bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,3));data[24:26]=(2).to_bytes(2,order_name);data[26:28]=(1).to_bytes(2,order_name);return bytes(data)
        sonic=file_entry(0xcc,b'SONIC');datafile=file_entry(0x33,b'DATA');directory=sonic+datafile+bytes(1024-64)
        destinations=[v.load(s.m,s.descriptor+o) for o in (0x20,0x24,0x28)]
        plan=[(511,destinations[0],root),(509,destinations[1],bytes(fat[:512])),(510,destinations[1]+512,bytes(fat[512:])),(507,destinations[2],directory[:512]),(506,destinations[2]+512,directory[512:])]
        for block,dest,data in plan:
            for part in range(2):transfer(data[part*256:(part+1)*256]);s.resume();harvest()
            require(v.sample(s.m,dest,512)==data,'actual raw block buffer')
        initial_calls=[b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e'];require(initial_calls==[[channel,b,d] for b,d,_ in plan],'actual root/table/linked directory read plan')
        require(v.sample(s.m,s.descriptor+10,3)==bytes((1,1,1)) and v.load(s.m,s.result_word)==0,'original initialization completion/cache flags')
        tokens=s.tokens();buffers=[v.sample(s.m,a,n) for a,n in zip(destinations,(512,1024,1024))];command_records=[]
        def command(label,start,count):
            # The filename bytes are caller input in the original command's
            # filename buffer, written only while the worker is idle. This is
            # not the task/context record or a decoded filesystem result.
            require(v.load(s.m,s.channel_record+8)==0,'caller writes only while idle');raw(s.m,s.channel_record+0x2c,label+bytes(13-len(label)))
            s.call('command20_arguments_api',0x8c647cc4,[channel,channel,0,0x8cdb0000],(start,count,0));harvest();s.call('command20_opcode_api',0x8c647cb4,[channel,20]);harvest();first=len(s.records);s.resume();return first
        for label,start,count,result in ((b'MISSING',0,1,0xffffff05),(b'DATA',0,1,0xffffff0b),(b'SONIC',2,0,0xffffff00)):
            first=command(label,start,count);harvest();require(v.load(s.m,s.result_word)==result and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'original rejected command result/idle fields')
            cb=[b for r in s.records[first:] for b in r['callback_events']];require(len(cb)==1 and cb[0]['arguments'][:3]==[channel,20,result],'original rejected command callback')
            s.call('command20_completed_idle',v.AI_SCHEDULE);harvest();command_records.append(dict(name=label.decode(),start=start,count=count,result=hex(result)))
        first=command(b'SONIC',0,1);harvest(-2);r=s.phases[-1][0]
        require(r['stop_pc']=='0x8c64b300' and s.s.pr==0x8c64bd42,'actual next file-chain validator boundary')
        require(s.s.r[4]==channel and s.s.r[5]==s.channel_record+0x2c,'actual original chain validator arguments')
        info=[b for b in r['boundaries'] if b['target']=='0x8c648534'];require(len(info)==1,'real fileinfo call');local=info[0]['arguments'][2]
        require(v.sample(s.m,local,18)==information(sonic,endian),'independent fileinfo bytes in original live caller frame')
        require(v.load(s.m,s.channel_record+8)==20 and not any(r['callback_events'] for r in s.records[first:]),'no fabricated completion at missing callee')
        require(s.tokens()==tokens and all(v.sample(s.m,a,len(b))==b for a,b in zip(destinations,buffers)),'metadata lookup adds no transport or buffer mutation')
        calls=[b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e'];require(calls==initial_calls,'no hidden extra block reads')
        require(v.load(s.m,s.output+0x30)==32,'lookup count remains original parsed/copied value')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(channel=channel,order=order,endian=endian,read_calls=calls,completed_command20_cases=command_records,pending_callee='0x8c64b300',live_info_output=hex(local),phases=s.records,independent_checks=case_checks,task_frames_never_repaired=True,completed_ids_never_supplied=True,decoded_metadata_never_supplied=True))
        print('FILE COMMAND TASK',len(records),channel,order,endian,comparisons,flush=True)
    if smoke:print('FILE COMMAND SMOKE PASS',comparisons,checks);return
    coverage=[]
    for body in v.manifest['routines']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'verify_movie_filesystem_command_info.py',P/'movie_filesystem_command_vectors.py',P/'movie_filesystem_command_session.py',P/'movie_input_interface_session.py',P/'movie_filesystem_command_task_fixtures.py',Path(v.__file__),P/'write_watch_filesystem_command.py',v.MP,P/'analysis/movie_filesystem_command_connection.json',v.effects.DLL]
    report=dict(task_sessions_passed=len(records),completed_initializations=8,completed_command20_error_cases=24,pending_chain_validator_cases=8,original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_qualified_nonlocal_routes=[],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual original command1 rawheader/root/FAT/directory reads initialize file search limit through rawword16 andoriginal648340copy. Caller filename bytes supplied to idle original command input buffer;original setters/scheduler own all taskframes/IDs/commands. Original command20 missing/type/range errors completecallbacks;positive path writes independently checked18bytefileinfo inlivecallerframe andstops atactual64b300. No parsedrecord/context/result/PRrepair. Wrapper/chain/transfer behavior incomplete;no newnonlocalroute credit orhardware/gameplay claim.')
    (P/'analysis/movie_filesystem_command_tasks_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('FILE COMMAND TASK PASS',comparisons,checks,flush=True)
if __name__=='__main__':main()
