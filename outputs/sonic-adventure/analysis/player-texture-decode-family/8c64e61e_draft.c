/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64e61e(int *param_1,undefined4 *param_2,undefined4 param_3,uint param_4,
                 undefined4 param_5)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  
  puVar1 = PTR_DAT_8c64e810;
  uVar2 = *(uint *)PTR_DAT_8c64e818;
  if (*(int *)(uVar2 * 0x44 + *(int *)PTR_DAT_8c64e810) != -1) {
    for (uVar2 = 0;
        (uVar2 < *(uint *)PTR_DAT_8c64e814 &&
        (*(int *)(uVar2 * 0x44 + *(int *)PTR_DAT_8c64e810) != -1)); uVar2 = uVar2 + 1) {
    }
  }
  if (uVar2 == *(uint *)PTR_DAT_8c64e814 - 1) {
    *(undefined4 *)PTR_DAT_8c64e818 = 0;
  }
  else {
    *(uint *)PTR_DAT_8c64e818 = uVar2 + 1;
  }
  iVar3 = uVar2 * 0x44;
  *param_1 = *(int *)puVar1 + iVar3;
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x18) = param_2[1];
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x1c) = param_2[2];
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x20) = param_2[3];
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x24) = param_2[4];
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x28) = param_2[5];
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x2c) = param_2[6];
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x30) = param_2[7];
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x34) = param_2[8];
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 0x14) = *param_2;
  *(undefined4 *)(*(int *)puVar1 + iVar3) = param_3;
  *(undefined4 *)(*(int *)puVar1 + iVar3 + 4) = param_5;
  *(uint *)(*(int *)puVar1 + iVar3 + 0x10) = param_4;
  if ((param_4 & 6) == 0) {
    *(undefined2 *)(*(int *)puVar1 + iVar3 + 0x40) = 0;
  }
  else {
    *(undefined2 *)(*(int *)puVar1 + iVar3 + 0x40) = 1;
  }
  if (*DAT_8c64e81c <= uVar2) {
    *DAT_8c64e81c = *DAT_8c64e81c + 1;
  }
  return;
}

