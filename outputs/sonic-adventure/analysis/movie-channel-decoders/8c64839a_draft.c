/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64839a(int param_1)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = param_1 * DAT_8c648490 + DAT_8c648494;
  *(int *)(iVar5 + 0x1c) = param_1 * 0x4c + DAT_8c648498;
  *(undefined4 *)(iVar5 + 0x30) = 1;
  *(undefined4 *)(iVar5 + 0x34) = *(undefined4 *)(PTR_DAT_8c64849c + *(int *)(iVar5 + 4) * 0x50);
  puVar1 = PTR_FUN_8c6484a4;
  iVar3 = param_1 * 0x80 + DAT_8c6484a0;
  iVar4 = *(int *)(iVar5 + 0x30);
  *(int *)(iVar5 + 0x30) = iVar4 + 1;
  (*(code *)puVar1)(param_1,*(undefined4 *)(iVar3 + 8),iVar4,*(undefined4 *)(iVar5 + 0x34));
  iVar3 = (*(code *)PTR_FUN_8c6484a8)(param_1,*(undefined4 *)(iVar5 + 0x1c));
  if (iVar3 < 0) {
    *(undefined1 *)(iVar5 + 9) = 0;
    *(undefined1 *)(iVar5 + 8) = 0;
LAB_8c64842a:
    iVar3 = (int)DAT_8c648492;
  }
  else {
    iVar3 = (*(code *)PTR_FUN_8c6484ac)(iVar5);
    puVar1 = PTR_FUN_8c6484b4;
    uVar2 = DAT_8c6484b0;
    if (-1 < iVar3) {
      *(undefined4 *)(*(int *)(iVar5 + 0x1c) + 0x30) =
           *(undefined4 *)(*(int *)(iVar5 + 0x1c) + 0x1c);
      iVar3 = (*(code *)puVar1)(param_1,0,uVar2);
      if (iVar3 < 0) goto LAB_8c64842a;
      *(undefined1 *)(iVar5 + 9) = 1;
      *(undefined1 *)(iVar5 + 10) = 0;
      *(undefined1 *)(iVar5 + 0xb) = 0;
      *(undefined1 *)(iVar5 + 0xc) = 0;
      iVar3 = *(int *)(iVar5 + 4);
      if (iVar3 == 0) {
        return 0xf;
      }
      if (iVar3 == 1) {
        return 0x21;
      }
      if (iVar3 == 2) {
        return 0x41;
      }
      if (iVar3 == 3) {
        return 0x7a;
      }
    }
    iVar3 = -0x7d;
  }
  return iVar3;
}

