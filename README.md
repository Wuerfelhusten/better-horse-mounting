# Better Horse Mounting

Mount horses from either side and choose your dismount side by looking left or right.

## Build

Requires Windows, Visual Studio 2026 C++ Build Tools (v145 and a Windows SDK), CMake 3.21 or newer, Git, vcpkg and C++23. Set `VCPKG_ROOT`, then run:

```powershell
cmake --preset FLATRIM
cmake --build --preset FLATRIM
```

Output: `build/flatrim/Release/BetterHorseMounting.dll`. The pinned CommonLib port enables SE and AE, and disables VR. Compilation uses `/W4 /WX`. The code style is defined in `.clang-format`.

For a runtime package, place the DLL under `SKSE/Plugins/`, copy the contents of `package/` into the archive root, and include `README.txt` and `LICENSE` (as `LICENSE.txt`). Include the license notices for the dependencies used by the build in the binary archive. Runtime dependencies are installed separately.

## OAR interface

The registered plugin name is `Better Horse Mounting`, distinct from the DLL filename. Both conditions require version `1.0.0.0`:

- `BetterHorseMounting_MountRight`
- `BetterHorseMounting_DismountRight`

Conditions accept either actor in the captured player/horse pair. The selection is latched before native alignment, expires after eight seconds and resets on load/new game. The plugin hooks horse activation and the Activate handler. Addresses and bindings come from CommonLib; the input virtual slot follows its version-dependent declaration.

## Provenance

Based on [SkyHorizon3/SKSE-Example-Plugin](https://github.com/SkyHorizon3/SKSE-Example-Plugin), commit `0e63aae45bd4754d3b12d2ad568386a3c294cc45`.

CommonLib is pinned to [alandtse/CommonLibVR](https://github.com/alandtse/CommonLibVR), commit `896f6d02e465576b6d044b36fd441835aa9c8bf6`. The OAR API comes from [ersh1/OpenAnimationReplacer](https://github.com/ersh1/OpenAnimationReplacer), commit `f4e7688b065175aff70aa523073857911e15aca3`, with explicit includes, REX module lookup and formatting applied.

The formatter configuration comes from [powerof3/SimpleActivateSKSE](https://github.com/powerof3/SimpleActivateSKSE), commit `18d8d1a17afc8cf3a07b0962e2a800d4a57c50d7`. Dependency license notices are included in the binary release archive.

## License

Plugin source is licensed under GPLv3. See [LICENSE](LICENSE). Third-party dependency notices are included in the binary release archive.
