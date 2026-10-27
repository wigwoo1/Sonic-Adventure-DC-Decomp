"""Independent warm startup oracles for the real expanded earlier caller."""
import ast,hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 parent=P/'verify_movie_record_register_startup.py';tree=ast.parse(parent.read_text());main=next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='main');construction=[]
 for node in main.body:
  if isinstance(node,ast.Assign) and any(isinstance(target,ast.Name) and target.id=='ns' for target in node.targets):break
  construction.append(node)
 scope={'P':P};exec(compile(ast.Module(body=construction,type_ignores=[]),str(parent),'exec'),scope);code=scope['code'].replace('movie_record_register','movie_frame_wait')
 assert code.count("for profile in ('warm','cold'):")==1;code=code.replace("for profile in ('warm','cold'):","for profile in ('warm',):")
 code=code.replace("r['stop_pc']=='0x8c601874'","r['stop_pc']=='0x8c051798'").replace("r['uninterrupted_declared_entry']=='0x8c0546c0'","r['uninterrupted_declared_entry']=='0x8c053ac0'").replace("r['boundary_comparisons']==4695","r['boundary_comparisons']==4709").replace("==['warm','cold']","==['warm']")
 marker="assert r['boundaries'][:len(prior['boundaries'])]==prior['boundaries'];checks+=1";assert code.count(marker)==1
 # A different enclosing caller changes register/stack/PR fields. Preserve
 # the nested target sequence, and compare all machine fields original/native
 # within this fresh run. Do not relabel target-only history as full identity.
 code=code.replace(marker,"start=next(i for i,b in enumerate(r['boundaries']) if b['target']=='0x8c0546c0');assert [b['target'] for b in r['boundaries'][start:start+len(prior['boundaries'])]]==[b['target'] for b in prior['boundaries']];checks+=1")
 code=code.replace("r['status']==-2","r['status']==-4")
 ns={'__name__':'frame_warm_startup_oracles','__file__':str(Path(__file__))};exec(compile(code,str(P/'verify_movie_midi_commands_startup.py'),'exec'),ns);ns['main']()
 target=P/'analysis/movie_frame_wait_startup_verification.json';report=json.loads(target.read_text());path=ROOT/'work/sonic-adventure/movie_frame_wait_startup_warm_capture.json';capture=json.loads(path.read_text());r=capture['record']
 previous=ROOT/'work/sonic-adventure/movie_record_address_startup_warm_capture.json';old=json.loads(previous.read_text())['record']['boundaries'];now=r['boundaries'];start=next(i for i,b in enumerate(now) if b['target']=='0x8c0546c0')
 assert start==4 and [b['target'] for b in now[start:start+len(old)]]==[b['target'] for b in old];report['independent_checks']+=2
 assert [b['target'] for b in now[:4]]==['0x8c053ac0','0x8c644d84','0x8c5fbb60','0x8c5fbcb6'];report['independent_checks']+=1
 assert [b['target'] for b in now[-3:]]==['0x8c6033e4','0x8c647ca4','0x8c051790'];report['independent_checks']+=1
 assert r['packet_native_calls']['0x8c051790']==1 and all(count==0 for address,count in r['packet_native_calls'].items() if address!='0x8c051790') and len(r['packet_native_calls'])==10;report['independent_checks']+=2
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 def word(a):return struct.unpack_from('<I',raw,a-0x8c010000)[0]
 end=(word(0x8c644d90)+0x800*4)&0xffffffff;expected={hex(word(0x8c5fbd0c)):word(0x8c5fbd08),hex(word(0x8c5fbd10)):end,hex(word(0x8c5fbd18)):end}
 assert r['memory_setup_words']==expected and r['record_poll_index']==0 and r['record_poll_word']==0;report['independent_checks']+=3
 assert r['status']==-4 and r['stop_pc']=='0x8c051798' and r['uninterrupted_declared_entry']=='0x8c053ac0';report['independent_checks']+=3
 observed={tuple(e) for e in capture['executed_edges']};assert ('0x8c053ada','0x8c053adc') in observed and ('0x8c053ada','0x8c053ad4') not in observed;report['independent_checks']+=2
 prior213=ROOT/'work/sonic-adventure/movie_earlier_initialize_startup_warm_capture.json';old213=json.loads(prior213.read_text())
 assert r['boundaries']==old213['record']['boundaries'];report['independent_checks']+=1
 assert r['frame_wait_state']['0x8c754ec0']==2 and r['frame_wait_state']['0x8c754ebc']==2;report['independent_checks']+=2
 assert capture['executed_rows'].count('0x8c051798')==1 and ['0x8c051798','0x8c051794'] in capture['executed_edges'];report['independent_checks']+=1
 report.pop('actual_next_frontier',None)
 report['actual_wait_pc']='0x8c051798';report['frame_counter_unchanged']=2
 report.update(compiled_source_count=214,partial_entry='0x8c053ac0',fresh_cold_profile=False,nested_prior_warm_target_sequence=len(old),historical_full_boundary_field_identity_claimed=False,current_warm_boundaries=4709,memory_setup_words=expected,record_poll_word=0,scope='One fresh214-source configured053ac0 warm startup reaches the actual positive frame-counter wait051798 after4709 boundaries. Counter and reload both remain2: no tick/callback/IRQ progress is invented. Research-only two-taken-test guard stops incomplete-4; unbounded code keeps the real wait. Full original/native machine and ordered boundaries match. Exact previous213 recorded boundary fields retained; prior final-machine identity is not claimed after newly executing wait instructions. Independent flash/SDK/MIDI/VRAM/Sound expectations pass. No fresh cold, physical timing/reset or gameplay proof.')
 files=[Path(__file__),parent,P/'verify_movie_midi_commands_startup.py',P/'capture_movie_frame_wait_startup.py',path,previous,prior213,binary];report['sha256'].update({str(p):digest(p) for p in files});target.write_text(json.dumps(report,indent=2)+'\n');print('FRAME WARM WAIT',report['independent_checks'],'checks;4709boundaries;counter2wait051798')
if __name__=='__main__':main()
