/* Ghidra draft only; not buildable/verified source. */

void FUN_8c645898(int param_1,undefined4 *param_2,int param_3,int param_4)

{
  undefined1 uVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined1 *puVar4;
  int iVar5;
  undefined1 *puVar6;
  
  iVar5 = *(int *)(param_3 + 8);
  param_2[1] = 0;
  *param_2 = 0;
  iVar3 = DAT_8c6458f4;
  param_2[9] = param_4 + 0x14;
  puVar2 = (undefined1 *)(param_1 * 8 + iVar3);
  param_2[10] = puVar2;
  puVar4 = puVar2 + 2;
  *puVar2 = *(undefined1 *)(iVar5 + 8);
  puVar6 = (undefined1 *)(iVar5 + 10);
  puVar2[1] = *(undefined1 *)(iVar5 + 9);
  iVar3 = 6;
  do {
    uVar1 = *puVar6;
    puVar6 = puVar6 + 1;
    iVar3 = iVar3 + -1;
    *puVar4 = uVar1;
    puVar4 = puVar4 + 1;
  } while (iVar3 != 0);
  return;
}

