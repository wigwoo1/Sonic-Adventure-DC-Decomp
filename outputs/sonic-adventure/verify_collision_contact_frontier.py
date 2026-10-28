"""Derived extended-edge inputs for the final signed-area rejection outcome."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_collision_contact_direct.py';source=parent.read_text()
marker="('collision_contact_direct'+suffix+'_verification.json')";assert source.count(marker)==1
source=source.replace(marker,"('collision_contact_frontier'+suffix+'_verification.json')")
marker='files=[Path(__file__),';assert source.count(marker)==1
source=source.replace(marker,"files=[Path(__file__),P/'verify_collision_contact_direct.py',")
ns=dict(__name__='contact_extended_edge_replay',__file__=str(Path(__file__)))
exec(compile(source,str(parent),'exec'),ns)
# A=(0,0,0), B=(0,0,2), C=(2,0,0): middle signed area is
# 4-2*x-2*z. Set x+z=2 with x and z opposite signs. The first
# two products are zero; only the third signed-area test rejects.
ns['configurations']=lambda:iter([dict(kind='plane',point=(3,.25,-1)),dict(kind='plane',point=(-1,.25,3))])
if __name__=='__main__':ns['main']()
