/* Ghidra draft only; not buildable/verified source. */

int FUN_8c648e36(int param_1,char *param_2,undefined4 param_3,int *param_4)

{
  undefined4 uVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  undefined1 auStack_44 [48];
  
  iVar5 = (int)DAT_8c648ed4;
  iVar6 = param_1 * iVar5 + DAT_8c648ee8;
  *param_4 = 0;
  if (*(char *)(iVar6 + 2) != '\0') {
    iVar3 = (*(code *)PTR_FUN_8c648eec)(param_1);
    if (iVar3 == 0) {
      FUN_8c64850a(param_1);
    }
    else {
      iVar3 = (*(code *)PTR_FUN_8c648ef0)(param_1);
      if ((iVar3 != 0) && (*(char *)(param_1 * iVar5 + DAT_8c648ee8 + 8) != '\0')) {
        iVar5 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c648ef4)(param_1);
        if (iVar5 < 0) {
          return (int)DAT_8c648ed8;
        }
        if ((*(int *)(*(int *)(iVar6 + 0x1c) + 0x38) != 0) &&
           (iVar5 = (*(code *)PTR_FUN_8c648ef8)(param_1,param_2), -1 < iVar5)) {
          iVar5 = (*(code *)PTR_FUN_8c648edc)(param_1,param_2,auStack_44);
          if (-1 < iVar5) {
            return (int)DAT_8c648eda;
          }
          iVar5 = (*(code *)PTR_FUN_8c649000)(param_1,auStack_44);
          if (-1 < iVar5) {
            return (int)DAT_8c648ff4;
          }
          uVar4 = (*(code *)PTR_FUN_8c649004)(param_1,0);
          if (((uVar4 & 0xffff) == DAT_8c649008) &&
             (iVar5 = (*(code *)PTR_FUN_8c64900c)(param_1), -1 < iVar5)) {
            (*(code *)PTR_FUN_8c649010)(param_1,iVar5);
            iVar6 = (*(code *)PTR_FUN_8c649014)(param_1);
            uVar1 = DAT_8c649018;
            if (iVar6 == 0) {
              return (int)DAT_8c648ff8;
            }
            *(undefined4 *)(iVar6 + 8) = 1;
            *(undefined4 *)(iVar6 + 4) = 2;
            *(int *)(iVar6 + 0x2c) = iVar5;
            *(undefined4 *)(iVar6 + 0x38) = uVar1;
            *(undefined4 *)(iVar6 + 0x30) = 0;
            *(int *)(iVar6 + 0x28) = param_1;
            *(undefined2 *)(iVar6 + 2) = 1;
            *(undefined1 *)(iVar6 + 1) = 0;
            pcVar7 = (char *)(iVar6 + 0x10);
            for (; puVar2 = PTR_FUN_8c64901c, *param_2 != '\0'; param_2 = param_2 + 1) {
              *pcVar7 = *param_2;
              pcVar7 = pcVar7 + 1;
            }
            *pcVar7 = '\0';
            (*(code *)puVar2)(iVar6 + 0x20,param_3);
            *param_4 = iVar6;
            return 0;
          }
        }
        return (int)DAT_8c648ff6;
      }
    }
  }
  return (int)DAT_8c648ed6;
}

