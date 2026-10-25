/* Ghidra draft only; not buildable/verified source. */

uint callback_8c668f40(int param_1)

{
  undefined *puVar1;
  int iVar2;
  byte in_MD;
  byte bVar3;
  byte in_RB;
  byte bVar4;
  byte in_BL;
  byte bVar5;
  byte in_FD;
  byte bVar6;
  byte in_M;
  byte extraout_M;
  byte extraout_M_00;
  byte extraout_M_01;
  byte extraout_M_02;
  byte bVar7;
  byte in_Q;
  byte extraout_Q;
  byte extraout_Q_00;
  byte extraout_Q_01;
  byte extraout_Q_02;
  byte bVar8;
  byte in_IMASK;
  byte bVar9;
  byte in_S;
  byte extraout_S;
  byte extraout_S_00;
  byte extraout_S_01;
  byte extraout_S_02;
  byte bVar10;
  byte in_T;
  byte extraout_T;
  byte extraout_T_00;
  byte bVar11;
  undefined4 uStack_18;
  undefined4 uStack_14;
  int iStack_10;
  uint uStack_c;
  
  puVar1 = PTR_DAT_8c668fe4;
  uStack_c = (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
  bVar5 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar3 = bVar5 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar9 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar6 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar5 = (bVar5 & 0x10) >> 4;
  bVar4 = (bVar3 & 0x20) >> 5;
  bVar3 = ((bVar3 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  iVar2 = (int)DAT_8c668fdc;
  *(undefined4 *)(param_1 + 4) = 1;
  *(int *)(iVar2 + *(int *)puVar1) = *(int *)(iVar2 + *(int *)puVar1) + 1;
  (*(code *)PTR_FUN_8c668fe8)();
  iVar2 = (int)DAT_8c668fde;
  *(undefined4 *)(iVar2 + *(int *)puVar1) = 0;
  iVar2 = iVar2 + -4;
  *(int *)(iVar2 + *(int *)puVar1) = *(int *)(iVar2 + *(int *)puVar1) + 1;
  (*(code *)PTR_FUN_8c668fec)(&iStack_10);
  bVar11 = 0;
  bVar7 = extraout_M;
  bVar8 = extraout_Q;
  bVar10 = extraout_S;
  if (0 < iStack_10) {
    (*(code *)PTR_FUN_8c668ff0)(&uStack_18,&uStack_14);
    if (*(int *)(*(int *)puVar1 + (int)DAT_8c668fe0) == 0) {
      (*(code *)PTR_FUN_8c668ff4)(uStack_18,uStack_14);
      bVar7 = extraout_M_01;
      bVar8 = extraout_Q_01;
      bVar10 = extraout_S_01;
      bVar11 = extraout_T;
    }
    else if (*(int *)(*(int *)puVar1 + (int)DAT_8c668fe0) == 1) {
      (*(code *)PTR_FUN_8c668ff8)(uStack_18,uStack_14);
      bVar7 = extraout_M_02;
      bVar8 = extraout_Q_02;
      bVar10 = extraout_S_02;
      bVar11 = extraout_T_00;
    }
    else {
      bVar11 = 0;
      bVar7 = extraout_M_00;
      bVar8 = extraout_Q_00;
      bVar10 = extraout_S_00;
    }
  }
  return (uStack_c & 0xf) << 4 |
         ((uint)bVar11 | (uint)bVar10 << 1 | (uint)bVar9 << 4 | (uint)bVar8 << 8 | (uint)bVar7 << 9
          | (uint)bVar6 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar4 << 0x1d | (uint)bVar3 << 0x1e) &
         (int)DAT_8c668fda;
}

