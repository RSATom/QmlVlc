cmake_minimum_required( VERSION 2.8.11 )

if( NOT MSVC )
    add_definitions( -std=c++11 )
endif()

add_subdirectory( libvlc_wrapper )

set( CMAKE_AUTOMOC ON )

find_package( Qt5Core )
find_package( Qt5Qml )
find_package( Qt5Quick )

file( GLOB SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    )

file( GLOB PLUGIN_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    QmlVlcPlugin.cpp
    QmlVlcPlugin.h
    )

list( REMOVE_ITEM SOURCE_FILES ${PLUGIN_SOURCE_FILES} )

if( NOT Qt5Multimedia_FOUND )
    file( GLOB MM_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        QmlVlcMm*.cpp
        QmlVlcMm*.h
        )
    list( REMOVE_ITEM SOURCE_FILES ${MM_SOURCE_FILES} )
endif()

add_library( QmlVlc STATIC ${SOURCE_FILES} )
set_target_properties( QmlVlc PROPERTIES POSITION_INDEPENDENT_CODE ON )
qt5_use_modules( QmlVlc Quick )
target_link_libraries( QmlVlc libvlc_wrapper )
