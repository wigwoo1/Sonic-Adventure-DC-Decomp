"""Original renderer producers plus explicit finite frame-timer inputs."""
from renderer_context_producer_disc_fixtures import *
import renderer_context_producer_disc_fixtures as previous
import write_watch_renderer_frame_submit as fast


def prepare(ram,c):
    m,s,f,cfg=previous.prepare(ram,c)
    cfg['ticks']=[0,20000000,20000001,20000002]
    return m,s,f,cfg
