"""Complete the compact reference event-list interface without losing events."""
from pathlib import Path
import write_watch_renderer_context_owner_capture as previous


def with_extend(original):
    # Full-clear evidence keeps a compact sequential span. Later original
    # store-queue prefetches use list.extend for their exact two-event pair.
    # Add only the missing list operation; preserve every old part and count.
    class CompleteEvents(type(original)):
        def extend(self, values):
            for value in values:
                self.append(value)
    result=CompleteEvents()
    result.parts=list(original.parts)
    result.count=original.count
    assert result.parts==original.parts and result.count==original.count
    return result


parent_watched=previous.namespace['watched_reference']


def watched_reference(*args):
    ref=parent_watched(*args)
    ref.events=with_extend(ref.events)
    return ref


namespace=dict(previous.namespace,watched_reference=watched_reference)
exec(compile(previous.code,'complete_event_list_frame_submission_comparison','exec'),namespace,local:={})
compare=local['compare']
