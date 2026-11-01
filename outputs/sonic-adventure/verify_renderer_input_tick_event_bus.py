"""Retain all TA-event platform contract cases on the new input callback DLL."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_renderer_filesystem_irq_event_bus.py'
code=parent.read_text().replace('import write_watch_renderer_filesystem_irq as w','import write_watch_renderer_input_tick_mmio as w').replace('renderer_filesystem_irq_event_bus_verification.json','renderer_input_tick_event_bus_verification.json').replace('build/renderer_filesystem_callback.dll','build/renderer_input_tick_mmio.dll')
exec(compile(code,str(parent),'exec'),globals())
