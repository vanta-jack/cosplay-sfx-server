<!-- AUTHOR METADATA
runtime: muse-ai
-->

# Incident 001: second button press lags ~15s

- **Status:** open — root cause NOT established.
- **Reported:** 2026-11-17, by the cosplay team (via Murphy).
- **System:** ESP32 sketch (`cosplay_voicelines.ino`) + Flask server
  (`server.py`), Lesley cosplay voice lines.

## Symptom

The first button press plays its voice line immediately. The second press
lags behind by approximately 15 seconds before its voice line plays.

## What was tested

The server code was run on a separate machine (Flask 3.1.3, pygame 2.6.1)
with silent stand-in `.wav` files, since the real sound files were not in
the handed-off zip:

- Rapid sequential requests: ~2 ms each.
- A request while a sound was playing: ~2 ms.
- Concurrent requests: ~2 ms each.
- A lingering keep-alive connection left open: the next request still
  answered in ~0 ms.
- Requests mimicking the ESP32 exactly (new connection, headers-only read,
  abrupt close with unread body), 5 pairs: second request ~0 ms each.

**The ~15s delay did NOT reproduce on the test machine.** The server's
request handling is fast under every pattern tried, including the ESP32's
exact connection behaviour.

## Hypotheses (all UNCONFIRMED — do not treat any as the cause)

1. **Single-threaded Flask dev server.** `app.run()` with no
   `threaded=True` serves one connection at a time. If the server parks on
   the first connection, the second request queues behind it. Not
   reproduced on Flask 3.1.3; the laptop's Flask version is unknown.
2. **ESP32 blocking in `http.GET()`.** The sketch's request call is
   synchronous — the whole button loop freezes until the reply arrives or
   the request times out. A slow reply on the hotspot would feel exactly
   like "15s before another line can play".
3. **Stale laptop IP.** The sketch hardcodes the laptop's hotspot address.
   If the hotspot handed the laptop a new address, requests go nowhere
   until they time out.
4. **Hotspot / network path.** Phone-hotspot retransmits or client
   handling could delay the reply; unmeasured.

## Mitigations applied (feat branch)

These target the most likely mechanisms; they are NOT a confirmed cure,
because the root cause is not established:

- Server: `app.run(..., threaded=True)` — one thread per request, so a
  stuck first connection cannot queue the second press behind it.
- ESP32: `http.setTimeout(3000)` — a dead request fails fast instead of
  freezing the button loop; `Connection: close` header plus reading the
  reply body, so connections close cleanly.
- Logging on both sides (server request durations in `server.log`, ESP32
  round-trip times over Serial) so the next occurrence produces evidence
  instead of a guess. See `docs/how-tos/use-logging-and-debugging.md`.

## What would confirm the cause

- Reproduce the lag with the new logging on, then compare the server's
  `served in X ms` against the ESP32's `Request took X ms` for the same
  press (see the how-to).
- Check the Flask version on the laptop and the laptop's current hotspot
  IP against the sketch.
