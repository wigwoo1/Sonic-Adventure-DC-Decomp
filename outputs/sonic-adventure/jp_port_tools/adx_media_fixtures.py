"""Read-only ISO/AFS fixtures from the supplied JP track; not a game I/O service."""
import hashlib,struct
class Disc:
    def __init__(self,path):
        self.file=path.open('rb');h=hashlib.sha256()
        for chunk in iter(lambda:self.file.read(8<<20),b''):h.update(chunk)
        self.sha256=h.hexdigest();assert self.sha256=='4ac0e256cf91520f794d7f831c193847b16397da3bde5c890e04e5330499c76c'
        pvd=self.sectors(45166,1);assert pvd[:6]==b'\x01CD001'
        root=self.directory(struct.unpack_from('<I',pvd,0x9e)[0]+150,struct.unpack_from('<I',pvd,0xa6)[0])
        fad,size=root['SONICADV'];self.files=self.directory(fad,size)
    def sectors(self,fad,count):
        out=bytearray()
        for i in range(count):
            self.file.seek((fad+i-45150)*2352+16);raw=self.file.read(2048);assert len(raw)==2048;out+=raw
        return bytes(out)
    def directory(self,fad,size):
        result={}
        for i in range((size+2047)//2048):
            data=self.sectors(fad+i,1);at=0
            while at<2048 and data[at]:
                length=data[at];record=data[at:at+length];assert length>=34 and len(record)==length
                name=record[33:33+record[32]]
                if name not in (b'\0',b'\1'):
                    name=name.decode('ascii').split(';')[0].upper();result[name]=(struct.unpack_from('<I',record,2)[0]+150,struct.unpack_from('<I',record,10)[0])
                at+=length
        return result
    def read(self,name,offset,count):
        fad,size=self.files[name.upper()];assert offset>=0 and count>=0 and offset+count<=size
        skip=offset%2048;return self.sectors(fad+offset//2048,(skip+count+2047)//2048)[skip:skip+count]
    def fixtures(self):
        rows=[]
        for name in ('TITLE.ADX','ECOAST1.ADX','CHAO.ADX'):
            rows.append(dict(name=name,archive=None,offset=0,bytes=self.files[name][1]))
        name='EVENT_ADX.AFS';head=self.read(name,0,8);assert head[:4]==b'AFS\0';count=struct.unpack_from('<I',head,4)[0]
        assert 0<count<=2148;self.voice_archive_file_count=count
        table=self.read(name,8,count*8)
        for i in range(count):
            offset,size=struct.unpack_from('<II',table,i*8)
            if size<64:continue
            header=self.read(name,offset,64)
            if header[:2]==b'\x80\x00' and header[4:8]==bytes([3,18,4,1]):
                rows.append(dict(name=f'event_adx.afs:{i}',archive=name,offset=offset,bytes=size))
                if len(rows)==6:break
        assert len(rows)==6;return rows
    def close(self):self.file.close()
