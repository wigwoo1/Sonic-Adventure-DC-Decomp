"""Real scene producers with an explicit host-backend completion contract."""
from renderer_frame_clock_fixtures import *
import renderer_frame_clock_fixtures as previous
import model_camera_final_reference as v
import write_watch_model_camera_final as fast
def prepare(ram,c):return previous.prepare(ram,c)
