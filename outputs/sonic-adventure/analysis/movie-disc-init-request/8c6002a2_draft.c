/* Ghidra draft only; not buildable/verified source. */

int callback_8c6002a2(char *param_1,int param_2)

{
  undefined *puVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  
  piVar2 = DAT_8c600358;
  puVar1 = PTR_DAT_8c600354;
  iVar5 = *(int *)PTR_DAT_8c600354;
  if (param_2 == 0) {
    param_2 = *(int *)(iVar5 + 0x10);
  }
  if ((*param_1 == '/') || (*param_1 == '\\')) {
    if (*(short *)*DAT_8c600358 == 1) {
      iVar5 = (*(code *)PTR_FUN_8c60035c)
                        (iVar5 + 0x80,*(undefined4 *)(iVar5 + 4),*(undefined4 *)(iVar5 + 8),param_1,
                         (short *)*DAT_8c600358 + 4);
    }
    else {
      iVar5 = (*(code *)PTR_FUN_8c600360)
                        (iVar5 + 0x80,*(undefined4 *)(iVar5 + 4),*(undefined4 *)(iVar5 + 8),param_1)
      ;
    }
    if (iVar5 == 0) {
      return -8;
    }
    iVar4 = (int)DAT_8c600342;
    iVar5 = DAT_8c600340 + iVar5;
  }
  else {
    iVar5 = (*(code *)PTR_FUN_8c600364)(param_1,*(undefined4 *)(iVar5 + 0x10),0);
    if (iVar5 == 0) {
      return -8;
    }
    piVar3 = (int *)(*(int *)(*(int *)puVar1 + 0x10) + 0x10 + (iVar5 + -1) * 0x2c);
    if ((*(byte *)(piVar3 + 2) & 2) == 0) {
      return -0xb;
    }
    iVar4 = piVar3[1];
    iVar5 = *piVar3 + (int)DAT_8c600470;
  }
  iVar4 = (*(code *)PTR_FUN_8c600474)(*(int *)puVar1 + 0x40,iVar5,param_2,iVar4);
  if (((*(short *)*piVar2 == 1) && (*(int *)(*piVar2 + 4) == iVar5)) &&
     ((iVar4 == 0 || (iVar4 == -0xc)))) {
    FUN_8c5ffcfe(param_2);
  }
  return iVar4;
}

