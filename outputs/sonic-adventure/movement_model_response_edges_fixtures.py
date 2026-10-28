"""Additional authored triangle inputs for movement angle/commit branches."""
from movement_model_response_fixtures import *
from movement_model_response_fixtures import prepare as _prepare

def prepare(ram,c):
    m,s,f,cfg=_prepare(ram,c)
    if c.get('downward_face'):
        for j,p in enumerate(((0,0,0),(2,0,0),(0,0,2))):vector(m,previous.FACES+0x18+j*12,p)
    return m,s,f,cfg
