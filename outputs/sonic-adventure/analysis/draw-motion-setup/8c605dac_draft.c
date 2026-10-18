/* Ghidra draft only; not buildable/verified source. */

void FUN_8c605dac(void)

{
  int *piVar1;
  undefined4 *puVar2;
  int *piVar3;
  uint *puVar4;
  int iVar5;
  uint uVar6;
  
  puVar4 = DAT_8c605ec4;
  puVar2 = DAT_8c605ebc;
  piVar1 = DAT_8c605eb8;
  iVar5 = (int)DAT_8c605eb4;
  *DAT_8c605ebc = *(undefined4 *)(iVar5 + *DAT_8c605eb8);
  puVar2[1] = *(undefined4 *)(iVar5 + 4 + *piVar1);
  puVar2[2] = *(undefined4 *)(iVar5 + 8 + *piVar1);
  piVar3 = DAT_8c605ec0;
  puVar2[3] = *(undefined4 *)(iVar5 + 0xc + *piVar1);
  puVar2[4] = *(undefined4 *)*piVar3;
  puVar2[7] = *(undefined4 *)(*piVar3 + 4);
  uVar6 = (uint)DAT_8c605eb6;
  puVar2[8] = *(undefined4 *)(*piVar3 + 0xc);
  if ((*puVar4 & uVar6) == 0) {
    puVar2[5] = *(undefined4 *)(*piVar3 + 8);
    puVar2[6] = *(undefined4 *)(*piVar3 + 0x10);
  }
  else {
    puVar2[5] = *(undefined4 *)(*piVar3 + 0x10);
    puVar2[6] = *(undefined4 *)(*piVar3 + 8);
  }
  return;
}

