/* Ghidra draft only; not buildable/verified source. */

undefined8 FUN_8c668b58(undefined4 param_1)

{
  undefined *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  puVar1 = PTR_FUN_8c668c48;
  iVar4 = 0;
  piVar6 = DAT_8c668c44;
  do {
    if (*piVar6 == 0) break;
    iVar4 = iVar4 + 1;
    piVar6 = (int *)((int)piVar6 + (int)DAT_8c668c30);
  } while (iVar4 < 8);
  if (iVar4 == 8) {
    piVar6 = (int *)0x0;
  }
  else {
    iVar4 = (int)DAT_8c668c32;
    *(undefined4 *)(iVar4 + (int)piVar6) = param_1;
    *(undefined4 *)(iVar4 + 4 + (int)piVar6) = 0;
    iVar3 = (int)DAT_8c668c36;
    iVar5 = (int)DAT_8c668c34;
    *(undefined4 *)(iVar4 + -4 + (int)piVar6) = 0;
    (*(code *)puVar1)(iVar3 + (int)piVar6,0,iVar5,
                      (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
    (*(code *)PTR_FUN_8c668c48)(DAT_8c668c38 + 4 + (int)piVar6,0);
    (*(code *)PTR_FUN_8c668c58)((int)DAT_8c668c3a + (int)piVar6);
    piVar2 = DAT_8c668c54;
    *piVar6 = 1;
    *piVar2 = *piVar2 + 1;
  }
  return CONCAT44((int)DAT_8c668c2e,piVar6);
}

