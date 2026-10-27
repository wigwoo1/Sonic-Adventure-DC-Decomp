/* Ghidra draft only; not buildable/verified source. */

void FUN_8c668e60(int param_1)

{
  int *piVar1;
  undefined *puVar2;
  undefined *puVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  byte in_FPSCR_RM;
  byte in_FPSCR_FLAG;
  byte in_FPSCR_ENABLE;
  byte in_FPSCR_CAUSE;
  byte in_FPSCR_DN;
  byte in_FPSCR_PR;
  byte in_FPSCR_SZ;
  byte in_FPSCR_FR;
  
  piVar1 = DAT_8c668ea4;
  *DAT_8c668eb4 = *DAT_8c668ea4;
  iVar7 = (int)DAT_8c668e9a;
  *piVar1 = param_1;
  iVar7 = iVar7 + param_1;
  (*(code *)PTR_FUN_8c668eac)
            (param_1 + 4,
             (uint)in_FPSCR_RM | (uint)in_FPSCR_FLAG << 2 | (uint)in_FPSCR_ENABLE << 7 |
             (uint)in_FPSCR_CAUSE << 0xc | (uint)in_FPSCR_DN << 0x12 | (uint)in_FPSCR_PR << 0x13 |
             (uint)in_FPSCR_SZ << 0x14 | (uint)in_FPSCR_FR << 0x15);
  puVar2 = PTR_FUN_8c668eb0;
  for (iVar6 = 0; piVar4 = DAT_8c668f3c, iVar6 < *(int *)(DAT_8c668f34 + param_1); iVar6 = iVar6 + 1
      ) {
    *(int *)(DAT_8c668f2e + param_1) = iVar7;
    puVar3 = PTR_LAB_8c668f38;
    iVar5 = *(int *)(iVar7 + 0xc);
    if (iVar5 != 0) {
      if (iVar5 == 3) {
        FUN_8c668cca(param_1,iVar7);
      }
      else if (iVar5 == 1) {
        if (*(uint *)(iVar7 + 0x14) <= *(uint *)(DAT_8c668f30 + param_1)) {
          (*(code *)puVar2)(iVar7 + 0x18,1);
        }
      }
      else if (iVar5 == 2) {
        *(undefined4 *)(iVar7 + 0xc) = 3;
        *(undefined **)(iVar7 + 0x20) = puVar3;
      }
    }
    iVar7 = iVar7 + DAT_8c668f32;
  }
  *(int *)(DAT_8c668f30 + param_1) = *(int *)(DAT_8c668f30 + param_1) + 1;
  *piVar1 = *piVar4;
  return;
}

