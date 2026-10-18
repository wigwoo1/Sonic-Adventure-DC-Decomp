/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0377d4) */

void FUN_8c0376d0(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  int iVar5;
  uint uVar6;
  ushort *puVar7;
  ushort *extraout_r2;
  ushort *extraout_r2_00;
  ushort *extraout_r2_01;
  ushort *extraout_r2_02;
  int extraout_r2_03;
  int iVar8;
  int extraout_r3;
  int unaff_r13;
  int in_GBR;
  
  puVar7 = *(ushort **)(param_1 + 0xc);
  iVar8 = *(int *)(param_1 + 0x10);
  iVar5 = (int)*(short *)(param_1 + 0x14);
  do {
    iVar8 = (uint)(*puVar7 & DAT_8c03772c) * 0x14 + iVar8;
    uVar6 = FUN_8c037618();
    *(uint *)(in_GBR + 0x2c) = uVar6;
    if ((uVar6 & DAT_8c037734) == 0) {
      *(undefined4 *)(in_GBR + 0x38) = *(undefined4 *)(in_GBR + 0x40);
      if ((*(uint *)(in_GBR + 0x2c) & DAT_8c0377b4) == 0) {
        FUN_8c03767c();
        FUN_8c037460();
        puVar7 = extraout_r2_02;
      }
      else {
        FUN_8c037872();
        FUN_8c03767c();
        FUN_8c0374d4();
        puVar7 = extraout_r2_01;
      }
    }
    else {
      FUN_8c037638();
      if ((*(uint *)(in_GBR + 0x2c) & DAT_8c03773c) == 0) {
        if ((*(uint *)(in_GBR + 0x2c) & DAT_8c037770) == 0) {
          *(undefined4 *)(in_GBR + 0x38) = *(undefined4 *)(in_GBR + 0x40);
        }
        else {
          *(undefined **)(in_GBR + 0x38) = PTR_DAT_8c037774;
        }
        FUN_8c03767c();
        FUN_8c037538();
        puVar7 = extraout_r2_00;
      }
      else {
        FUN_8c037872();
        if ((*(uint *)(in_GBR + 0x2c) & DAT_8c037740) == 0) {
          *(undefined4 *)(in_GBR + 0x38) = *(undefined4 *)(in_GBR + 0x40);
        }
        else {
          *(undefined **)(in_GBR + 0x38) = PTR_DAT_8c037744;
        }
        FUN_8c03767c();
        FUN_8c0375b0();
        puVar7 = extraout_r2;
      }
    }
    puVar4 = PTR_FUN_8c037868;
    puVar3 = PTR_FUN_8c037844;
    puVar2 = PTR_FUN_8c037820;
    puVar1 = PTR_FUN_8c037808;
    uVar6 = ((uint)*puVar7 & (int)DAT_8c0377de) >> 0xc;
    if (unaff_r13 == 0) {
      if ((*(uint *)(iVar8 + 0x10) & DAT_8c037800) == 0) {
        *(undefined4 *)(in_GBR + 0x54) = *(undefined4 *)(PTR_DAT_8c03781c + uVar6);
        (*(code *)puVar2)();
      }
      else {
        *(undefined4 *)(in_GBR + 0x54) = *(undefined4 *)(PTR_DAT_8c037804 + uVar6);
        (*(code *)puVar1)();
      }
    }
    else if ((*(uint *)(iVar8 + 0x10) & DAT_8c03783c) == 0) {
      *(undefined4 *)(in_GBR + 0x54) = *(undefined4 *)(PTR_DAT_8c037864 + uVar6);
      (*(code *)puVar4)();
    }
    else {
      *(undefined4 *)(in_GBR + 0x54) = *(undefined4 *)(PTR_DAT_8c037840 + uVar6);
      (*(code *)puVar3)();
    }
    FUN_8c0376b0();
    puVar7 = (ushort *)(extraout_r2_03 + 0x18);
    iVar5 = iVar5 + -1;
    iVar8 = extraout_r3;
  } while (iVar5 != 0);
  return;
}

