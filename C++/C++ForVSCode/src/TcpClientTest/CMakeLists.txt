cmake_minimum_required(VERSION 3.20)
project(TcpClientTest)

# CMAKE_SOURCE_DIR	最上位のCMakeLists.txtがあるディレクトリ（ソースルート）
# CMAKE_CURRENT_SOURCE_DIR	現在処理中の CMakeLists.txt があるディレクトリ
# CMAKE_BINARY_DIR	最上位のビルドディレクトリ（cmake -B で指定した場所）
# CMAKE_CURRENT_BINARY_DIR	現在処理中のディレクトリに対応するビルドディレクトリ

add_executable(TcpClientTest
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/TcpComm/TcpAttrib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/TcpComm/TcpClient.cpp
)

target_include_directories(TcpClientTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/Base
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/TcpComm
)
