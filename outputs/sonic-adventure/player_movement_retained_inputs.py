"""Prior live-record inputs/assertions through the extended272-source DLL."""
from movement_special_tail_live_record_fixtures import *
import write_watch_player_movement_path as fast
import json,types
v=types.SimpleNamespace(**vars(fast.old))
# Select the actual retained subject for coverage metadata only. Execution,
# reference instruction rows and every old assertion stay unchanged.
v.ANALYSIS_ROUTINES=json.loads((v.P/'analysis/movement_special_tail_translation.json').read_text())['routines']
