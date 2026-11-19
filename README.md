# AssemblerProject

A **Hack Assembler** implemented in **C** using **Visual Studio** (`HackAssembler.sln`), translating Hack assembly (`.asm`) source files into `.hack` machine code.

## Architecture & Modules

- **`HackAssembler.c`:** Coordinates assembly passes, file I/O, and translation flow.
- **`Parser.c` / `Parser.h`:** Reads and parses assembly commands, labels, and fields.
- **`Code.c` / `Code.h`:** Encodes assembly fields/instructions into `.hack` output representations.
- **`SymbolTable.c` / `SymbolTable.h`:** Manages predefined symbols, labels, and variable addresses.

## Test Suites

Sample `.asm` inputs and expected `.hack` outputs are included under `inputs and outputs for testing/`:
- `add`
- `max`
- `pong`
- `rect`

## Build & Run (Windows / Visual Studio)

1. Open `HackAssembler.sln` in Visual Studio.
2. Build the `HackAssembler` project (`x64` Debug/Release) to generate `HackAssembler.exe`.
3. Run the executable on a target `.asm` file to produce the corresponding `.hack` output.

## Author

Lihi Elias — B.Sc. Software Engineering Student
