/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6088bc(int *param_1)

{
  uint *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined1 *puVar6;
  
  puVar2 = PTR_FUN_8c608910;
  puVar1 = DAT_8c60890c;
  puVar6 = &DAT_8c7997c5;
  uVar4 = 1;
  for (uVar5 = 0; uVar5 < (uint)param_1[1]; uVar5 = uVar5 + 1) {
    iVar3 = (*(code *)puVar2)(*(undefined4 *)(uVar5 * 0xc + *param_1 + 8),puVar6);
    if (iVar3 < 0) {
      uVar4 = 0xffffffff;
      *puVar1 = uVar5;
    }
  }
  return uVar4;
}

