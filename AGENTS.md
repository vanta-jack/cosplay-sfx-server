<!-- AUTHOR METADATA
runtime: muse-ai
-->

# AGENTS.md

Notes for agents (human or AI) working in this repo. Keep it short; the
code and the docs in `docs/` are the source of truth.

## Authorship is provenance

- The original code is Earl Justine Dela Cruz's, preserved untouched in
  the first commit. Do not rewrite it to look like yours, and do not
  present someone else's work as your own.
- Every document an agent writes carries this header at the top, so
  machine-written text is always identifiable:

  ```html
  <!-- AUTHOR METADATA
  runtime: muse-ai
  -->
  ```

- An agent's words are filed as evidence, not as the author's own. If a
  decision was made by someone else, say who made it.

## Do not speculate

- Incident reports state what is known and mark what is not. A cause that
  has not been reproduced is a hypothesis, never a conclusion.
- Decision records cover only decisions made from the handoff onward,
  about our own additions. Never write a decision record that guesses at
  the rationale of the original code.

## No secrets in repo text

- Never write a password, token, or key value into a document, an issue,
  or a pull request. If a credential is hardcoded in the code, flag it
  without quoting the value, and leave the code itself as received unless
  told otherwise.

## Workflow

- Issue first, then a branch, then a pull request linked to the issue.
  Merging to main only happens once the issue is confirmed resolved.
- Keep commits small and scoped: one concern per commit.

## Documentation (Diataxis)

- All documentation lives under `docs/`: how-to guides in `docs/how-tos/`,
  incident reports in `docs/incidents/`, decision records in `docs/adrs/`.
- How-to guides are task-oriented: how to do one thing, step by step, no
  background essays.
- Root files (`README.md`, this file, `CONTRIBUTING.md`) point to
  directories, not to specific files, so the docs can move without the
  root files going stale.
