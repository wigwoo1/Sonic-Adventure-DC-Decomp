/* Read actual SDK queues; helpers do not synthesize host packets or ACKs. */
__declspec(dllexport) int peer_commands(uint64_t *out)
{
    if(!out||!sdgWrkPtr)return 1;
    struct sd_host_commands *host=&sdgWrkPtr->host;
    out[0]=host->primary_num;out[1]=host->busy;out[2]=host->second_num;
    out[3]=host->second_write;out[4]=host->second_read;out[5]=host->second_max;
    out[6]=sdgG2DmaTriggerNum;out[7]=sdgG2DmaEndNum;return 0;
}
__declspec(dllexport) int peer_second(uint32_t index,uint8_t *out)
{
    if(!out||!sdgWrkPtr||index>=SD_2ND_HOST_CMD_MAX)return 1;
    struct sd_host_command_entry *entry=&sdgWrkPtr->host.second[index];
    memcpy(out,&entry->command.command,4);memcpy(out+4,entry->command.params,14);
    out[18]=(uint8_t)entry->pending;return 0;
}
