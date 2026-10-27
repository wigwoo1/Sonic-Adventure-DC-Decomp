/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65f1aa(void)

{
  byte bVar1;
  
  if (*(int *)((int)DAT_8c65f286 + *DAT_8c65f290) != 0) {
    return 0xfffffffd;
  }
  if ((*(uint *)(*(int *)((int)DAT_8c65f288 + *DAT_8c65f290) + 0x18) & 1) != 0) {
    return 0xffffffff;
  }
  bVar1 = **(byte **)PTR_PTR_8c65f28c;
  **(byte **)PTR_PTR_8c65f28c = bVar1 | 0x80;
  if (bVar1 != 0) {
    FUN_8c65f348(PTR_s____FATAL_ERROR_Cannot_get_semaph_8c65f294);
    return 0xfffffffe;
  }
  return 0;
}

