"""Reuse the unchanged206-stage complete-memory observer with the207-stage model."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'write_watch_buffer_initialize.py'
code=parent.read_text();marker='import movie_buffer_initialize_reference as old';assert code.count(marker)==1
code=code.replace(marker,'import movie_float_initialize_reference as old')
exec(compile(code,str(parent),'exec'),globals())
