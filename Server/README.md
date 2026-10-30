# Nova recreation service — experimental CPU prototype

This service performs synthesis fitting with the actual Nova engine. It is not a
hosted service, a GPU backend or a newly trained neural model. Audio goes into the
analysis worker; responses contain only editable synthesis parameters/models.

From the repository root:

```sh
docker build -f Server/Dockerfile -t nova-recreation .
# Generate a strong random token and provide it through your host's secret store.
docker run --rm --memory=2g --cpus=2 -p 127.0.0.1:8080:8080 --env NOVA_SERVER_TOKEN nova-recreation
```

For internet use, deploy on a trusted Linux account, use HTTPS at a reverse proxy,
disable response buffering, set a 200 s upstream timeout and a 24 MiB request
limit. Keep the container port private behind that proxy. /health reports protocol
and engine; POST /v1/recreate requires Authorization: Bearer TOKEN and a PCM16 WAV
(1–2 channels, 8–96 kHz, 0.05–60 seconds, max 24 MiB). One task runs at a time;
concurrent requests return 429. Default search budget is 3600, maximum 4800.

The response is NDJSON: running heartbeats then done with protocol=1, at most three
PatchVersion XML snapshots and report. The client validates all parameter IDs,
ranges, model integrity, and tonal pitch before applying. Tokens/addresses are
session only; never part of a preset or project. Each upload is user confirmed.

Requests are not logged. Temporary WAV/results are deleted on normal completion,
error or disconnect; use an ephemeral volume/tmpfs for the container /tmp. SIGKILL,
container crashes and host snapshots require normal hosting retention controls.
Disconnect kills the active worker group. Compute has a 180 s limit. No audio is
retained for training and no third-party model/API is contacted by this service.

Test with native builds under Xvfb:

```sh
xvfb-run -a python3 Server/test_service.py build-native/nova_cloud_worker build-native/cloud-checks
```

Not validated in FL Studio. A public production endpoint, GPU implementation,
new training data/models, real instrument listening tests and mixed-source
separation remain future work. Do not advertise perfect recreation or 99.99%.
