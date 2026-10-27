"""Preserved task admission with new request execution; no task state replacement."""
import types
import movie_channel_io_fixtures as prior
import write_watch_channel_request as fast
v=fast.old;INPUT_OBJECT=prior.INPUT_OBJECT;GLOBAL_OBJECT=prior.GLOBAL_OBJECT
put=prior.put;layout=prior.layout;configure=prior.configure
call=types.FunctionType(prior.call.__code__,dict(prior.call.__globals__,v=v,fast=fast),'call',prior.call.__defaults__)
