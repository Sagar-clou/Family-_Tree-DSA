# Family Tree Generator (C)

A simple, menu-driven family tree program written in C. It stores family members in a dynamically allocated tree and lets you add children, display the hierarchy, search for a member, and count all members.

## Features

- Choose the root family member when the program starts (defaults to `Sagar` if left blank).
- Add a child under an existing parent.
- Display the family hierarchy using an ASCII tree.
- Search for a family member by name.
- Count all members in the tree.
- Validate basic input and check memory-allocation failures.
- Release allocated memory when exiting.

## Project files

- `Family Tree.c` — program source code.
- `README.md` — this guide.

## Requirements

- A C compiler, such as GCC or Clang.
- A terminal or command prompt.

## Compile

Because the source filename contains spaces, wrap it in quotes.

### GCC

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic "Family Tree.c" -o family_tree
```

### Clang

```bash
clang -std=c11 -Wall -Wextra -Wpedantic "Family Tree.c" -o family_tree
```

## Run

On Linux or macOS:

```bash
./family_tree
```

On Windows (MinGW GCC):

```powershell
.\family_tree.exe
```

## How to use

1. Start the program.
2. Enter the root member's name, or press Enter to use `Sagar`.
3. Select an option from the menu:
   - `1` — Add family member. Enter the parent's name and the child's name.
   - `2` — Display the whole family tree.
   - `3` — Search for a family member by name.
   - `4` — Show the total number of family members.
   - `5` — Exit the program.

Names are matched exactly, including capitalization. For example, `Sagar` and `sagar` are treated as different names.

## Example

```text
Enter the root family member's name (default: Sagar):

========== FAMILY TREE GENERATOR ==========
1. Add family member
2. Display family tree
3. Search for a family member
4. Show total member count
5. Exit
===========================================
Choose an option (1-5): 1
Enter parent's name: Sagar
Enter child's name: Rahul
Added Rahul as a child of Sagar.

Choose an option (1-5): 2

Family Tree (2 members)
Sagar
   `-- Rahul
```

## Notes and limitations

- This is an in-memory program: the family tree is not saved after the program exits.
- A member is found by the first matching name in the tree. If the same name appears more than once, searching by name may find the earlier match.
- The program models parent-to-child relationships only; it does not separately store spouses, dates, or other family details.

## Possible future improvements

- Save and load the family tree from a file.
- Support deleting or editing family members.
- Use unique IDs to distinguish people who share the same name.
- Add spouse and other relationship types.

## License

You may use, modify, and share this educational project. Add a formal license file if you plan to publish it for broader reuse.
