"""Original context producers with explicit, already-latched device events."""
from renderer_frame_ta_fixtures import *
import renderer_frame_ta_fixtures as previous
import renderer_frame_irq_reference as v
import write_watch_renderer_frame_irq as fast


def prepare(ram,c):return previous.prepare(ram,c)
