#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int fd_src, fd_dst, n;
  char buf[512];
  char src[64];
  char base[10];
  int len;

  if(argc != 2) {
    fprintf(2, "Uso: restore <archivo>\n");
    exit(1);
  }

  restore_file();

  len = strlen(argv[1]);
  if(len > 8) len = 8;
  memset(base, 0, sizeof(base));
  memmove(base, argv[1], len);

  strcpy(src, "/backups/");
  strcpy(src + strlen(src), base);
  strcpy(src + strlen(src), ".bak");

  if((fd_src = open(src, O_RDONLY)) < 0) {
    fprintf(2, "restore: no existe respaldo para %s\n", argv[1]);
    exit(1);
  }

  unlink(argv[1]);
  if((fd_dst = open(argv[1], O_CREATE | O_WRONLY)) < 0) {
    fprintf(2, "restore: no se pudo restaurar %s\n", argv[1]);
    close(fd_src);
    exit(1);
  }

  while((n = read(fd_src, buf, sizeof(buf))) > 0) {
    if(write(fd_dst, buf, n) != n){
      fprintf(2, "restore: error al escribir datos\n");
      close(fd_src);
      close(fd_dst);
      exit(1);
    }
  }

  close(fd_src);
  close(fd_dst);
  printf("Restauracion exitosa: %s recuperado desde %s\n", argv[1], src);
  exit(0);
}
