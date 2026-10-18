/* Recovered from USA Rev A routine 0x8c04c5e0 and its literal pool.
 * Valid terminated PRS input and sufficient output storage are preconditions,
 * matching the original two-pointer interface. This function uses native pointers.
 */
#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct prs_bits { const uint8_t *cursor; uint32_t flags, remaining; };
static uint32_t next_bit(struct prs_bits *reader) {
 if(reader->remaining==0){reader->flags=*reader->cursor++;reader->remaining=8;}
 uint32_t bit=reader->flags&1u;reader->flags>>=1;--reader->remaining;return bit;
}
SA_EXPORT uint32_t sa_prs_decompress_8c04c5e0(const uint8_t *input,uint8_t *output) {
 struct prs_bits reader={input,0,0};uint8_t *start=output;
 for(;;) {
  if(next_bit(&reader)){*output++=*reader.cursor++;continue;}
  uint32_t length;int32_t distance;
  if(next_bit(&reader)) {
   uint32_t packed=reader.cursor[0]|((uint32_t)reader.cursor[1]<<8);reader.cursor+=2;
   if(packed==0)return (uint32_t)(output-start);
   distance=(int32_t)(packed>>3)-8192;length=packed&7u;
   length=length==0?(uint32_t)*reader.cursor++ + 1:length+2;
  }else{
   length=next_bit(&reader)<<1;length|=next_bit(&reader);length+=2;
   distance=(int32_t)*reader.cursor++-256;
  }
  const uint8_t *source=output+distance;
  do{*output++=*source++;}while(--length!=0);
 }
}
