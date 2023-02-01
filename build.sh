##################################
#  Project builder script        # 
##################################

Usage()
{
	echo "Usage: $0 -t <build type debug/release> -c <do you want a clean build?>
	      Example: $0 -t debug -c y
	      Where:
		     -t - Build type [debug/release].
		     -c - Whether to clean the project [y/n].
	     "
	exit 1
}

# Default assignements for command line arguments.
arg_type=debug
arg_clean=n

while [ $# -ne 0 ]; do
	case $1 in
		-t)
			arg_type=$2
			shift 2
			;;
		-c)
			arg_clean=$2
			shift 2
			;;
		*)
			Usage
	esac
done

type=''
case $arg_type in
	debug ) type='DEBUG';;
	release ) type='RELEASE';;
	*) echo "Error: Unrecognized argument value: '$arg_type'."; echo; echo "See usage."; Usage; exit -1;;
esac

clean=''
case $arg_clean in
	y ) clean=make_clean;;
	n ) clean=;;
	*) echo "Error: Unrecognized argument value: '$arg_clean'.";echo; echo "See usage.";Usage; exit -1;;
esac

build_dir_name=build
build_dir=`pwd`/$build_dir_name
build_sys=cmake
build_tool=make
log_dir_name=test.log
log_file=$build_dir/$log_dir_name

build_project() {
	echo "Info: CMake starts ..."
	if [ ! -d $build_dir ]; then
		echo "Info: Creating $build_dir"
		mkdir $build_dir
	fi
	cd $build_dir
	$build_sys -G "Unix Makefiles" -DCMAKE_BUILD_TYPE:INTERNAL=$type .. 2>&1 > log_file
	cmexit_code=${PIPESTATUS[0]}
	if [[ $cmexit_code -ne 0 ]]
	then
		echo "Error: ${build_sys} aborted."
		exit -1
	fi
	$build_tool 2>&1 > log_file
	mexit_code=${PIPESTATUS[0]}
	if [[ $mexit_code -ne 0 ]]
	then
		echo "Error: ${build_tool} aborted."
		exit -1
	fi
	cd ..
	echo "Info: Build done successfully"
}

make_clean() {
	echo "Info: Cleaning."
	if [[ ! -d $build_dir ]]; then
		echo "Warning: Couldn't find $build_dir. Creating $build_dir."
		mkdir $build_dir
		cd $build_dir
	else
		cd $build_dir
		$build_tool clean
	fi
	cd ..
	echo "Info: Cleaning is done successfully."
	build_project
}
if [ -n "$clean" ]; then
      make_clean
else
      build_project
fi
