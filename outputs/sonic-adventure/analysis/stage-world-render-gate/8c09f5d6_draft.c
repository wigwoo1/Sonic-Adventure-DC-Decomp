/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09f5d6(short *param_1,short *param_2)

{
  short sVar1;
  short *psVar2;
  short *psVar3;
  undefined4 uVar4;
  
  uVar4 = 0xfffffffc;
  psVar3 = param_1;
  do {
    sVar1 = *psVar3;
    psVar2 = (short *)((int)psVar3 + *(int *)(psVar3 + 2) + 0xb & 0xfffffffc);
    if ((((sVar1 != 0) && (sVar1 != 1)) && (sVar1 != 2)) && ((sVar1 != 3 && (sVar1 != 4)))) {
      if (sVar1 == 5) {
        FUN_8c09f67c(param_1,psVar3 + 4,psVar2,uVar4);
      }
      else if (sVar1 == 6) {
        FUN_8c09f6c8(param_1,psVar3 + 4,psVar2);
      }
      else if (sVar1 == 7) {
        FUN_8c09f708(param_1,psVar3 + 4,psVar2);
      }
    }
    psVar3 = psVar2;
  } while (psVar2 < param_2);
  return;
}

