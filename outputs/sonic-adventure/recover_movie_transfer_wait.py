"""Preserve the original request body; bound only its genuine completion wait."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;old=P/'source/sa_movie_bulk_family.c';text=old.read_text();prefix=text[:text.index('SA_STAGE_EXPORT int sa_movie_bulk_family_')]
a=text.index('SA_STAGE_EXPORT int sa_movie_bulk_family_8c65a5dc(');b=text.find('\nSA_STAGE_EXPORT int ',a+1);body=text[a:b if b>=0 else len(text)]
body=body.replace('sa_movie_bulk_family_8c65a5dc','sa_movie_transfer_wait_8c65a5dc')
marker=' uint32_t entry_pr=*pr;';assert body.count(marker)==1
body=body.replace(marker,marker+'\n unsigned completion_wait_visits=0;(void)completion_wait_visits;')
marker='L8c65a6ec: ';assert body.count(marker)==1
guard='L8c65a6ec:\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x8c65a6ecu;if(!(*sr&1u) && ++completion_wait_visits>=2u)return -4;\n#endif\n '
body=body.replace(marker,guard)
source=P/'source/sa_movie_transfer_wait.c';source.write_text(prefix+body)
report=dict(source=source.relative_to(P).as_posix(),new_registry_credit=0,bounded_research_wait=dict(pc='0x8c65a6ec',continue_when_t=False,max_continue_tests=2,status=-4),production_behavior='Without SA_BOUND_RETAIL_STALLS the original completion loop retains its count and waits indefinitely if no real completion arrives.',scope='The older immutable request source is preserved. The new adapter differs only by export name and an explicitly compiled research guard at the actual completion wait. No callback, memory completion flag, IRQ or helper return is supplied. The full request remains partial.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in (Path(__file__),old,source,P/'analysis/movie_bulk_family_translation.json')})
(P/'analysis/movie_transfer_wait_translation.json').write_text(json.dumps(report,indent=2)+'\n');print('Original request completion wait retained; research stop after two continued tests')
