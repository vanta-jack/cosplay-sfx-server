<!-- AUTHOR METADATA
runtime: muse-ai
-->

# cosplay-sfx-server

Lesley cosplay voiceline server. Two buttons on an ESP32 trigger voice lines
played by a Python server on a laptop, with LEDs for the ultimate.

- **Button 1** (GPIO 4): single click = voice line 1, double click = voice
  line 2, hold 2s = voice line 3.
- **Button 2** (GPIO 5): ultimate — alternate ult voice line +
  `lesley_ult.wav`, with a 4-LED charging sequence and a scope light.

## Files

- `server.py` — Flask + pygame server, port 5000, `GET /play?track=N`.
- `cosplay_voicelines.ino` — ESP32 sketch (buttons, LEDs, HTTP requests).
- `docs/how-tos/` — how to use the logging and debugging capabilities.
- `incidents/` — incident reports.
- `adrs/` — architecture decision records. See
  `adrs/010-describe-the-adr.md` first: it explains what these ADRs do and
  do not cover.

## Quick start

Server (on the laptop, in this folder, with the `.wav` files beside it):

```bash
pip install flask pygame
python server.py
```

ESP32: open `cosplay_voicelines.ino` in the Arduino IDE, set the Wi-Fi
credentials and the laptop's current IP in `serverBaseUrl`, upload.

## Origin

The original code is Earl Justine Dela Cruz's, received as a zip via email
on 2026-11-17 and preserved untouched in the first commit. Later commits
are fixes and logging added for handoff debugging; see the incident report
in `incidents/` and the pull request that carried them.
