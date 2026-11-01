"""Actual renderer producers using the additive TA DMA backend."""
from renderer_frame_submit_clock_fixtures import *
import renderer_frame_submit_clock_fixtures as previous
import renderer_frame_ta_reference as v
import write_watch_renderer_frame_ta as fast


def prepare(ram,c):return previous.prepare(ram,c)
