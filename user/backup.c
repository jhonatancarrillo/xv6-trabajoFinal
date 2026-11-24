#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int fd_src, fd_dst, n;
  char buf[512];
  char dest[64];
  char base[10];
  int len;

  if(argc != 2) {
    fprintf(2, "Uso: backup <archivo>\n");
    exit(1);
  }

  create_backup();
  mkdir("/backups");

  if((fd_src = open(argv[1], O_RDONLY)) < 0) {
    fprintf(2, "backup: no se puede abrir %s\n", argv[1]);
    exit(1);
  }

  len = strlen(argv[1]);
  if(len > 8) len = 8;
  memset(base, 0, sizeof(base));
  memmove(base, argv[1], len);

  strcpy(dest, "/backups/");
  strcpy(dest + strlen(dest), base);
  strcpy(dest + strlen(dest), ".bak");

  unlink(dest);
  if((fd_dst = open(dest, O_CREATE | O_WRONLY)) < 0) {
    fprintf(2, "backup: no se pudo crear %s\n", dest);
    close(fd_src);
    exit(1);
  }

  while((n = read(fd_src, buf, sizeof(buf))) > 0) {
    if(write(fd_dst, buf, n) != n) {
      fprintf(2, "backup: error de escritura\n");
      close(fd_src);
      close(fd_dst);
      exit(1);
    }
  }

  close(fd_src);
  close(fd_dst);
  printf("Respaldo exitoso: %s -> %s\n", argv[1], dest);
  exit(0);
}
