"""Replay all prior alias/control assertions unchanged against the250-source DLL."""
from pathlib import Path
P=Path(__file__).resolve().parent
parent=P/'verify_collision_contact_counter_alias.py'
source=parent.read_text()
marker='import collision_contact_fixtures as fixture'
assert source.count(marker)==1
source=source.replace(marker,'import collision_setup_contact_fixtures as fixture')
marker="dest=P/'analysis/collision_contact_counter_alias_verification.json'"
assert source.count(marker)==1
source=source.replace(marker,"dest=P/'analysis/collision_setup_contact_alias_verification.json'")
marker='files = [Path(__file__),'
assert source.count(marker)==1
source=source.replace(marker,"files = [Path(__file__),P/'verify_collision_contact_counter_alias.py',P/'collision_setup_contact_fixtures.py',")
namespace=dict(__name__='setup_retained_contact_alias',__file__=str(Path(__file__)))
exec(compile(source,str(parent),'exec'),namespace)
if __name__=='__main__':namespace['main']()
