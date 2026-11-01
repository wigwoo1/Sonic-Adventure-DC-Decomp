"""Ordinary moving-record inputs with the actual original0751c0 callback.

Callback selection is authored input, not proof of retail callback assignment.
No callback result, contact output or execution frame is supplied.
"""
from movement_special_tail_context_fixtures import *
import movement_special_tail_context_fixtures as previous

CALLBACK_TABLE = 0x8c78c648
IMPULSE = 0x8c0751c0


def prepare(ram, c):
    m, s, f, cfg = previous.prepare(ram, c)
    view = c.get('view', 0x8c000000)
    ptr = lambda a: view | (a & 0xffffff)
    for offset in (0, 0x1c):
        put(m, DATA+offset, c.get('callback', IMPULSE))
        vector(m, DATA+offset+0x3c, (number(0x3f800001), 0, 0))
    record = DATA + (0x1c if c.get('index', 0) else 0)
    character = record-0x10 if c.get('callback_record_alias') else D
    put(m, CALLBACK_TABLE+4*c.get('index', 0), ptr(character))
    return m, s, f, cfg
