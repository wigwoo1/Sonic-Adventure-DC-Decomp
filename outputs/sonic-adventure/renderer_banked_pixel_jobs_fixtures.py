"""Real scene producers with an explicit host-backend completion contract."""
from renderer_frame_clock_fixtures import *
import renderer_frame_clock_fixtures as previous
import renderer_banked_pixel_jobs_reference as v
import write_watch_renderer_banked_pixel_jobs as fast
def prepare(ram,c):return previous.prepare(ram,c)
