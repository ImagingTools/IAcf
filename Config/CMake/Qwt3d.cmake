include_directories("$ENV{QWT3DDIR_0_3_1}/include")

#target_link_directories(${PROJECT_NAME} PUBLIC $ENV{QWT3DDIR_0_3_1}/Lib/${TARGETNAME})

target_link_libraries(${PROJECT_NAME} qwt3d)

