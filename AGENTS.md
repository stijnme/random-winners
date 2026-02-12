# AGENTS.md - Development Guidelines for random-winners

This document provides guidelines for agents working on the random-winners C project, which randomly selects winners from a list of participants.

## Build Commands

### Compilation
```bash
# Standard build with warnings and optimization
clang -Wall -Wextra -O2 -o random-winners random-winners.c

# Debug build (enables debug output)
clang -Wall -Wextra -DDEBUG -o random-winners-debug random-winners.c

# Run the build script
./build.sh
```

### Cleaning Build Artifacts
```bash
# Remove compiled executables
rm -f random-winners random-winners-debug
```

## Testing Commands

### Manual Testing
Since this is a simple utility without automated tests, use these manual testing approaches:

```bash
# Test with sample participants file
./random-winners particpants.txt 3

# Test with invalid input (should show usage)
./random-winners

# Test with too many winners (should error)
./random-winners particpants.txt 100

# Test with empty file (should error)
echo "" > empty.txt && ./random-winners empty.txt 1
```

### Valgrind Memory Testing
```bash
# Check for memory leaks
valgrind --leak-check=full ./random-winners particpants.txt 2
```

## Code Style Guidelines

### File Structure
- Single main source file: `random-winners.c`
- Build script: `build.sh`
- Input files: Text files with one participant per line

### Naming Conventions
- **Functions**: `snake_case` (e.g., `read_lines`, `free_lines`)
- **Variables**: `snake_case` (e.g., `line_count`, `char **lines`)
- **Constants**: `UPPER_CASE` with underscores (e.g., `MAX_LINE_LENGTH`)

### Code Formatting
- **Indentation**: 2 spaces (no tabs)
- **Braces**: Opening brace on same line as function/control statement
- **Line Length**: Under 100 characters when possible
- **Spacing**: One space after commas, spaces around operators

### Includes and Headers
- Group standard headers first, in alphabetical order
- Include ALL necessary headers to avoid implicit function declarations
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // Required for getpid()
```

### Memory Management
- Always check return values of `malloc`/`realloc`
- Free all allocated memory before program exit
- Use dedicated cleanup functions like `free_lines()`
- Return error codes or NULL on allocation failure

### Error Handling
- Use negative integers for error codes
- Use `fprintf(stderr, ...)` for error output
- Validate command line arguments and business logic constraints
- Provide helpful error messages with context

### Function Design
- Use `const` for input parameters that aren't modified
- Pass output parameters by pointer
- Return error codes, not just success/failure
- Keep functions focused and under 50 lines
- Document purpose, parameters, and return values in comments

### String Handling
- Define maximum buffer sizes as constants
- Use `fgets()` with size limits to prevent overflow
- Always null-terminate strings
- Remove trailing whitespace manually

### Random Number Generation
- Seed with combination of time and process ID for better entropy
- Implement Fisher-Yates shuffle correctly
- Use `unsigned int` for seed values

### Debug Support
- Use `#ifdef DEBUG` for debug output
- Compile with `-DDEBUG` flag to enable
- Keep debug code minimal and focused

### Command Line Interface
- Provide clear usage instructions with examples
- Use `atoi()` for integer conversion with validation
- Provide specific error messages for invalid inputs

### File I/O
- Always check `fopen()` return value
- Close files immediately after use
- Handle different line endings (\n, \r\n)
- Read line by line with `fgets()`

### Constants and Magic Numbers
- Replace magic numbers with named constants
- Use meaningful names: `INITIAL_CAPACITY`, `MAX_LINE_LENGTH`

### Code Comments
- Document function purpose above definitions
- Explain complex logic and algorithms
- Use TODO/FIXME for known issues

### Portability
- Use C99 standard features
- Avoid platform-specific code
- Test compilation on different systems
- Treat compiler warnings as errors

### Git Workflow
- Use imperative mood for commit messages
- Keep first line under 50 characters
- Use descriptive branch names

### Testing Checklist
Before committing changes:
- [ ] Compiles without warnings (`clang -Wall -Wextra -O2`)
- [ ] Runs basic functionality test
- [ ] Handles error cases appropriately
- [ ] No memory leaks (valgrind check)
- [ ] Works with edge cases (empty file, single participant)

### Project Structure
```
random-winners/
├── random-winners.c    # Main source file
├── build.sh           # Build script
├── particpants.txt    # Sample participants file
├── AGENTS.md         # This file
└── README.md         # User documentation
```

This codebase follows a simple, maintainable structure suitable for small utilities. Focus on correctness, safety, and clarity in all modifications.
