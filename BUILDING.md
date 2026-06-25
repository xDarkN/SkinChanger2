# Building Core Skin Changer

This project is a C++20 Visual Studio project that uses several external libraries through NuGet.

## Prerequisites

Install:

1. **Visual Studio 2022** Community, Professional, Enterprise, or Build Tools.
2. **Desktop development with C++** workload.
3. **MSVC v143 - VS 2022 C++ x64/x86 build tools**.
4. Internet access for NuGet restore.

## Dependencies

The project uses NuGet-managed dependencies from `packages.config`, including:

- `nlohmann.json`
- `openssl-native`
- `curl`
- `Microsoft.DXSDK.D3DX`

## Recommended Build Flow

1. Optional but recommended: run `setup_project.bat` to download NuGet and restore packages.
2. Open:

```text
src/ext-cs2-skin-changer.sln
```

3. Select:

```text
Release | x64
```

4. Build Solution.

The compiled executable should be located at:

```text
src\x64\Release\CoreSkinChanger.exe
```

## PowerShell Build

```powershell
cd C:\Projects\SkinChanger2\src

$msbuildPaths = @(
  "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
  "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
  "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
)

$msbuild = $msbuildPaths | Where-Object { Test-Path $_ } | Select-Object -First 1

if (-not $msbuild) {
  Write-Host "MSBuild not found. Install Visual Studio 2022 with Desktop development with C++." -ForegroundColor Red
} else {
  & $msbuild .\ext-cs2-skin-changer.sln /m /p:Configuration=Release /p:Platform=x64
}
```

## Run

```powershell
cd C:\Projects\SkinChanger2\src\x64\Release
.\CoreSkinChanger.exe
```

Press `INSERT` to toggle the menu.

## Expected Warnings

You may see warnings from old external dependencies, including missing `libcurl.pdb` debug symbols or conversion warnings.

Those warnings are not fatal. The target result is:

```text
0 Error(s)
```

## Common Troubleshooting

### Missing OpenSSL DLLs

If Windows reports missing DLLs such as:

```text
libcrypto-1_1-x64.dll
libssl-1_1-x64.dll
```

restore NuGet packages and copy the OpenSSL DLLs from the package folder into:

```text
src\x64\Release
```

### NuGet Restore Failed

Make sure NuGet is available and internet access is working.

Try:

```powershell
cd C:\Projects\SkinChanger2\src
.\nuget.exe restore .\ext-cs2-skin-changer.sln
```

### Header or Linker Errors

Confirm that NuGet packages exist under:

```text
src\packages
```

If OpenSSL headers are missing, verify that the project includes paths similar to:

```text
$(SolutionDir)packages\openssl-native.1.0.1\build\native\include
```

## Analyst Notes

The project uses a standard `.vcxproj` / `.sln` structure with NuGet references. No external makefiles are required for the normal Visual Studio build path.