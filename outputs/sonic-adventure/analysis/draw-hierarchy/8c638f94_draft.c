/* Ghidra draft only; not buildable/verified source. */

void FUN_8c638f94(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
                 undefined8 *param_9)

{
  undefined4 in_fr1;
  undefined4 in_fr0;
  undefined4 in_fr3;
  undefined4 in_fr2;
  undefined4 in_fr13;
  undefined4 in_fr12;
  undefined4 in_fr15;
  undefined4 in_fr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  undefined8 in_xd14;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)param_9 + 0x3c) = in_fr15;
    *(undefined4 *)(param_9 + 7) = in_fr14;
    *(undefined4 *)((int)param_9 + 0x34) = in_fr13;
    *(undefined4 *)(param_9 + 6) = in_fr12;
    *(undefined4 *)((int)param_9 + 0x2c) = param_8;
    *(undefined4 *)(param_9 + 5) = param_7;
    *(undefined4 *)((int)param_9 + 0x24) = param_6;
    *(undefined4 *)(param_9 + 4) = param_5;
    *(undefined4 *)((int)param_9 + 0x1c) = param_4;
    *(undefined4 *)(param_9 + 3) = param_3;
    *(undefined4 *)((int)param_9 + 0x14) = param_2;
    *(undefined4 *)(param_9 + 2) = param_1;
    *(undefined4 *)((int)param_9 + 0xc) = in_fr3;
    *(undefined4 *)(param_9 + 1) = in_fr2;
    *(undefined4 *)((int)param_9 + 4) = in_fr1;
    *(undefined4 *)param_9 = in_fr0;
  }
  else {
    param_9[7] = in_xd14;
    param_9[6] = CONCAT44(in_fr14,in_fr15);
    param_9[5] = in_xd12;
    param_9[4] = CONCAT44(in_fr12,in_fr13);
    param_9[3] = in_xd10;
    param_9[2] = CONCAT44(param_7,param_8);
    param_9[1] = in_xd8;
    *param_9 = CONCAT44(param_5,param_6);
    param_9[-1] = in_xd6;
    param_9[-2] = CONCAT44(param_3,param_4);
    param_9[-3] = in_xd4;
    param_9[-4] = CONCAT44(param_1,param_2);
    param_9[-5] = in_xd2;
    param_9[-6] = CONCAT44(in_fr2,in_fr3);
    param_9[-7] = in_xd0;
    param_9[-8] = CONCAT44(in_fr0,in_fr1);
  }
  return;
}

