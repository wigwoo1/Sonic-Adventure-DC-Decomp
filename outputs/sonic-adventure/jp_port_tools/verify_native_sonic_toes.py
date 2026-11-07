"""Actual original toe control, canonical data mutations and owned view lifetime."""
import argparse,inspect,json
from pathlib import Path
import verify_native_toe_models as data_suite

def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','build','binary','numeric-dll','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
 args=ap.parse_args();output=args.output.resolve();assert not output.exists()
 original=inspect.getsource(data_suite.main);marker=' for bank in range(2):\n';assert original.count(marker)==1;prefix=original.split(marker)[0]
 prefix=prefix.replace('assert count==2849','assert count==len(b.get(\'sonic_run_data_old_record_names\',b[\'toe_models_old_record_names\']))+len(b.get(\'sonic_run_data_added_registry\',b[\'toe_models_added_registry\']))')
 prefix=prefix.replace('d.mkdir();b=','d.mkdir();(d/\'private_toe_control_caller.py\').write_text(caller_source);b=')
 tail=''' from verify_native_character_tree import Node
 from verify_native_motion_store import Info as MotionInfo
 import threading
 class OwnerInfo(C.Structure):_fields_=[('phase',C.c_uint32),('leases',C.c_uint32),('thread',C.c_uint32),('native_error',C.c_uint32),('steps',C.c_uint64)]
 start=bind(frame,'jp_sonic_toes_bind',C.c_int);step=bind(frame,'jp_sonic_toes_step',C.c_int,C.c_void_p);close=bind(frame,'jp_sonic_toes_close',C.c_int);snapshot=bind(frame,'jp_sonic_toes_snapshot',C.c_int,C.c_void_p);layout_fn=bind(frame,'jp_sonic_toes_layout',C.c_int,C.c_void_p);layout=(C.c_uint32*7)();check(layout_fn(layout)==0 and layout_fn(None)==1,'actual native player record layout diagnostic');native_layout=list(layout);lp32=b['sonic_toes_original_LP32_player_layout']
 def info():
  v=OwnerInfo();check(snapshot(C.byref(v))==0,'native toe ownership snapshot');return v
 check(start()==5 and info().native_error==2,'missing actual Motion owner rejected with preserved native cause')
 check(bind(lib,'jp_motions_initialize',C.c_int,C.c_size_t,C.c_uint32)(8<<20,8)==0,'real small view pool initialized')
 check(start()==5 and info().phase==0 and info().leases==0 and info().native_error==7,'partial twelve-view bind rolls back actual acquired leases and preserves limit cause')
 mi=MotionInfo();check(bind(lib,'jp_motions_snapshot',C.c_int,C.c_void_p)(C.byref(mi))==0 and mi.live_leases==0 and shutdown()==0,'failed partial bind leaves actual Motion owner clean and closable')
 frames=(0.,1.,4.,7.999,8.,8.5,12.,15.999,16.,16.001,20.,23.999,24.,24.001,28.,31.999,32.)
 for bank in range(2):
  check(bind(lib,'jp_motions_initialize',C.c_int,C.c_size_t,C.c_uint32)(8<<20,32)==0 and start()==0,'actual twelve model/node/normal consumers bind')
  check(info().phase==1 and info().leases==12 and shutdown()==7 and start()==2,'actual owned views prevent shutdown and duplicate bind')
  ram=bytearray(initial);record=C.create_string_buffer(native_layout[0]+64);record_ptr=C.addressof(record)+32;foreign=[];sentinel=OwnerInfo();C.memset(C.byref(sentinel),0x5a,C.sizeof(sentinel));saved=bytes(sentinel)
  def wrong_thread():foreign.extend((step(record_ptr),close(),start(),snapshot(C.byref(sentinel))))
  thread=threading.Thread(target=wrong_thread);thread.start();thread.join();check(foreign==[3]*4 and bytes(sentinel)==saved and info().steps==0,'foreign thread cannot mutate toe owner/data or caller output');check(step(None)==1,'null active player input rejected')
  for action,frame_value in [(13,x) for x in frames]+[(0,16.),(14,8.),(1,32.)]:
   native_record=bytearray(b'\\x5a'*(native_layout[0]+64));original_record=bytearray(b'\\x5a'*(lp32[0]+64));struct.pack_into('<H',native_record,32+native_layout[1],action);struct.pack_into('<f',native_record,32+native_layout[2],frame_value);struct.pack_into('<H',original_record,32+lp32[1],action);struct.pack_into('<f',original_record,32+lp32[2],frame_value)
   C.memmove(record,bytes(native_record),len(native_record));ram[0xe80000:0xe80000+len(original_record)]=original_record;f=Fpu();f.fpscr=0x40001|(bank<<21);s=executor.State();s.pc=table['sonic_blend_toes'];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.r[4:7]=[0,0,0x8ce80020];s.sr=0x70000000
   reference(ram,s,fpu=f,vector_instruction=service,special={'gbr':0,'r_bank':[0]*8},instruction_budget=3000000);check(step(record_ptr)==0 and s.r[15]==0x8cf00000,'complete original toe control and native step return');compare(ram)
   weights=[]
   for native_off,original_off in zip(native_layout[3:5],lp32[3:5]):
    value=ram[0xe80020+original_off:0xe80024+original_off];native_record[32+native_off:36+native_off]=value;original_record[32+original_off:36+original_off]=value;weights.append(bytes(value).hex())
   check(bytes(record)==bytes(native_record) and bytes(ram[0xe80000:0xe80000+len(original_record)])==bytes(original_record),'both exact toe weights and whole player/guard bytes match original writes')
   records.append(dict(bank=bank,action=action,frame=frame_value,weight_words=weights))
  check(info().steps==len(frames)+3 and close()==0 and info().phase==0 and info().leases==0,'actual toe owner retires twelve views after complete original steps')
  for side in ('plus','minus'):
   node=Node.from_address(byname['sonic_toe_'+side+'_z'].data);check(node.model==byname['sonic_toe_'+side+'_z_model'].data,'close restores actual canonical own toe model')
  check(shutdown()==0 and all(C.string_at(e.data,e.host_bytes)==before[e.name] for e in entries),'actual Motion shutdown restores every canonical record and toe buffer');check(close()==2 and step(record_ptr)==2,'closed owner prevents stale player use')
 out=dict(status='complete_original_Sonic_toe_control_exact_weights_canonical_model_mutations_and_twelve_view_lifetimes_verified',checks=checks,original_complete_toe_control_calls=len(records),records=records,native_player_layout=native_layout,original_LP32_player_layout=lp32,canonical_model_records=count,all_canonical_records_compared_after_each_step=True,whole_player_record_and_guard_bytes_verified=True,partial_acquisition_rollback_and_error_cause_verified=True,creating_thread_and_live_view_shutdown_protection_verified=True,original_player_draw_callback_mainloop_or_gameplay_verified=False,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_binary_sha256=sha(a.binary),reference_hashes=reference_hashes,original_data_verifier_sha256=sha(data_suite.__file__),derived_caller_sha256=sha(d/'private_toe_control_caller.py'),playable=False,overall_completion_percent=10)
 (d/'verification.json').write_text(json.dumps(out,indent=2)+'\\n');print(out['status'],checks,'checks;',len(records),'whole original calls',flush=True)
'''
 source=prefix+tail;namespace=dict(vars(data_suite),data_suite=data_suite,caller_source=source,__file__=__file__);exec(compile(source,str(output/'private_toe_control_caller.py'),'exec'),namespace);namespace['main']()
if __name__=='__main__':main()
