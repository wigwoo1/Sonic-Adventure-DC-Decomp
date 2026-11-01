"""Replay prior live-record assertions without modifying their source or proofs."""
import player_movement_path_reference
from pathlib import Path
P=Path(__file__).resolve().parent
parent=P/'verify_movement_special_tail_live_records.py';source=parent.read_text()
assert source.count('import movement_special_tail_live_record_fixtures as inputs')==1
source=source.replace('import movement_special_tail_live_record_fixtures as inputs','import player_movement_retained_inputs as inputs')
source=source.replace('movement_special_tail_live_record_verification.json','player_movement_retained_verification.json')
source=source.replace("P/'movement_special_tail_live_record_fixtures.py'","P/'player_movement_retained_inputs.py'")
assert not (P/'analysis/player_movement_retained_verification.json').exists()
exec(compile(source,str(parent),'exec'),globals())
