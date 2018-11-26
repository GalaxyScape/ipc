all: message_queue pipe_block_writer pipe pipe_sync

message_queue: message_queue.c
	clang -o message_queue message_queue.c -Wall -pthread -lrt

pipe_block_writer: pipe_block_writer.c
	clang -o pipe_block_writer.c pipe_block_writer.c -Wall -pthread -lrt

pipe: pipe.c
	clang -o pipe pipe.c -Wall -pthread -lrt

pipe_sync: pipe_sync.c
	clang -o pipe_sync pipe_sync.c -Wall -pthread -lrt

shm_cli: shm_cli.c
	clang -o shm_cli shm_cli.c -Wall -pthread -lrt

shm_ser: shm_ser.c
	clang -o shm_ser shm_ser.c -Wall -pthread -lrt

#program2: program2.c
#	clang -o program2 program2.c