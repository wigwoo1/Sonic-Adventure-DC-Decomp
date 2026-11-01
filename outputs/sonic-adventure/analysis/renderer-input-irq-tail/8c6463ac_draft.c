/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6463ac(void)

{
  ushort uVar1;
  int *piVar2;
  int *piVar3;
  uint *puVar4;
  uint *puVar5;
  int *piVar6;
  int iVar7;
  uint uVar8;
  
  piVar3 = DAT_8c6464e0;
  piVar2 = DAT_8c6464d8;
  uVar1 = DAT_8c6464ca;
  if (*DAT_8c6463d0 == 0) {
    iVar7 = 0;
  }
  else {
    uVar8 = (uint)(short)DAT_8c6464ca;
    *DAT_8c6464dc = *DAT_8c6464d8 + -1;
    *piVar2 = 0;
    puVar5 = DAT_8c6464e8;
    puVar4 = DAT_8c6464e4;
    if (*piVar3 != 0) {
      *DAT_8c6464e8 = *DAT_8c6464e4 & uVar8;
      puVar5[1] = (int)(short)((ushort)(*puVar4 >> 0x10) & uVar1);
    }
    piVar2 = DAT_8c6464ec;
    *piVar3 = 0;
    if (*piVar2 != 0) {
      *DAT_8c6464f4 = *DAT_8c6464f0;
      *DAT_8c6464f8 = *DAT_8c6464f8 & 0xfffffff7;
      *piVar2 = 0;
    }
    piVar3 = DAT_8c646504;
    piVar2 = DAT_8c646500;
    puVar4 = DAT_8c6464fc;
    if ((*DAT_8c6464fc & (int)DAT_8c6464cc) == 0) {
      iVar7 = (*(code *)PTR_FUN_8c646618)();
      piVar6 = DAT_8c646620;
      if (iVar7 < 0) {
        FUN_8c646654();
        piVar3[4] = piVar3[4] + 1;
        if (*piVar2 != 0) {
          (*(code *)*piVar2)(1,0xfffffff9,piVar3[2],iVar7);
        }
      }
      else {
        *DAT_8c64661c = 0;
        *(undefined2 *)((int)piVar3 + *piVar6 * 2 + 0x34) = 0;
        *(ushort *)((int)piVar3 + *piVar6 * 2 + 0x2c) = (ushort)*DAT_8c646624 & uVar1;
        FUN_8c6461ae();
        *piVar3 = *piVar3 + 1;
      }
    }
    else {
      if (((*DAT_8c6464fc & (int)DAT_8c6464ce) != 0) &&
         (DAT_8c646504[7] = DAT_8c646504[7] + 1, *piVar2 != 0)) {
        (*(code *)*piVar2)(1,0xfffffffe,piVar3[2],*puVar4);
      }
      if (((*puVar4 & (int)DAT_8c6464d0) != 0) && (piVar3[8] = piVar3[8] + 1, *piVar2 != 0)) {
        (*(code *)*piVar2)(1,0xfffffffd,piVar3[2],*puVar4);
      }
      if (((*puVar4 & (int)DAT_8c6464d2) != 0) && (piVar3[9] = piVar3[9] + 1, *piVar2 != 0)) {
        (*(code *)*piVar2)(1,0xfffffffc,piVar3[2],*puVar4);
      }
      if (((*puVar4 & (int)DAT_8c6464d4) != 0) && (piVar3[10] = piVar3[10] + 1, *piVar2 != 0)) {
        (*(code *)*piVar2)(1,0xfffffffb,piVar3[2],*puVar4);
      }
      *puVar4 = (int)DAT_8c6464cc;
      FUN_8c646654();
      iVar7 = (*(code *)PTR_FUN_8c646508)();
      if (iVar7 < 0) {
        if (*piVar2 != 0) {
          (*(code *)*piVar2)(1,0xfffffffa,piVar3[2],iVar7);
        }
      }
      else {
        FUN_8c6461ae();
      }
    }
  }
  return iVar7;
}

