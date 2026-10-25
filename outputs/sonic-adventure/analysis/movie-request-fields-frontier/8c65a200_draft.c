/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65a200(void)

{
  uint *puVar1;
  undefined4 uVar2;
  int iVar3;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  (*(code *)PTR_FUN_8c65a274)
            (DAT_8c65a270,DAT_8c65a26c,4,1,1,1,
             (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  uVar2 = DAT_8c65a270;
  puVar1 = DAT_8c65a26c;
  *DAT_8c65a26c = *DAT_8c65a26c | 1;
  (*(code *)PTR_FUN_8c65a278)(puVar1,uVar2,4,1,1,1);
  iVar3 = (int)DAT_8c65a26a;
  do {
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  return 0;
}

