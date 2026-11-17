<!-- AUTHOR METADATA
runtime: muse-ai
-->

# How to use the logging and debugging capabilities

## What gets logged

**Server (`server.py`)** — logs go to the console AND to `server.log` in the
same folder as `server.py`. Each line has a timestamp:

- Startup: each sound file as it loads, then `All sounds loaded.`
- Every `/play` request: the track number, the requesting client, and how
  long the server took to serve it (`served in X ms`).
- Invalid tracks: a warning with the track value.
- A missing sound file at startup: an error naming the file, then the
  server stops (same as before, but now the missing file is named).

**ESP32 (`cosplay_voicelines.ino`)** — logs go to Serial at 115200 baud.
Open the Arduino IDE's Serial Monitor at 115200 to see them:

- Button events: single/double/long press, ultimate trigger.
- Every request: the HTTP response code, and `Request took X ms` — the
  round-trip time from the ESP32's side.

## Changing the log level

```bash
LOG_LEVEL=DEBUG python server.py
```

Default is `INFO`. Valid values: `DEBUG`, `INFO`, `WARNING`, `ERROR`.
An invalid value falls back to `INFO`.

## Debugging a lag: which side is slow?

If a press ever lags again, the two logs together say where the time went:

1. Note the time of the slow press.
2. Check `server.log` for that request's `served in X ms`.
   - Small (a few ms): the server answered fast — the wait was NOT on the
     server side.
   - Large: the server itself was slow; check what else the laptop was
     doing.
3. Check the ESP32 Serial output for the same press's `Request took X ms`.
   - Large while the server's number is small: the wait was on the
     network/ESP32 side (Wi-Fi, hotspot, connection setup).

## Checklist before assuming the code is at fault

- Is the laptop's IP still the one in `serverBaseUrl` in the sketch?
  (Phone hotspots hand out new addresses; a stale IP means requests go
  nowhere until they time out.)
- Is `server.py` still running on the laptop?
- Is the ESP32 still on the Wi-Fi? (`WiFi Disconnected` in Serial.)
- Is anything else using port 5000 on the laptop?

## Notes

- `server.log` grows while the server runs. It is safe to delete when the
  server is stopped; a fresh one is created on the next start.
- Run the server from this folder so the `.wav` files and `server.log`
  land beside it.
