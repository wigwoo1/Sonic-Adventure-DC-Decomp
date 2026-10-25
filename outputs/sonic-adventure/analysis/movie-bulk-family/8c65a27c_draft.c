/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65a27c(void)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  (*(code *)PTR_FUN_8c65a2f4)
            (DAT_8c65a2f0,PTR_DAT_8c65a2ec,4,1,1,1,
             (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  uVar2 = DAT_8c65a2f0;
  puVar1 = PTR_DAT_8c65a2ec;
  *(uint *)PTR_DAT_8c65a2ec = *(uint *)PTR_DAT_8c65a2ec & 0xfffffffe;
  (*(code *)PTR_FUN_8c65a2f8)(puVar1,uVar2,4,1,1,1);
  iVar3 = (int)DAT_8c65a2e8;
  do {
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  return 0;
}

