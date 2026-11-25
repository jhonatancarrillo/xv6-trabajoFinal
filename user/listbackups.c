#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int fd;
  struct dirent de;
  struct stat st;
  char buf[512], *p;

  list_backups();

  if((fd = open("/backups", O_RDONLY)) < 0) {
    printf("No existen respaldos aun (directorio /backup vacio).\n");
    exit(0);
  }

  if(fstat(fd, &st) < 0 || st.type != T_DIR) {
    fprintf(2, "listbackups: /backup no es un directorio\n");
    close(fd);
    exit(1);
  }

  printf("=== RESPALDOS DISPONIBLES EN /backup ===\n");
  strcpy(buf, "/backups");
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if(de.inum == 0)
      continue;
    if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) >= 0) {
      printf("- %s  (%d bytes)\n", de.name, (int)st.size);
    }
  }

  close(fd);
  exit(0);
}
