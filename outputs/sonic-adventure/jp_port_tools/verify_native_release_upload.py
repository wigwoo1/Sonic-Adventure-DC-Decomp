"""Real curl file POSTs to loopback, including binary bytes and an HTTP failure.
Uses a synthetic credential; never touches GitHub or user credentials.
"""
import argparse,hashlib,importlib.util,json,threading
from http.server import BaseHTTPRequestHandler,ThreadingHTTPServer
from pathlib import Path
def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','output'):ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure/publish_github_snapshot.py'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    spec=importlib.util.spec_from_file_location('publisher',p);pub=importlib.util.module_from_spec(spec);spec.loader.exec_module(pub)
    seen=[];dummy='synthetic_local_transport_test_value'
    class Receiver(BaseHTTPRequestHandler):
        def do_POST(self):
            remaining=int(self.headers['Content-Length']);h=hashlib.sha256();total=0
            while remaining:
                block=self.rfile.read(min(65536,remaining));assert block;remaining-=len(block);total+=len(block);h.update(block)
            seen.append(dict(bytes=total,sha256=h.hexdigest(),auth_matches=self.headers.get('Authorization')=='Bearer '+dummy,content_type=self.headers.get('Content-Type'),route=self.path))
            result=json.dumps(dict(state='uploaded',size=total,digest='sha256:'+h.hexdigest())).encode()
            self.send_response(403 if self.path=='/fail' else 201);self.send_header('Content-Type','application/json');self.send_header('Content-Length',str(len(result)));self.end_headers();self.wfile.write(result)
        def log_message(self,*args):pass
    server=ThreadingHTTPServer(('127.0.0.1',0),Receiver);thread=threading.Thread(target=server.serve_forever,daemon=True);thread.start();checks=0
    url='http://127.0.0.1:'+str(server.server_port)
    try:
        for name,raw in [('checksum with spaces.txt',b'abc123  research.zip\n'),('binary with spaces.bin',bytes(range(256))*32769)]:
            path=d/name;path.write_bytes(raw);result=pub.stream_asset(url+'/assets',path,dummy)
            assert result['state']=='uploaded' and result['size']==len(raw) and result['digest']=='sha256:'+hashlib.sha256(raw).hexdigest();checks+=3
            assert seen[-1]['auth_matches'] and seen[-1]['content_type']=='application/octet-stream';checks+=2
            assert seen[-1]['sha256']==hashlib.sha256(path.read_bytes()).hexdigest();checks+=1
        try:pub.stream_asset(url+'/fail',d/'checksum with spaces.txt',dummy)
        except RuntimeError as error:
            assert 'curl 22' in str(error) and dummy not in str(error);checks+=2
        else:raise AssertionError('HTTP upload failure accepted')
        try:pub.stream_asset(url+'/assets',d/'binary with spaces.bin',dummy+'\ninvalid')
        except ValueError:checks+=1
        else:raise AssertionError('header injection accepted')
        assert len(seen)==3 and sum(x['route']=='/fail' for x in seen)==1;checks+=1
    finally:server.shutdown();server.server_close();thread.join()
    result=dict(status='actual_native_curl_file_streaming_spaces_HTTP_failure_and_no_GitHub_mutation_verified',checks=checks,real_received_files=seen,
        publisher_sha256=hashlib.sha256(p.read_bytes()).hexdigest(),verifier_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        credential_used='synthetic local test only',real_GitHub_upload_recovery_proof='work/sonic-adventure/jp-native-gdfs-native-stream-upload-v2/verification.json',
        TLS_GitHub_endpoint_tested_by_this_local_suite=False,network_scope='loopback only',remote_repository_modified=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
