# Core Skin Changer Redesign Notes

## Goal

Turn the application interface into a premium black/green desktop dashboard aligned with the Core Keys visual direction.

## Completed

- Rebranded visible app identity to **Core Skin Changer**
- Renamed Release output to `CoreSkinChanger.exe`
- Added Core-style black/green theme
- Added premium dashboard shell
- Added cleaner sidebar/topbar styling
- Polished dashboard cards and status chips
- Added local patch/backups to `.gitignore`
- Stabilized the workflow around small staged commits

## Visual Direction

Palette:

```text
Background: deep black
Panels: dark graphite / green-tinted black
Accent: neon Core green
Text: off-white
Secondary text: muted grey-green
Borders: low-opacity green
```

Design principles:

- Keep the UI functional first.
- Prefer shared component polish over replacing full functional pages.
- Avoid fake/non-functional mock pages.
- Avoid breaking existing selection/search/grid logic.
- Patch one risky page at a time.

## Current Stable State

The stable branch is:

```text
core-skin-remix
```

Latest major visual stage:

```text
Overhaul Core Skin Changer dashboard
```

## Next Recommended Work

1. Polish shared UI components safely.
2. Improve `SkinCard` styling without changing click behavior.
3. Improve page spacing without replacing page logic.
4. Add README screenshot assets.
5. Prepare a release tag after full test pass.

## Lessons Learned

The safest approach is:

```text
Do not replace full pages blindly.
Patch shared visual components first.
Keep existing data/search/click logic intact.
Build after every change.
Commit only stable stages.
```