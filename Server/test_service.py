import sys,threading,tempfile,wave,math,struct,urllib.request,urllib.error,json,subprocess
from pathlib import Path
from service import Service
worker=str(Path(sys.argv[1]).resolve());output=Path(sys.argv[2]).resolve();output.mkdir(parents=True,exist_ok=True)
token='synthetic-test-token-not-a-user-secret'
server=Service(('127.0.0.1',0),worker,token,budget=3600);thread=threading.Thread(target=server.serve_forever,daemon=True);thread.start();base=f'http://127.0.0.1:{server.server_port}'
def request(body,auth=True):return urllib.request.Request(base+'/v1/recreate',data=body,headers={'Content-Type':'audio/wav',**({'Authorization':'Bearer '+token} if auth else {})})
try:
    health=json.load(urllib.request.urlopen(base+'/health'));assert health['ready'] and health['protocol']==1
    for body,auth,expected in [(b'x'*44,False,401),(b'x'*44,True,400)]:
        try:urllib.request.urlopen(request(body,auth));raise AssertionError('invalid request accepted')
        except urllib.error.HTTPError as e:assert e.code==expected,(e.code,expected)
    with tempfile.TemporaryDirectory() as folder:
        audio=Path(folder)/'test.wav'
        with wave.open(str(audio),'wb') as f:
            f.setnchannels(1);f.setsampwidth(2);f.setframerate(16000);f.writeframes(b''.join(struct.pack('<h',int(7000*math.sin(2*math.pi*440*i/16000)*min(1,i/160)*min(1,(16000-i)/800))) for i in range(16000)))
        client=Path(worker).with_name('nova_cloud_client_test')
        result=subprocess.run([str(client),base,str(audio),token],capture_output=True,text=True,timeout=190)
        assert result.returncode==0,result.stdout+result.stderr
        print(result.stdout.strip())
    report={'protocol':1,'health':'PASS','authentication':'PASS','malformed_wav':'PASS','real_plugin_http_worker_roundtrip':'PASS','undo_and_session_only_credentials':'PASS','hosted_service':False,'new_neural_training':False,'gpu':False}
    (output/'service-validation.json').write_text(json.dumps(report,indent=2));print('PASS: service validation, no hosted/GPU claims')
finally:server.shutdown();server.server_close();thread.join(timeout=5)
