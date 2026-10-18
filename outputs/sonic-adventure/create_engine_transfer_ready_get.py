from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_engine_transfer_ready_get.c';s=source.read_text()
io=(p/'source/sa_engine_transfer_read_strided_io.c').read_text()
a=s.index('static uint32_t rd(');z=s.index('static int gt(',a)
b=io.index('static uint32_t rd(');e=io.index('static int gt(',b)
s=s[:a]+io[b:e]+s[z:]
s='#include "input_io.h"\n'+s
s=s.replace('uint32_t *macl,void *ctx', 'uint32_t *macl,const struct sa_input_io *io,void *ctx').replace('rd(m,','rd(io,m,').replace('sx(m,','sx(io,m,').replace('wr(m,','wr(io,m,').replace('retail Tikal helper','retail transfer readiness getter')
source.write_text(s)
stem='engine_transfer_ready_get'
(p/f'build_{stem}.ps1').write_text((p/'build_engine_transfer_wait.ps1').read_text().replace('engine_transfer_wait',stem))
s=(p/'register_engine_transfer_wait.py').read_text().replace('engine_transfer_wait',stem).replace(str(0x8c10eee0),str(0x8c10fb48)).replace(str(0x8c10ef12),str(0x8c10fb56)).replace('==32','==48')
(p/f'register_{stem}.py').write_text(s)
