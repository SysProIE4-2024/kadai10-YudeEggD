/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

// system関数のクローン
int mysystem(char *command) {
  int pid, status;
  if (command == NULL)  // NULLポインタの場合
    return 1;
  if ((pid=fork()) < 0) // forkに失敗した場合
    return -1;
    
  if (pid != 0) {
    // 親プロセスの動作
    while(wait(&status) != pid)
      ;
  } else {
    // 子プロセスの動作
    execl("/bin/sh", "sh", "-c", command, NULL);
    perror("/bin/sh");
    exit(127);
  }

  return status;
}

/* 実行例
  % make                    <- コンパイルエラーなし
  cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c 

  % echo aaa                <- コマンドラインで実行
  aaa

  % ./mysysmain "echo aaa"  <- mysysmainで実行
  mysystem:                 <- mysystemの結果
  aaa
  retval = 00000000
  system:                   <- systemの結果
  aaa
  retval = 00000000

  % ./mysysmain "rmdir nothing" <- 実行時エラー
  mysystem:
  rmdir: nothing: No such file or directory
  retval = 00000100
  system:
  rmdir: nothing: No such file or directory
  retval = 00000100

  % ./mysysmain "nacmd"        <- 存在しないコマンド
  mysystem:
  sh: nacmd: command not found
  retval = 00007f00
  system:
  sh: nacmd: command not found
  retval = 00007f00

  % ./mysysmain                             <- 引数不足
  使い方 : ./mysysmain コマンド文字列          <- Usageの表示

*/
