/* Ghidra draft only; not buildable/verified source. */

void FUN_8c605e0a(void)

{
  int *piVar1;
  undefined4 *puVar2;
  int *piVar3;
  uint *puVar4;
  int iVar5;
  
  puVar4 = DAT_8c605ec4;
  puVar2 = DAT_8c605ebc;
  piVar1 = DAT_8c605eb8;
  iVar5 = (int)DAT_8c605eb4;
  *(undefined4 *)(iVar5 + *DAT_8c605eb8) = *DAT_8c605ebc;
  *(undefined4 *)(iVar5 + 4 + *piVar1) = puVar2[1];
  *(undefined4 *)(iVar5 + 8 + *piVar1) = puVar2[2];
  piVar3 = DAT_8c605ec0;
  *(undefined4 *)(iVar5 + 0xc + *piVar1) = puVar2[3];
  *(undefined4 *)*piVar3 = puVar2[4];
  *(undefined4 *)(*piVar3 + 4) = puVar2[7];
  *(undefined4 *)(*piVar3 + 0xc) = puVar2[8];
  if ((*puVar4 & (int)DAT_8c605eb6) == 0) {
    *(undefined4 *)(*piVar3 + 8) = puVar2[5];
    *(undefined4 *)(*piVar3 + 0x10) = puVar2[6];
  }
  else {
    *(undefined4 *)(*piVar3 + 0x10) = puVar2[5];
    *(undefined4 *)(*piVar3 + 8) = puVar2[6];
  }
  return;
}

