# Imjector
Imjector is a DLL injector developed in C++ with an front end created using ImGui. Program allows users to explicitly choose their preferred injection method from a variety of options.

![](https://github.com/epsilonr/imjector/blob/main/img/ss1.png)
![](https://github.com/epsilonr/imjector/blob/main/img/ss0.png)

## Building
If you don't want to tackle with building you can download pre-compiled binaries from [here](https://github.com/rft0/imjector/releases).

#### Requirements
* MSVC
* Clang (Optional)
* GNU Make (Optional)

#### Process
* Get [ImGui](https://github.com/ocornut/imgui/tree/docking) with d3d9 backend and copy them into src/imgui.
* Download GNU Make from choco and LLVM. (Instead you can use cl.exe too)
* Run `make`.
