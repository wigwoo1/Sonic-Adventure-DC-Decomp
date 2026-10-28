"""Use the unchanged original model and comparison ABI with the new route DLL."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'collision_angle_reference.py';code=parent.read_text()
marker="effects.DLL=ROOT/'work/sonic-adventure/build/collision_angle.dll'";assert code.count(marker)==1
code=code.replace(marker,"effects.DLL=ROOT/'work/sonic-adventure/build/collision_normalize.dll'")
assert code.count('effects.LIB.sa_collision_angle_')==3
code=code.replace('effects.LIB.sa_collision_angle_','effects.LIB.sa_collision_normalize_')
exec(compile(code,str(parent),'exec'),globals())
