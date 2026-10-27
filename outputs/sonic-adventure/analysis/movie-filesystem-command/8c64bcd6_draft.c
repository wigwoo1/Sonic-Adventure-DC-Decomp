/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64bcd6(int param_1,undefined4 param_2,int param_3,uint param_4,uint param_5)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  undefined1 auStack_30 [4];
  ushort local_2c;
  char local_2a;
  
  iVar3 = param_1 * DAT_8c64bd68 + DAT_8c64bd70;
  *(undefined4 *)(iVar3 + 0x30) = 1;
  puVar1 = PTR_FUN_8c64bd74;
  *(uint *)(iVar3 + 0x34) = param_5;
  iVar3 = (*(code *)puVar1)(param_1,param_2,auStack_30);
  if (-1 < iVar3) {
    if (local_2a == '\x01') {
      uVar2 = (uint)local_2c;
      if (((param_4 < uVar2) && (param_5 <= uVar2)) && (param_4 + param_5 <= uVar2)) {
        iVar3 = FUN_8c64b300(param_1,param_2);
        if (-1 < iVar3) {
          uVar2 = 0;
          if (param_5 != 0) {
            do {
              iVar3 = (*(code *)PTR_FUN_8c64bd78)(param_1,param_4,param_3);
              if (iVar3 < 0) {
                return iVar3;
              }
              uVar2 = uVar2 + 1;
              param_3 = param_3 + DAT_8c64bd9c;
              param_4 = param_4 + 1;
            } while (uVar2 < param_5);
          }
          iVar3 = 0;
        }
      }
      else {
        iVar3 = (int)DAT_8c64bd6c;
      }
    }
    else {
      iVar3 = (int)DAT_8c64bd6a;
    }
  }
  return iVar3;
}

