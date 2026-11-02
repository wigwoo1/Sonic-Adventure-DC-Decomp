/* Ghidra draft only; not buildable/verified source. */

void FUN_8c051f64(undefined8 param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  undefined8 *puVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  undefined4 in_fr15;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined1 local_18 [4];
  undefined1 local_14 [4];
  
  puVar3 = PTR_FUN_8c052100;
  puVar2 = PTR_FUN_8c05200c;
  puVar1 = PTR_FUN_8c051ff4;
  if (in_FPSCR_SZ == '\0') {
    puVar7 = local_14;
  }
  else {
    puVar7 = local_18;
  }
  *(undefined4 *)(puVar7 + -4) = in_PR;
  iVar5 = *DAT_8c051ff0;
  if ((*(ushort *)(iVar5 + 4) & 1) != 0) {
    puVar6 = *(undefined8 **)(iVar5 + 0x10);
    iVar4 = (int)*(short *)(iVar5 + 2);
    if ((*(ushort *)(iVar5 + 4) & 0x10) == 0) {
      for (; 0 < iVar4; iVar4 = iVar4 + -1) {
        uVar8 = (undefined4)param_1;
        if (in_FPSCR_SZ == '\0') {
          in_fr15 = *(undefined4 *)puVar6;
        }
        else {
          in_xd14 = *puVar6;
        }
        (*(code *)puVar1)(*(undefined4 *)((int)puVar6 + 0x14));
        param_1 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          param_1 = CONCAT44(in_fr15,uVar8);
        }
        (*(code *)puVar3)(param_1,*(undefined4 *)(puVar6 + 2));
        puVar6 = puVar6 + 3;
      }
    }
    else {
      for (; 0 < iVar4; iVar4 = iVar4 + -1) {
        uVar8 = (undefined4)param_1;
        if (in_FPSCR_SZ == '\0') {
          in_fr15 = *(undefined4 *)puVar6;
        }
        else {
          in_xd14 = *puVar6;
        }
        (*(code *)puVar1)(*(undefined4 *)((int)puVar6 + 0x14));
        param_1 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          param_1 = CONCAT44(in_fr15,uVar8);
        }
        (*(code *)puVar2)(param_1,*(undefined4 *)((int)puVar6 + 0xc),*(undefined4 *)(puVar6 + 2));
        puVar6 = puVar6 + 3;
      }
    }
  }
  return;
}

