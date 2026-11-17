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
- `docs/` — all documentation: how-to guides in `docs/how-tos/`, incident
  reports in `docs/incidents/`, decision records in `docs/adrs/`.

## Quick start

Server (on the laptop, in this folder, with the `.wav` files beside it):

```bash
pip install flask pygame
python server.py
```

ESP32: open `cosplay_voicelines.ino` in the Arduino IDE, set the Wi-Fi
credentials and the laptop's current IP in `serverBaseUrl`, upload.

## Testing a pull request (no git experience needed)

Merging to main only happens once the issue is confirmed resolved, so
changes are tested on their branch first — never on main.

1. Install [GitHub Desktop](https://desktop.github.com/) and sign in with
   your GitHub account.
2. On this repo's GitHub page, click **Code → Open with GitHub Desktop →
   Clone**.
3. In GitHub Desktop, click **Current Branch** and pick the branch named in
   the pull request — for the current fix, that is
   `feat/fix-second-press-delay-and-logging`.
4. Run the server from the cloned folder, as in Quick start above.
5. Upload the sketch from the Arduino IDE, as in Quick start above.
6. Press the buttons and watch what happens. If something lags, check the
   logs — the how-to guides in `docs/how-tos/` explain where they are and
   how to read them.
7. Report back on the pull request: say whether the problem is gone, and
   paste the log lines from the slow press if it is not.

If you prefer the terminal instead of GitHub Desktop:

```bash
git clone https://github.com/vanta-jack/cosplay-sfx-server.git
cd cosplay-sfx-server
git checkout feat/fix-second-press-delay-and-logging
```

Do not merge the pull request yourself — merging happens after the issue
is confirmed resolved.

## Origin

The original code is Earl Justine Dela Cruz's, received as a zip via email
on 2026-11-17 and preserved untouched in the first commit. Later commits
are fixes and logging added for handoff debugging; see the incident reports
in `docs/incidents/` and the pull request that carried them.
