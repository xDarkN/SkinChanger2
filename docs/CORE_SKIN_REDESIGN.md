# Core Skin Changer Redesign Notes

## Design direction

Core Skin Changer uses a premium black/green dashboard style inspired by the Core Keys brand:

- dark graphite backgrounds
- neon green accent states
- rounded cards and panels
- sidebar-first navigation
- dashboard-style page headers
- clean spacing and readable sections

## Main UI areas

- Dashboard
- Weapons
- Knives
- Gloves
- Music Kits
- Settings / Configs

## Suggested next polish passes

1. Add consistent icon glyphs for sidebar items.
2. Add a proper About panel with version/build information.
3. Improve empty/loading states for skin database pages.
4. Add screenshot assets for GitHub README preview.
5. Consider moving large UI sections out of `menu.h` into smaller page/component headers.

## Engineering cleanup ideas

- Split reusable UI components into dedicated helpers.
- Keep docs and build instructions updated whenever output names change.
- Avoid committing restored NuGet packages, build output, or local `.user` files.