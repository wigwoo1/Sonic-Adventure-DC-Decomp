/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6434e0(undefined4 *param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int *piVar4;
  
  puVar1 = PTR_FUN_8c643594;
  piVar4 = *(int **)((int)DAT_8c643588 + *(int *)PTR_DAT_8c64358c);
  iVar3 = DAT_8c643590;
  if (*(int *)PTR_DAT_8c64358c != 0) {
    (*(code *)PTR_FUN_8c643594)(PTR_s_Create__8c643598,FUN_8c667b7a);
    puVar2 = PTR_s__02X_8c64359c;
    iVar3 = 0;
    do {
      (*(code *)puVar1)(puVar2,iVar3);
      if (*piVar4 != 1) {
        (*(code *)puVar1)(PTR_s_Success_Handle__08X_8c6435a8,piVar4);
        (*(code *)PTR_FUN_8c6435b0)();
        iVar3 = (int)DAT_8c64358a;
        *param_1 = piVar4;
        *(int *)(iVar3 + *(int *)PTR_DAT_8c64358c) = *(int *)(iVar3 + *(int *)PTR_DAT_8c64358c) + 1;
        iVar3 = 0;
        goto LAB_8c643566;
      }
      iVar3 = iVar3 + 1;
      piVar4 = piVar4 + 8;
    } while (iVar3 < 0x40);
    (*(code *)puVar1)(PTR_s_Error__8c6435a0);
    iVar3 = DAT_8c6435a4;
    *param_1 = 0;
  }
LAB_8c643566:
  if (iVar3 != 0) {
    (*(code *)PTR_FUN_8c6435b8)(PTR_FUN_8c6435b4,iVar3);
  }
  return iVar3;
}

