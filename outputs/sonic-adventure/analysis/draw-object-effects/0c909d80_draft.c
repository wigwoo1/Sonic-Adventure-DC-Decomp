/* Ghidra draft only; not buildable/verified source. */

void callback_candidate_c909d80(int param_1)

{
  switch(*(undefined2 *)(param_1 + 0x812)) {
  case 0:
    FUN_0c908f3e(param_1);
    break;
  case 1:
  case 2:
  case 3:
    FUN_0c909054(param_1);
    break;
  case 4:
    FUN_0c9092da(param_1);
    break;
  case 5:
    FUN_0c909560(param_1);
    break;
  case 6:
    FUN_0c9097e4(param_1);
    break;
  case 7:
    FUN_0c909aa0(param_1);
  }
  if (**(char **)(param_1 + 0xec) == '\x06') {
    (*(code *)PTR_callback_candidate_c908d80_0c909e84)(param_1,param_1 + 0x810);
  }
  else {
    (*(code *)PTR_callback_candidate_c908b00_0c909e80)(param_1,param_1 + 0x810);
  }
  *(short *)(param_1 + 0x844) = *(short *)(param_1 + 0x844) + 1;
  *(short *)(param_1 + 0x884) = *(short *)(param_1 + 0x884) + 1;
  return;
}

