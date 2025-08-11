# Using This Project with Augment VS Code Extension

This guide helps you use the crypto project as a base for working with the Augment AI coding assistant in Visual Studio Code.

## Prerequisites

1. **Visual Studio Code** installed
2. **Augment extension** installed from VS Code marketplace
3. **C++ extension** for VS Code (for IntelliSense)
4. A C++ compiler (g++, clang, or MSVC)

## Setting Up the Project

### 1. Open in VS Code
```bash
cd /workspace/my-crypto-project
code .
```

### 2. Configure Augment
1. Open the Augment panel (usually on the sidebar)
2. Ensure Augment is connected and authenticated
3. The extension should automatically detect this as a C++ project

## Using Augment with This Codebase

### Example Tasks to Try with Augment:

#### 1. Understanding the Code
Ask Augment to:
- "Explain how the encryption algorithms in the encryption/ folder work"
- "What does the packer.cpp file do?"
- "Show me the flow of the menu system"

#### 2. Code Completion
Start typing in any file and Augment will suggest:
- Function implementations
- Variable names
- Comments and documentation

#### 3. Refactoring
Ask Augment to help:
- "Refactor the menu system to use a class-based approach"
- "Simplify the handleFileEncryption function"
- "Add error handling to the file operations"

#### 4. Adding Features
Use Augment to:
- "Add a new encryption algorithm"
- "Implement file compression before encryption"
- "Create unit tests for the SimpleEncryptor class"

#### 5. Debugging
Ask Augment:
- "Why might this encryption function fail?"
- "Help me debug this segmentation fault"
- "What are potential security issues in this code?"

## Best Practices with Augment

### 1. Context-Aware Questions
Augment works best when you:
- Have relevant files open
- Provide specific context
- Reference actual code sections

### 2. Iterative Development
- Start with small changes
- Let Augment suggest improvements
- Test frequently

### 3. Code Review
Use Augment to:
- Review your code for bugs
- Suggest optimizations
- Improve code style

## Sample Augment Prompts for This Project

### For Learning:
```
"Explain the difference between AES-CBC and AES-CTR in this project"
"How does the file packing functionality work?"
"What's the purpose of the PE file manipulation features?"
```

### For Development:
```
"Help me implement the TODO in handleFileEncryption()"
"Add input validation to the menu choices"
"Create a function to list available encryption algorithms"
```

### For Refactoring:
```
"Convert the SimpleEncryptor to use the actual AES implementation"
"Make the menu system more modular"
"Add a configuration file parser"
```

## Project-Specific Augment Features

### 1. Understanding Complex Encryption
The encryption algorithms provide great examples for Augment to explain:
- Cryptographic concepts
- Implementation details
- Security considerations

### 2. C++ Best Practices
Ask Augment about:
- Memory management in the file operations
- RAII patterns for file handles
- Modern C++ alternatives to current code

### 3. Cross-Platform Development
Use Augment to help make the code portable:
- "How can I make this work on Linux/Mac?"
- "Replace Windows-specific functions with cross-platform alternatives"

## Exercises to Try

1. **Basic**: Use Augment to complete the TODO items in `simple_main.cpp`
2. **Intermediate**: Ask Augment to help integrate one of the encryption algorithms
3. **Advanced**: Have Augment help you create a GUI wrapper using Qt or wxWidgets

## Tips for Maximum Productivity

1. **Keep relevant files open** - Augment uses open files as context
2. **Be specific** - Instead of "fix this", say "add error handling for file not found"
3. **Iterate** - Build on Augment's suggestions incrementally
4. **Verify** - Always test Augment's code suggestions
5. **Learn** - Ask Augment to explain its suggestions

## Common Augment Commands

- **Explain**: "Explain this function"
- **Refactor**: "Refactor this to use modern C++"
- **Fix**: "Fix the compilation error"
- **Implement**: "Implement this TODO"
- **Test**: "Write a test for this function"
- **Document**: "Add documentation comments"

Remember: Augment is a tool to enhance your coding, not replace your thinking. Use it to learn, explore, and accelerate your development process!