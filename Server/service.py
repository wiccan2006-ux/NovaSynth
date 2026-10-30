"""Experimental Nova CPU service. Put behind a TLS reverse proxy for internet use."""
import hmac,json,os,subprocess,tempfile,time,wave,threading,io
from http.server import BaseHTTPRequestHandler,ThreadingHTTPServer
from pathlib import Path
MAX_BYTES=24*1024*1024
class Service(ThreadingHTTPServer):
    daemon_threads=True
    def __init__(self,address,worker,token,budget=3600,timeout=180):
        super().__init__(address,Handler);self.worker=worker;self.token=token;self.budget=max(160,min(4800,budget));self.timeout=timeout;self.slots=threading.BoundedSemaphore(1)
class Handler(BaseHTTPRequestHandler):
    def log_message(self,*args):pass # No request/audio/token logging.
    def do_GET(self):
        if self.path!='/health':self.send_error(404);return
        body=json.dumps({'protocol':1,'engine':'Nova CPU prototype','ready':True}).encode();self.send_response(200);self.send_header('Content-Type','application/json');self.send_header('Content-Length',str(len(body)));self.end_headers();self.wfile.write(body)
    def do_POST(self):
        if self.path!='/v1/recreate':self.send_error(404);return
        self.connection.settimeout(15)
        if not hmac.compare_digest(self.headers.get('Authorization',''),f'Bearer {self.server.token}'):
            self.send_error(401);return
        try:n=int(self.headers.get('Content-Length','0'))
        except ValueError:self.send_error(400);return
        if n<44 or n>MAX_BYTES:self.send_error(413);return
        if self.headers.get('Content-Type')!='audio/wav':self.send_error(415);return
        if not self.server.slots.acquire(False):self.send_error(429);return
        process=None;started=False
        try:
            body=self.rfile.read(n)
            if len(body)!=n:raise ValueError('Incomplete WAV')
            with wave.open(io.BytesIO(body)) as audio:
                rate=audio.getframerate();frames=audio.getnframes()
                if audio.getnchannels() not in (1,2) or audio.getsampwidth()!=2 or not 8000<=rate<=96000 or not .05<=frames/rate<=60.001:raise ValueError('WAV outside limits')
                if len(audio.readframes(frames))!=frames*audio.getnchannels()*2:raise ValueError('Truncated WAV')
            with tempfile.TemporaryDirectory(prefix='nova-job-') as folder:
                src=Path(folder)/'reference.wav';out=Path(folder)/'result.json';src.write_bytes(body);del body
                self.send_response(200);self.send_header('Content-Type','application/x-ndjson');self.send_header('Cache-Control','no-store');self.send_header('X-Accel-Buffering','no');self.end_headers();started=True
                process=subprocess.Popen([self.server.worker,str(src),str(out),str(self.server.budget)],stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL,start_new_session=True)
                deadline=time.monotonic()+self.server.timeout
                while process.poll() is None:
                    self.wfile.write(b'{"status":"running"}\n');self.wfile.flush()
                    if time.monotonic()>deadline:raise TimeoutError('Compute time limit exceeded')
                    time.sleep(1)
                if process.returncode or not out.exists():raise RuntimeError('Could not recreate a verified tonal instrument; use an isolated note')
                if out.stat().st_size>1800000:raise ValueError('Result too large')
                result=json.loads(out.read_text());assert result['protocol']==1 and result['status']=='done' and 1<=len(result['candidates'])<=3
                self.wfile.write(json.dumps(result,ensure_ascii=True,separators=(',',':')).encode()+b'\n');self.wfile.flush()
        except (BrokenPipeError,ConnectionError):pass
        except Exception as exc:
            try:
                if not started:self.send_error(400);return
                self.wfile.write(json.dumps({'status':'error','error':str(exc)[:240]}).encode()+b'\n');self.wfile.flush()
            except (BrokenPipeError,ConnectionError):pass
        finally:
            if process and process.poll() is None:
                import signal
                try:os.killpg(process.pid,signal.SIGTERM);process.wait(timeout=3)
                except (ProcessLookupError,subprocess.TimeoutExpired):
                    try:os.killpg(process.pid,signal.SIGKILL);process.wait()
                    except ProcessLookupError:pass
            self.server.slots.release()
if __name__=='__main__':
    token=os.environ.get('NOVA_SERVER_TOKEN','')
    if len(token)<24:raise SystemExit('Set NOVA_SERVER_TOKEN to at least 24 random characters')
    Service(('0.0.0.0',int(os.environ.get('PORT','8080'))),os.environ.get('NOVA_WORKER','/app/worker'),token,int(os.environ.get('NOVA_BUDGET','3600'))).serve_forever()
