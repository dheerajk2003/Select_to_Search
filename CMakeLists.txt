cmake_minimum_required(VERSION 3.10)

project(STS)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(NOTIFY REQUIRED libnotify)
pkg_check_modules(PNG REQUIRED libpng)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(CJSON REQUIRED libcjson)
pkg_check_modules(X11 REQUIRED x11)

add_executable(sts src/main.c)

target_include_directories(sts PUBLIC
    ${GLIB_INCLUDE_DIRS}
    ${NOTIFY_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${CJSON_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
)

target_link_libraries(sts
    ${GLIB_LIBRARIES}
    ${NOTIFY_LIBRARIES}
    ${PNG_LIBRARIES}
    ${CURL_LIBRARIES}
    ${CJSON_LIBRARIES}
    ${X11_LIBRARIES}
)