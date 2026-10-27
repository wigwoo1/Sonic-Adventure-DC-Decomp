/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65ff12(undefined1 *param_1)

{
  if (param_1[7] == '\x01') {
    if (param_1[8] == '\x05') {
      *param_1 = 1;
    }
    else {
      *param_1 = 0;
    }
  }
  else if (((byte)param_1[8] == DAT_8c65ff4a) || ((byte)param_1[8] == 0)) {
    *param_1 = 0;
  }
  else {
    *param_1 = 1;
  }
  return 0;
}

