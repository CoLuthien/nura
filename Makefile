CC = gcc
CFLAGS = -Wall
#$* <- 확장자가 없는 현재의 목표 파일(Target)

#$@ <- 현재의 목표 파일(Target)

#$< <- 현재의 목표 파일(Target)보다 더 최근에 갱신된 파일 이름

#$? <- 현재의 목표 파일(Target)보다 더 최근에 갱신된 파일이름


src = $(wildcard )