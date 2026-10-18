/* Ghidra draft only; not buildable/verified source. */

void FUN_8c037638(void)

{
  int iVar1;
  int in_r0;
  int *piVar2;
  
  piVar2 = *(int **)(**(int **)PTR_DAT_8c037670 + in_r0 * 0xc + 8);
  if (*piVar2 != *(int *)PTR_DAT_8c037674) {
    *(int *)PTR_DAT_8c037674 = *piVar2;
    iVar1 = DAT_8c037678;
    *(uint *)(DAT_8c037678 + 8) = *(uint *)(DAT_8c037678 + 8) & 0xffffffc0 | piVar2[2] & 0x3fU;
    *(int *)(iVar1 + 0xc) = piVar2[3];
  }
  return;
}

