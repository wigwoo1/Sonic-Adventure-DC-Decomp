/* Ghidra draft only; not buildable/verified source. */

undefined8 FUN_8c63a770(undefined8 *param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  undefined8 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined8 uVar10;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  char in_FPSCR_SZ;
  undefined4 uVar6;
  undefined4 uVar9;
  undefined4 uVar11;
  
  if (param_1 != (undefined8 *)0x0) {
    if (in_FPSCR_SZ == '\0') {
      uVar3 = *(undefined4 *)param_2;
      uVar1 = *(undefined4 *)((int)param_2 + 4);
      uVar6 = *(undefined4 *)(param_2 + 1);
      uVar4 = *(undefined4 *)((int)param_2 + 0xc);
      uVar9 = *(undefined4 *)(param_2 + 2);
      uVar7 = *(undefined4 *)((int)param_2 + 0x14);
      uVar11 = *(undefined4 *)(param_2 + 3);
      *(undefined4 *)((int)param_1 + 0x1c) = *(undefined4 *)((int)param_2 + 0x1c);
      *(undefined4 *)(param_1 + 3) = uVar11;
      *(undefined4 *)((int)param_1 + 0x14) = uVar7;
      *(undefined4 *)(param_1 + 2) = uVar9;
      *(undefined4 *)((int)param_1 + 0xc) = uVar4;
      *(undefined4 *)(param_1 + 1) = uVar6;
      *(undefined4 *)((int)param_1 + 4) = uVar1;
      *(undefined4 *)param_1 = uVar3;
      uVar3 = *(undefined4 *)(param_2 + 4);
      uVar1 = *(undefined4 *)((int)param_2 + 0x24);
      uVar2 = CONCAT44(uVar3,uVar1);
      uVar6 = *(undefined4 *)(param_2 + 5);
      uVar4 = *(undefined4 *)((int)param_2 + 0x2c);
      uVar9 = *(undefined4 *)(param_2 + 6);
      uVar7 = *(undefined4 *)((int)param_2 + 0x34);
      uVar11 = *(undefined4 *)(param_2 + 7);
      *(undefined4 *)((int)param_1 + 0x3c) = *(undefined4 *)((int)param_2 + 0x3c);
      *(undefined4 *)(param_1 + 7) = uVar11;
      *(undefined4 *)((int)param_1 + 0x34) = uVar7;
      *(undefined4 *)(param_1 + 6) = uVar9;
      *(undefined4 *)((int)param_1 + 0x2c) = uVar4;
      *(undefined4 *)(param_1 + 5) = uVar6;
      *(undefined4 *)((int)param_1 + 0x24) = uVar1;
      *(undefined4 *)(param_1 + 4) = uVar3;
    }
    else {
      uVar2 = *param_2;
      uVar12 = param_2[1];
      uVar5 = param_2[2];
      uVar13 = param_2[3];
      uVar8 = param_2[4];
      uVar14 = param_2[5];
      uVar10 = param_2[6];
      param_1[3] = param_2[7];
      param_1[2] = uVar10;
      param_1[1] = uVar14;
      *param_1 = uVar8;
      param_1[-1] = uVar13;
      param_1[-2] = uVar5;
      param_1[-3] = uVar12;
      param_1[-4] = uVar2;
      uVar2 = param_2[8];
      uVar12 = param_2[9];
      uVar5 = param_2[10];
      uVar13 = param_2[0xb];
      uVar8 = param_2[0xc];
      uVar14 = param_2[0xd];
      uVar10 = param_2[0xe];
      param_1[3] = param_2[0xf];
      param_1[2] = uVar10;
      param_1[1] = uVar14;
      *param_1 = uVar8;
      param_1[-1] = uVar13;
      param_1[-2] = uVar5;
      param_1[-3] = uVar12;
      param_1[-4] = uVar2;
    }
    return uVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar2 = CONCAT44(*(undefined4 *)param_2,*(undefined4 *)((int)param_2 + 4));
  }
  else {
    uVar2 = *param_2;
  }
  return uVar2;
}

