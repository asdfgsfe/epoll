1.signal(SIGPIPE, SIG_IGN);
2.signal(SIGCHLD, SIG_IGN);
3.int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
4.int listtenfd = socket(PF_INT, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
5.struct sockadd_in servadd; 初始化ip地址结构 bind的时候用
6.setsockopt(); 设置地址重复利用
7.bind
8.listen(listenfd, SO_MAXCONN)
9. 进入epoll核心操作
	//poll 结构体
	struct pollfd {
		int   fd;         /* file descriptor */
	      	short events;     /* requested events */
		short revents;    /* returned events */
	};

	//epoll 结构体
	typedef union epoll_data {
		void        *ptr;
	        int          fd;
		__uint32_t   u32;
		__uint64_t   u64;
	} epoll_data_t;

	struct epoll_event {
		__uint32_t   events;      /* Epoll events */
		epoll_data_t data;        /* User data variable */
	};

	
	in epollfd = epoll_create1(EPOLL_CLOEXEC);
	struct epoll_event event;
	event.data.fd = listenfd;
	evnet.enenvs = POLLIN || EPOLLET;
	epoll_ctl(epollfd, EPOLL_ETL_ADD, listenfd, &events);

	std::vector<struct epoll_event> events;
	//events.emplace_back(std::move(event)); //不用添加到里面 为什么？？ 因为有listenfd记录了 epoll不借助这个 而是利用epollfd

	while (1)
	{
		int nready = epoll_wait(epollfd, evnets.data(), events.size(), -1);
		if (nready == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
		}
		for (int i = 0; i < nready; ++i)
		{
			assert(i < events.size());
			if (events[i].data.fd == listenfd && events[i].events & POLLIN)
			{
				int connfd = ::accept4(listenfd, )
				if (connfd == -1)
				{
					if (errno == EMFILE)
					{
						idlefd = accpet(listenfd, NULL, NULL);
						close(idlefd);
						idlefd = open("/dev/null", ...);
					}
					exit(1);
				}
				struct epoll_event conn_event;
				conn_event.data.fd = connfd;
				conn_event.evnets |= POLLIN;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &conn_event);
				events.emplace_back(std::move(conn_event));
			}
			else if (events[i].events & POLLIN)
			{
				//处理读事件
			}
			--nready;

		}
	}

