/* Ghidra draft only; not buildable/verified source. */

int FUN_0c90451c(int param_1)

{
  code *pcVar1;
  code *pcVar2;
  code *pcVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int unaff_r11;
  int *piVar8;
  int iVar9;
  
  pcVar1 = DAT_0c904640;
  piVar8 = *(int **)(param_1 + 4);
  if (param_1 != 0) {
    unaff_r11 = (*DAT_0c904640)(0x34,&PTR_DAT_0c91c3d0);
    pcVar2 = DAT_0c904644;
    if (unaff_r11 == 0) {
      return 0;
    }
    (*DAT_0c904644)(unaff_r11,param_1,0x34);
    pcVar3 = DAT_0c904648;
    if (piVar8 != (int *)0x0) {
      piVar4 = (int *)(*pcVar1)(0x28);
      if (piVar4 == (int *)0x0) {
        (*pcVar3)(unaff_r11);
        return 0;
      }
      (*pcVar2)(piVar4,piVar8,0x28);
      *(int **)(unaff_r11 + 4) = piVar4;
      if (*piVar8 != 0) {
        iVar5 = (*pcVar1)(piVar8[2] * 0xc);
        *piVar4 = iVar5;
        if (iVar5 == 0) {
          (*pcVar3)(piVar4);
          (*pcVar3)(unaff_r11);
          return 0;
        }
        (*pcVar2)(*piVar4,*piVar8,piVar8[2] * 0xc);
      }
      if (piVar8[1] != 0) {
        iVar5 = (*pcVar1)(piVar8[2] * 0xc);
        piVar4[1] = iVar5;
        if (iVar5 == 0) {
          if (*piVar4 != 0) {
            (*pcVar3)(*piVar4);
          }
          (*pcVar3)(piVar4);
          (*pcVar3)(unaff_r11);
          return 0;
        }
        (*pcVar2)(piVar4[1],piVar8[1],piVar8[2] * 0xc);
      }
      if (piVar8[4] != 0) {
        iVar5 = (*pcVar1)((uint)*(ushort *)((int)piVar8 + 0x16) * 0x14);
        piVar4[4] = iVar5;
        if (iVar5 == 0) {
          if (*piVar4 != 0) {
            (*pcVar3)(*piVar4);
          }
          if (piVar4[1] != 0) {
            (*pcVar3)(piVar4[1]);
          }
          (*pcVar3)(piVar4);
          (*pcVar3)(unaff_r11);
          return 0;
        }
        (*pcVar2)(piVar4[4],piVar8[4],(uint)*(ushort *)((int)piVar8 + 0x16) * 0x14);
      }
      if (piVar8[3] != 0) {
        iVar5 = (*pcVar1)((uint)*(ushort *)(piVar8 + 5) * 0x18);
        piVar4[3] = iVar5;
        if (iVar5 == 0) {
          if (*piVar4 != 0) {
            (*pcVar3)(*piVar4);
          }
          if (piVar4[1] != 0) {
            (*pcVar3)(piVar4[1]);
          }
          if (piVar4[4] != 0) {
            (*pcVar3)(piVar4[4]);
          }
          (*pcVar3)(piVar4);
          (*pcVar3)(unaff_r11);
          return 0;
        }
        (*pcVar2)(piVar4[3],piVar8[3],(uint)*(ushort *)(piVar8 + 5) * 0x18);
        iVar5 = piVar4[3];
        iVar9 = piVar8[3];
        if (*(int *)(iVar9 + 0xc) != 0) {
          iVar6 = (*pcVar1)((uint)*(ushort *)(iVar9 + 2) * 0xc);
          *(int *)(iVar5 + 0xc) = iVar6;
          if (iVar6 == 0) {
            if (*piVar4 != 0) {
              (*pcVar3)(*piVar4);
            }
            if (piVar4[1] != 0) {
              (*pcVar3)(piVar4[1]);
            }
            if ((piVar4[4] != 0) && ((*pcVar3)(piVar4[4]), piVar4[4] != 0)) {
              (*pcVar3)(piVar4[3]);
            }
            (*pcVar3)(piVar4);
            (*pcVar3)(unaff_r11);
            return 0;
          }
          (*pcVar2)(*(undefined4 *)(iVar5 + 0xc),*(undefined4 *)(iVar9 + 0xc),
                    (uint)*(ushort *)(iVar9 + 2) * 0xc);
        }
      }
    }
  }
  if (*(int *)(param_1 + 0x2c) != 0) {
    uVar7 = FUN_0c90451c(*(undefined4 *)(param_1 + 0x2c));
    *(undefined4 *)(unaff_r11 + 0x2c) = uVar7;
  }
  if (*(int *)(param_1 + 0x30) != 0) {
    uVar7 = FUN_0c90451c(*(undefined4 *)(param_1 + 0x30));
    *(undefined4 *)(unaff_r11 + 0x30) = uVar7;
  }
  return unaff_r11;
}

