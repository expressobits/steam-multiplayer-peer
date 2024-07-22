#!/usr/bin/env python
import os


def normalize_path(val, env):
    return val if os.path.isabs(val) else os.path.join(env.Dir("#").abspath, val)


def validate_parent_dir(key, val, env):
    if not os.path.isdir(normalize_path(os.path.dirname(val), env)):
        raise UserError("'%s' is not a directory: %s" % (key, os.path.dirname(val)))

libname = "steam-multiplayer-peer"
projectdir = "steam-multiplayer-peer"

localEnv = Environment(tools=["default"], PLATFORM="")

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Add(
    BoolVariable(
        key="compiledb",
        help="Generate compilation DB (`compile_commands.json`) for external tools",
        default=localEnv.get("compiledb", False),
    )
)
opts.Add(
    PathVariable(
        key="compiledb_file",
        help="Path to a custom `compile_commands.json` file",
        default=localEnv.get("compiledb_file", "compile_commands.json"),
        validator=validate_parent_dir,
    )
)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()
env["compiledb"] = False

env.Tool("compilation_db")
compilation_db = env.CompilationDatabase(
    normalize_path(localEnv["compiledb_file"], localEnv)
)
env.Alias("compiledb", compilation_db)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

# Local dependency paths, adapt them to your setup
steam_lib_path = "steam-multiplayer-peer/sdk/redistributable_bin"

if env['platform'] in ('macos', 'osx'):
    # Set the correct Steam library
    steam_lib_path += "/osx"
    steamworks_library = 'libsteam_api.dylib'

elif env['platform'] in ('linuxbsd', 'linux'):
    # Set correct Steam library
    steam_lib_path += "/linux64" if env['arch'] == 'x86_64' else "/linux32"
    steamworks_library = 'libsteam_api.so'

elif env['platform'] == "windows":
    steam_lib_path += "/win64" if env['arch'] == 'x86_64' else ""
    steamworks_library = 'steam_api64.dll' if env['arch'] == 'x86_64' else 'steam_api.dll'

env.Append(LIBPATH=[steam_lib_path])
env.Append(CPPPATH=['steam-multiplayer-peer/sdk/public'])
env.Append(LIBS=[
    steamworks_library.replace(".dll", "")
])

env.Append(CPPPATH=['steam-multiplayer-peer/'])
sources = [
    Glob('steam-multiplayer-peer/*.cpp'),
    ]

file = "{}{}{}".format(libname, env["suffix"], env["SHLIBSUFFIX"])

if env["platform"] == "macos" or env["platform"] == "ios":
    platlibname = "{}.{}.{}".format(libname, env["platform"], env["target"])
    file = "{}.framework/{}".format(env["platform"], platlibname, platlibname)

libraryfile = "bin/{}/{}".format(env["platform"], file)
library = env.SharedLibrary(
    libraryfile,
    source=sources,
)

copy = env.InstallAs("{}/addons/{}/bin/{}/{}".format(projectdir, projectdir, env["platform"], file), library)

default_args = [library, copy]
if localEnv.get("compiledb", False):
    default_args += [compilation_db]
Default(*default_args)