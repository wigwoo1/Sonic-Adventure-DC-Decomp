"""Replay existing contact inputs through the additive250-source runtime."""
import types
import collision_contact_fixtures as previous
import collision_setup_fixtures as setup
from collision_contact_fixtures import *
fast=setup.fast
v=types.SimpleNamespace(**setup.v.__dict__)
v.ANALYSIS_ROUTINES=previous.v.ANALYSIS_ROUTINES
prepare=types.FunctionType(previous.prepare.__code__,dict(previous.prepare.__globals__,v=v),'prepare')
