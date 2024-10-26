#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char input[256];

    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;

        FILE *file = fopen("output.asm", "w");
        if (file == NULL) {
            perror("Failed to open file");
            return 1;
        }

        fprintf(file, 
            "SECTION .data\n"
            "    message db '%s', 10, 0\n" 
            "    messageLen equ $ - message\n"
            "\n"
            "SECTION .text\n"
            "    global _start\n"
            "\n"
            "_start:\n"
            "    mov eax, 4          ; syscall: write\n"
            "    mov ebx, 1          ; file descriptor: stdout\n"
            "    mov ecx, message     ; pointer to message\n"
            "    mov edx, messageLen  ; message length\n"
            "    int 0x80            ; call kernel\n"
            "\n"
            "    mov eax, 1          ; syscall: exit\n"
            "    xor ebx, ebx        ; exit code: 0\n"
            "    int 0x80            ; call kernel\n", 
            input);

        fclose(file);

        if (system("nasm -f elf64 output.asm -o output.o") == -1) {
            perror("Failed to run nasm");
            return 1;
        }
        
        if (system("ld output.o -o output") == -1) {
            perror("Failed to run ld");
            return 1;
        }

        if (remove("output.o") == -1)
         perror("Failed to delete output.o");

        if (remove("output.asm") == -1)
         perror("Failed to delete output.asm");
    }

    return 0;
}
