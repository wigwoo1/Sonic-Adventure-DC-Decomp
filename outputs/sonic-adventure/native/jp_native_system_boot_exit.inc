/* Same System owner and same existing bootstrap registration. */
static int system_boot_close(uint32_t kind)
{
    uint64_t token=kind?system_owner.print_token:system_owner.card_token;
    return system_owner.terminal?jp_boot_lifetime_finish(kind,token,system_owner.boot_timeout):
        jp_boot_lifetime_close(kind,token,system_owner.boot_timeout);
}
static int system_boot_release(void)
{
    if(system_owner.terminal)return 0;
    for(uint32_t kind=0;kind<2;kind++){
        uint64_t *token=kind?&system_owner.print_token:&system_owner.card_token;
        if(*token){int rc=jp_boot_lifetime_cancel(kind,*token);if(rc)return rc;*token=0;}
    }
    return 0;
}
int jp_system_main_initialization_guard(uint32_t held)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(held>1)return JP_SYSTEM_ARGUMENT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY)return JP_SYSTEM_STATE;
    if(system_owner.main_guard==held)return held?JP_SYSTEM_REENTRANT:0;
    rc=jp_exit_hold_bootstrap(bootstrap_cleanup,&system_owner,held);if(rc)return result(JP_SYSTEM_EXIT,0,rc);
    system_owner.main_guard=held;return 0;
}
int jp_system_adopt_boot_lifetimes(uint32_t resources,uint32_t timeout)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY||!system_owner.main_guard||system_owner.card_token||system_owner.print_token)return JP_SYSTEM_STATE;
    if((resources&~JP_EXIT_MAIN_LIFETIMES)||!timeout||timeout>60000)return JP_SYSTEM_ARGUMENT;
    uint64_t card=0,print=0;
    if(resources&JP_EXIT_CARD){rc=jp_boot_lifetime_claim(JP_BOOT_CARD,&card);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_CARD,rc);}
    if(resources&JP_EXIT_PRINT){
        rc=jp_boot_lifetime_claim(JP_BOOT_PRINT,&print);
        if(rc){
            if(card){int cancelled=jp_boot_lifetime_cancel(JP_BOOT_CARD,card);if(cancelled){system_owner.card_token=card;system_owner.info.resources|=JP_EXIT_CARD;system_owner.boot_timeout=timeout;return result(JP_SYSTEM_CLEANUP,JP_EXIT_CARD,cancelled);}}
            return result(JP_SYSTEM_CLEANUP,JP_EXIT_PRINT,rc);
        }
    }
    system_owner.card_token=card;system_owner.print_token=print;system_owner.boot_timeout=timeout;system_owner.info.resources|=resources;
    rc=jp_exit_attach_boot_lifetimes(bootstrap_cleanup,&system_owner,card,print,timeout);
    if(rc)return result(JP_SYSTEM_EXIT,resources,rc);
    return jp_system_main_initialization_guard(0);
}
