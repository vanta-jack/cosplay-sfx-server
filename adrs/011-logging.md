<!-- AUTHOR METADATA
runtime: muse-ai
-->

# ADR 011: Add logging on the server and the ESP32

- **Status:** accepted.
- **Date:** 2026-11-17.

## Context

Two things made logging necessary. First, the repo is a handoff: the
person who will debug this next (Earl) was not the person who wrote the
fix, so the system has to explain itself. Second, the ~15s second-press
delay (see `incidents/001-second-press-15s-delay.md`) has an
unestablished cause — the next occurrence needs evidence, not guesses.

## Decision

Add logging on both sides:

- **Server:** Python `logging` — timestamps on every line, console plus
  `server.log`, per-request duration (`served in X ms`), startup
  sound-loading lines, and a `LOG_LEVEL` environment variable
  (default `INFO`).
- **ESP32:** round-trip timing for each request (`Request took X ms`
  over Serial), alongside the existing button-event and response-code
  prints.

Nothing about playback behaviour changes; logging only observes.

## Alternatives considered

- **Print-only (status quo):** rejected — no timestamps, nothing kept
  after the fact, so a delay cannot be investigated afterwards.
- **No logging:** rejected — leaves the incident un-debuggable on the
  next occurrence.
- **Structured JSON logging:** rejected — overkill for a laptop prop
  server; plain timestamped lines are enough and readable without tools.

## Consequences

- A `server.log` file appears next to `server.py`; it is safe to delete
  when the server is stopped.
- Future incidents can be diagnosed by comparing the server's
  `served in X ms` against the ESP32's `Request took X ms` (see
  `docs/how-tos/use-logging-and-debugging.md`).
