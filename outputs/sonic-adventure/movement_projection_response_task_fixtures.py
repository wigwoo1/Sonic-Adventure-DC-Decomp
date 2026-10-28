"""Preserved original task construction and APIs using current 225-source DLL."""
import types
import movie_channel_request_task_fixtures as prior
import write_watch_movement_projection_response as fast
v=fast.old;INPUT_OBJECT=prior.INPUT_OBJECT;GLOBAL_OBJECT=prior.GLOBAL_OBJECT
put=prior.put;layout=prior.layout;configure=prior.configure
call=types.FunctionType(prior.call.__code__,dict(prior.call.__globals__,v=v,fast=fast),'call',prior.call.__defaults__)
