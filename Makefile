all: message_queue test_pipe_size_with_nonblock pipe pipe_sync shm_cli shm_ser pipe_writer_block

message_queue: message_queue.c
	clang -o message_queue message_queue.c -Wall -pthread -lrt

test_pipe_size_with_nonblock: test_pipe_size_with_nonblock.c
	clang -o test_pipe_size_with_nonblock test_pipe_size_with_nonblock.c -Wall -pthread -lrt

pipe: pipe.c
	clang -o pipe pipe.c -Wall -pthread -lrt

pipe_sync: pipe_sync.c
	clang -o pipe_sync pipe_sync.c -Wall -pthread -lrt

shm_cli: shm_cli.c
	clang -o shm_cli shm_cli.c -Wall -pthread -lrt

shm_ser: shm_ser.c
	clang -o shm_ser shm_ser.c -Wall -pthread -lrt

pipe_writer_block: pipe_writer_block.c
	clang -o pipe_writer_block pipe_writer_block.c  -Wall -pthread -lrt

clean:
	rm message_queue test_pipe_size_with_nonblock pipe pipe_sync shm_cli shm_ser pipe_writer_block