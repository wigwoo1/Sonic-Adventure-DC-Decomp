from pathlib import Path
import ast,re
p=Path('outputs/sonic-adventure');source=(p/'source/sa_engine_cache_writeback.c').read_text()
tree=ast.parse((p/'ghidra/specialize_input_io.py').read_text())
helpers=next(ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='helpers' for t in n.targets))
a=source.index('static uint32_t rd(');b=source.index('static int gt(',a)
body=re.sub(r'\b(rd|sx|wr)\(m,',r'\1(&cache->registers,m,',source[b:])
body=body.replace('int sa_engine_cache_writeback(','int sa_engine_cache_writeback_io(',1).replace(',void *ctx,int (*call)',',const struct sa_cache_io *cache,void *ctx,int (*call)',1)
body=body.replace('sa_sh4_cache_writeback(ctx,','cache->writeback(cache->registers.user,')
(p/'source/sa_engine_cache_writeback_io.c').write_text('#include "cache_io.h"\n'+source[:a]+helpers+body)
