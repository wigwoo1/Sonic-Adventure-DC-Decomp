/* Ghidra draft only; not buildable/verified source. */

void FUN_8c601a34(undefined4 param_1,int param_2,int param_3)

{
  undefined2 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined *puVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  
  iVar5 = DAT_8c601ae8;
  puVar3 = DAT_8c601ae0;
  puVar2 = DAT_8c601adc;
  *DAT_8c601ad8 = param_1;
  puVar4 = DAT_8c601ae4;
  *puVar2 = 0;
  puVar6 = PTR_FUN_8c601aec;
  iVar9 = (int)DAT_8c601ad2;
  *puVar3 = 1;
  *puVar4 = 0;
  (*(code *)puVar6)(iVar5,0,iVar9,FUN_8c010b58);
  if ((param_3 != 0) && (param_2 != 0)) {
    *DAT_8c601adc = 1;
    (*(code *)PTR_FUN_8c601af0)(param_2,param_3);
  }
  iVar9 = DAT_8c601af4;
  iVar11 = 0;
  do {
    iVar10 = iVar11 * 0x34 + iVar5;
    uVar8 = (*(code *)PTR_FUN_8c601af8)(iVar11);
    *(undefined4 *)(iVar10 + 0x30) = uVar8;
    FUN_8c601bb2(iVar10);
    *(undefined4 *)(iVar11 * 4 + iVar9) = 0;
    *(undefined1 *)(DAT_8c601afc + iVar11) = 0;
    *(undefined1 *)(DAT_8c601b00 + iVar11) = 0;
    puVar7 = DAT_8c601b08;
    uVar1 = DAT_8c601ad6;
    iVar11 = iVar11 + 1;
  } while (iVar11 < 0x18);
  *DAT_8c601b04 = (char)DAT_8c601ad4;
  *puVar7 = (char)uVar1;
  return;
}

