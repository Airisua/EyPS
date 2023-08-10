cmake_minimum_required(VERSION 3.23)
project(EyPS)

set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_VERBOSE_MAKEFILE ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(EyPS_HTTP
        http/HttpServer.h
        http/HttpServer.cpp
        http/HttpSession.h
        http/ssl_session.cpp
        http/ssl_session.h
        http/detect_session.cpp
        http/detect_session.h
        http/plain_session.cpp
        http/plain_session.h
        http/HttpHandleReq.h
        http/GenericHandler.h
        http/LoginHandler.h
        http/objects/loginAccountReq.h
        http/auth/PwdAuthenticate.cpp
        http/auth/PwdAuthenticate.h
        http/auth/Authenticator.h
        http/objects/LoginRet.h
        http/objects/loginTokenReq.h
        http/auth/TokenAuthenticate.cpp
        http/auth/TokenAuthenticate.h
        http/objects/ComboTokenReq.h
        http/auth/SessionKeyAuthenticate.cpp
        http/auth/SessionKeyAuthenticate.h
        http/objects/ComboTokenRes.h)

set(EyPS_GAME
        ${CMAKE_SOURCE_DIR}/external/kcp/ikcp.c
        game/GameServer.h
        game/GameServer.cpp
        game/Kcp.cpp
        game/Kcp.h
        )

set(EyPS_util
        util/bufferView.cpp
        util/bufferView.h
        util/dynamicBuffer.cpp
        util/dynamicBuffer.h
        util/Crypto.cpp
        util/Crypto.h
        util/BasePacket.cpp
        util/BasePacket.h
        util/PacketIds.h
        util/util.h
        util/psConfig.h
        util/psConfig.cpp
        util/log.cpp
        util/log.h
        util/Singleton.h
        pool/IOServicePool.cpp
        pool/IOServicePool.h)

set(EyPS_packet
        game/packet/PacketHandler.h
        game/packet/PacketManager.h
        game/packet/HandlePingReq.cpp
        game/packet/HandlePingReq.h
        game/packet/HandlePlayerTokenReq.cpp
        game/packet/HandlePlayerTokenReq.h
        game/packet/HandlePlayLoginReq.cpp
        game/packet/HandlePlayLoginReq.h
        game/player/player.cpp
        game/player/player.h
        )

set(EyPS_database
        database/Account.cpp
        database/Account.h
        database/databaseManager.cpp
        database/databaseManager.h
        database/databaseHelper.cpp
        database/databaseHelper.h)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/resources/proto protos)

add_executable(EyPS main.cpp ${EyPS_HTTP} ${EyPS_GAME} ${EyPS_util} ${protos} ${EyPS_packet} ${EyPS_database})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread  -Wa,-mbig-obj")

# -Wa,-mbig-obj 解决 file too big

# protobuf
find_package(Protobuf ${Protobuf_FIND_VERSION} QUIET CONFIG)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

if ( Protobuf_FOUND )
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
    message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
    message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
else()
    message( WARNING "Protobuf not found")
endif()

if( OPENSSL_FOUND )
    message(STATUS "openssl version : ${OPENSSL_VERSION}" )
else()
    message( WARNING "openssl not found" )
endif()

# boost
find_package(Boost  REQUIRED COMPONENTS log)
if( Boost_FOUND )
    message(STATUS "Boost version : ${Boost_VERSION}")
    message( STATUS "Boost include path : ${Boost_INCLUDE_DIRS}" )
else()
    message( WARNING "Boost not found")
endif()

# lua
find_package(Lua REQUIRED)

message(STATUS "LUA lib: ${LUA_LIBRARIES}")
message(STATUS "LUA include: ${LUA_INCLUDE_DIR}")

#mongo
find_package(core REQUIRED)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)


target_include_directories (EyPS PUBLIC
        ${CMAKE_SOURCE_DIR}/external/kcp
        ${CMAKE_SOURCE_DIR}/external/rapidjson-1.1.0/include
        ${LUA_INCLUDE_DIR}
        ${CORE_INCLUDE_DIR}
        )


if(WIN32)
    target_link_libraries(EyPS wsock32 ws2_32)
endif()
target_link_libraries(EyPS
        protobuf::libprotoc
        protobuf::libprotobuf
        protobuf::libprotobuf-lite
        OpenSSL::applink
        OpenSSL::SSL
        OpenSSL::Crypto
        Boost::headers
        Boost::log
        ${LUA_LIBRARIES}
        mongo::bsoncxx_shared
        mongo::mongocxx_shared
        )

