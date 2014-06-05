import subprocess
import os

APPNAME = 'libsub'
VERSION = '0.01.0devel'

def options(opt):
    opt.load('compiler_cxx')
    opt.add_option('--enable-debug', action='store_true', default=False, help='build with debugging information and without optimisation')
    opt.add_option('--static', action='store_true', default=False, help='build libsub statically and link statically to cxml')

def configure(conf):
    conf.load('compiler_cxx')
    conf.env.append_value('CXXFLAGS', ['-Wall', '-Wextra', '-D_FILE_OFFSET_BITS=64', '-D__STDC_FORMAT_MACROS'])
    conf.env.append_value('CXXFLAGS', ['-DLIBSUB_VERSION="%s"' % VERSION])

    conf.env.ENABLE_DEBUG = conf.options.enable_debug
    conf.env.STATIC = conf.options.static

    if conf.options.enable_debug:
        conf.env.append_value('CXXFLAGS', '-g')
    else:
        conf.env.append_value('CXXFLAGS', '-O3')

    if conf.options.static:
        conf.env.HAVE_CXML = 1
        conf.env.STLIB_CXML = ['cxml', 'glibmm-2.4', 'glib-2.0', 'pcre', 'sigc-2.0', 'rt', 'xml++-2.6', 'xml2', 'z']
        conf.check_cfg(package='libcxml', atleast_version='0.08', args='--cflags', uselib_store='CXML', mandatory=True)
    else:
        conf.check_cfg(package='libcxml', atleast_version='0.08', args='--cflags --libs', uselib_store='CXML', mandatory=True)

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

    conf.check_cxx(fragment = """
    			      #include <boost/filesystem.hpp>\n
    			      int main() { boost::filesystem::copy_file ("a", "b"); }\n
			      """,
                   msg = 'Checking for boost filesystem library',
                   libpath = '/usr/local/lib',
                   lib = ['boost_filesystem', 'boost_system'],
                   uselib_store = 'BOOST_FILESYSTEM')

    conf.check_cxx(fragment = """
    			      #include <boost/locale.hpp>\n
    			      int main() { boost::locale::conv::to_utf<char> ("a", "cp850"); }\n
			      """,
                   msg = 'Checking for boost locale library',
                   libpath = '/usr/local/lib',
                   lib = ['boost_locale', 'boost_system'],
                   uselib_store = 'BOOST_LOCALE')

    conf.recurse('test')

def build(bld):
    create_version_cc(bld, VERSION)

    bld(source='libsub.pc.in',
        version=VERSION,
        includedir='%s/include' % bld.env.PREFIX,
        libs="-L${libdir} -lsub -lboost_system",
        install_path='${LIBDIR}/pkgconfig')

    bld.recurse('src')
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
