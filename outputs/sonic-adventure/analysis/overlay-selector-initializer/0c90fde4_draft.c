/* Ghidra draft only; not buildable/verified source. */

void FUN_0c90fde4(int param_1)

{
  undefined *puVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  int iVar4;
  char in_FPSCR_SZ;
  
  puVar3 = *(undefined1 **)(param_1 + 0x20);
  iVar4 = *(int *)(param_1 + 0x24);
  *puVar3 = 0;
  puVar3[1] = 0;
  *(undefined2 *)(puVar3 + 6) = 0;
  *(undefined4 *)(puVar3 + 0x14) = 0;
  *(undefined4 *)(puVar3 + 0x1c) = 0;
  *(undefined4 *)(iVar4 + 0x7c) = 0;
  *(undefined4 *)(iVar4 + 0x80) = 0;
  *(undefined4 *)(iVar4 + 0x84) = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar4 + 0xe0) = *(undefined4 *)(*(int *)(iVar4 + 0xec) + 0x24);
  }
  else {
    *(undefined8 *)(iVar4 + 0xe0) = *(undefined8 *)(*(int *)(iVar4 + 0xec) + 0x24);
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar4 + 0xdc) = *(undefined4 *)(*(int *)(iVar4 + 0xec) + 0x20);
  }
  else {
    *(undefined8 *)(iVar4 + 0xdc) = *(undefined8 *)(*(int *)(iVar4 + 0xec) + 0x20);
  }
  puVar1 = PTR_callback_candidate_c90bd40_0c90fecc;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar4 + 0xe4) = *(undefined4 *)(*(int *)(iVar4 + 0xec) + 0x28);
  }
  else {
    *(undefined8 *)(iVar4 + 0xe4) = *(undefined8 *)(*(int *)(iVar4 + 0xec) + 0x28);
  }
  *(undefined2 *)(puVar3 + 6) = 0;
  (*(code *)puVar1)(puVar3,iVar4);
  (*(code *)PTR_FUN_0c90fed0)(puVar3,iVar4);
  (*(code *)PTR_FUN_0c90fed4)(puVar3,iVar4);
  (*(code *)PTR_callback_candidate_c90853e_0c90fed8)(puVar3,iVar4);
  (*(code *)PTR_FUN_0c90fedc)(puVar3,iVar4);
  (*(code *)PTR_callback_candidate_c904e80_0c90fee0)(puVar3,iVar4);
  (*(code *)PTR_callback_candidate_c9048ec_0c90fee4)(puVar3,iVar4);
  (*(code *)PTR_FUN_0c90fee8)(puVar3,iVar4);
  uVar2 = DAT_0c90feec;
  *(uint *)(iVar4 + 0x60) = *(uint *)(iVar4 + 0x60) & 0xfffffff1;
  *(ushort *)(*(int *)(iVar4 + 0xec) + 0x36) =
       *(ushort *)(*(int *)(iVar4 + 0xec) + 0x36) & (ushort)uVar2;
  *(undefined4 *)(iVar4 + 0x74) = 0;
  *(undefined2 *)(iVar4 + 0x78) = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar4 + 0xd8) = (undefined4)DAT_0c90fef0;
  }
  else {
    *(ulonglong *)(iVar4 + 0xd8) = CONCAT44(DAT_0c90fef0._4_4_,(undefined4)DAT_0c90fef0);
  }
  if ((*(ushort *)(*(int *)(iVar4 + 0xec) + 0x36) & 0x40) != 0) {
    FUN_0c90fb82(puVar3,iVar4);
    return;
  }
  if ((*(ushort *)(*(int *)(iVar4 + 0xec) + 0x36) & 4) != 0) {
    FUN_0c90fca0(puVar3,iVar4);
    return;
  }
  if ((*(ushort *)(*(int *)(iVar4 + 0xec) + 0x36) & 8) != 0) {
    FUN_0c90fd42(puVar3,iVar4);
    return;
  }
  return;
}

