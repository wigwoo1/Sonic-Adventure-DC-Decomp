/* Ghidra draft only; not buildable/verified source. */

undefined4 callback_8c661300(int *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  puVar1 = DAT_8c6615ec;
  iVar3 = *param_1;
  iVar2 = param_1[1];
  iVar4 = param_1[2];
  uVar7 = *DAT_8c6615e0;
  uVar5 = *DAT_8c6615e4;
  uVar6 = *DAT_8c6615e8;
  *DAT_8c6615ec = uVar7;
  puVar1[1] = uVar5;
  puVar1[2] = uVar6;
  puVar1[3] = *(undefined4 *)(iVar3 + 4);
  puVar1[4] = *(undefined4 *)(iVar3 + 8);
  puVar1[5] = *(undefined4 *)(iVar3 + 0xc);
  puVar1[6] = *(undefined4 *)(iVar3 + 0x18);
  puVar1[7] = *(undefined4 *)(iVar2 + 4);
  puVar1[8] = *(undefined4 *)(iVar2 + 8);
  puVar1[9] = *(undefined4 *)(iVar2 + 0xc);
  puVar1[10] = *(undefined4 *)(iVar2 + 0x18);
  puVar1[0xb] = *(undefined4 *)(iVar4 + 4);
  puVar1[0xc] = *(undefined4 *)(iVar4 + 8);
  puVar1[0xd] = *(undefined4 *)(iVar4 + 0xc);
  puVar1[0xe] = *(undefined4 *)(iVar4 + 0x18);
  *DAT_8c6615f0 = 0x3c;
  uVar5 = FUN_8c66118a(uVar7);
  FUN_8c661160(uVar5,0);
  *DAT_8c6615f4 = *(undefined4 *)(iVar3 + 0xc);
  return 0;
}

