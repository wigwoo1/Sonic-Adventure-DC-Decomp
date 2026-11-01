/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c606a88(int param_1)

{
  int iVar1;
  undefined *puVar2;
  uint uVar3;
  uint uVar4;
  int *piVar5;
  
  puVar2 = PTR_DAT_8c606b48;
  iVar1 = DAT_8c606b20;
  piVar5 = *(int **)(param_1 * 0xc + *(int *)*DAT_8c606b40 + 8);
  if ((*piVar5 != *(int *)PTR_DAT_8c606b44) || (*(int *)PTR_DAT_8c606b48 != piVar5[1])) {
    *(int *)PTR_DAT_8c606b44 = *piVar5;
    uVar3 = DAT_8c606b4c;
    *(int *)puVar2 = piVar5[1];
    uVar4 = DAT_8c606b50;
    *(uint *)(iVar1 + 8) = *(uint *)(iVar1 + 8) & 0xffffffc0 | piVar5[2] & 0x3fU;
    if (uVar4 < (uVar3 & piVar5[3])) {
      return 0xffffffff;
    }
    *(int *)(iVar1 + 0xc) = piVar5[3];
  }
  return 1;
}

