<!-- AUTHOR METADATA
runtime: muse-ai
-->

# Contributing

## Workflow

1. **Open an issue first.** Pick a template in `.github/ISSUE_TEMPLATE/`
   — bug report if something is broken, feature request if you want
   something new. Describe what happened, what you expected, and what
   you saw in the logs.
2. **Work on a branch** named after the fix, e.g.
   `feat/fix-second-press-delay-and-logging`.
3. **Open a pull request** from the branch, linked to the issue
   (`Fixes #N` in the description).
4. **Merging to main only happens once the issue is confirmed resolved.**
   Do not merge the pull request yourself.

## Testing a change

- Run the server from the repo folder (`pip install flask pygame`, then
  `python server.py`) and upload the sketch from the Arduino IDE. The
  README has a step-by-step guide for testing a pull request, written for
  someone with no git experience.
- The how-to guides in `docs/how-tos/` explain the logging and how to
  read it when something goes wrong.

## Documentation

- All documentation lives under `docs/` and follows the Diataxis shape:
  how-to guides in `docs/how-tos/`, incident reports in
  `docs/incidents/`, decision records in `docs/adrs/`.
- Every document carries the author metadata header (see `AGENTS.md`).
- Incident reports mark what is not known as not known. Decision records
  cover only decisions made from the handoff onward.
- Never put a password, token, or key value into a document, an issue,
  or a pull request.

## Agents

Agent-specific governance (authorship, speculation, secrets) lives in
`AGENTS.md`.
