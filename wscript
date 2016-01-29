import subprocess
import os

APPNAME = 'libsub'
VERSION = '1.1.9devel'
API_VERSION = '-1.0'

def options(opt):
    opt.load('compiler_cxx')
    opt.add_option('--enable-debug', action='store_true', default=False, help='build with debugging information and without optimisation')
    opt.add_option('--static', action='store_true', default=False, help='build libsub statically and link statically to cxml and dcp')
    opt.add_option('--target-windows', action='store_true', default=False, help='set up to do a cross-compile to make a Windows package')
    opt.add_option('--disable-tests', action='store_true', default=False, help='disable building of tests')

def configure(conf):
    conf.load('compiler_cxx')
    conf.env.append_value('CXXFLAGS', ['-Wall', '-Wextra', '-D_FILE_OFFSET_BITS=64', '-D__STDC_FORMAT_MACROS'])
    conf.env.append_value('CXXFLAGS', ['-DLIBSUB_VERSION="%s"' % VERSION])

    conf.env.ENABLE_DEBUG = conf.options.enable_debug
    conf.env.STATIC = conf.options.static
    conf.env.TARGET_WINDOWS = conf.options.target_windows
    conf.env.DISABLE_TESTS = conf.options.disable_tests
    conf.env.API_VERSION = API_VERSION

    if conf.options.enable_debug:
        conf.env.append_value('CXXFLAGS', '-g')
    else:
        conf.env.append_value('CXXFLAGS', '-O3')

    conf.check_cfg(package='openssl', args='--cflags --libs', uselib_store='OPENSSL', mandatory=True)

    if conf.options.static:
        conf.env.HAVE_CXML = 1
        conf.env.LIB_CXML = ['glibmm-2.4', 'glib-2.0', 'pcre', 'sigc-2.0', 'rt', 'xml++-2.6', 'xml2', 'pthread', 'lzma', 'dl', 'z']
        conf.env.STLIB_CXML = ['cxml']
        conf.check_cfg(package='libcxml', atleast_version='0.14.0', args='--cflags', uselib_store='CXML', mandatory=True)
        conf.env.HAVE_ASDCPLIB_CTH = 1
        conf.env.STLIB_ASDCPLIB_CTH = ['asdcp-cth', 'kumu-cth']
        conf.env.LIB_ASDCPLIB_CTH = ['ssl', 'crypto']
        conf.check_cfg(package='libasdcp-cth', atleast_version='0.0.1', args='--cflags', uselib_store='ASDCPLIB_CTH', mandatory=True)
    else:
        conf.check_cfg(package='libcxml', atleast_version='0.14.0', args='--cflags --libs', uselib_store='CXML', mandatory=True)
        conf.check_cfg(package='libasdcp-cth', atleast_version='0.0.1', args='--cflags --libs', uselib_store='ASDCPLIB_CTH', mandatory=True)

    boost_lib_suffix = ''
    if conf.env.TARGET_WINDOWS:
        boost_lib_suffix = '-mt'

    conf.check_cxx(fragment="""
                            #include <boost/version.hpp>\n
                            #if BOOST_VERSION < 104500\n
                            #error boost too old\n
                            #endif\n
                            int main(void) { return 0; }\n
                            """,
                   mandatory=True,
                   msg='Checking for boost library >= 1.45',
                   okmsg='yes',
                   errmsg='too old\nPlease install boost version 1.45 or higher.')

    conf.check_cxx(fragment="""
    			    #include <boost/filesystem.hpp>\n
    			    int main() { boost::filesystem::copy_file ("a", "b"); }\n
			    """,
                   msg='Checking for boost filesystem library',
                   libpath='/usr/local/lib',
                   lib=['boost_filesystem%s' % boost_lib_suffix, 'boost_system%s' % boost_lib_suffix],
                   uselib_store='BOOST_FILESYSTEM')

    conf.check_cxx(fragment="""
    			    #include <boost/locale.hpp>\n
    			    int main() { boost::locale::conv::to_utf<char> ("a", "cp850"); }\n
			    """,
                   msg='Checking for boost locale library',
                   libpath='/usr/local/lib',
                   lib=['boost_locale%s' % boost_lib_suffix, 'boost_system%s' % boost_lib_suffix],
                   uselib_store='BOOST_LOCALE')

    conf.check_cxx(fragment="""
    			    #include <boost/regex.hpp>\n
    			    int main() { boost::regex re ("foo"); }\n
			    """,
                   msg='Checking for boost regex library',
                   libpath='/usr/local/lib',
                   lib=['boost_regex%s' % boost_lib_suffix, 'boost_system%s' % boost_lib_suffix],
                   uselib_store='BOOST_REGEX')

    if not conf.env.DISABLE_TESTS:
        conf.recurse('test')

def build(bld):
    create_version_cc(bld, VERSION)

    if bld.env.TARGET_WINDOWS:
        boost_lib_suffix = '-mt'
    else:
        boost_lib_suffix = ''

    bld(source='libsub%s.pc.in' % bld.env.API_VERSION,
        version=VERSION,
        includedir='%s/include/libsub%s' % (bld.env.PREFIX, bld.env.API_VERSION),
        libs="-L${libdir} -lsub%s -lboost_system%s" % (bld.env.API_VERSION, boost_lib_suffix),
        install_path='${LIBDIR}/pkgconfig')

    bld.recurse('src')
    if not bld.env.DISABLE_TESTS:
        bld.recurse('test')
    bld.recurse('tools')

    bld.add_post_fun(post)

def dist(ctx):
    ctx.excl = 'TODO core *~ .git build .waf* .lock* doc/*~ src/*~ test/ref/*~ __pycache__ GPATH GRTAGS GSYMS GTAGS'

def create_version_cc(bld, version):
    if os.path.exists('.git'):
        cmd = "LANG= git log --abbrev HEAD^..HEAD ."
        output = subprocess.Popen(cmd, shell=True, stderr=subprocess.STDOUT, stdout=subprocess.PIPE).communicate()[0].splitlines()
        o = output[0].decode('utf-8')
        commit = o.replace ("commit ", "")[0:10]
    else:
        commit = "release"

    try:
        text =  '#include "version.h"\n'
        text += 'char const * sub::git_commit = \"%s\";\n' % commit
        text += 'char const * sub::version = \"%s\";\n' % version
        if bld.env.ENABLE_DEBUG:
            debug_string = 'true'
        else:
            debug_string = 'false'
        text += 'bool const built_with_debug = %s;\n' % debug_string
        print('Writing version information to src/version.cc')
        o = open('src/version.cc', 'w')
        o.write(text)
        o.close()
    except IOError:
        print('Could not open src/version.cc for writing\n')
        sys.exit(-1)

def post(ctx):
    if ctx.cmd == 'install':
        ctx.exec_command('/sbin/ldconfig')
