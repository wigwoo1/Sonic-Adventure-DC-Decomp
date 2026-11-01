/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0990de(int param_1)

{
  int *piVar1;
  int *piVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined *puVar5;
  int iVar6;
  undefined *puVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  undefined2 *puVar11;
  
  puVar11 = &DAT_8c19de24;
  piVar9 = *(int **)(PTR_PTR_8c099190 + param_1 * 4);
  if (piVar9 == (int *)0x0) {
    *DAT_8c09918c = 0;
  }
  else {
    iVar8 = piVar9[1];
    *DAT_8c09918c = iVar8;
    if (iVar8 != 0) {
      if (*piVar9 == 0) {
        FUN_8c098ee0(iVar8,&DAT_8c19de24);
      }
      else {
        FUN_8c099750(*piVar9,iVar8);
      }
    }
    puVar5 = PTR_s_CON_REGULAR_E_8c0991a4;
    puVar4 = PTR_FUN_8c0991a0;
    puVar3 = PTR_s_CON_REGULAR_8c09919c;
    piVar2 = DAT_8c099198;
    piVar1 = DAT_8c099194;
    iVar8 = piVar9[3];
    while (iVar8 != 0) {
      piVar10 = piVar9 + 2;
      if (*piVar10 == 0) {
        FUN_8c098ee0(iVar8,puVar11);
      }
      else {
        iVar6 = (*(code *)puVar4)(*piVar10,puVar3);
        if (iVar6 == 0) {
          puVar7 = puVar5;
          if (*piVar2 == 0) {
            puVar7 = (undefined *)*piVar10;
          }
          FUN_8c099750(puVar7,iVar8);
        }
        else {
          FUN_8c099750(*piVar10,iVar8);
        }
      }
      *piVar1 = iVar8;
      iVar8 = piVar9[5];
      piVar9 = piVar10;
    }
    *piVar1 = 0;
  }
  return;
}

