/* Ghidra draft only; not buildable/verified source. */

undefined1 FUN_8c602998(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = (*(code *)PTR_FUN_8c6029fc)(PTR_DAT_8c6029f8[param_1]);
  iVar2 = (*(code *)PTR_FUN_8c6029fc)(param_1);
  if ((((param_1 == 2) || (param_1 == 8)) || (param_1 == 0xe)) || (param_1 == 0x14)) {
    iVar3 = 1;
  }
  else {
    iVar3 = 0;
  }
  return PTR_DAT_8c602a00[(uint)*(byte *)(iVar1 + 0x12 + iVar3) * 4 + (uint)*(byte *)(iVar2 + 0x12)]
  ;
}

