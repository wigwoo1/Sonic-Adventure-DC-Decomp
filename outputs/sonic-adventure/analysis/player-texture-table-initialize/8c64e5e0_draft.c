/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64e5e0(undefined4 param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  puVar2 = PTR_DAT_8c64e814;
  puVar1 = PTR_DAT_8c64e810;
  *(undefined4 *)PTR_DAT_8c64e810 = param_1;
  *(undefined4 *)puVar2 = param_2;
  iVar6 = 0;
  for (uVar4 = 0; puVar3 = DAT_8c64e81c, uVar4 < *(uint *)puVar2; uVar4 = uVar4 + 1) {
    *(undefined4 *)(*(int *)puVar1 + iVar6) = 0xffffffff;
    *(undefined4 *)(*(int *)puVar1 + iVar6 + 4) = 0xffffffff;
    iVar5 = *(int *)puVar1 + iVar6;
    iVar6 = iVar6 + 0x44;
    *(undefined2 *)(iVar5 + 0x40) = 0;
  }
  *(undefined4 *)PTR_DAT_8c64e818 = 0;
  *puVar3 = 0;
  return;
}

