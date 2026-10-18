/* USA Rev A, 0x8c0100d8..0x8c010131. Recovered startup routine.
 * Precondition: valid original pointers into a 16 MiB Dreamcast RAM image.
 * Scratch registers remain explicit until callers have been recovered.
 */
#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_state { uint32_t r[16]; uint32_t sr, pr, pc; };
static uint32_t read32(const uint8_t *ram, uint32_t address) {
 uint32_t i=address-0x8c000000u;
 return (uint32_t)ram[i] | ((uint32_t)ram[i+1]<<8) |
        ((uint32_t)ram[i+2]<<16) | ((uint32_t)ram[i+3]<<24);
}
static void set_hs(struct sa_state *s, uint32_t left, uint32_t right) {
 s->sr=(s->sr&~1u)|(left>=right);
}
static uint32_t signed_byte(uint8_t b) { return b<128 ? b : (uint32_t)b-256u; }
SA_EXPORT void sa_initialize_sections_8c0100d8(uint8_t *ram, struct sa_state *s) {
 /* Reread end descriptors inside each loop, as the original does. */
 s->r[3]=0x8c1111acu; s->r[4]=0; s->r[6]=0x8c1111b8u;
 s->r[5]=read32(ram,s->r[3]);
 for (;;) {
  s->r[3]=read32(ram,s->r[6]); set_hs(s,s->r[5],s->r[3]);
  if (s->sr&1u) break;
  ram[s->r[5]-0x8c000000u]=(uint8_t)s->r[4]; ++s->r[5];
 }
 s->r[3]=0x8c11119cu; s->r[2]=0x8c111194u;
 s->r[5]=read32(ram,s->r[3]); s->r[7]=0x8c1111a8u; s->r[6]=read32(ram,s->r[2]);
 for (;;) {
  s->r[2]=read32(ram,s->r[7]); set_hs(s,s->r[5],s->r[2]);
  if(s->sr&1u)break;
  s->r[3]=signed_byte(ram[s->r[6]-0x8c000000u]); ++s->r[6];
  ram[s->r[5]-0x8c000000u]=(uint8_t)s->r[3]; ++s->r[5];
 }
 s->r[2]=0x8c66c2ecu; s->r[6]=0x8c66c2f8u; s->r[5]=read32(ram,s->r[2]);
 for (;;) {
  s->r[2]=read32(ram,s->r[6]); set_hs(s,s->r[5],s->r[2]);
  if(s->sr&1u)break;
  ram[s->r[5]-0x8c000000u]=(uint8_t)s->r[4]; ++s->r[5];
 }
 s->r[2]=0x8c66c2d8u; s->r[3]=0x8c66c2d0u;
 s->r[4]=read32(ram,s->r[2]); s->r[6]=0x8c66c2e4u; s->r[5]=read32(ram,s->r[3]);
 for (;;) {
  s->r[3]=read32(ram,s->r[6]); set_hs(s,s->r[4],s->r[3]);
  if(s->sr&1u)break;
  s->r[2]=signed_byte(ram[s->r[5]-0x8c000000u]); ++s->r[5];
  ram[s->r[4]-0x8c000000u]=(uint8_t)s->r[2]; ++s->r[4];
 }
 s->pc=s->pr; /* rts, with nop in its delay slot */
}
