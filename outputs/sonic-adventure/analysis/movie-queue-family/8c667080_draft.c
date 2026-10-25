/* Ghidra draft only; not buildable/verified source. */

int FUN_8c667080(undefined4 param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  puVar1 = PTR_DAT_8c66713c;
  iVar3 = *(int *)PTR_DAT_8c66713c;
  iVar2 = DAT_8c667140;
  if ((iVar3 != 0) &&
     (iVar2 = DAT_8c667144, *(int *)(DAT_8c667136 + iVar3) < *(int *)(DAT_8c667136 + -0xc + iVar3)))
  {
    iVar2 = (int)DAT_8c667138;
    *(undefined4 *)(*(int *)(iVar2 + iVar3) * 8 + *(int *)(iVar2 + -8 + iVar3)) = param_1;
    *(undefined4 *)
     (*(int *)(iVar2 + *(int *)puVar1) * 8 + *(int *)(iVar2 + -8 + *(int *)puVar1) + 4) = param_2;
    *(int *)(iVar2 + *(int *)puVar1) = *(int *)(iVar2 + *(int *)puVar1) + 1;
    if (*(int *)(iVar2 + *(int *)puVar1) == *(int *)(iVar2 + -4 + *(int *)puVar1)) {
      *(undefined4 *)((int)DAT_8c667138 + *(int *)puVar1) = 0;
    }
    *(int *)((int)DAT_8c667136 + *(int *)puVar1) = *(int *)((int)DAT_8c667136 + *(int *)puVar1) + 1;
    iVar2 = 0;
  }
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c66714c)
              (PTR_FUN_8c667148,iVar2,
               (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  }
  return iVar2;
}

