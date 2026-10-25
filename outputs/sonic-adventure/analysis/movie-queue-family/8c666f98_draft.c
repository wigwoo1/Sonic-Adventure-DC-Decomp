/* Ghidra draft only; not buildable/verified source. */

int FUN_8c666f98(undefined4 *param_1,undefined4 *param_2)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  puVar1 = PTR_DAT_8c667070;
  iVar3 = *(int *)PTR_DAT_8c667070;
  iVar2 = DAT_8c667074;
  if (iVar3 != 0) {
    iVar2 = (int)DAT_8c66706c;
    *param_1 = *(undefined4 *)(*(int *)(iVar2 + iVar3) * 8 + *(int *)(iVar2 + -0xc + iVar3));
    *param_2 = *(undefined4 *)
                (*(int *)(iVar2 + *(int *)puVar1) * 8 + *(int *)(iVar2 + -0xc + *(int *)puVar1) + 4)
    ;
    *(undefined4 *)(*(int *)(iVar2 + *(int *)puVar1) * 8 + *(int *)(iVar2 + -0xc + *(int *)puVar1))
         = 0;
    *(undefined4 *)
     (*(int *)(iVar2 + *(int *)puVar1) * 8 + *(int *)(iVar2 + -0xc + *(int *)puVar1) + 4) = 0;
    *(int *)(iVar2 + *(int *)puVar1) = *(int *)(iVar2 + *(int *)puVar1) + 1;
    if (*(int *)(iVar2 + *(int *)puVar1) == *(int *)(iVar2 + -8 + *(int *)puVar1)) {
      *(undefined4 *)((int)DAT_8c66706c + *(int *)puVar1) = 0;
    }
    *(int *)((int)DAT_8c66706e + *(int *)puVar1) = *(int *)((int)DAT_8c66706e + *(int *)puVar1) + -1
    ;
    iVar2 = 0;
  }
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c66707c)
              (PTR_FUN_8c667078,iVar2,
               (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  }
  return iVar2;
}

