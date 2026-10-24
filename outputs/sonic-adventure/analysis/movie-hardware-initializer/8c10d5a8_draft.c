/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10d5a8(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  undefined *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  
  puVar2 = PTR_FUN_8c10d74c;
  puVar1 = PTR_DAT_8c10d744;
  iVar9 = (int)DAT_8c10d72a;
  uVar7 = DAT_8c10d748 & 0xffffffe0;
  *(uint *)PTR_DAT_8c10d744 = uVar7;
  (*(code *)puVar2)(uVar7,0,iVar9);
  iVar9 = (int)DAT_8c10d72c;
  iVar10 = 0;
  **(undefined4 **)puVar1 = DAT_8c10d750;
  *(undefined4 *)(iVar9 + *(int *)puVar1) = 1;
  *(undefined4 *)((int)DAT_8c10d72e + *(int *)puVar1) = 0;
  do {
    iVar9 = iVar10 * 0x34;
    *(int *)(*(int *)puVar1 + 4 + iVar9) = iVar10;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x2c) = 0;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x30) = 0;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x34) = 0;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 8) = 0;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0xc) = 0;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x10) = 0;
    puVar2 = PTR_FUN_8c10d754;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x14) = 0;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x18) = 0;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x1c) = 3;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x20) = 0;
    *(undefined4 *)(*(int *)puVar1 + iVar9 + 0x24) = 0;
    (*(code *)puVar2)(*(int *)puVar1 + 4 + iVar9);
    iVar9 = iVar10 * 4;
    iVar10 = iVar10 + 1;
    *(undefined4 *)((int)DAT_8c10d730 + *(int *)puVar1 + iVar9) = 0;
    *(undefined4 *)((int)DAT_8c10d732 + *(int *)puVar1 + iVar9) = 0;
    *(undefined4 *)(iVar9 + (int)DAT_8c10d734 + *(int *)puVar1) = 0;
    uVar6 = DAT_8c10d75c;
  } while (iVar10 < 4);
  iVar9 = (int)DAT_8c10d736;
  *(undefined4 *)(iVar9 + *(int *)puVar1) = 0;
  uVar3 = DAT_8c10d758;
  *(undefined4 *)(iVar9 + 4 + *(int *)puVar1) = 0;
  (*(code *)PTR_FUN_8c10d760)(uVar6,uVar3);
  iVar10 = (int)DAT_8c10d738;
  *(undefined4 *)((int)DAT_8c10d72c + *(int *)puVar1) = 1;
  puVar4 = PTR_FUN_8c10d768;
  puVar2 = PTR_LAB_8c10d764;
  iVar9 = *(int *)puVar1;
  uVar6 = (*(code *)PTR_FUN_8c10d768)(iVar10,PTR_LAB_8c10d764,0x60,iVar9 + 4);
  iVar10 = (int)DAT_8c10d73a;
  *(undefined4 *)(iVar9 + 0x28) = uVar6;
  iVar9 = *(int *)puVar1;
  uVar6 = (*(code *)puVar4)(iVar10,puVar2,0x60,iVar9 + 0x38);
  iVar8 = (int)DAT_8c10d73c;
  *(undefined4 *)(iVar9 + 0x5c) = uVar6;
  iVar10 = *(int *)puVar1;
  uVar6 = (*(code *)puVar4)(iVar8,puVar2,0x60,iVar10 + 0x6c);
  iVar9 = (int)DAT_8c10d73e;
  *(undefined4 *)(iVar10 + 0x90) = uVar6;
  iVar9 = *(int *)puVar1 + 4 + iVar9;
  uVar6 = (*(code *)puVar4)((int)DAT_8c10d740,puVar2,0x60,iVar9);
  puVar5 = DAT_8c10d76c;
  *(undefined4 *)(iVar9 + 0x24) = uVar6;
  *puVar5 = 0;
  return 0;
}

