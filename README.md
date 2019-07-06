Асинхронный TCP/UDP сервер и TCP/UDP клиент.

Сборка сервера:
Необходимо использовать параметр -DTYPE=server в CMake.
Общий скрипт сборки:

    mkdir s_build || cd s_build
    cmake -DTYPE=server ..
    make

Сборка клиента:
Необходимо использовать параметр -DTYPE=client в CMake.
Общий скрипт сборки:

    mkdir c_build || cd c_build
    cmake -DTYPE=client ..
    make

Прошу обратить внимание на замечание в Server/server.cpp:43