/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c603b84(void)

{
  byte *pbVar1;
  undefined *puVar2;
  undefined4 uVar3;
  int iVar4;
  byte local_c;
  byte local_b;
  
  uVar3 = (*(code *)PTR_FUN_8c603bd8)();
  iVar4 = (*(code *)PTR_FUN_8c603be0)(DAT_8c603bdc,&local_c,2);
  pbVar1 = DAT_8c603be4;
  if ((iVar4 == 0) && ((local_c != DAT_8c603bd4 || (local_b != DAT_8c603bd4)))) {
    *DAT_8c603be4 = local_c;
    puVar2 = PTR_FUN_8c603be8;
    pbVar1[1] = local_b;
    (*(code *)puVar2)(DAT_8c603be4,2);
  }
  return uVar3;
}

