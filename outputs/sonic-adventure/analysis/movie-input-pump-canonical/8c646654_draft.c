/* Ghidra draft only; not buildable/verified source. */

int FUN_8c646654(void)

{
  int *piVar1;
  int iVar2;
  undefined *puVar3;
  int *piVar4;
  uint *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  code *pcVar11;
  undefined4 *puVar12;
  
  puVar3 = PTR_FUN_8c646738;
  iVar2 = DAT_8c646730;
  piVar1 = DAT_8c64672c;
  *DAT_8c646734 = 1;
  iVar6 = (*(code *)puVar3)(&DAT_a05f6908);
  if (iVar6 == -2) {
    *(undefined4 *)((int)DAT_8c646726 + *piVar1) = 1;
  }
  else {
    if (iVar6 < 0) {
      if ((code *)*DAT_8c646898 != (code *)0x0) {
        (*(code *)*DAT_8c646898)(2,0xfffffff8,*(undefined4 *)(iVar2 + 8),iVar6);
      }
    }
    else {
      (*(code *)PTR_FUN_8c64673c)();
      *(ushort *)(iVar2 + 0x30 + *DAT_8c646740 * 2) =
           ((ushort)*DAT_8c646744 & DAT_8c646728) +
           *(short *)(iVar2 + 0x34 + *DAT_8c646740 * 2) * ((ushort)*DAT_8c646748 & DAT_8c646728);
      FUN_8c6461ae();
      (*(code *)PTR_FUN_8c64674c)();
      iVar7 = (*(code *)PTR_FUN_8c646750)();
      if (iVar7 != 0) {
        if (((code *)*DAT_8c646754 == (code *)0x0) ||
           (iVar7 = (*(code *)*DAT_8c646754)(), iVar7 != 0)) {
          iVar7 = FUN_8c64658a();
          *DAT_8c646758 = iVar7;
        }
        puVar12 = DAT_8c64675c;
        if ((*(int *)((int)DAT_8c64672a + *piVar1) != 0) && (*DAT_8c646758 != 0)) {
          *DAT_8c646760 = *DAT_8c64675c;
          *puVar12 = *(undefined4 *)(*piVar1 + (int)DAT_8c64672a);
          *DAT_8c646764 = *DAT_8c646764 | 8;
          piVar4 = DAT_8c64676c;
          *DAT_8c646768 = 1;
          *piVar4 = *piVar4 + 1;
        }
      }
      puVar12 = DAT_8c646770;
      for (iVar7 = 0; puVar5 = DAT_8c646894, iVar7 < 0x30; iVar7 = iVar7 + 1) {
        if ((puVar12[2] != 0) && (iVar8 = thunk_FUN_8c65f3f4(*puVar12,puVar12[1]), iVar8 != 0)) {
          pcVar11 = (code *)puVar12[2];
          puVar12[2] = 0;
          uVar9 = FUN_8c646868(*puVar12);
          uVar10 = FUN_8c646856(*puVar12);
          (*pcVar11)(*puVar12,puVar12[1],uVar10,uVar9);
        }
        puVar12 = puVar12 + 3;
      }
      *(undefined4 *)((int)DAT_8c64688a + *piVar1) = 0;
      *puVar5 = *puVar5 ^ 1;
      *(int *)(iVar2 + 4) = *(int *)(iVar2 + 4) + 1;
    }
    *DAT_8c64689c = 0;
  }
  return iVar6;
}

