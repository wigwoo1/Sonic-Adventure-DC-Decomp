/* Ghidra draft only; not buildable/verified source. */

void callback_candidate_c90853e(int param_1)

{
  undefined1 uVar1;
  undefined1 uVar2;
  char cVar3;
  int iVar4;
  char cVar5;
  char cVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  
  iVar4 = *(int *)(param_1 + 0xec);
  if (*(char *)(iVar4 + 0x5c) < -0x23) {
    cVar5 = '\0';
  }
  else if (*(char *)(iVar4 + 0x5c) < '#') {
    cVar5 = '\x01';
  }
  else {
    cVar5 = '\x02';
  }
  if (*(char *)(iVar4 + 0x5d) < -0x23) {
    cVar6 = '\0';
  }
  else if (*(char *)(iVar4 + 0x5d) < '#') {
    cVar6 = '\x01';
  }
  else {
    cVar6 = '\x02';
  }
  if (*(char *)(iVar4 + 0x5e) < -0x23) {
    cVar3 = '\0';
  }
  else if (*(char *)(iVar4 + 0x5e) < '#') {
    cVar3 = '\x01';
  }
  else {
    cVar3 = '\x02';
  }
  uVar1 = PTR_DAT_0c908740[(int)cVar3 + cVar5 * 3 + cVar6 * 9];
  uVar2 = PTR_DAT_0c908744[(int)cVar3 + cVar5 * 3 + cVar6 * 9];
  uVar7 = *(undefined4 *)(PTR_DAT_0c908748 + cVar3 * 4 + cVar5 * 0xc + cVar6 * 0x24);
  uVar8 = *(undefined4 *)(PTR_DAT_0c90874c + cVar3 * 4 + cVar5 * 0xc + cVar6 * 0x24);
  *(undefined4 *)(param_1 + 0x7f4) = uVar7;
  *(undefined4 *)(param_1 + 0x7e4) = uVar8;
  *(undefined4 *)(param_1 + 0x7e8) = uVar8;
  FUN_0c9084e6(param_1,uVar1,0);
  FUN_0c9084fc(param_1,uVar2);
  FUN_0c908530(param_1,uVar8,uVar7);
  *(undefined4 *)(param_1 + 2000) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x7d4) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x7d8) = 0xffffffff;
  return;
}

