from flask import Flask, request
import pygame

app = Flask(__name__)
pygame.mixer.init(frequency=44100, size=-16, channels=2)

# Loaded once at startup, so playback on a button press is instant
sounds = {
    "1": pygame.mixer.Sound("lesley_voice1.wav"),
    "2": pygame.mixer.Sound("lesley_voice2.wav"),
    "3": pygame.mixer.Sound("lesley_voice3.wav"),
}
ult_main = pygame.mixer.Sound("lesley_ult.wav")
ult_voices = [pygame.mixer.Sound("ult1.wav"), pygame.mixer.Sound("ult2.wav")]
ult_index = 0


@app.route("/play")
def play_track():
    global ult_index
    track = request.args.get("track")

    if track in sounds:
        print(f"Playing Track {track}")
        sounds[track].play()
    elif track == "4":
        print(f"Playing Ultimate: ult{ult_index + 1}.wav + lesley_ult.wav")
        ult_voices[ult_index].play()
        ult_main.play()
        ult_index = (ult_index + 1) % 2
    else:
        return "Invalid track", 400

    return "Playing", 200


if __name__ == "__main__":
    # threaded=True: each request gets its own thread, so a slow or stuck
    # connection from one press can never queue up the next press behind it.
    # The default dev server is single-threaded, which is the prime suspect
    # for "first press fine, second press lags ~15s".
    app.run(host="0.0.0.0", port=5000, threaded=True)