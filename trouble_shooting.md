- 遇到过同学想通过无名信号量来同步进程的做法，我一开始也没注意到这个问题，只是发现`sem_post`在一个进程中操作返回值正确，value也的确增加了，但是在另一个进程中value却保持不变-_-;
    自己写代码的时候直接避开了这个坑，在一开始调试别人的代码也直接默认对方不会犯这样的错误了。
    调试别人的代码真的非常的困难，因为首先要理清对方的思路，然后才能开始找问题，与此同时还会很容易犯*推己及人*这样的错。

- `printf`大法好
