/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c603cd2(undefined1 *param_1)

{
  byte bVar1;
  ushort uVar2;
  short sVar3;
  undefined4 uVar4;
  int iVar5;
  byte *pbVar6;
  
  uVar2 = DAT_8c603d9c;
  iVar5 = 0x3c;
  pbVar6 = param_1 + 2;
  do {
    bVar1 = *pbVar6;
    pbVar6 = pbVar6 + 1;
    if (bVar1 != DAT_8c603d9c) goto LAB_8c603d04;
    iVar5 = iVar5 + -1;
  } while (0 < iVar5);
  sVar3 = (*(code *)PTR_FUN_8c603da0)(0x3e,param_1);
  if (*(short *)(param_1 + 0x3e) == sVar3) {
    for (iVar5 = 0x40; iVar5 != 0; iVar5 = iVar5 + -1) {
      *param_1 = (char)uVar2;
      param_1 = param_1 + 1;
    }
    uVar4 = 1;
  }
  else {
LAB_8c603d04:
    uVar4 = 0;
  }
  return uVar4;
}

