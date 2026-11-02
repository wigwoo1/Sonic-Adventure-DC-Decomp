/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0195a8) */

void FUN_8c01957e(void)

{
  undefined4 *puVar1;
  byte *pbVar2;
  ushort *puVar3;
  undefined *puVar4;
  undefined2 *puVar5;
  undefined4 *puVar6;
  undefined2 uVar7;
  int iVar8;
  int *piVar9;
  
  iVar8 = (*(code *)PTR_FUN_8c01967c)(2,0,PTR_original_entry_8c0197ba_8c019678);
  pbVar2 = DAT_8c019680;
  uVar7 = (*(code *)PTR_FUN_8c019684)();
  puVar4 = PTR_DAT_8c01968c;
  puVar3 = DAT_8c019688;
  *(undefined2 *)pbVar2 = uVar7;
  puVar1 = DAT_8c01966c;
  *puVar3 = (ushort)*pbVar2;
  puVar5 = DAT_8c019690;
  *(short *)pbVar2 = (short)(char)((ushort)*(undefined2 *)pbVar2 >> 8);
  *puVar5 = *(undefined2 *)puVar4;
  puVar5 = DAT_8c019694;
  *puVar1 = *(undefined4 *)(puVar4 + 4);
  *puVar5 = *(undefined2 *)(puVar4 + 2);
  *DAT_8c019670 = *(undefined4 *)(puVar4 + 8);
  piVar9 = (int *)(PTR_DAT_8c019698 + (short)*puVar3 * 4 + *(short *)pbVar2 * 0x20);
  *DAT_8c01969c = *piVar9;
  *DAT_8c019668 = *piVar9 + 0x40;
  FUN_8c01951e();
  puVar4 = PTR_original_entry_8c019768_8c0196a4;
  *(undefined **)(iVar8 + 0x18) = PTR_original_entry_8c019760_8c0196a0;
  puVar1 = DAT_8c0196a8;
  *(undefined **)(iVar8 + 0x14) = puVar4;
  puVar6 = DAT_8c0196ac;
  *puVar1 = 0;
  puVar1 = DAT_8c0196b4;
  *puVar6 = 0xffffffff;
  puVar4 = PTR_FUN_8c0196b8;
  *DAT_8c0196b0 = 0xffffffff;
  *puVar1 = 0x31c7;
  (*(code *)puVar4)(0x31c7);
  return;
}

