# Computer Vision — TU Dresden WS 2026/27

Computer Vision exercises in C++17 and OpenCV. The goal is to implement image
processing algorithms and compare the results with OpenCV's built-in functions.

## Exercises

| Folder | Topic | Build target |
| --- | --- | --- |
| `CV_Ex_1_code` | Point operators and histogram equalization | `Ex1_start` |
| `CV_Ex_2_code` | Convolution and image filters — starter | `CV_Ex2_Start` |
| `CV_Ex2_Solution` | Convolution and image filters — solution | `CV_Ex2_Solution` |

Each build target is the `.cpp` filename without its extension. For example,
`CV_Ex2_Solution.cpp` builds `build/Debug/CV_Ex2_Solution.exe`.

## Requirements (Windows)

- Visual Studio 2022 or its Build Tools, with **Desktop development with C++**
  and a Windows SDK installed.
- CMake **3.21 or newer**, available in the terminal.
- An OpenCV installation compatible with the compiler and x64 architecture.
- For the VS Code workflow: VS Code with Microsoft's **C/C++** extension.

The current workspace uses these OpenCV paths:

- CMake package: `E:/OpenCV/opencv/build`
- Runtime DLLs: `E:\OpenCV\opencv\build\x64\vc16\bin`

If OpenCV is installed elsewhere, replace these paths in the terminal commands
below and in `.vscode/tasks.json` and `.vscode/launch.json` when using VS Code.

## Build and run in VS Code

1. Open the **whole `CV_26WS` folder** in VS Code.
2. Open the exercise's `.cpp` file, such as
   `CV_Ex2_Solution/CV_Ex2_Solution.cpp`, and keep it as the active editor tab.
3. Press **Ctrl+Shift+B** to configure and build that exercise.
4. To run it, select **Debug active exercise** in the **Run and Debug** panel
   and press **F5**. F5 also configures and builds automatically, so you can
   skip step 3 when you want to build and run together.

Click an image window to give it keyboard focus. Press **Space** to continue
to the next result or **Esc** to quit.

These shortcuts use the existing `.vscode/tasks.json` and `.vscode/launch.json`.
The `.vscode` folder is ignored by Git; if it is absent in another checkout,
use the terminal workflow below.

## Build and run in PowerShell

Open a PowerShell terminal in the **`CV_26WS` root folder**. The examples below
build and run the exercise 2 solution.

### 1. Configure the project

Run this before the first build, after adding an exercise or its images folder,
or after changing the OpenCV location:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DOpenCV_DIR=E:/OpenCV/opencv/build
```

### 2. Build the exercise

```powershell
cmake --build build --config Debug --target CV_Ex2_Solution
```

Replace `CV_Ex2_Solution` with another target from the table above to build a
different exercise. To build all exercises:

```powershell
cmake --build build --config Debug
```

The executables are written to the root **`build/Debug`** folder. Building
compiles the program; it does not start it.

### 3. Run the exercise

The solution must run with its **exercise's `build` folder as the working
directory** so that relative image paths resolve correctly. This is different
from the root `build/Debug` folder that contains the executable.

From the `CV_26WS` root folder, run:

```powershell
$env:PATH = "E:\OpenCV\opencv\build\x64\vc16\bin;$env:PATH"
Push-Location .\CV_Ex2_Solution\build
& ..\..\build\Debug\CV_Ex2_Solution.exe
Pop-Location
```

The first line makes OpenCV's DLLs available for this terminal session.
`Pop-Location` returns to the root folder after the program exits. For another
exercise, change both the folder in `Push-Location` and the executable name.
In VS Code, F5 handles the working directory and DLL path automatically.

## Image files

Keep an `images` folder next to each exercise's `.cpp` file. The build copies
it into that exercise's `build/images` folder. Running from the exercise's
`build` folder then supports both `./images/...` and `../images/...` paths.

The exercise 2 starter locates `images/fruits_noisy.jpg` next to its source
file, using the source path recorded at build time. It also works when launched
from another folder. If you move the project, rebuild it to update that path.

The exercise 2 solution loads `../images/fruits_noisy.jpg`. Its images are
already present in the current workspace. If `CV_Ex2_Solution/images` is missing
in another copy, run this once from the root folder, then configure and build:

```powershell
Copy-Item -Path .\CV_Ex_2_code\images -Destination .\CV_Ex2_Solution -Recurse -Force
```

## Troubleshooting

- **`cmake` is not recognized:** make sure CMake is installed and on `PATH`,
  then reopen the terminal or VS Code.
- **Visual Studio compiler or Windows SDK cannot be found:** install the C++
  workload and Windows SDK through the Visual Studio Installer, then configure again.
- **OpenCV cannot be found during configuration:** check that `OpenCV_DIR`
  points to the folder containing `OpenCVConfig.cmake`.
- **An OpenCV DLL is missing when running:** check the runtime DLL folder in
  the PowerShell `PATH` command or `.vscode/launch.json`.
- **The program cannot load an image or reports an empty-image assertion:**
  check that the image exists and that the working directory is the exercise's
  `build` folder. If you just added an `images` folder, configure and build again.
- **VS Code builds the wrong target:** activate the intended `.cpp` tab before
  pressing Ctrl+Shift+B or F5.

## Add a new exercise

1. Create an exercise folder directly inside `CV_26WS`.
2. Put the standalone `.cpp` file and its `images` folder inside it.
3. Open the `.cpp` file and press F5, or configure and build from PowerShell
   using the filename without `.cpp` as the target.

The root CMake setup discovers these files automatically. Every `.cpp` file
becomes a separate executable, so filenames must be unique across exercises.
A program split across multiple implementation files, or requiring libraries
beyond OpenCV, needs an explicit CMake target instead.
