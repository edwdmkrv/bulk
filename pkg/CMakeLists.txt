cmake_minimum_required (VERSION 3.12)

# Project stuff
project (bulk_pkg
	VERSION 0.0.${TRAVIS_BUILD_NUMBER}
	DESCRIPTION "Homework 7 Bulks of commands implementation package"
	HOMEPAGE_URL "https://github.com/edwdmkrv/bulk/tree/master/pkg"
	LANGUAGES CXX
)

# CPack stuff
set (CPACK_GENERATOR DEB)

set (CPACK_COMPONENT_PROG_DESCRIPTION
"bulk
The sample C++ bulks of commands program.")

set (CPACK_DEB_COMPONENT_INSTALL ON)
set (CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_PACKAGE_ARCHITECTURE "amd64")
set (CPACK_PACKAGE_CONTACT myname@company.com)
set (CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

include (CPack)
