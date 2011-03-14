VERSION=lcc

exec_prefix=build	

if test $# -gt 0 -a "xx$1" != "xx"
then
    TARGET=$1
else
    os=uname -s || echo "UNKNOWN-OS"
    if test "xx$os" = "xxunix"
    then
	TARGET=hera-unix

    else
	echo "There is no support for TARGET=$os in this version of lcc"
	exit 1
    fi
fi

if test $# -gt 1 -a "xx$2" != "xx"
then
    HOST=$2
else
    HOST=`echo $TARGET | tr - ' ' | awk '{print $2}'`
fi

echo
echo Building $VERSION for TARGET=$TARGET HOST=$HOST with exec_prefix=$exec_prefix 
echo

VERSIONNUMBER=`echo $VERSION | sed -e s/lcc-//`

# Where the installed files will go:
INSTALLBINDIR=$exec_prefix/bin
INSTALLLIBDIR=$exec_prefix/lib/$VERSION
INSTALLINCDIR=$INSTALLLIBDIR/include
INSTALLMANDIR=$exec_prefix/man

# rcp supports the -p (preserve file time stamp) flag, but not
# all cp implementations do, so use it instead
CP="rcp -p"
RM="rm -f"

TARGETOS=`echo $TARGET | tr - /`

# First do a sanity check to make sure that we can find suitable
# TARGET and HOST values, either from the command line, or internally
# by default.
if test ! -r etc/$HOST.c
then
    echo "=====================Cannot find driver host file etc/$HOST.c"
    echo "=====================Usage: $0 target host"
    echo "=====================Target = hera-unix"
    echo "======================Host   = one of: `/bin/ls -1 etc | sed -e 's/[.][ch]//g'`"
    exit 1
fi

TARGETOS=`echo $TARGET | tr - /`
if test ! -d $TARGETOS
then
    echo "======================Cannot find target/os directory $TARGETOS"
    echo "======================Usage: $0 target host"
    echo "======================Target = hera-unix"
    echo "======================Host   = one of: `/bin/ls -1 etc | sed -e 's/[.][ch]//g'`"
    exit 1
fi

if test "xx$TMPDIR" = "xx"
then
    TMPDIR=/usr/tmp
fi

echo "The build requires about 11MB of free space under $TMPDIR"
echo "Check that you have enough:"
# We try to use the -k flag for df to get sensible size units, but not
# all df implementations support it, so we try bare df if df -k fails:
df -k $TMPDIR 2>/dev/null || df $TMPDIR

# NB: Cannot do this in /tmp, because ld step to create lcc fails on
# Sun Solaris 5.2, sigh...
BUILDDIR=$TMPDIR/$VERSION/$TARGETOS; export BUILDDIR

echo
echo Creating temporary build directory tree in $BUILDDIR
mkdir -p $BUILDDIR

if test "xx$os" = "xxLinux"
then
    TMPDIRNAME=/tmp/lcc.dirname.$$
    dirname `find /usr/lib/gcc-lib/*/[0-9]* /usr/local/lib/gcc-lib/*/[0-9]* -name cpp 2>/dev/null | head -1` >$TMPDIRNAME
    echo ==============Using gcc release from `cat $TMPDIRNAME`
    if test -h $BUILDDIR/gcc
    then
	rm -f $BUILDDIR/gcc
    fi
    ln -s `cat $TMPDIRNAME` $BUILDDIR/gcc
    rm -f $TMPDIRNAME
fi

echo
echo Checking for ANSI/ISO Standard C compiler
if test "xx$CC" = "xx"
then
    COMPILERS="c89 cc gcc lcc"
else
    COMPILERS="$CC"
fi


echo "#include <stdio.h>"   >$BUILDDIR/try.c
echo "#include <stdlib.h>" >>$BUILDDIR/try.c
echo 'int main(void) { printf("Hello" " " "there"); return (0); }' >>$BUILDDIR/try.c
CC=
echo COMPILERS=$COMPILERS

for cc in $COMPILERS
do
    echo Trying CC="$cc $CFLAGS" for compilation of Standard C test file
    echo $cc $CFLAGS -c -o $BUILDDIR/try.o $BUILDDIR/try.c
    if $cc $CFLAGS -c -o $BUILDDIR/try.o $BUILDDIR/try.c 2>/dev/null
    then
	CC=$cc
	export CC
	$RM $BUILDDIR/try.c $BUILDDIR/try.o
	break
    fi
done

if test "xx$CC" = "xx"
then
    echo ============= Cannot find an ANSI/ISO Standard C compiler on your system
    exit 1
else
    echo Using ANSI/ISO Standard C compiler: CC="$CC $CFLAGS"
fi

# Delete include files (from a possibly-failed prior installation)
$RM -r $BUILDDIR/include

# and recreate an empty include file directory
mkdir -p $BUILDDIR/include

# then copy in the needed include files
$CP include/$TARGETOS/*.h $BUILDDIR/include

echo
echo Making lcc driver ...
echo make CC="$CC $CFLAGS" LD="$CC $CFLAGS" HOSTFILE=etc/$HOST.c lcc CFLAGS="'"-DLCCDIR=\"$INSTALLLIBDIR/\""'"
make CC="$CC $CFLAGS" LD="$CC $CFLAGS" HOSTFILE=etc/$HOST.c lcc CFLAGS="'"-DLCCDIR=\"$INSTALLLIBDIR/\""'"

echo
echo Building compiler and accessories ...
# We need LD here to be a Standard C compiler; on DEC Ultrix, LD
# defaults to cc, which fails because it is for K&R C
echo make CC="$CC $CFLAGS" LD="$CC $CFLAGS" all
make CC="$CC $CFLAGS" LD="$CC $CFLAGS" all

echo
echo Compiler test 1 ...
echo "skipping for the moment"

echo Compiler test 2 ...
echo "skipping for the moment"


echo Compiler test 3 ...
echo "skipping for the moment"


echo Cross-compiler code generation tests ...
REMOTEHOST=noexecute; export REMOTEHOST
echo "skipping for the moment"

echo
echo Installing user-callable programs ...
if test ! -d $INSTALLBINDIR
then
    mkdir -p $INSTALLBINDIR
fi


for f in lburg lcc bprint
do
    $RM $INSTALLBINDIR/$f
    $RM $INSTALLBINDIR/$f-$VERSIONNUMBER
    $CP $BUILDDIR/$f $INSTALLBINDIR/$f
    ln $INSTALLBINDIR/$f $INSTALLBINDIR/$f-$VERSIONNUMBER
    chmod 775 $INSTALLBINDIR/$f
    ls -l $INSTALLBINDIR/$f $INSTALLBINDIR/$f-$VERSIONNUMBER
done

echo
echo Installing auxiliary programs and run-time libraries ...
if test ! -d $INSTALLLIBDIR
then
    mkdir -p $INSTALLLIBDIR
fi

for f in rcc cpp
do
    $RM $INSTALLLIBDIR/$f
    $CP $BUILDDIR/$f $INSTALLLIBDIR/$f
    chmod 775 $INSTALLLIBDIR/$f
    ls -l $INSTALLLIBDIR/$f
done
for f in bbexit.o
do
    $RM $INSTALLLIBDIR/$f
    $CP $BUILDDIR/$f $INSTALLLIBDIR/$f
    chmod 664 $INSTALLLIBDIR/$f
    ls -l $INSTALLLIBDIR/$f
done
for f in liblcc.a librcc.a
do
    $RM $INSTALLLIBDIR/$f
    $CP $BUILDDIR/$f $INSTALLLIBDIR/$f
    chmod 664 $INSTALLLIBDIR/$f
    ranlib $INSTALLLIBDIR/$f || true
    ls -l $INSTALLLIBDIR/$f
done


if test "xx$os" = "xxLinux"
then
    echo Installing gcc link ...
    TMPDIRNAME=/tmp/lcc.dirname.$$
    dirname `find /usr/lib/gcc-lib/*/[0-9]* /usr/local/lib/gcc-lib/*/[0-9]* -name cpp 2>/dev/null | head -1` >$TMPDIRNAME
    echo Using gcc release from `cat $TMPDIRNAME`
    if test -h $INSTALLLIBDIR/gcc
    then
	rm -f $INSTALLLIBDIR/gcc
    fi
    ln -s `cat $TMPDIRNAME` $INSTALLLIBDIR/gcc
    rm -f $TMPDIRNAME
fi

echo
echo Installing include files ...
mkdir -p $INSTALLINCDIR
$CP include/$TARGETOS/*.h $INSTALLINCDIR
ls -lR $INSTALLINCDIR/*

echo
echo Installing man pages ...

if test ! -d $INSTALLMANDIR
then
    mkdir -p $INSTALLMANDIR
fi

if test ! -d $INSTALLMANDIR/man1
then
    mkdir -p $INSTALLMANDIR/man1
fi

if test ! -d $INSTALLMANDIR/cat1
then
    mkdir -p $INSTALLMANDIR/cat1
    chmod 777 $INSTALLMANDIR/cat1
fi

for f in doc/*.1
do
    g=`basename $f`
    chmod 777 $INSTALLMANDIR/cat1/$g 2>/dev/null
    $RM $INSTALLMANDIR/cat1/$g
    chmod 777 $INSTALLMANDIR/man1/$g 2>/dev/null
    $RM $INSTALLMANDIR/man1/$g
    $CP $f $INSTALLMANDIR/man1/$g
    chmod 664 $INSTALLMANDIR/man1/$g
    /bin/ls -l $INSTALLMANDIR/man1/$g
done

echo
echo "вроде как что-то работает...  DZ"
