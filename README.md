# IPC
操作系统课程设计 实验三 

- 这个实验包括 管道 消息队列 共享内存 
- 实验本身的逻辑不算复杂，但是基于返回值的错误处理使得代码比较长
- 中文资料(包括课程参考资料)都是使用*SYSTEM V IPC*,但是较新版本的Linux内核对于*POSIX IPC*的支持已经相当完备了，所以我在这里全部使用新的API

-参考资料主要是[Linux Programming Interface](http://man7.org/tlpi/index.html) , [man7.org](man7.org)页面本身的叙述也非常的详细.

