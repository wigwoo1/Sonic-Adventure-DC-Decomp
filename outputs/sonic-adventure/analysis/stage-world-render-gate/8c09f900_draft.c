/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09f900(short *param_1,short *param_2)

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
    if (((sVar1 != 0) && (sVar1 != 1)) && (sVar1 != 2)) {
      if (sVar1 == 3) {
        FUN_8c09f884(param_1,psVar3 + 4,psVar2,uVar4);
      }
      else if (sVar1 == 4) {
        FUN_8c09f8e2(param_1,psVar3 + 4,psVar2);
      }
    }
    psVar3 = psVar2;
  } while (psVar2 < param_2);
  return;
}

