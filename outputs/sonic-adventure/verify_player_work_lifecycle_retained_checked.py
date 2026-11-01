"""Load the established host import chain before replaying the retained suite."""
# The reference adapter establishes its existing4096 host import floor. No
# guest instruction limit, execution state, assertion or expected result changes.
import player_work_allocation_reference
from pathlib import Path
parent=Path(__file__).resolve().parent/'verify_player_work_lifecycle_retained.py'
exec(compile(parent.read_text(),str(parent),'exec'),globals())
