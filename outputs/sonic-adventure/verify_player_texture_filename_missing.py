"""Exercise the recovered filename linker through a genuine missing-file lookup."""
import hashlib,json,shlex
from pathlib import Path
import player_texture_link_fixtures as q
P=q.P;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_filename_missing_verification.json';assert not target.exists()
 base=q.prepare(q.v.seed_ram(),dict(real_tables=True));base[1].r[15]=0x8cfff000;records=[];seen=set();edges=set();checks=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def phase(entry,args,label):
  nonlocal base
  m,s,f,c=base;sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  result,path,m,s,f=q.fast.compare(m,s,f,c);require(result['status']==0,(label,result['status'],result['stop_pc']));require(result['full_state_match'] and result['ordered_boundaries_match'],'entire original/native state and ordered calls');require(s.r[15]==sp and s.pc==s.pr==0x8cfed000,'balanced public return')
  c.update(other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr']);base=m,s,f,c;seen.update(path)
  for i,a in enumerate(path):
   text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
  records.append(dict(label=label,entry=hex(entry),status=0,return_value=hex(s.r[0]),steps=len(path),boundaries=result['boundaries'],disc_read_attempts=result['disc_read_attempts'],disc_reads=result['disc_reads'],full_state_match=True,ordered_boundaries_match=True));print(label,hex(s.r[0]),len(path),flush=True);return s.r[0]
 phase(0x8c098a10,[],'original heap initialization');ctx=(phase(0x8c098b0c,[1,0x4020],'original filesystem workspace allocation')+31)&~31;directory=(phase(0x8c098b0c,[1,0x30020],'original directory allocation')+31)&~31
 m=base[0];q.put(m,q.get(m,0x8c6000e4),0);q.put(m,0x8c673420,0);inventory=P/'disc_inventory.json';disc=json.loads(inventory.read_text());track=next(x for x in reversed([shlex.split(s) for s in disc['gdi'].splitlines()[1:] if s.strip()]) if x[2]=='4');q.put(m,0x8c66c530,int(track[1])+166)
 require(phase(0x8c601618,[4,ctx,4096,directory],'original real-disc filesystem initialization')==0,'mount success')
 name=0x8cda0000;data=b'SONICADV\0';m=base[0];m[name&0xffffff:(name&0xffffff)+len(data)]=data;require(phase(0x8c6002a2,[name,0],'original SONICADV directory selection')==0,'directory success')
 table=phase(0x8c098b0c,[256,0x44],'original texture table allocation');require(table!=0,'actual texture table');phase(q.TABLE_INIT,[table,256],'original texture table initialization')
 descriptor=phase(0x8c098b0c,[1,8],'original caller-list allocation');array=phase(0x8c098b0c,[1,12],'original caller-record allocation');strings=phase(0x8c098b0c,[1,28],'original caller-name allocation');require(descriptor and array and strings,'actual caller allocations');phase(q.DESCRIPTOR_INIT,[descriptor,array,strings,1],'original caller-name initializer')
 # This new public caller input requests one file. It does not overwrite a
 # game-produced resource descriptor or supply any callee's return value.
 m=base[0];q.put(m,descriptor+4,1);data=b'MISSING.PVR\0';m[strings&0xffffff:(strings&0xffffff)+len(data)]=data
 result=phase(0x8c608c82,[descriptor],'original filename texture API with genuinely missing input file');require(result==0xffffffff,'independent public missing-file failure convention')
 m=base[0];out=q.get(m,array+8);require(table<=out<table+256*0x44 and (out-table)%0x44==0,'descriptor pointer actually produced inside original table');error=q.get(m,out+0x42,2);references=q.get(m,out+0x40,2)
 require(error==2 and references==0,'independent original open-failure descriptor error2 andzero references');require(q.get(m,0x8c88f7a8)==0,'independent failed list index zero')
 targets=[b['target'] for b in records[-1]['boundaries']];require(all(hex(a) in targets for a in (0x8c64c374,0x8c64efcc,0x8c64e054,0x8c6016d6,0x8c64e61e)),'actual linker/name/open/error-descriptor chain executes')
 t=json.loads((P/'analysis/player_texture_link_translation.json').read_text());coverage=[]
 for b in t['routines']:
  wanted=set(b['reachable_addresses']);outcomes={tuple(x) for x in b['conditional_edges']};coverage.append(dict(address=b['address'],static_rows=len(wanted),executed_rows=len(wanted&set(map(hex,seen))),missing_rows=sorted(wanted-set(map(hex,seen))),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),unresolved_outcomes=sorted(outcomes-edges)))
 files=[Path(__file__),P/'player_texture_link_fixtures.py',P/'player_texture_link_reference.py',P/'write_watch_player_texture_link.py',inventory,R/'work/sonic-adventure/build/player_texture_link.dll']
 report=dict(status='ordinary_filename_texture_API_missing_file_returns_normally_with_actual_error_descriptor',original_native_comparisons=len(records),checks=checks,phases=records,coverage_by_routine=coverage,input_filename='MISSING.PVR',texture_table=hex(table),caller_list=hex(descriptor),produced_descriptor=hex(out),descriptor_error=error,descriptor_references=references,guest_return_value=hex(result),supplied_allocator_results=0,supplied_game_callee_outputs=0,supplied_private_execution_frames=0,main_filename_linker_qualified=False,new_original_routine_credit=0,visible_textured_rendering_qualified=False,overall_completion_percent=10,scope='Public filename API608c82with a genuine missing-file input from actual original calloc/name initialization andmounted retail directory. Original64c374/64efcc/open/error-descriptor production andnormal unwinding match complete state/boundaries. Error2/zero references/failed index0checked independently. This is a valid input/error path witness,not complete1528-row qualification orsuccessful texture loading.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('MISSING FILE LINKER PROOF',len(records),checks,'error',error,flush=True)
if __name__=='__main__':main()
