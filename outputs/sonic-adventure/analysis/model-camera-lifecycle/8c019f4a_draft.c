/* Ghidra draft only; not buildable/verified source. */

void FUN_8c019f4a(undefined4 *param_1)

{
  char cVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  undefined *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int iVar7;
  undefined4 in_fr3;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  puVar3 = DAT_8c01a0c4;
  puVar2 = PTR_DAT_8c01a0c0;
  if (*(int *)(*(int *)PTR_DAT_8c01a0c0 + 0xc) != 0) {
    uVar6 = param_1[1];
    *DAT_8c01a0c4 = *param_1;
    puVar3[1] = uVar6;
    uVar6 = param_1[3];
    puVar3[2] = param_1[2];
    puVar3[3] = uVar6;
    uVar6 = param_1[5];
    puVar3[4] = param_1[4];
    puVar3[5] = uVar6;
    uVar6 = param_1[7];
    puVar3[6] = param_1[6];
    puVar3[7] = uVar6;
    uVar6 = param_1[9];
    puVar3[8] = param_1[8];
    puVar3[9] = uVar6;
    uVar6 = param_1[0xb];
    puVar3[10] = param_1[10];
    puVar3[0xb] = uVar6;
    uVar6 = param_1[0xd];
    puVar3[0xc] = param_1[0xc];
    puVar3[0xd] = uVar6;
    uVar6 = param_1[0xf];
    puVar3[0xe] = param_1[0xe];
    puVar4 = PTR_DAT_8c01a0c8;
    puVar3[0xf] = uVar6;
    if (in_FPSCR_SZ == '\0') {
      in_fr3 = param_1[8];
    }
    else {
      in_xd2 = *(undefined8 *)(param_1 + 8);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar4 + 0xc) = in_fr3;
      in_fr3 = param_1[9];
    }
    else {
      *(undefined8 *)(*(int *)puVar4 + 0xc) = in_xd2;
      in_xd2 = *(undefined8 *)(param_1 + 9);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar4 + 0x10) = in_fr3;
      in_fr3 = param_1[10];
    }
    else {
      *(undefined8 *)(*(int *)puVar4 + 0x10) = in_xd2;
      in_xd2 = *(undefined8 *)(param_1 + 10);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar4 + 0x14) = in_fr3;
    }
    else {
      *(undefined8 *)(*(int *)puVar4 + 0x14) = in_xd2;
    }
    puVar5 = DAT_8c01a0d4;
    puVar3 = DAT_8c01a0cc;
    iVar7 = *(int *)puVar2;
    if (*(char *)(iVar7 + 8) == '\x05') {
      (*(code *)PTR_FUN_8c01a0d0)(DAT_8c01a0cc);
      (**(code **)(*(int *)puVar2 + 0xc))(DAT_8c01a0cc);
    }
    else if ((*(byte *)(iVar7 + 0xb) & 2) == 0) {
      (*(code *)PTR_FUN_8c01a0dc)(*DAT_8c01a0d8 + *(short *)(iVar7 + 4) * 0x40,&DAT_8c6b706c);
      (**(code **)(*(int *)puVar2 + 0xc))(DAT_8c01a0cc);
    }
    else {
      uVar6 = DAT_8c01a0d4[1];
      *DAT_8c01a0cc = *DAT_8c01a0d4;
      puVar3[1] = uVar6;
      uVar6 = puVar5[3];
      puVar3[2] = puVar5[2];
      puVar3[3] = uVar6;
      uVar6 = puVar5[5];
      puVar3[4] = puVar5[4];
      puVar3[5] = uVar6;
      uVar6 = puVar5[7];
      puVar3[6] = puVar5[6];
      puVar3[7] = uVar6;
      puVar3[8] = puVar5[8];
      puVar3[8] = *(undefined4 *)(*(int *)puVar2 + 0x14);
      (**(code **)(*(int *)puVar2 + 0xc))();
    }
    cVar1 = *(char *)(*(int *)puVar2 + 7);
    if (cVar1 == '\x03') {
      FUN_8c01a60c(param_1);
    }
    else if (cVar1 == '\x02') {
      FUN_8c01a680(param_1);
    }
    else if (cVar1 == '\x01') {
      FUN_8c01a718(param_1);
    }
    iVar7 = *(int *)puVar2;
    if (((*(int *)(iVar7 + 0x14) != 0) || ((*(byte *)(iVar7 + 0xb) & 1) == 0)) &&
       (0x3c < *DAT_8c01a0e0)) {
      (**(code **)(iVar7 + 0x10))
                (param_1,*(undefined4 *)PTR_DAT_8c01a0e8,*(undefined4 *)PTR_DAT_8c01a0e4);
      *(int *)(*(int *)PTR_DAT_8c01a0e4 + 0x14) = *(int *)(*(int *)PTR_DAT_8c01a0e4 + 0x14) + 1;
    }
  }
  return;
}

