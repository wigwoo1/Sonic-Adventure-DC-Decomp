/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c900100(undefined4 param_1,int param_2)

{
  undefined *puVar1;
  code *pcVar2;
  undefined4 uVar3;
  char cVar4;
  int iVar5;
  undefined1 *puVar6;
  int iVar7;
  undefined8 uVar8;
  char in_FPSCR_SZ;
  
  pcVar2 = DAT_0c9002c4;
  puVar1 = PTR_DAT_0c9002c0;
  puVar6 = *(undefined1 **)(param_2 + 0x20);
  iVar7 = *(int *)(param_2 + 0x24);
  *puVar6 = 0;
  puVar6[1] = 0;
  *(undefined2 *)(puVar6 + 6) = 0;
  (*pcVar2)(param_2,puVar1,3,1,3);
  *(ushort *)(*(int *)(puVar6 + 0x38) + 2) =
       *(ushort *)(*(int *)(puVar6 + 0x38) + 2) & (ushort)DAT_0c9002c8;
  *(undefined4 *)(puVar6 + 0x14) = 0;
  *(undefined4 *)(puVar6 + 0x1c) = 0;
  *(undefined4 *)(iVar7 + 0x7c) = 0;
  *(undefined4 *)(iVar7 + 0x80) = 0;
  *(undefined4 *)(iVar7 + 0x84) = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar7 + 0xe0) = *(undefined4 *)(*(int *)(iVar7 + 0xec) + 0x24);
  }
  else {
    *(undefined8 *)(iVar7 + 0xe0) = *(undefined8 *)(*(int *)(iVar7 + 0xec) + 0x24);
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar7 + 0xdc) = *(undefined4 *)(*(int *)(iVar7 + 0xec) + 0x20);
  }
  else {
    *(undefined8 *)(iVar7 + 0xdc) = *(undefined8 *)(*(int *)(iVar7 + 0xec) + 0x20);
  }
  puVar1 = PTR_callback_candidate_c90bd40_0c9002cc;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar7 + 0xe4) = *(undefined4 *)(*(int *)(iVar7 + 0xec) + 0x28);
  }
  else {
    *(undefined8 *)(iVar7 + 0xe4) = *(undefined8 *)(*(int *)(iVar7 + 0xec) + 0x28);
  }
  puVar6[3] = 0;
  *(undefined2 *)(puVar6 + 6) = 0;
  (*(code *)puVar1)(puVar6,iVar7);
  (*(code *)PTR_FUN_0c9002d0)(puVar6,iVar7);
  (*(code *)PTR_FUN_0c9002d4)(puVar6,iVar7);
  (*(code *)PTR_FUN_0c9002d8)(puVar6,iVar7);
  (*(code *)PTR_callback_candidate_c90853e_0c9002dc)(puVar6,iVar7);
  (*(code *)PTR_FUN_0c9002e0)(puVar6,iVar7);
  (*(code *)PTR_callback_candidate_c904e80_0c9002e4)(puVar6,iVar7);
  (*(code *)PTR_callback_candidate_c9048ec_0c9002e8)(puVar6,iVar7);
  (*(code *)PTR_FUN_0c9002ec)(puVar6,iVar7);
  (*(code *)PTR_FUN_0c9002f0)(puVar6,iVar7);
  (*DAT_0c9002f4)(puVar6,*(undefined4 *)(param_2 + 0x24),4);
  if (in_FPSCR_SZ == '\0') {
    uVar8 = CONCAT44(_DAT_0c9002f8,param_1);
  }
  else {
    uVar8 = CONCAT44(PTR_callback_candidate_c910960_0c9002fc,_DAT_0c9002f8);
  }
  (*(code *)PTR_callback_candidate_c910960_0c9002fc)(uVar8,param_2,0,0);
  uVar3 = DAT_0c900300;
  *(uint *)(iVar7 + 0x60) = *(uint *)(iVar7 + 0x60) & 0xfffffff1;
  *(ushort *)(*(int *)(iVar7 + 0xec) + 0x36) =
       *(ushort *)(*(int *)(iVar7 + 0xec) + 0x36) & (ushort)uVar3;
  *(undefined4 *)(iVar7 + 0x74) = 0;
  *(undefined2 *)(iVar7 + 0x78) = 0;
  *(uint *)(iVar7 + 0x60) = *(uint *)(iVar7 + 0x60) & 0xfffffeff;
  cVar4 = (*DAT_0c900304)();
  if ((cVar4 != -1) && (iVar5 = (*DAT_0c900308)(), *(int *)(iVar7 + 0xec) == iVar5)) {
    (*DAT_0c90030c)(0,param_2);
                    /* WARNING: Could not recover jumptable at 0x0c90023e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*DAT_0c900310)();
    return;
  }
  return;
}

