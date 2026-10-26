"""Retain every full-startup oracle and check the real render-buffer return path."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 parent=P/'verify_movie_graphics_init_startup.py';source=parent.read_text().replace('movie_graphics_init','movie_render_buffer')
 marker="assert r['packet_native_calls']['0x8c656940']==1";assert source.count(marker)==1
 source=source.replace(marker,"assert sum(b['target']=='0x8c656940' for b in r['boundaries'])==1")
 source=source.replace("r['stop_pc']=='0x8c608d88'","r['stop_pc']=='0x8c65393a'").replace("actual_next_frontier='0x8c608d88'","actual_next_frontier='0x8c65393a'").replace("r['boundary_comparisons']==4353","r['boundary_comparisons']==4366")
 namespace=dict(__name__='render_startup_inherited_oracles',__file__=str(Path(__file__)))
 exec(compile(source,str(parent),'exec'),namespace);namespace['main']()
 target=P/'analysis/movie_render_buffer_startup_verification.json';report=json.loads(target.read_text());files=[Path(__file__),parent,P/'capture_movie_render_buffer_startup.py'];prefixes=[];dll=digest(ROOT/'work/sonic-adventure/build/movie_render_buffer.dll')
 route=['0x8c653f08','0x8c654286','0x8c6542e0','0x8c663540','0x8c663c86','0x8c66452a','0x8c664ac0','0x8c664b12','0x8c65393a']
 for profile,count in (('warm',4366),('cold',585664)):
  previous=ROOT/f'work/sonic-adventure/movie_graphics_handoff_startup_{profile}_capture.json';current=ROOT/f'work/sonic-adventure/movie_render_buffer_startup_{profile}_capture.json';old=json.loads(previous.read_text());new=json.loads(current.read_text());a=old['record']['boundaries'];b=new['record']['boundaries'];r=new['record'];files.extend((previous,current))
  assert new['full_original_native_comparison'] and r['full_original_native_comparison'] and new['dll_sha256']==dll;report['independent_checks']+=3
  assert len(b)==count==len(a)+9 and b[:len(a)]==a and a[-1]['target']=='0x8c6536ec';report['independent_checks']+=3
  assert [x['target'] for x in b[len(a):]]==route;report['independent_checks']+=1
  descriptor=a[-1]['arguments'][0];assert a[-1]['arguments']==[0x8c890564,128,128,256] and a[-1]['pr']=='0x8c6406da';report['independent_checks']+=2
  assert next(x for x in b[len(a):] if x['target']=='0x8c66452a')['arguments'][:3]==[1,32768,descriptor];report['independent_checks']+=1
  assert b[-1]['arguments'][0]==descriptor and b[-1]['pr']=='0x8c6406e6';report['independent_checks']+=1
  assert all(sum(x['target']==entry for x in b)==1 for entry in ('0x8c608d88','0x8c653dc0','0x8c663a60'));report['independent_checks']+=1
  prefixes.append(dict(profile=profile,previous_boundaries=len(a),current_boundaries=len(b),exact_prior_202_source_boundary_prefix=True,actual_render_calls=route,original_descriptor=hex(descriptor),original_allocation_bytes=32768,render_dispatch_return_reached_original_next_call=True))
 report['prior_202_source_prefix_checks']=prefixes;report['actual_next_frontier']='0x8c65393a';report['dll_sha256']=dll
 report['sha256'].update({str(p):digest(p) for p in files});report['scope']='Two fresh uninterrupted configured0546c0startup comparisons in203-sourceDLL retainall independent full8MiBclear,Soundfills,realtransfer/IRQ/RTE/request,flash/CRC,39queue-record/32MIDI oracles. Exact202-sourceboundaryprefixmatches through6536ec. Realdescriptor formatting/sizing/bankmaxima/32768bytebank1splitallocation executes;renderdispatch returnsandoriginalparent calls65393a. CompleteCPU/FPU/RAM/VRAM/Sound/device/runtime/flash/orderedboundaries compare. No helperresult/checkpoint/priming/shortenedclear/copy/historiccoldsubstitution. Explicitrawplatform/configurationinputs;notphysicaltiming/resetboot/pixels/gameplay/playableport. Graphicsrootstillpartialtwooutcomes.'
 target.write_text(json.dumps(report,indent=2)+'\n');print('Render startup verified',prefixes,'checks',report['independent_checks'])

if __name__=='__main__':main()
