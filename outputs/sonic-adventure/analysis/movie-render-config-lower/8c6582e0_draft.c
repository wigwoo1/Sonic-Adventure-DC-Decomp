/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c658344) */
/* WARNING: Removing unreachable block (ram,0x8c6583d2) */

void FUN_8c6582e0(uint param_1,uint param_2,uint param_3,uint param_4,uint param_5,uint param_6,
                 uint param_7,uint param_8,uint param_9,uint param_10,int param_11,int param_12,
                 int param_13,uint param_14,uint param_15,uint param_16,uint param_17,uint param_18,
                 int param_19,int param_20,uint param_21,int param_22,int param_23,int param_24,
                 int param_25,uint param_26,uint param_27,uint param_28,uint param_29)

{
  int *piVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  undefined4 uVar6;
  
  uVar6 = 0x1e0;
  iVar3 = (int)DAT_8c658460;
  *DAT_8c658468 = *(int *)((int)&param_1 + iVar3);
  *DAT_8c65846c = *(int *)(&stack0xfffffff8 + iVar3);
  *DAT_8c658470 = *(undefined4 *)(&stack0xfffffffc + iVar3);
  *DAT_8c658474 =
       param_22 << 1 | param_21 | param_23 << 2 | param_25 << 3 | param_13 << 4 | param_24 << 5 |
       param_11 << 6 | param_12 << 7 | param_19 << 8 | param_20 << 9;
  uVar5 = (uint)DAT_8c658462;
  *DAT_8c658478 = param_2 & uVar5 | (param_1 & uVar5) << 0x10;
  *DAT_8c65847c = (param_2 & uVar5) << 0x10;
  *DAT_8c658480 = param_3 & uVar5 | (param_6 & uVar5) << 0x10;
  *DAT_8c658484 = param_5 & uVar5 | (param_4 & uVar5) << 0x10;
  *DAT_8c658488 = param_28 & uVar5 | (param_29 & uVar5) << 0x10;
  uVar5 = (uint)DAT_8c658464;
  *DAT_8c65848c =
       param_7 & 0x7f | (param_8 & 0xf) << 8 | (param_9 & uVar5) << 0xc | (param_10 & 0x7f) << 0x16;
  *DAT_8c658490 =
       param_14 & 1 | (param_15 & 1) << 1 | (param_16 & 1) << 2 | (param_17 & 1) << 3 |
       (param_18 & 0xf) << 4 | (param_26 & 1) << 8 | (param_27 & 0x3f) << 0x10;
  piVar1 = DAT_8c658468;
  *DAT_8c658498 = *DAT_8c658468 + *DAT_8c658494;
  *DAT_8c6584a0 = *DAT_8c65846c + *DAT_8c65849c | (DAT_8c65846c[1] + piVar1[-1]) * 0x10000;
  puVar2 = PTR_FUN_8c6584a8;
  if ((*DAT_8c6584a4 & 2) != 0) {
    iVar3 = (int)DAT_8c658466;
    do {
      uVar4 = (*(code *)puVar2)(iVar3,uVar6);
    } while ((uVar4 & uVar5) == 0);
    do {
      uVar4 = (*(code *)puVar2)(iVar3);
    } while ((uVar4 & uVar5) != 0);
  }
  puVar2 = PTR_thunk_FUN_8c653444_8c658574;
  (*(code *)PTR_thunk_FUN_8c653444_8c658574)((int)DAT_8c658560,*DAT_8c658578 | 8);
  uVar6 = DAT_8c658580;
  if ((*DAT_8c65857c & 1) == 0) {
    uVar6 = 0;
  }
  (*(code *)puVar2)(0x44,uVar6);
  (*(code *)puVar2)(0x5c,0);
  (*(code *)puVar2)((int)DAT_8c658562,*DAT_8c658584);
  (*(code *)puVar2)((int)DAT_8c658564,*DAT_8c658588);
  (*(code *)puVar2)((int)DAT_8c658566,*DAT_8c65858c);
  (*(code *)puVar2)((int)DAT_8c658568,*DAT_8c658590);
  (*(code *)puVar2)((int)DAT_8c65856a,*DAT_8c658594);
  (*(code *)puVar2)((int)DAT_8c65856c,*DAT_8c658598);
  (*(code *)puVar2)((int)DAT_8c65856e,*DAT_8c65859c);
  (*(code *)puVar2)((int)DAT_8c658570,*DAT_8c6585a0);
  (*(code *)puVar2)((int)DAT_8c658572,*DAT_8c6585a4);
  return;
}

