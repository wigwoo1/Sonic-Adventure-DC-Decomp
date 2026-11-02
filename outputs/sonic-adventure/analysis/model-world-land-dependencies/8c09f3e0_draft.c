/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c09f3e0(int param_1)

{
  short sVar1;
  short *psVar2;
  undefined4 uVar3;
  short *psVar4;
  
  if (*(char *)(param_1 + 3) == '\x04') {
    sVar1 = *(short *)(param_1 + 8);
    psVar2 = (short *)(param_1 + 8);
    while (sVar1 != 0) {
      sVar1 = *psVar2;
      psVar4 = (short *)((int)psVar2 + *(int *)(psVar2 + 2) + 0xb & 0xfffffffc);
      if (sVar1 == 1) {
        FUN_8c09f5d6(psVar2 + 4,psVar4);
      }
      else if (sVar1 == 3) {
        FUN_8c09f900(psVar2 + 4,psVar4);
      }
      else if (sVar1 == 4) {
        FUN_8c09f966(psVar2 + 4,psVar4);
      }
      psVar2 = psVar4;
      sVar1 = *psVar4;
    }
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}

