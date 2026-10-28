"""Original constructors with optional declared raw metadata words only."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'movie_input_interface_session.py'
source=parent.read_text();marker='import movie_input_interface_task_fixtures as tasks';assert source.count(marker)==1
source=source.replace(marker,'import collision_angle_task_fixtures as tasks')
marker='def observe(self,geometry=511,status=8,unplug=False,block_data=None):';assert source.count(marker)==1
source=source.replace(marker,marker.replace('block_data=None','block_data=None,metadata_words=None,ack=False'))
marker="        payload=(2).to_bytes(4,'little')+metadata";assert source.count(marker)==1
source=source.replace(marker,"        for offset,value in (metadata_words or {}).items():\n            assert offset in (2,6,8,10,12,16,18,20) and 0<=value<=65535\n            metadata[offset:offset+2]=value.to_bytes(2,byte_order)\n"+marker)
marker='        if block_data is not None:';assert source.count(marker)==1
source=source.replace(marker,"        if ack:\n            assert block_data is None\n            payload=b''\n"+marker)
exec(compile(source,str(parent),'exec'),globals())
