"""Unchanged original interface/task constructors using the current stage."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'movie_input_interface_session.py'
source=parent.read_text();marker='import movie_input_interface_task_fixtures as tasks';assert source.count(marker)==1
exec(compile(source.replace(marker,'import movie_filesystem_state_task_fixtures as tasks'),str(parent),'exec'),globals())
