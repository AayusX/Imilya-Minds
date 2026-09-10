# Quantum Search — Imilya Minds

> A futuristic-themed desktop search utility with an **offline Q&A database**,
> search history, and a sleek cyberpunk loading screen.

Part of the **Imilya Minds** collection: a C++/Qt6 application that searches a
local knowledge base, remembers your history, and presents results in animated,
card-based panels.

## Features

- 🌌 **Animated loading screen** with progress bar
- 🔎 **Search engine selector** interface
- 📚 **Offline Q&A database** — works without internet
- 🕘 **Search history** with persistence
- 🃏 **Card-based results** with animations
- 🌑 **Dark futuristic UI** — cyan accents on dark theme

## Getting Started

```bash
cd mypro/cpp-search-app
mkdir build && cd build
cmake ..
make
./bin/CPPSearchApp
```

## Tech Stack

- **C++17**, **Qt 6** (Core, Widgets), Qt MOC (AUTOMOC)
- **CMake** ≥ 3.16

## Project Structure

```
└── mypro/cpp-search-app/
    ├── CMakeLists.txt
    ├── build_and_run.sh
    ├── demo_features.md       # feature tour
    ├── logo.png
    ├── include/               # headers (MainWindow.h, …)
    └── src/                   # implementation
```

## License

See the LICENSE file in this repository.