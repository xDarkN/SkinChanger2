# Core Skin Changer Release Checklist

Use this checklist before merging `core-skin-remix` into `master`.

## Git State

```powershell
cd C:\Projects\SkinChanger2
git status
git log --oneline -10
```

Expected:

```text
nothing to commit, working tree clean
```

## Build

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

Expected:

```text
0 Error(s)
```

Warnings from external packages are acceptable if there are no errors.

## Runtime Smoke Test

```powershell
cd C:\Projects\SkinChanger2\src\x64\Release
.\CoreSkinChanger.exe
```

Check:

- App starts.
- Console says `Core Skin Changer Started`.
- Press `INSERT` and menu opens.
- Dashboard renders correctly.
- Sidebar tabs switch pages.
- Search boxes still accept input.
- Existing cards/buttons still respond.

## Files To Avoid Committing

These should remain local only:

```text
core_skin_stage*.ps1
_backup_core_skin_stage*/
menu_broken_stage*_debug.h
RenderWeaponTab_current.txt
```

## Merge To Master

Only after build and smoke test pass:

```powershell
cd C:\Projects\SkinChanger2

git checkout master
git pull origin master
git merge core-skin-remix
git push origin master
git checkout core-skin-remix
```

## Optional Tag

```powershell
git tag core-ui-v3
git push origin core-ui-v3
```