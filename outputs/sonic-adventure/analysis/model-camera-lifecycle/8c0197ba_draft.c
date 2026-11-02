/* Ghidra draft only; not buildable/verified source. */

void original_entry_8c0197ba(int param_1)

{
  char cVar1;
  undefined *puVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  char *pcVar6;
  undefined1 *puVar7;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_14 [4];
  undefined1 auStack_10 [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar7 = auStack_10;
  }
  else {
    puVar7 = auStack_14;
  }
  *(undefined4 *)(puVar7 + -4) = in_PR;
  *(int *)(puVar7 + -8) = param_1;
  pcVar6 = *(char **)(param_1 + 0x20);
  (*(code *)PTR_FUN_8c01985c)();
  piVar3 = DAT_8c019a34;
  puVar2 = PTR_DAT_8c019860;
  cVar1 = *pcVar6;
  if (cVar1 == '\0') {
    *(char **)PTR_DAT_8c019840 = pcVar6;
    *pcVar6 = '\x01';
    *(undefined4 *)puVar2 = 1;
  }
  else if (cVar1 == '\x01') {
    if (*DAT_8c019a20 != -1) {
      (*(code *)PTR_FUN_8c019a24)(*DAT_8c019a20,0);
    }
    FUN_8c019400();
    FUN_8c019f4a(pcVar6);
    FUN_8c01a100(pcVar6);
    *pcVar6 = '\x02';
    *(undefined **)(*(int *)(puVar7 + -8) + 0x14) = PTR_LAB_8c019a28;
    (*(code *)PTR_FUN_8c019a2c)();
  }
  else if (cVar1 == '\x02') {
    *DAT_8c019a30 = *DAT_8c019a30 + 1;
    puVar2 = PTR_FUN_8c019a38;
    if (*piVar3 == 2) {
      *piVar3 = 0;
      (*(code *)puVar2)();
    }
    (*(code *)PTR_FUN_8c019a3c)();
    (*(code *)PTR_FUN_8c019a40)();
    FUN_8c01995e();
    puVar2 = PTR_DAT_8c019a44;
    piVar5 = *(int **)PTR_DAT_8c019a44;
    if (*piVar5 == 0) {
      piVar5[5] = piVar5[5] + 1;
    }
    else {
      piVar5[5] = 0;
      **(undefined4 **)puVar2 = 0;
      if ((*(char *)(*(int *)puVar2 + 9) == '\0') && (*DAT_8c019a48 != 0)) {
        iVar4 = (*(code *)PTR_FUN_8c019a4c)(0);
        *(undefined2 *)(iVar4 + 10) = 0x1e;
      }
    }
    *DAT_8c019a48 = 0;
    FUN_8c019f4a(pcVar6);
    FUN_8c01a100(pcVar6);
    (*(code *)PTR_FUN_8c019a50)();
    (*(code *)PTR_FUN_8c019a58)(pcVar6,*(undefined4 *)PTR_DAT_8c019a54);
    (*(code *)PTR_FUN_8c019a5c)();
    FUN_8c01a5ae();
    if (*piVar3 == 1) {
      *piVar3 = 2;
    }
  }
  *DAT_8c019a60 = *DAT_8c019a60 + 1;
  *DAT_8c019a64 = *DAT_8c019a64 + 1;
  return;
}

