"""Retain full startup oracles and verify the accounting/driver-return frontier."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 parent=P/'verify_movie_graphics_init_startup.py';source=parent.read_text().replace('movie_graphics_init','movie_graphics_handoff')
 marker="assert r['packet_native_calls']['0x8c656940']==1";assert source.count(marker)==1
 source=source.replace(marker,"assert sum(b['target']=='0x8c656940' for b in r['boundaries'])==1")
 source=source.replace("r['stop_pc']=='0x8c608d88'","r['stop_pc']=='0x8c6536ec'").replace("actual_next_frontier='0x8c608d88'","actual_next_frontier='0x8c6536ec'").replace("r['boundary_comparisons']==4353","r['boundary_comparisons']==4357")
 namespace=dict(__name__='accounting_startup_inherited_oracles',__file__=str(Path(__file__)))
 exec(compile(source,str(parent),'exec'),namespace);namespace['main']()
 path=P/'analysis/movie_graphics_handoff_startup_verification.json';report=json.loads(path.read_text());files=[Path(__file__),parent,P/'capture_movie_graphics_handoff_startup.py'];prefixes=[];dll=digest(ROOT/'work/sonic-adventure/build/movie_graphics_handoff.dll')
 for profile in ('warm','cold'):
  previous=ROOT/f'work/sonic-adventure/movie_graphics_init_startup_{profile}_capture.json';current=ROOT/f'work/sonic-adventure/movie_graphics_handoff_startup_{profile}_capture.json';old=json.loads(previous.read_text());new=json.loads(current.read_text());a=old['record']['boundaries'];b=new['record']['boundaries'];r=new['record'];files.extend((previous,current))
  assert new['full_original_native_comparison'] and new['dll_sha256']==dll and r['full_original_native_comparison'];report['independent_checks']+=3
  assert len(b)==len(a)+4 and b[:len(a)]==a and a[-1]['target']=='0x8c608d88';report['independent_checks']+=3
  assert r['packet_native_calls']=={'0x8c608d88':1,'0x8c653dc0':1,'0x8c663a60':1};report['independent_checks']+=1
  assert b[-2]['target']=='0x8c6406ac' and b[-2]['pr']=='0x8c054768' and b[-1]['target']=='0x8c6536ec';report['independent_checks']+=2
  prefixes.append(dict(profile=profile,previous_boundaries=len(a),current_boundaries=len(b),exact_prior_201_source_boundary_prefix=True,actual_query_calls=r['packet_native_calls'],driver_return_reached_original_next_call=True))
 report['prior_201_source_prefix_checks']=prefixes;report['actual_next_frontier']='0x8c6536ec';report['dll_sha256']=dll
 report['sha256'].update({str(p):digest(p) for p in files});report['scope']='Two fresh uninterrupted configured0546c0startupcomparisons in202-sourceDLL retainall independent full8MiBclear,Soundfills,realtransfer/IRQ/RTE/request,flash/CRC,39queue-record/32MIDI oracles. Completeprior201-sourceboundaryprefix matches exactly through608d88. Original query executesreal653dc0/663a60 anddriverinitialization returns;earliercaller invokes6406ac andstops honestlyatmissing6536ec. No helper result,guestcheckpoint,shortenedclear/copy,priming orhistoriccoldsubstitution. CPU/FPU/RAM/VRAM/Sound/device/runtime/flash/orderedboundaries compare;rawplatform/configurationdata explicit. Graphicsrootstillpartialtwooutcomes;no pixels,physicalIRQtiming,audio,resetboot,gameplay orplayableport claim.'
 path.write_text(json.dumps(report,indent=2)+'\n');print('Accounting startup verified',prefixes,'checks',report['independent_checks'])
if __name__=='__main__':main()
