/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6087f4(void)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined4 uVar8;
  
  puVar1 = PTR_DAT_8c6088a8;
  uVar8 = 0;
  uVar6 = 0;
  iVar5 = 0;
  iVar4 = 0;
  iVar3 = 0;
  do {
    if (*(uint *)PTR_DAT_8c6088b4 <= uVar6) {
      *(undefined4 *)PTR_DAT_8c6088b8 = 0xffffffff;
      return;
    }
    iVar7 = uVar6 * 0x44;
    iVar2 = *(int *)(*(int *)puVar1 + iVar7 + 0x10);
    if (iVar2 == 2) {
      (*(code *)PTR_FUN_8c6088ac)(*(int *)puVar1 + iVar7 + 0x18,uVar8);
      *(uint *)(*(int *)puVar1 + iVar7 + 0x10) =
           *(uint *)(*(int *)puVar1 + iVar7 + 0x10) & 0xfffffffd;
      if (*(int *)(*(int *)puVar1 + iVar7 + 0x10) == 0) {
LAB_8c60885c:
        (*(code *)PTR_FUN_8c6088b0)(*(int *)puVar1 + iVar7);
      }
      else {
        *(undefined2 *)(*(int *)puVar1 + iVar7 + 0x40) = 0;
        *(undefined4 *)(*(int *)puVar1 + iVar7 + 0x34) = 0;
      }
    }
    else if (iVar2 == 4) goto LAB_8c60885c;
    if ((*(int *)(*(int *)puVar1 + iVar3) != -1) && (*(int *)(*(int *)puVar1 + iVar4 + 0x10) == 0))
    {
      (*(code *)PTR_FUN_8c6088b0)(*(int *)puVar1 + iVar5);
    }
    iVar4 = iVar4 + 0x44;
    iVar3 = iVar3 + 0x44;
    uVar6 = uVar6 + 1;
    iVar5 = iVar5 + 0x44;
  } while( true );
}

