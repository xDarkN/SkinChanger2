# Core Skin Changer

Core Skin Changer is a Windows C++20 desktop application with a premium black/green Core Keys-inspired interface.

This branch focuses on a full visual rebrand, UI/UX cleanup, page layout polish, and cleaner project documentation.

## Current remix status

- Rebranded visible app identity to **Core Skin Changer**.
- Renamed the Release output executable to `CoreSkinChanger.exe`.
- Added a Core-style black/green theme.
- Added a dashboard-oriented shell with sidebar navigation.
- Refreshed major UI pages for a more polished product feel.
- Moved app config storage toward `%APPDATA%\Core Skin Changer\`.

## Build requirements

- Visual Studio 2022
- Desktop development with C++ workload
- MSVC v143 x64/x86 build tools
- NuGet restore enabled

## Build flow

1. Optionally run `setup_project.bat` to restore NuGet packages.
2. Open `src/ext-cs2-skin-changer.sln` in Visual Studio 2022.
3. Select `Release` configuration and `x64` platform.
4. Build the solution.
5. Run `src/x64/Release/CoreSkinChanger.exe`.

See [`BUILDING.md`](BUILDING.md) for dependency and troubleshooting details.

## Dependencies

The project uses NuGet packages configured through `packages.config`:

- nlohmann.json
- OpenSSL
- curl
- Microsoft.DXSDK.D3DX

## Repository notes

Temporary patch scripts and generated backup folders are ignored by Git:

- `core_skin_stage*.ps1`
- `_backup_core_skin_stage*/`

Build outputs, Visual Studio user files, restored NuGet packages, and binaries should stay untracked.