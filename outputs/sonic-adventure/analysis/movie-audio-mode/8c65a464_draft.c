/* Ghidra draft only; not buildable/verified source. */

int FUN_8c65a464(void)

{
  int iVar1;
  int iVar2;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  iVar2 = DAT_8c65a504;
  if (*(int *)PTR_DAT_8c65a500 != 0) {
    iVar1 = *(int *)PTR_DAT_8c65a500 + (int)DAT_8c65a4fa;
    iVar2 = DAT_8c65a508;
    if (*(int *)(iVar1 + 0x14) != (int)DAT_8c65a4fc) {
      iVar2 = *(int *)(iVar1 + 0x18);
      *(int *)(iVar1 + 0x18) = iVar2 + 1;
      if (*(int *)(iVar1 + 0x18) == *(int *)(iVar1 + 0x10)) {
        *(undefined4 *)(iVar1 + 0x18) = 0;
      }
      *(undefined4 *)(iVar2 * 0x1c + *(int *)(iVar1 + 0xc) + 0x14) = 1;
      (*(code *)PTR_FUN_8c65a50c)
                ((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
      *(int *)(iVar1 + 0x14) = *(int *)(iVar1 + 0x14) + 1;
      iVar2 = 0;
    }
  }
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c65a514)(PTR_FUN_8c65a510,iVar2);
  }
  return iVar2;
}

