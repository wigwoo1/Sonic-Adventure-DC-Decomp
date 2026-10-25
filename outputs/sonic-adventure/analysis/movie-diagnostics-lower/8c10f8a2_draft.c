/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10f8a2(char *param_1,int param_2)

{
  char *pcVar1;
  char *pcVar2;
  
  if ('4' < param_1[param_2]) {
    pcVar1 = param_1 + param_2 + -1;
    param_1[param_2] = '0';
    *pcVar1 = *pcVar1 + '\x01';
    pcVar2 = pcVar1;
    for (; param_1 < pcVar1; pcVar1 = pcVar1 + -1) {
      if ('9' < *pcVar1) {
        *pcVar2 = *pcVar2 + -10;
        pcVar2[-1] = pcVar2[-1] + '\x01';
      }
      pcVar2 = pcVar2 + -1;
    }
    pcVar1 = param_1 + 1;
    if ('9' < *pcVar1) {
      *pcVar1 = *pcVar1 + -10;
      *param_1 = *param_1 + '\x01';
    }
  }
  return;
}

