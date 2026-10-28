"""Raw binary32 initial query geometry for the original angle-fold obligation."""
from movement_orientation_response_fixtures import *
import movement_orientation_response_fixtures as previous
def prepare(ram,c):
    m,s,f,cfg=previous.prepare(ram,c)
    for j,word in enumerate(c.get('orientation_words',())):put(m,C+0x48+4*j,word)
    return m,s,f,cfg
