import threading,unittest,urllib.request,urllib.error
from service import Service,MAX_BYTES
class ProtocolTests(unittest.TestCase):
 def setUp(self):
  self.s=Service(('127.0.0.1',0),'/no-worker-required','a'*32);self.t=threading.Thread(target=self.s.serve_forever,daemon=True);self.t.start();self.url=f'http://127.0.0.1:{self.s.server_port}/v1/recreate'
 def tearDown(self):self.s.shutdown();self.s.server_close();self.t.join()
 def code(self,body=b'x'*44,headers=None):
  h={'Content-Type':'audio/wav','Authorization':'Bearer '+'a'*32};h.update(headers or {})
  try:return urllib.request.urlopen(urllib.request.Request(self.url,data=body,headers=h)).status
  except urllib.error.HTTPError as e:return e.code
 def test_auth(self):self.assertEqual(self.code(headers={'Authorization':'wrong'}),401)
 def test_bad_audio(self):self.assertEqual(self.code(),400)
 def test_size(self):self.assertEqual(self.code(headers={'Content-Length':str(MAX_BYTES+1)}),413)
 def test_busy(self):
  self.s.slots.acquire()
  try:self.assertEqual(self.code(),429)
  finally:self.s.slots.release()
if __name__=='__main__':unittest.main()
