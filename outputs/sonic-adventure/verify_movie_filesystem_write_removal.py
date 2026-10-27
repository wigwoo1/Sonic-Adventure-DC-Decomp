"""Extend unchanged write-task assertions with three raw-removal timing cases."""
import itertools,json,sys
from pathlib import Path
P=Path(__file__).resolve().parent
parent=P/'verify_movie_filesystem_write_tasks.py';source=parent.read_text();original=source
def replace(before,after,count=1):
    global source
    assert source.count(before)==count,(before,source.count(before));source=source.replace(before,after)
replace("if kind=='mutex_data':","if kind in ('mutex_data','pre_data_removal'):",2)
replace("if kind=='mutex_commit' and part==3:","if kind in ('mutex_commit','pre_commit_removal') and part==3:")
replace("if kind=='mutex_commit' and part==4 and not error:","if kind in ('mutex_commit','pre_commit_removal') and part==4 and not error:")
marker='yields.append(hex(frame_pc()));put(s.m,backend.LOCK,0,1);s.resume();harvest()'
replace(marker,"yields.append(hex(frame_pc()));scan_removal() if kind.startswith('pre_') else None;failures+=int(kind.startswith('pre_'));put(s.m,backend.LOCK,0,1);s.resume();harvest()",2)
replace("            if kind=='special' and commit:status=251;error=True","            if kind=='special' and commit:status=251;error=True\n            if kind=='post_commit_removal' and commit:unplug=True;error=True")
replace("result={'bad_data':-255,","result={'pre_data_removal':-65536,'pre_commit_removal':-65536,'post_commit_removal':-65536,'bad_data':-255,")
marker='        harvest();s.queue();harvest();transfer();s.resume();harvest()'
replace(marker,"""        scan_events=[]
        def scan_removal():
            # The task is waiting on the shared mutex. Release it only while
            # the scheduler is not executing, so the actual transport scanner
            # can run. Do not schedule the worker until removal is parsed.
            require(s.tokens()[0:2]==[0,0],'no storage request before removal scan')
            sp=v.load(s.m,s.task+0x18);frame=v.sample(s.m,sp,s.idle_sp+64-sp);contexts=v.sample(s.m,s.owner+0x210,32*0x124)
            put(s.m,backend.LOCK,0,1)
            begin=len(s.phases);s.submit();harvest()
            require(v.load(s.m,s.context+0xfc0)==0,'original scanner submitted and awaits response')
            observation=s.observe(status=7,unplug=True,ack=True);s.pump();harvest()
            require(v.sample(s.m,sp,len(frame))==frame and v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'raw removal scan preserves waiting task frames')
            require(s.tokens()[0:2]==[0,0],'scan added no storage request')
            scan_events.append(dict(raw_observation=observation,actual_targets=[b['target'] for r,_ in s.phases[begin:] for b in r['boundaries']]))
        harvest();s.queue();harvest();transfer();s.resume();harvest()""")
replace("records.append(dict(configuration=config,","records.append(dict(raw_removal_scans=scan_events,configuration=config,")
replace("P/'movie_filesystem_write_vectors.py'","parent,P/'movie_filesystem_write_vectors.py'")
replace('movie_filesystem_write_tasks','movie_filesystem_write_removal_tasks',source.count('movie_filesystem_write_tasks'))
replace('==len(expected)==23','==len(expected)==12')
replace('task_sessions_passed=23,completed_initializations=23','task_sessions_passed=12,completed_initializations=12')
namespace=dict(__name__='write_removal_replay',__file__=str(Path(__file__)),parent=parent)
exec(compile(source,str(parent),'exec'),namespace)
def configurations():
    for kind in ('pre_data_removal','pre_commit_removal','post_commit_removal'):
        for i,(order,endian) in enumerate(itertools.product((0,1),(0,1))):yield dict(kind=kind,channel=7 if i&1 else 0,order=order,endian=endian,count=1,start=0)
namespace['configurations']=configurations
if __name__=='__main__':namespace['main']()
