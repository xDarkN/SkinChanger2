# Core Skin Changer

A C++20 desktop project with a premium black/green **Core Skin Changer** interface.

This branch focuses on a visual and UX remix:
- Core black/green dashboard shell
- Premium sidebar and dashboard layout
- Cleaner shared UI components
- Rebranded executable: `CoreSkinChanger.exe`
- Safer repository docs and build instructions

> This repository is being worked on in stages. The current focus is UI/UX, branding, docs, and build stability.

## Current Branch

Active development branch:

```text
core-skin-remix
```

Recommended workflow:

```powershell
git checkout core-skin-remix
git pull origin core-skin-remix
```

## Build Output

The Release x64 build outputs:

```text
src\x64\Release\CoreSkinChanger.exe
```

## Quick Build

Use Visual Studio 2022:

1. Open `src/ext-cs2-skin-changer.sln`
2. Select `Release`
3. Select `x64`
4. Build Solution

Or use PowerShell/MSBuild:

```powershell
cd C:\Projects\SkinChanger2\src

$msbuildPaths = @(
  "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
  "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
  "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
)

$msbuild = $msbuildPaths | Where-Object { Test-Path $_ } | Select-Object -First 1
& $msbuild .\ext-cs2-skin-changer.sln /m /p:Configuration=Release /p:Platform=x64
```

## Run

```powershell
cd C:\Projects\SkinChanger2\src\x64\Release
.\CoreSkinChanger.exe
```

Press `INSERT` to toggle the menu.

## Local Patch Files

Local patch scripts and temporary backups are ignored by Git:

```text
core_skin_stage*.ps1
_backup_core_skin_stage*/
menu_broken_stage*_debug.h
RenderWeaponTab_current.txt
```

## Notes

The project may show warnings from external packages, especially missing debug symbols from older dependencies. The important build result is:

```text
0 Error(s)
```

For detailed setup instructions, see [`BUILDING.md`](BUILDING.md).

For the current redesign notes, see [`docs/CORE_SKIN_REDESIGN.md`](docs/CORE_SKIN_REDESIGN.md).

For release checks, see [`docs/RELEASE_CHECKLIST.md`](docs/RELEASE_CHECKLIST.md).