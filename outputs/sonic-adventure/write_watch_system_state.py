"""Use the unchanged complete-memory observer with corrected209-stage console ABI."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'write_watch_buffer_initialize.py'
code=parent.read_text();marker='import movie_buffer_initialize_reference as old';assert code.count(marker)==1
code=code.replace(marker,'import movie_system_state_reference as old')
marker="assert console_ref.service(expected,0,0,0,0,0)==(0,None)";assert code.count(marker)==1
code=code.replace(marker,"assert console_ref.service(expected,0,0,0,0,0)==(0,0)")
exec(compile(code,str(parent),'exec'),globals())
