#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define CMD_BACKUP "backup"
#define CMD_RESTORE "restore"

// Función para ejecutar comandos como procesos hijos
int run_cmd2(char *cmd, char *arg1, char *arg2) {
    int pid = fork();
    if (pid < 0) {
        printf("Error en fork\n");
        return -1;
    }
    if (pid == 0) {
        char *argv[] = {cmd, arg1, 0}; // <-- ELIMINAMOS arg2
        exec(cmd, argv);
        printf("Error: no se pudo ejecutar %s\n", cmd);
        exit(1);
    }
    int status = 0;
    wait(&status);
    return status;
}

// Función para crear un archivo con contenido específico
int create_file(char *name, char *data, int len) {
    int fd = open(name, O_CREATE | O_WRONLY | O_TRUNC);
    if (fd < 0) return -1;
    if (len > 0 && write(fd, data, len) != len) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

// Función para verificar si un archivo tiene el contenido esperado
int verify_file(char *name, char *expected, int len) {
    int fd = open(name, O_RDONLY);
    if (fd < 0) return 0;

    struct stat st;
    if (fstat(fd, &st) < 0 || st.size != len) {
        close(fd);
        return 0;
    }

    char buf[256];
    int total = 0;
    while (total < len) {
        int n = read(fd, buf, sizeof(buf));
        if (n <= 0) break;
        for (int i = 0; i < n; i++) {
            if (buf[i] != expected[total + i]) {
                close(fd);
                return 0;
            }
        }
        total += n;
    }
    close(fd);
    return (total == len);
}

int main(void) {
    printf("=== INICIANDO PRUEBAS PASO 4 (CP01 - CP08) ===\n\n");
    char *texto = "Datos criticos de prueba xv6\n";
    int len = strlen(texto);

    // CP01: Respaldo
    printf("[CP01] Backup archivo existente: ");
    create_file("cp01.txt", texto, len);
    if (run_cmd2(CMD_BACKUP, "cp01.txt", "") == 0)
        printf("OK\n");
    else
        printf("FALLÓ\n");

    // CP02 y CP03: Restaurar y verificar
    unlink("cp01.txt");
    printf("[CP02] Restore archivo respaldado: ");
    if (run_cmd2(CMD_RESTORE, "cp01.txt", "") == 0)
        printf("OK\n");
    else
        printf("FALLÓ\n");

    printf("[CP03] Verificacion de integridad: ");
    if (verify_file("cp01.txt", texto, len))
        printf("OK\n");
    else
        printf("FALLÓ\n");

    // CP04: Error controlado en backup
    printf("[CP04] Backup archivo inexistente (espera error controlado): ");
    if (run_cmd2(CMD_BACKUP, "noexiste_cp04.txt", "") != 0)
        printf("OK (error controlado)\n");
    else
        printf("FALLÓ\n");

    // CP05: Error controlado en restore
    printf("[CP05] Restore copia inexistente (espera error controlado): ");
    if (run_cmd2(CMD_RESTORE, "noexiste_cp05.txt", "") != 0)
        printf("OK (error controlado)\n");
    else
        printf("FALLÓ\n");

    // CP06: Restaurar varias veces
    printf("[CP06] Restaurar varias veces: ");
    int r1 = run_cmd2(CMD_RESTORE, "cp01.txt", "");
    int r2 = run_cmd2(CMD_RESTORE, "cp01.txt", "");
    if (r1 == 0 && r2 == 0 && verify_file("cp01.txt", texto, len))
        printf("OK\n");
    else
        printf("FALLÓ\n");

    // CP07: Archivo vacio
    printf("\n[CP07] Archivo vacio (0 bytes): ");
    create_file("vacio.txt", "", 0);
    run_cmd2(CMD_BACKUP, "vacio.txt", "");
    unlink("vacio.txt");
    run_cmd2(CMD_RESTORE, "vacio.txt", "");
    if (verify_file("vacio.txt", "", 0))
        printf("OK\n");
    else
        printf("FALLÓ\n");

    // CP08: Archivo extenso (2048 bytes)
    printf("[CP08] Archivo extenso (2048 bytes): ");
    char extenso[2048];
    for (int i = 0; i < 2048; i++) extenso[i] = 'A' + (i % 26);
    create_file("extenso.txt", extenso, 2048);
    run_cmd2(CMD_BACKUP, "extenso.txt", "");
    unlink("extenso.txt");
    run_cmd2(CMD_RESTORE, "extenso.txt", "");
    if (verify_file("extenso.txt", extenso, 2048))
        printf("OK\n");
    else
        printf("FALLÓ\n");

    printf("\n=== FIN DE PRUEBAS ===\n");
    exit(0);
}
