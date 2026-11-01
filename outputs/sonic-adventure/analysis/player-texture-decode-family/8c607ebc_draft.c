/* Ghidra draft only; not buildable/verified source. */

void FUN_8c607ebc(undefined4 *param_1,undefined4 param_2,int param_3)

{
  ushort uVar1;
  int iVar2;
  undefined1 *puVar3;
  int iVar4;
  ushort local_5c [2];
  int local_58;
  undefined1 auStack_54 [4];
  undefined4 local_50;
  undefined1 *local_4c;
  undefined1 auStack_48 [44];
  
  uVar1 = *(ushort *)(param_3 + 4);
  param_1 = (undefined4 *)*param_1;
  local_4c = auStack_48 + 2;
  local_58 = (int)*(short *)(param_3 + 6);
  iVar4 = 0;
  local_50 = param_2;
  if (0 < local_58) {
    do {
      FUN_8c606f20(auStack_48,local_50);
      FUN_8c606f66(local_5c,auStack_48);
      param_1[1] = param_1[1] & DAT_8c607f84 | (uint)local_5c[0];
      iVar2 = 2;
      if ((uVar1 & 1) != 0) {
        (*(code *)PTR_FUN_8c607f88)(*param_1,local_4c,0x1c);
        iVar2 = 0x1e;
      }
      if ((uVar1 & 2) != 0) {
        iVar2 = iVar2 + 2;
      }
      if ((uVar1 & 4) != 0) {
        iVar2 = iVar2 + 2;
      }
      puVar3 = auStack_48 + iVar2;
      if ((DAT_8c607f80 & uVar1) == 0) {
        if ((uVar1 & 8) != 0) {
          FUN_8c606f6c(param_1 + 2,puVar3);
        }
      }
      else if ((uVar1 & 8) != 0) {
        FUN_8c606f6c(auStack_54,puVar3);
        param_1[1] = param_1[1] | DAT_8c607f8c;
      }
      iVar4 = iVar4 + 1;
      param_1 = param_1 + 3;
    } while (iVar4 < local_58);
  }
  return;
}

