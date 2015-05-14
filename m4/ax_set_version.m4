
AC_DEFUN([AX_SET_VERSION], [
	# Update version if possible
	if test -f scripts/setversion.sh; then
		scripts/setversion.sh ${PACKAGE_VERSION}
	fi

	# Still no version file... create one
	if test ! -f VERSION; then
    	echo ${PACKAGE_VERSION} > VERSION
	fi
	release_version=`cat VERSION`

	echo "Checking for RELEASE_VERSION... " $release_version
	AC_DEFINE_UNQUOTED(RELEASE_VERSION,["$release_version"],"Release version")
])
