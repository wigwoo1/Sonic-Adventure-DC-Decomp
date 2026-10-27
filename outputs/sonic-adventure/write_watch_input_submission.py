"""Use the unchanged complete-memory observer with new224-stage console ABI."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'write_watch_buffer_initialize.py'
code=parent.read_text();marker='import movie_buffer_initialize_reference as old';assert code.count(marker)==1
code=code.replace(marker,'import movie_input_submission_reference as old')
marker="assert console_ref.service(expected,0,0,0,0,0)==(0,None)";assert code.count(marker)==1
code=code.replace(marker,"assert console_ref.service(expected,0,0,0,0,0)==(0,0)")
exec(compile(code,str(parent),'exec'),globals())

# Read completed native MACL without changing execution or comparison logic.
marker=' return dict(';assert compare_code.count(marker)==1
address_compare_code=compare_code.replace(marker,' return dict(final_macl=ml.value,')
exec(compile(address_compare_code,'record_address_full_comparison','exec'),dict(compare.__globals__),address_local:={})
compare=address_local['compare']

# Observe the mandatory TAS purge independently of packet/result expectations.
purge_code=address_compare_code
marker=' native_cache_writebacks=[]';assert purge_code.count(marker)==1
purge_code=purge_code.replace(marker,marker+'\n native_cache_purges=[]')
marker='  if not status and op==2:native_cache_writebacks.append(a)';assert purge_code.count(marker)==1
purge_code=purge_code.replace(marker,marker+'\n  if not status and op==3:native_cache_purges.append(a)')
marker=' return dict(';assert purge_code.count(marker)==1
purge_code=purge_code.replace(marker,marker+'cache_purge_sequence=native_cache_purges,')
exec(compile(purge_code,'request_purge_full_comparison','exec'),dict(compare.__globals__),purge_local:={})
compare=purge_local['compare']
