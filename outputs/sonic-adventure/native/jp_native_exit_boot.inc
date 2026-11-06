/* Appended to the existing Exit authority. No upstream/MainBoot import. */
static int boot_exit_activity(void)
{
    uint64_t card=state.bound?state.card_token:bootstrap.card_token;
    uint64_t print=state.bound?state.print_token:bootstrap.print_token;
    int rc=card?jp_boot_lifetime_can_close(JP_BOOT_CARD,card):0;
    if(!rc&&print)rc=jp_boot_lifetime_can_close(JP_BOOT_PRINT,print);
    return !rc?0:rc==JP_BOOT_LIFE_THREAD?JP_EXIT_THREAD:rc==JP_BOOT_LIFE_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_SERVICE;
}
int jp_exit_attach_boot_lifetimes(jp_boot_cleanup_identity cleanup,void *context,
    uint64_t card,uint64_t print,uint32_t timeout)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(state.bound||bootstrap.started||bootstrap.cleanup!=cleanup||bootstrap.context!=context||bootstrap.card_token||bootstrap.print_token)return JP_EXIT_ALREADY_BOUND;
    if(!timeout||timeout>60000)return JP_EXIT_ARGUMENT;
    for(uint32_t kind=0;kind<2;kind++){
        uint64_t token=kind?print:card;if(!token)continue;
        struct jp_boot_life_info info;
        if(jp_boot_lifetime_snapshot(kind,&info)||info.token!=token||info.terminal||info.closing)return JP_EXIT_ARGUMENT;
        rc=jp_boot_lifetime_can_close(kind,token);if(rc)return rc==JP_BOOT_LIFE_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_ARGUMENT;
    }
    bootstrap.card_token=card;bootstrap.print_token=print;bootstrap.boot_timeout=timeout;
    if(card)bootstrap.resources|=JP_EXIT_CARD;if(print)bootstrap.resources|=JP_EXIT_PRINT;
    return 0;
}
