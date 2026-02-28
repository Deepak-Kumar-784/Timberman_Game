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
- **SFML 3** — the code targets the SFML 3 API (uses `sf::Sprite` constructor, `Font::openFromFile`, `std::optional<Event>`, etc.)
- MSYS2 UCRT64 with `g++` and SFML 3 libraries installed (path used by VS Code task: `C:\msys64\ucrt64\bin\g++.exe`)
- VS Code (for the preconfigured build task)

If you use a different compiler or platform, adapt the commands accordingly.

## Build & Run

### Option A — VS Code Task (recommended)

1. Open any source file in `src/` (e.g., `src\code.cpp`).
2. Run the task: `Terminal → Run Build Task → C/C++: g++.exe build active file`.
3. The compiled `.exe` is created alongside the source file by the default task.
4. Optionally copy it to `bin/` or run directly.

### Option B — Manual compile (simple, produces `a.exe`)

From the `src` folder (assets are loaded via `../assets/...`):

```powershell
Set-Location "<repo-root>\src"
g++ code.cpp -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio
./a.exe
```

Replace `<repo-root>` with the actual path where you cloned this repository.

Notes:

- This project targets **SFML 3**; ensure your installed SFML libraries are version 3.x.
- The default `g++` behavior here creates `a.exe` in `src/`.
- Running from `src/` ensures `../assets/...` paths resolve correctly.
- To name the executable explicitly, use `-o` (e.g., `-o timber.exe`).

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

- The codebase targets **SFML 3** — do not mix with SFML 2 headers or libraries.- You can compile directly from `src/` (creating `a.exe`) or use the VS Code task.
- If you prefer keeping binaries in `bin/`, compile with `-o ..\bin\timber.exe` and run from `bin/`.
- For multi-file builds, consider adding a dedicated task or `Makefile`/CMake later.

## License

See `LICENSE` for details.

If you use third‑party assets, ensure you have the rights to do so and credit appropriately.
