<!-- AUTHOR METADATA
runtime: muse-ai
-->

# ADR 010: What the ADRs in this repo cover

- **Status:** accepted.
- **Date:** 2026-11-17.

## Context

This repository was handed off. The original code — `server.py` and
`cosplay_voicelines.ino` — was written by Earl Justine Dela Cruz and
received as a zip via email on 2026-11-17. The people adding to it from
the handoff onward (Muse, via Murphy) were not present for the original
design and do not know why the original choices were made: why Flask,
why pygame, why this button mapping, why these LED timings.

## Decision

ADRs in this repo record only decisions made from the handoff onward,
about our own additions. We will not write ADRs that speculate about the
rationale of the original code. If a reason for an original choice is
unknown, it stays unknown.

## Consequences

- The original design stays undocumented by choice — its first commit
  preserves the code, not its reasons.
- Future ADRs are numbered from 011 and concern only what was added
  after the handoff.
- If Earl later documents his own reasons, those go in as his words, not
  as our reconstruction.
