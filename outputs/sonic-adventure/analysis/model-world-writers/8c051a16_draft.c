/* Ghidra draft only; not buildable/verified source. */

void FUN_8c051a16(void)

{
  undefined4 *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined8 *puVar6;
  undefined2 *puVar7;
  undefined2 *puVar8;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  puVar3 = PTR_DAT_8c051b20;
  puVar2 = PTR_DAT_8c051b1c;
  *DAT_8c051b18 = 0;
  puVar4 = DAT_8c051b24;
  *puVar2 = 0;
  *puVar3 = 0;
  puVar5 = DAT_8c051b28;
  puVar1 = DAT_8c051b00;
  *puVar4 = 0;
  puVar6 = DAT_8c051b2c;
  *puVar1 = 0;
  *puVar5 = 0;
  puVar3 = PTR_DAT_8c051b34;
  puVar2 = PTR_DAT_8c051b30;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)puVar6 = 0;
  }
  else {
    *puVar6 = in_xd2;
  }
  puVar7 = DAT_8c051b38;
  *(undefined4 *)puVar2 = 1;
  *(undefined4 *)puVar3 = 1;
  puVar2 = PTR_DAT_8c051b40;
  puVar8 = DAT_8c051b3c;
  *puVar7 = 0;
  puVar3 = PTR_DAT_8c051b44;
  *puVar8 = 0;
  *(undefined4 *)puVar2 = 0;
  *(undefined4 *)puVar3 = 0;
  return;
}

