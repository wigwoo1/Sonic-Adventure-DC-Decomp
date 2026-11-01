"""Same original producers and native DLL; additional read-only observations."""
from renderer_frame_clock_fixtures import *
import renderer_frame_clock_fixtures as previous
import write_watch_renderer_frame_pacing as fast
def prepare(ram,c):return previous.prepare(ram,c)
