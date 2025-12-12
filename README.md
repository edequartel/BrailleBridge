# BrailleBridge — Developer & Deployment Manual

BrailleBridge is a Windows application that acts as a bridge between
braille hardware (via SAM / Dolphin), modern .NET applications, and
external interfaces (e.g. web, services, or educational tooling).

This document explains:
- the project structure
- how to build and run BrailleBridge
- how to publish it correctly
- how to create a working installer
- common pitfalls (especially .NET runtime issues)

---

## 1. Project overview

BrailleBridge is a **WPF application** built on:

- .NET 8 (Windows Desktop)
- SAM (32-bit and 64-bit native DLLs)
- JSON-based keymaps
- Windows-only deployment

Key goals:
- Communicate with braille displays
- Abstract SAM-specific details
- Provide a stable base for educational and accessibility tooling

---

## 2. Requirements

### Development machine
- Windows 10 or 11 (64-bit)
- Visual Studio 2022 or newer
- .NET SDK 8.x installed
- SAM SDK / runtime available
- Inno Setup 6.x (for installer)

### Target machine
- Windows 10 or 11 (64-bit)
- Either:
  - .NET 8 Desktop Runtime installed, **or**
  - Self-contained build (recommended)

---

## 3. Repository structure (typical)

```

BrailleBridge/
├─ BrailleBridge.sln
├─ WPF_BS_SAM_Braille/
│  ├─ WPF_BS_SAM_Braille.csproj
│  ├─ App.xaml
│  ├─ MainWindow.xaml
│  ├─ Services/
│  ├─ ViewModels/
│  ├─ sam/
│  │  ├─ SAM32.dll
│  │  └─ SAM64.dll
│  └─ keymaps/
│     └─ *.json
├─ installer/
│  ├─ BrailleBridge.iss
│  └─ assets/
└─ README.md

```

---

## 4. Running BrailleBridge in development

### Important: Debug builds are **not deployable**

Debug output is for **local testing only**.

You can run locally via:
- Visual Studio → Start Debugging
- or by launching:
```

bin\Debug\net8.0-windows\WPF_BS_SAM_Braille.exe

````

⚠️ **Do not distribute Debug builds**  
They may trigger .NET runtime install dialogs on other machines.

---

## 5. Publishing BrailleBridge (correct way)

### Why publish?

Only **publish output** creates a proper `.NET apphost` that:
- correctly detects installed runtimes
- works on other machines
- can be safely installed

---

### 5.1 Framework-dependent publish (requires .NET 8 Desktop Runtime)

From the folder containing `WPF_BS_SAM_Braille.csproj`:

```cmd
dotnet publish -c Release -r win-x64 --self-contained false
````

Output folder:

```
bin\Release\net8.0-windows\win-x64\publish\
```

Contents include:

* `WPF_BS_SAM_Braille.exe`
* `.runtimeconfig.json`
* `.deps.json`
* all required DLLs
* `keymaps\` folder

---

### 5.2 Self-contained publish (recommended)

This version **does not require .NET to be installed** on the target PC.

```cmd
dotnet publish -c Release -r win-x64 --self-contained true
```

Advantages:

* No .NET runtime installer needed
* No “Install .NET Desktop Runtime” dialog
* Most reliable for schools and managed PCs

Trade-off:

* Larger output size

---

## 6. Verifying publish output

Always test **before** making an installer:

```cmd
"C:\...\bin\Release\net8.0-windows\win-x64\publish\WPF_BS_SAM_Braille.exe"
```

If this EXE:

* starts without errors
* does not ask for .NET

Then it is safe to install.

---

## 7. Creating the installer (Inno Setup)

### Golden rule

> **Never install from `bin\Debug` or `bin\Release` directly.
> Always install from the `publish` folder.**

---

### 7.1 Inno Setup — self-contained build (recommended)

```ini
[Files]
Source: "C:\Developer\Software development\BrailleBridge\bin\Release\net8.0-windows\win-x64\publish\*";
DestDir: "{app}";
Flags: recursesubdirs createallsubdirs ignoreversion
```

This copies:

* all files
* all subfolders
* preserves structure

No .NET runtime logic is needed.

---

### 7.2 Inno Setup — framework-dependent build

If you choose framework-dependent publish, you must:

* install .NET 8 Desktop Runtime **before** launching the app

However, this approach is more fragile and **not recommended** unless required.

---

## 8. Common problems & solutions

### “You must install .NET Desktop Runtime” dialog

Causes:

* Installing Debug build
* Missing `.deps.json` / `.runtimeconfig.json`
* Wrong EXE launched (old shortcut)
* Installer copied only some files

Fix:

* Publish the app
* Install the **entire publish folder**
* Prefer self-contained build

---

### App works on dev PC but not on another PC

Cause:

* Debug output used
* Missing native DLLs
* Missing subfolders (keymaps)

Fix:

* Use recursive copy from publish folder
* Verify `keymaps\` exists under `{app}`

---

## 9. SAM DLL notes

* `SAM64.dll` → requires 64-bit process
* A 32-bit app cannot load it
* Self-contained win-x64 ensures correct architecture

Only ship `SAM32.dll` if you truly support 32-bit.

---

## 10. Recommended production setup

**Best practice for BrailleBridge:**

* Publish:

  ```
  dotnet publish -c Release -r win-x64 --self-contained true
  ```
* Installer:

  * Copy entire publish folder recursively
  * No runtime installers
* Result:

  * One installer
  * Works everywhere
  * No .NET prompts

---

## 11. Maintenance tips

* Always republish after:

  * updating .NET SDK
  * changing native DLLs
  * changing keymaps
* Keep installer paths updated to the latest publish folder
* Never mix Debug and Publish artifacts

---

## 12. License / attribution

BrailleBridge is developed for educational and accessibility use.
Refer to the repository license and third-party licenses (SAM, .NET)
for redistribution terms.

---

## 13. Contact

Repository owner:
**Eric de Quartel**
[https://github.com/edequartel](https://github.com/edequartel)

For issues, use GitHub Issues or project documentation.
