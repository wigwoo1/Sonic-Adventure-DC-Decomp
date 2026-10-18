/* Ghidra draft only; not buildable/verified source. */

void FUN_8c07b2b8(uint param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = (int *)((param_1 & 0xff) * 4 + DAT_8c07b300);
  if ((*piVar2 != 0) && (iVar1 = *(int *)(*piVar2 + 0x70), iVar1 != 0)) {
    *(undefined4 *)(iVar1 + 0xc) = *(undefined4 *)(PTR_PTR_8c07b304 + param_2 * 4);
  }
  return;
}

