bcc -mm -c graph.c
bcc -mm -c error.c
bcc -mm -c main.c

bcc -mm -c board.c
bcc -mm -c tile.c

bcc -mm -c sounds.c
REM bcc -mm -c score.c


bcc -mm main.obj graph.obj error.obj board.obj tile.obj sounds.obj
