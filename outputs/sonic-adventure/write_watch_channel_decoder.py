"""Use the unchanged complete-memory observer with new220-stage console ABI."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'write_watch_buffer_initialize.py'
code=parent.read_text();marker='import movie_buffer_initialize_reference as old';assert code.count(marker)==1
code=code.replace(marker,'import movie_channel_decoder_reference as old')
marker="assert console_ref.service(expected,0,0,0,0,0)==(0,None)";assert code.count(marker)==1
code=code.replace(marker,"assert console_ref.service(expected,0,0,0,0,0)==(0,0)")
exec(compile(code,str(parent),'exec'),globals())

# Read completed native MACL without changing execution or comparison logic.
marker=' return dict(';assert compare_code.count(marker)==1
address_compare_code=compare_code.replace(marker,' return dict(final_macl=ml.value,')
exec(compile(address_compare_code,'record_address_full_comparison','exec'),dict(compare.__globals__),address_local:={})
compare=address_local['compare']
