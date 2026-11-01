/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09e778(int param_1,int param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  uint *puVar3;
  undefined4 extraout_r2;
  code *pcVar4;
  undefined4 uVar5;
  undefined *puVar6;
  
  if (*DAT_8c09e784 == 0) {
    (*(code *)PTR_FUN_8c09e800)();
    iVar2 = DAT_8c09e808;
    puVar3 = DAT_8c09e804;
    if ((param_3 & 0x20) == 0) {
      puVar3 = DAT_8c09e804 + -1;
    }
    uVar1 = *puVar3 >> 0x18 & 0x1c;
    *(uint *)(DAT_8c09e808 + -4) = uVar1;
    *(uint *)(iVar2 + -8) = uVar1;
    pcVar4 = (code *)PTR_FUN_8c09e814;
    if ((param_3 & 0x10) == 0) {
      pcVar4 = (code *)PTR_FUN_8c09e818;
    }
    iVar2 = (*pcVar4)(param_1,param_2,param_3);
    if (iVar2 != 0) {
      (*(code *)PTR_FUN_8c09e81c)(*(undefined4 *)(param_1 + 0x18));
      (*(code *)PTR_FUN_8c09e820)((int)*(short *)(*(int *)(param_1 + 0x1c) + param_2 * 0x14 + 0x10))
      ;
      uVar5 = DAT_8c09e860;
      pcVar4 = (code *)PTR_FUN_8c09e864;
      if ((DAT_8c09e824 & param_3) != 0) {
        uVar5 = DAT_8c09e828;
        pcVar4 = (code *)PTR_FUN_8c09e82c;
      }
      (*(code *)PTR_FUN_8c09e868)(uVar5);
      puVar6 = DAT_8c09e870;
      if ((DAT_8c09e86c & param_3) == 0) {
        puVar6 = PTR_DAT_8c09e874;
      }
      (*pcVar4)(puVar6);
      (*(code *)PTR_FUN_8c09e878)(extraout_r2);
    }
    (*(code *)PTR_FUN_8c09e87c)();
    return;
  }
  return;
}

