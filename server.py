import logging
import os
import time

from flask import Flask, request
import pygame

# --- Logging ---------------------------------------------------------------
# Logs go to the console AND to server.log next to this file, so a delay can
# be investigated after the fact. Level via the LOG_LEVEL env var
# (default INFO; e.g. LOG_LEVEL=DEBUG python server.py).
LOG_LEVEL = os.environ.get("LOG_LEVEL", "INFO").upper()
logging.basicConfig(
    level=getattr(logging, LOG_LEVEL, logging.INFO),
    format="%(asctime)s [%(levelname)s] %(message)s",
    handlers=[
        logging.StreamHandler(),
        logging.FileHandler("server.log"),
    ],
)
logger = logging.getLogger("cosplay-sfx-server")

app = Flask(__name__)
pygame.mixer.init(frequency=44100, size=-16, channels=2)


def load_sound(path):
    """Load one wav file, logging success or failure by name."""
    logger.info("Loading sound: %s", path)
    try:
        sound = pygame.mixer.Sound(path)
    except Exception:
        logger.exception("Failed to load sound: %s", path)
        raise
    logger.info("Loaded sound: %s", path)
    return sound


# Loaded once at startup, so playback on a button press is instant
sounds = {
    "1": load_sound("lesley_voice1.wav"),
    "2": load_sound("lesley_voice2.wav"),
    "3": load_sound("lesley_voice3.wav"),
}
ult_main = load_sound("lesley_ult.wav")
ult_voices = [load_sound("ult1.wav"), load_sound("ult2.wav")]
ult_index = 0
logger.info("All sounds loaded. Ready on port 5000.")


@app.route("/play")
def play_track():
    global ult_index
    started = time.monotonic()
    track = request.args.get("track")
    client = request.remote_addr

    if track in sounds:
        logger.info("track=%s client=%s -> playing lesley_voice%s.wav", track, client, track)
        sounds[track].play()
    elif track == "4":
        logger.info("track=%s client=%s -> playing ult%d.wav + lesley_ult.wav", track, client, ult_index + 1)
        ult_voices[ult_index].play()
        ult_main.play()
        ult_index = (ult_index + 1) % 2
    else:
        logger.warning("track=%s client=%s -> invalid track (400)", track, client)
        return "Invalid track", 400

    elapsed_ms = (time.monotonic() - started) * 1000
    logger.info("track=%s client=%s -> served in %.1f ms", track, client, elapsed_ms)
    return "Playing", 200


if __name__ == "__main__":
    # threaded=True: each request gets its own thread, so a slow or stuck
    # connection from one press can never queue up the next press behind it.
    # The default dev server is single-threaded, which is the prime suspect
    # for "first press fine, second press lags ~15s".
    app.run(host="0.0.0.0", port=5000, threaded=True)
