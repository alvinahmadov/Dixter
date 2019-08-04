
if(WIN32)
    set(BOOST_ROOT "C:/Dev/boost_1_66_0" CACHE PATH "")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}/boost")
elseif(UNIX)
	set(BOOST_ROOT "/usr" CACHE PATH "")
	set(OPENMPI TRUE)
endif()