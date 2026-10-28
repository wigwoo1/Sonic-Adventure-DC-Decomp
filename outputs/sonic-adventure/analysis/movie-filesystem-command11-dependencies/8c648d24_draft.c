/* Ghidra draft only; not buildable/verified source. */

int FUN_8c648d24(int param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_40 [8];
  undefined2 local_38;
  undefined1 local_36;
  undefined4 local_34;
  undefined4 local_18;
  
  iVar1 = DAT_8c648dc8;
  *param_3 = 0;
  if (*(char *)(iVar1 + param_1 * DAT_8c648dba) != '\0') {
    iVar1 = (*(code *)PTR_FUN_8c648dcc)(param_1);
    if (iVar1 == 0) {
      FUN_8c64850a(param_1);
    }
    else {
      iVar1 = (*(code *)PTR_FUN_8c648dd0)(param_1);
      if ((iVar1 != 0) && (*(char *)(DAT_8c648dd4 + param_1 * DAT_8c648dba) != '\0')) {
        iVar1 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c648dd8)(param_1);
        if (iVar1 < 0) {
          return (int)DAT_8c648dbe;
        }
        iVar1 = (*(code *)PTR_FUN_8c648ddc)(param_1,param_2);
        if (-1 < iVar1) {
          iVar1 = (*(code *)PTR_FUN_8c648de0)(param_1,param_2);
          if (iVar1 < 0) {
            return (int)DAT_8c648dc0;
          }
          iVar1 = (*(code *)PTR_FUN_8c648de4)(param_1);
          if (iVar1 != 0) {
            iVar2 = (*(code *)PTR_FUN_8c648edc)(param_1,param_2,auStack_40);
            *DAT_8c648ee0 = iVar2;
            if (iVar2 < 0) {
              (*(code *)PTR_FUN_8c648ee4)(iVar1);
              return (int)DAT_8c648ed2;
            }
            *(undefined4 *)(iVar1 + 4) = 0;
            *(undefined4 *)(iVar1 + 0x34) = local_18;
            *(undefined4 *)(iVar1 + 0x38) = local_34;
            *(undefined4 *)(iVar1 + 0x40) = local_34;
            *(int *)(iVar1 + 0x28) = param_1;
            *(undefined2 *)(iVar1 + 2) = local_38;
            *(undefined1 *)(iVar1 + 1) = local_36;
            *param_3 = iVar1;
            return 0;
          }
        }
        return (int)DAT_8c648dc2;
      }
    }
  }
  return (int)DAT_8c648dbc;
}

