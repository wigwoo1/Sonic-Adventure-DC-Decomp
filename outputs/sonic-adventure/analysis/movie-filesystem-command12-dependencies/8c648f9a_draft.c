/* Ghidra draft only; not buildable/verified source. */

int FUN_8c648f9a(int param_1,char *param_2,undefined4 param_3,int *param_4)

{
  undefined4 uVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  undefined1 auStack_44 [48];
  
  iVar3 = DAT_8c649020;
  *param_4 = 0;
  if (*(char *)(iVar3 + param_1 * DAT_8c648ffa) != '\0') {
    iVar3 = (*(code *)PTR_FUN_8c649024)(param_1);
    if (iVar3 == 0) {
      FUN_8c64850a(param_1);
    }
    else {
      iVar3 = (*(code *)PTR_FUN_8c649028)(param_1);
      if ((iVar3 != 0) && (*(char *)(DAT_8c64902c + param_1 * DAT_8c648ffa) != '\0')) {
        iVar3 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c64912c)(param_1);
        if (iVar3 < 0) {
          return (int)DAT_8c649120;
        }
        iVar3 = (*(code *)PTR_FUN_8c649130)(param_1,param_2);
        if (-1 < iVar3) {
          iVar3 = (*(code *)PTR_FUN_8c649134)(param_1,param_2,auStack_44);
          if ((-1 < iVar3) && (iVar3 = FUN_8c649c68(param_1,param_2), iVar3 < 0)) {
            return iVar3;
          }
          iVar3 = (*(code *)PTR_FUN_8c649138)(param_1);
          if (-1 < iVar3) {
            (*(code *)PTR_FUN_8c64913c)(param_1,iVar3);
            iVar4 = (*(code *)PTR_FUN_8c649140)(param_1);
            uVar1 = DAT_8c649144;
            if (iVar4 == 0) {
              return (int)DAT_8c649124;
            }
            *(undefined4 *)(iVar4 + 8) = 0;
            *(undefined4 *)(iVar4 + 4) = 1;
            *(int *)(iVar4 + 0x2c) = iVar3;
            *(undefined4 *)(iVar4 + 0x38) = uVar1;
            *(undefined4 *)(iVar4 + 0x30) = 0;
            *(int *)(iVar4 + 0x28) = param_1;
            *(undefined2 *)(iVar4 + 2) = 0;
            *(undefined1 *)(iVar4 + 1) = 0;
            pcVar5 = (char *)(iVar4 + 0x10);
            for (; puVar2 = PTR_FUN_8c649148, *param_2 != '\0'; param_2 = param_2 + 1) {
              *pcVar5 = *param_2;
              pcVar5 = pcVar5 + 1;
            }
            *pcVar5 = '\0';
            (*(code *)puVar2)(iVar4 + 0x20,param_3);
            *param_4 = iVar4;
            return 0;
          }
        }
        return (int)DAT_8c649122;
      }
    }
  }
  return (int)DAT_8c648ffc;
}

