"""Private original headers with four declarations importing real shared state."""
import hashlib
from pathlib import Path

REPAIRS={
    'src/camera/camera.h':((b'extern struct task_work *camera_work;',b'__declspec(dllimport) extern struct task_work *camera_work;'),),
    'src/system/pad.h':((b'extern PDS_PERIPHERAL pad_records[8];',b'__declspec(dllimport) extern PDS_PERIPHERAL pad_records[8];'),),
    'gcc/src/system/port.h':((b'extern int frame_redrawing;',b'__declspec(dllimport) extern int frame_redrawing;'),
                           (b'extern Bool event_skipping;',b'__declspec(dllimport) extern Bool event_skipping;')),
    'src/system/frame.h':(),
}

def prepare(upstream,destination):
    original={};generated={}
    for name,edits in REPAIRS.items():
        raw=(upstream/name).read_bytes();original[name]=hashlib.sha256(raw).hexdigest()
        for before,after in edits:
            assert raw.count(before)==1,(name,before);raw=raw.replace(before,after)
        target=destination/name;assert not target.exists();target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)
        generated[name]=hashlib.sha256(raw).hexdigest()
    return original,generated
