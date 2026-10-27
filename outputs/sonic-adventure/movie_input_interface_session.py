"""Original interface/task constructors with explicit external device observations."""
import movie_input_interface_task_fixtures as tasks
import movie_channel_request_fixtures as backend
v=tasks.v;put=tasks.put
INTERFACE_CELL=0x8c89c54c
MMIO=0xa05f6c00
DEVICE_BUFFER=0x8cd80000

class Session:
    def __init__(self,ram,channel=0,command=1,mode=0,order=0,endian=0,callback=0x8c05180c,axes=0x80420f07,retry_budget=0):
        self.channel=channel;self.command=command;self.mode=mode;self.order=order;self.endian=endian;self.callback=callback
        self.context=tasks.INPUT_OBJECT;self.records=[];self.seen=set();self.edges=set();self.phases=[]
        self.c=v.Input(dict(entry=v.REGISTER_CHANNELS,mode=mode,sr=0x400000a1,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,register_arguments=[1<<channel,0,0,0]))
        self.m,self.s,self.f=v.prepare(ram,self.c);self.f.fpscr=0x40001|(mode<<19)
        for bank in range(2):
            for reg in range(16):self.f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
        self.initial_fpu=bytes(self.f);self.mapped=tasks.layout(self.m,channel)['mapped']
        backend.configure(self.m,self.mapped,context=self.context,bank=order,order=order)
        self.d=tasks.configure(self.m,channel,1,0,2)
        for off,value in ((0xfc0,1),(0x10f4,MMIO),(0x10a8,backend.FRAME),(0x10ac,backend.FRAME+0x10000),(0x10b8,order),(0x10b4,order)):
            put(self.m,self.context+off,value)
        for unit in {self.mapped,self.mapped//6*6}:put(self.m,self.context+0xf90+unit,1,1)
        port=self.context+4+(self.mapped//6)*8
        put(self.m,port,0x1f,1);put(self.m,port+1,0x1f,1)
        put(self.m,self.d['status'],1,1);put(self.m,self.d['status']+4,9,1)
        put(self.m,self.d['info']+4,axes)
        self.c['startup_overrides']={0x5f6c18:0};self.c['scan_samples']=[0x1122]
        self.call('interface_initialize',0x8c647e16,[0x21,1<<channel,DEVICE_BUFFER,0])
        self.interface=v.load(self.m,INTERFACE_CELL)
        self.call('interface_select',0x8c647de8,[endian])
        self.call('retry_policy_api',0x8c647534,[retry_budget])
        self.descriptor=v.load(self.m,0x8c648368)+channel*0x148
        self.output=v.load(self.m,self.descriptor+0x1c)
        self.call('task_admit',v.REGISTER_CHANNELS,[1<<channel,0,0,0]);self.call('task_idle',v.AI_SCHEDULE)
        self.owner=v.load(self.m,v.load(self.m,0x8c660880));self.task=self.owner+0x210
        self.idle_sp=v.load(self.m,self.task+0x18)
        self.channel_record=v.load(self.m,0x8c6479bc)+channel*0x80
        self.result_word=v.load(self.m,0x8c6479c4)+channel*4

    def tokens(self):return [v.load(self.m,self.d['status']+off) for off in (0x18,0x1c,0x20)]

    def call(self,phase,entry,args=None,stack=()):
        r,path,self.m,self.s,self.f=tasks.call(self.m,self.s,self.f,self.c,entry,args,stack)
        self.c['startup_overrides']={0x5f6800+j*4:value for j,value in enumerate(r['startup_registers']) if value}
        self.seen.update(path)
        for j,pc in enumerate(path):
            text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):self.edges.add((hex(pc),hex(path[j+distance])))
        record=dict(phase=phase,entry=hex(entry),status=r['status'],stop_pc=r['stop_pc'],steps=len(path),tokens=self.tokens(),targets=[b['target'] for b in r['boundaries']],callback_events=[b for b in r['boundaries'] if b['pr']=='0x8c647bb4'])
        self.records.append(record);self.phases.append((r,path))
        return r,path

    def queue(self):
        before=v.sample(self.m,self.owner+0x210,32*0x124)
        for entry,args,stack in ((0x8c647c50,[self.callback],()),(0x8c647c6c,[0x8c05180c],()),(0x8c647cc4,[self.channel,self.channel,0,0],(0,0,0)),(0x8c647cb4,[self.channel,self.command],())):
            self.call('command_api',entry,args,stack)
        self.command_apis_preserved_task_records=v.sample(self.m,self.owner+0x210,32*0x124)==before
        return self.call('task_queue',v.AI_SCHEDULE)

    def submit(self):return self.call('transport_submit',0x8c65efda)

    def observe(self,geometry=511,status=8,unplug=False,block_data=None):
        # Device-side input only. Never change a request ID, task record, saved
        # context, interface callback, decoded output or parser return here.
        before=self.tokens();self.c['startup_overrides'][0x5f6c18]=0
        active=v.load(self.m,self.context+0x10b4);raw=[]
        metadata=bytearray(116);byte_order='big' if self.endian else 'little'
        metadata[0:2]=geometry.to_bytes(2,byte_order);metadata[4:6]=geometry.to_bytes(2,byte_order)
        metadata[14]=0x12;metadata[15]=0x34
        payload=(2).to_bytes(4,'little')+metadata
        if block_data is not None:
            assert len(block_data)==256
            payload=(2).to_bytes(4,'little')+bytes(4)+bytes(block_data)
        for unit in range(self.mapped//6*6,self.mapped//6*6+6):
            pointer=v.load(self.m,self.context+0xfe8+(1-active)*96+unit*4)
            header=bytes([status if unit==self.mapped else 5,0x20,0 if unplug else 0x1f,len(payload)//4])
            packet=header+payload
            if self.order:packet=b''.join(packet[j:j+4][::-1] for j in range(0,len(packet),4))
            self.m[pointer&0xffffff:(pointer&0xffffff)+len(packet)]=packet
            raw.append(dict(unit=unit,pointer=hex(pointer),header=list(header),raw_sha256=__import__('hashlib').sha256(packet).hexdigest()))
        assert self.tokens()==before
        self.last_observation=dict(geometry=geometry,status=status,unplug=unplug,block_data=block_data is not None,raw_inputs=raw,request_ids_unchanged=True)
        self.c['scan_samples']=[0x1122]
        return self.last_observation

    def pump(self):return self.call('response_pump',0x8c646654,[0,0,0,0])
    def resume(self):return self.call('task_resume',v.AI_SCHEDULE)

    def metadata_expected(self):
        # Interface contract: ten16bit fields and two bytes, followed by actual
        # capability-word axes decode and integer division. The input snapshot
        # is taken after response transport, before the resumed reader executes.
        pointer=v.load(self.m,self.d['header']+8)+8
        raw=v.sample(self.m,pointer,24);endian='big' if self.endian else 'little'
        wanted=[v.load(self.m,self.output+j*4) for j in range(19)]
        for dest,offset in zip((0,1,2,3,4,5,6,7,8,13),(0,2,4,6,8,10,12,16,18,20)):
            wanted[dest]=int.from_bytes(raw[offset:offset+2],endian)
        wanted[9]=raw[14];wanted[10]=raw[15];wanted[11]=511;wanted[14]=128
        capability=v.load(self.m,self.d['info']+4)
        horizontal=(capability>>20)&15;vertical=(capability>>16)&15
        wanted[15]=vertical;wanted[16]=horizontal
        wanted[17]=512//vertical if vertical else 0;wanted[18]=512//horizontal if horizontal else 0
        return wanted
