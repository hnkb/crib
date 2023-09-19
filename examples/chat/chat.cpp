
#include "chat.h"
#include <iostream>


#if !defined(_WIN32)

#	include <unistd.h>
#	include <poll.h>

class combinedPoll
{
public:
	combinedPoll(Socket& sock)
	{
		sources[0].fd = STDIN_FILENO;
		sources[0].events = POLLIN;
		sources[1].fd = sock;
		sources[1].events = POLLIN;
	}

	void poll()
	{
		::poll(sources, sizeof(sources) / sizeof(pollfd), -1);
		// TODO: check poll() return value for error
	}

	bool socketClosed() const { return sources[1].revents & (POLLERR | POLLHUP); }
	bool socketHasData() const { return sources[1].revents & POLLIN; }
	bool stdinHasData() const { return sources[0].revents & POLLIN; }

private:
	pollfd sources[2];
};

#else

class combinedPoll
{
public:
	combinedPoll(Socket& sock)
	{
		HANDLE sources[2];

		const int ix_socket = 0;
		const int ix_stdin = 1;

		sources[ix_socket] = WSACreateEvent();
		if (sources[ix_socket] == WSA_INVALID_EVENT)
			throw Socket::lastError();

		if (WSAEventSelect(sock, sources[ix_socket], FD_READ | FD_CLOSE))
			throw Socket::lastError();

		// auto result = WaitForMultipleObjectsEx(1, sources, FALSE, INFINITE, FALSE);
		// std::cout << "Event triggered " << result << "\n";

		sources[ix_stdin] = GetStdHandle(STD_INPUT_HANDLE);

		char buffer[1024];


		// recv(sock, buffer, sizeof(buffer), 0);


		// const HANDLE sources[] { GetStdHandle(STD_INPUT_HANDLE), WSACreateEvent(),
		// INVALID_HANDLE_VALUE }; WSAEventSelect(sock, sources[1], FD_READ);

		// INPUT_RECORD r[512];
		// DWORD read;
		// ReadConsoleInput(sources[ix_stdin], r, 512, &read);
		// printf("Read: %d\n", read);

		for (int i = 0; i < 13; i++)
		{
			auto result = WaitForMultipleObjects(1, sources, FALSE, INFINITE);

			std::cout << "Return value " << result << "\n";

			if (result == WAIT_OBJECT_0 + ix_socket)
			{
				auto received = recv(sock, buffer, sizeof(buffer), 0);
				buffer[received] = 0;
				std::cout << " [other]: " << buffer << "\n";
				// WSAResetEvent(sources[result]);
			}

			if (result == WAIT_OBJECT_0 + ix_stdin)
			{
				// fgets(buffer, sizeof(buffer), stdin);
				INPUT_RECORD r[512];
				DWORD read;
				ReadConsoleInput(sources[ix_stdin], r, 512, &read);
				printf("Read: %d\n", read);
			}


			WSAResetEvent(sources[ix_socket]);
		}
	}
};


#endif

using crib::Socket;


void chat(Socket& sock)
{
	combinedPoll sources(sock);

	char buffer[1024];

	while (true)
	{
		sources.poll();

		if (sources.socketClosed())
		{
			std::cout << "Connection closed by the other party.\n\n";
			break;
		}

		if (sources.socketHasData())
		{
			auto received = recv(sock, buffer, sizeof(buffer), 0);
			buffer[received] = 0;
			if (received == 0)
			{
				std::cout << "Connection closed by the other party.\n\n";
				break;
			}
			std::cout << " [other]: " << buffer << "\n";
		}

		if (sources.stdinHasData())
		{
			fgets(buffer, sizeof(buffer), stdin);
			auto len = strlen(buffer);
			if (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')
			{
				buffer[len - 1] = 0;
				len--;
			}

			if (len == 0 || std::string("quit") == buffer || std::string("exit") == buffer)
				break;

			auto sent = send(sock, buffer, len, 0);
			if (sent != len)
				throw Socket::lastError();
		}
	}
}
