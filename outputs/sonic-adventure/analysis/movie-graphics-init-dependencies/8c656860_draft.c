/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c656860(undefined4 param_1,uint param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  
  *DAT_8c6569a0 = 0;
  iVar4 = *DAT_8c6569a4;
  while ((iVar4 != 0 && ((*(code *)*DAT_8c6569a8)(DAT_8c6569a8[1]), *DAT_8c6569a0 < 0xb))) {
    iVar4 = *DAT_8c6569a4;
  }
  puVar1 = DAT_8c6569ac;
  *DAT_8c6569ac = param_1;
  *DAT_8c6569b0 = param_2;
  *DAT_8c6569b4 = param_2 & 0xf;
  puVar1[2] = 0;
  puVar2 = DAT_8c6569b8;
  *DAT_8c6569b8 = 0xffffffff;
  puVar3 = DAT_8c6569bc;
  *DAT_8c6569bc = 0xffffffff;
  puVar1[5] = 0xffffffff;
  puVar2[3] = 0;
  puVar3[3] = 0;
  puVar1[8] = 0;
  return 0;
}

