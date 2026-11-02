"""Real scene producers with an explicit host-backend completion contract."""
from renderer_frame_clock_fixtures import *
import renderer_frame_clock_fixtures as previous
import model_stage_resources_reference as v
import write_watch_model_stage_resources as fast
def prepare(ram,c):return previous.prepare(ram,c)
