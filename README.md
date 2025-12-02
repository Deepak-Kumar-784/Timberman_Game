# Timberman Game

Chop fast, think faster. Timberman is a tiny, twitchy arcade experience where every swing matters. Keep the timer alive by chopping, dodge branches with quick side swaps, and chase your personal best.

This repo packages a simple C++ implementation with lightweight assets for backgrounds, sprites, and sounds — ideal for learning, tinkering, or just having a bit of fun.

## Highlights

- Snappy arcade loop with score and time pressure
- Simple controls: chop and dodge branches
- Lightweight C++ project structure for quick builds
- Assets organized for easy swaps and modding

This project now uses a single-file entrypoint (`src/code.cpp`) for simplicity.

## Project Structure

```
.
├─ src/                 # C++ sources
│  └─ code.cpp          # Single-file entrypoint with game loop
├─ assets/              # Game assets
│  ├─ fonts/            # Fonts (e.g., KOMIKAP_.ttf)
│  ├─ graphics/         # Sprites/backgrounds
│  ├─ image/            # Misc images / placeholders
│  └─ sound/            # SFX (chop/death/timeout)
├─ bin/                 # Local build artifacts (ignored)
├─ .vscode/             # Editor configs and build tasks
├─ LICENSE              # Project license
└─ README.md            # This guide
```

## Requirements

- Windows with PowerShell 5.1 (default in this workspace)
- MSYS2 UCRT64 with `g++` (path used by VS Code task):
  - `C:\msys64\ucrt64\bin\g++.exe`
- VS Code (for the preconfigured build task)

If you use a different compiler or platform, adapt the commands accordingly.

## Build & Run

### Option A — VS Code Task (recommended)

1. Open any source file in `src/` (e.g., `src\code.cpp`).
2. Run the task: `Terminal → Run Build Task → C/C++: g++.exe build active file`.
3. The compiled `.exe` is created alongside the source file by the default task.
4. Optionally copy it to `bin/` or run directly.

### Option B — Manual compile (PowerShell)

From the project root:

```powershell
Set-Location "d:\Git Folder\Timber_prereq"
"C:\msys64\ucrt64\bin\g++.exe" -fdiagnostics-color=always -g src\code.cpp -o bin\timberman.exe
bin\timberman.exe
```

Adjust the entry file if your main is in another source.

## Controls (typical)

- Left/Right input to dodge branches
- Chop to reduce tree height and extend time

The exact key mapping depends on your implementation in `src\code.cpp`. Update this section if you customize controls.

## Customizing Assets

- Replace files under `assets/graphics` to reskin the game
- Drop new fonts into `assets/fonts` and load them in code
- Swap sound effects in `assets/sound` for different feedback

Keep filenames consistent with the code or update paths accordingly.

## Development Notes

- The workspace includes a VS Code C++ build task that compiles the currently open file. For multi-file builds, consider adding a dedicated task or a simple `Makefile`/CMake later.
- Build outputs are kept in `bin/` locally (and typically ignored by git).

## License

See `LICENSE` for details.

If you use third‑party assets, ensure you have the rights to do so and credit appropriately.
