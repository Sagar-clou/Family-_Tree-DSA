#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 100

typedef struct Individual {
    char name[MAX_NAME];
    struct Individual **children;
    size_t child_count;
} Individual;

typedef struct {
    Individual *root;
} FamilyTree;

static void trim_newline(char *text) {
    text[strcspn(text, "\r\n")] = '\0';
}

static int read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    if (strchr(buffer, '\n') == NULL && !feof(stdin)) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        printf("Input is too long. Please try again.\n");
        buffer[0] = '\0';
        return -1;
    }

    trim_newline(buffer);
    return 1;
}

static int is_blank(const char *text) {
    while (*text) {
        if (!isspace((unsigned char)*text)) return 0;
        text++;
    }
    return 1;
}

static Individual *create_individual(const char *name) {
    Individual *person = malloc(sizeof *person);
    if (person == NULL) {
        fprintf(stderr, "Error: not enough memory to create a family member.\n");
        return NULL;
    }

    snprintf(person->name, sizeof person->name, "%s", name);
    person->children = NULL;
    person->child_count = 0;
    return person;
}

static int add_child(Individual *parent, Individual *child) {
    Individual **new_children;

    if (parent == NULL || child == NULL) return 0;

    new_children = realloc(parent->children,
                           (parent->child_count + 1) * sizeof *parent->children);
    if (new_children == NULL) {
        fprintf(stderr, "Error: not enough memory to add this family member.\n");
        return 0;
    }

    parent->children = new_children;
    parent->children[parent->child_count] = child;
    parent->child_count++;
    return 1;
}

static FamilyTree *create_family_tree(const char *root_name) {
    FamilyTree *tree = malloc(sizeof *tree);
    if (tree == NULL) {
        fprintf(stderr, "Error: not enough memory to create the family tree.\n");
        return NULL;
    }

    tree->root = create_individual(root_name);
    if (tree->root == NULL) {
        free(tree);
        return NULL;
    }
    return tree;
}

static Individual *find_individual(Individual *node, const char *name) {
    size_t i;
    Individual *found;

    if (node == NULL) return NULL;
    if (strcmp(node->name, name) == 0) return node;

    for (i = 0; i < node->child_count; i++) {
        found = find_individual(node->children[i], name);
        if (found != NULL) return found;
    }
    return NULL;
}

static void visualize_tree(const Individual *node, size_t depth, int is_last) {
    size_t i;

    if (node == NULL) return;

    for (i = 0; i < depth; i++) {
        printf("   ");
    }
    if (depth > 0) {
        printf("%s", is_last ? "`-- " : "|-- ");
    }
    printf("%s\n", node->name);

    for (i = 0; i < node->child_count; i++) {
        visualize_tree(node->children[i], depth + 1, i == node->child_count - 1);
    }
}

static size_t count_members(const Individual *node) {
    size_t total = 1, i;
    if (node == NULL) return 0;

    for (i = 0; i < node->child_count; i++) {
        total += count_members(node->children[i]);
    }
    return total;
}

static void free_tree(Individual *node) {
    size_t i;
    if (node == NULL) return;

    for (i = 0; i < node->child_count; i++) {
        free_tree(node->children[i]);
    }
    free(node->children);
    free(node);
}

static void add_member(FamilyTree *family) {
    char parent_name[MAX_NAME];
    char child_name[MAX_NAME];
    Individual *parent, *child;
    int result;

    result = read_line("Enter parent's name: ", parent_name, sizeof parent_name);
    if (result <= 0) return;
    result = read_line("Enter child's name: ", child_name, sizeof child_name);
    if (result <= 0) return;

    if (is_blank(parent_name) || is_blank(child_name)) {
        printf("Names cannot be empty.\n");
        return;
    }

    parent = find_individual(family->root, parent_name);
    if (parent == NULL) {
        printf("Parent not found. Check the spelling and try again.\n");
        return;
    }

    child = create_individual(child_name);
    if (child == NULL) return;

    if (!add_child(parent, child)) {
        free(child);
        return;
    }

    printf("Added %s as a child of %s.\n", child_name, parent_name);
}

static void show_menu(void) {
    printf("\n========== FAMILY TREE GENERATOR ==========\n");
    printf("1. Add family member\n");
    printf("2. Display family tree\n");
    printf("3. Search for a family member\n");
    printf("4. Show total member count\n");
    printf("5. Exit\n");
    printf("===========================================\n");
}

int main(void) {
    FamilyTree *family;
    char root_name[MAX_NAME];
    char input[32];
    int choice, result;

    result = read_line("Enter the root family member's name (default: Sagar): ",
                       root_name, sizeof root_name);
    if (result <= 0) {
        printf("Using default root name: Sagar\n");
        snprintf(root_name, sizeof root_name, "%s", "Sagar");
    } else if (is_blank(root_name)) {
        snprintf(root_name, sizeof root_name, "%s", "Sagar");
    }

    family = create_family_tree(root_name);
    if (family == NULL) return EXIT_FAILURE;

    for (;;) {
        show_menu();
        result = read_line("Choose an option (1-5): ", input, sizeof input);
        if (result == 0) {
            printf("\nInput closed. Exiting safely.\n");
            break;
        }
        if (result < 0) continue;

        if (sscanf(input, "%d", &choice) != 1) {
            printf("Please enter a number from 1 to 5.\n");
            continue;
        }

        switch (choice) {
            case 1:
                add_member(family);
                break;
            case 2:
                printf("\nFamily Tree (%zu member%s)\n",
                       count_members(family->root),
                       count_members(family->root) == 1 ? "" : "s");
                visualize_tree(family->root, 0, 1);
                break;
            case 3: {
                char search_name[MAX_NAME];
                Individual *found;
                result = read_line("Enter the name to search for: ",
                                   search_name, sizeof search_name);
                if (result <= 0) break;
                found = find_individual(family->root, search_name);
                if (found != NULL) {
                    printf("Found family member: %s\n", found->name);
                } else {
                    printf("No family member named '%s' was found.\n", search_name);
                }
                break;
            }
            case 4:
                printf("Total family members: %zu\n", count_members(family->root));
                break;
            case 5:
                printf("Goodbye! Family tree memory released.\n");
                free_tree(family->root);
                free(family);
                return EXIT_SUCCESS;
            default:
                printf("Invalid option. Please choose a number from 1 to 5.\n");
        }
    }

    free_tree(family->root);
    free(family);
    return EXIT_SUCCESS;
}
