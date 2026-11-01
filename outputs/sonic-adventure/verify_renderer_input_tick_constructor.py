"""Recheck all constructor/accessor oracles on the final callback DLL."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_renderer_input_constructor.py'
code=parent.read_text().replace('import renderer_filesystem_irq_fixtures as q','import renderer_input_tick_mmio_fixtures as q').replace('renderer_input_constructor_verification.json','renderer_input_tick_constructor_verification.json').replace('build/renderer_filesystem_callback.dll','build/renderer_input_tick_mmio.dll')
code=code.replace('files=[Path(__file__),','files=[Path(__file__),parent,P/\'renderer_input_tick_mmio_reference.py\',P/\'write_watch_renderer_input_tick_mmio.py\',')
exec(compile(code,str(parent),'exec'),globals())
